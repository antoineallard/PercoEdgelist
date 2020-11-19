/*
 *
 * This class provides the functions to perform bond percolation on an edgelist.
 *
 * Compilation requires the c++11 standard.
 *   Example: g++ -O3 -std=c++11 my_code.cpp
 *
 * Author:  Antoine Allard
 * WWW:     antoineallard.info
 * Date:    Nov. 2020
 *
 * Version 1.1
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

// Standard template library
#include <algorithm> // std::max_element, std::count
#include <cstdlib>   // std::terminate
#include <ctime>     // std::time(NULL)
#include <fstream>   // std::ifstream
#include <iostream>  // std::cerr
#include <map>       // std::map
#include <sstream>   // std::stringstream
#include <string>    // std::string, std::getline
#include <random>    // std::mt19937, std::uniform_real_distribution
#include <set>       // std::set
#include <utility>   // std::swap, std::pair, std::make_pair
#include <vector>    // std::vector


namespace pgl
{
  class edgelist_perco_t
  {
    private:
      // Number of vertices.
      int nb_vertices;
      // Adjacency lists.
      std::vector<std::vector<int> > adjacency_list;
      // Edgelist.
      std::set< std::pair<int, int> > edgelist;
      // ID of the cluster to which each vertex belongs.
      std::vector<int> clust_id;
      // Distribution of the size of components.
      std::vector<int> dist_clust_size;
      // Random number generator
      std::mt19937 engine;
      std::uniform_real_distribution<double> uniform_01;
      // Utilities.
      void merge_clusters(std::vector<int> &size);
      int get_root(int i);
      // Loads the data.
      void load_edgelist(std::string edgelist_filename);
      // Generates a random edgelist where each edge is preserved with probability T.
      int generate_random_adjacency_list(double T);
      // Find the size of all clusters.
      void find_dist_clust_size();
    public:
      // Constructor.
      edgelist_perco_t(std::string edgelist_filename);
      // Performs bond percolation.
      int bond_percolate(double T);
      // Returns the size of the component to which vertex "v" belongs.
      int get_component_size(int v);
      // Finds the number of components.
      int get_nb_components();
      // Returns the number of vertices in the graph.
      int get_nb_vertices() {return nb_vertices;};
      // Returns the numerical ID of a randomly chosen vertex.
      int get_random_vertex();
      // Finds the size of the largest percolation cluster.
      int get_size_largest_perco_component();
      // Returns the size of the component to which a random vertex belongs.
      int get_size_random_perco_component();
      // Finds the size of the second largest percolation cluster.
      int get_size_second_largest_perco_component();
  };
}


// =================================================================================================
// =================================================================================================
pgl::edgelist_perco_t::edgelist_perco_t(std::string edgelist_filename)
{
  // Initializes the random number generator.
  engine.seed(std::time(NULL));
  // Initializes the objet prior to percolation runs.
  load_edgelist(edgelist_filename);
}


// =================================================================================================
// =================================================================================================
int pgl::edgelist_perco_t::bond_percolate(double T)
{
  // Loads the percolated edgelist.
  int nb_edges = generate_random_adjacency_list(T);
  // Finds the distribution of the size of components.
  find_dist_clust_size();
  // Returns the number of remaining edges.
  return nb_edges;
}


// =================================================================================================
// =================================================================================================
void pgl::edgelist_perco_t::find_dist_clust_size()
{
  // Initializes the containers.
  dist_clust_size.clear();
  clust_id.clear();
  clust_id.resize(nb_vertices, 0);
  // std::vector<int> new_clust_id(nb_vertices, 1);
  // Starts with every vertex as an isolated cluster.
  std::vector<int> clust_size(nb_vertices, 1);
  for(int i=0; i<nb_vertices; i++)
  {
    clust_id[i] = i;
  }
  // Merges clusters until the minimal set is obtained.
  // while(clust_id != new_clust_id)
  // {
    // new_clust_id = clust_id;
    merge_clusters(clust_size);
  // }
  // Gets the distribution of the size of components.
  int s_max = *max_element(clust_id.begin(), clust_id.end());
  clust_size.clear();
  dist_clust_size.resize(s_max + 1, 0);
  for(int i(0); i<nb_vertices; ++i)
  {
    ++dist_clust_size[get_root(i)];
  }
}


// =================================================================================================
// =================================================================================================
int pgl::edgelist_perco_t::generate_random_adjacency_list(double T)
{
  // Variables.
  int v1, v2;
  int nb_edges(0);
  // Iterators.
  std::set< std::pair<int, int> >::iterator it, end;
  // Resets the containers.
  adjacency_list.clear();
  adjacency_list.resize(nb_vertices);
  // Loops over all edges.
  it = edgelist.begin();
  end = edgelist.end();
  for(; it!=end; ++it)
  {
    if(uniform_01(engine) < T)
    {
      // Identifies the vertices.
      v1 = it->first;
      v2 = it->second;
      // Adds the percolated edge.
      adjacency_list[v1].push_back(v2);
      adjacency_list[v2].push_back(v1);
      // Counts the number of remaining edges.
      ++nb_edges;
    }
  }
  // Returns the number of remaining edges.
  return nb_edges;
}


// =================================================================================================
// =================================================================================================
int pgl::edgelist_perco_t::get_component_size(int v)
{
  // Returns the size of the component to which vertex "v" belongs.
  return dist_clust_size[get_root(v)];
}


// =================================================================================================
// =================================================================================================
int pgl::edgelist_perco_t::get_nb_components()
{
  // Counts the number of components whose "size is not zero".
  int nb_components = 0;
  for(int i(0), ii(dist_clust_size.size()); i<ii; ++i)
  {
    if(dist_clust_size[i] > 0)
    {
      ++nb_components;
    }
  }
  // Returns the number of components.
  return nb_components;
}


// =================================================================================================
// =================================================================================================
int pgl::edgelist_perco_t::get_random_vertex()
{
  return (int) (uniform_01(engine) * nb_vertices);
}


// =================================================================================================
// =================================================================================================
int pgl::edgelist_perco_t::get_root(int i)
{
  while(i != clust_id[i])
  {
    clust_id[i] = clust_id[clust_id[i]];
    i = clust_id[i];
  }
  return i;
}


// =================================================================================================
// =================================================================================================
int pgl::edgelist_perco_t::get_size_largest_perco_component()
{
  // Returns the size of the largest component.
  return *std::max_element(dist_clust_size.begin(), dist_clust_size.end());
}


// =================================================================================================
// =================================================================================================
int pgl::edgelist_perco_t::get_size_random_perco_component()
{
  return get_component_size( get_random_vertex() );
}


// =================================================================================================
// =================================================================================================
int pgl::edgelist_perco_t::get_size_second_largest_perco_component()
{
  // Size of the largest component.
  int max = *std::max_element(dist_clust_size.begin(), dist_clust_size.end());
  // Checks if the second largest component is of the same size as the largest one.
  int count = std::count(dist_clust_size.begin(), dist_clust_size.end(), max);
  if(count > 1)
  {
    return max;
  }
  // Finds the size of the second largest component.
  int second_largest_clust_size = 0;
  for(int i(0), ii(dist_clust_size.size()); i<ii; ++i)
  {
    if(dist_clust_size[i] >= second_largest_clust_size)
    {
      if(dist_clust_size[i] < max)
      {
        second_largest_clust_size = dist_clust_size[i];
      }
    }
  }
  // Returns the size of the second largest component.
  return second_largest_clust_size;
}


// =================================================================================================
// =================================================================================================
void pgl::edgelist_perco_t::load_edgelist(std::string edgelist_filename)
{
  // Name and ID conversion.
  std::map<std::string, int> Name2ID;
  // Stream objects.
  std::fstream edgelist_file;
  std::stringstream one_line;
  // Variables.
  int v1, v2;
  // Initializes the number of vertices.
  nb_vertices = 0;
  // String objects.
  std::string full_line, name1_str, name2_str;
  // Iterators.
  std::map< std::string, int >::iterator name_it;

  // Opens the stream and terminates if the operation did not succeed.
  edgelist_file.open(edgelist_filename.c_str(), std::fstream::in);
  if( !edgelist_file.is_open() )
  {
    std::cerr << "ERROR: Could not open file: " << edgelist_filename << "." << std::endl;
    std::terminate();
  }
  // Reads the edgelist file line by line.
  while( !edgelist_file.eof() )
  {
    // Reads a line of the file.
    std::getline(edgelist_file, full_line);
    edgelist_file >> std::ws;
    one_line.str(full_line);
    one_line >> std::ws;
    one_line >> name1_str >> std::ws;
    // Skips a line of comment.
    if(name1_str == "#")
    {
      one_line.clear();
      continue;
    }
    one_line >> name2_str >> std::ws;
    one_line.clear();
    // Ignores self-loops.
    if(name1_str != name2_str)
    {
      // Is name1 new?
      name_it = Name2ID.find(name1_str);
      if(name_it == Name2ID.end())
      {
        Name2ID[name1_str] = nb_vertices;
        v1 = nb_vertices;
        ++nb_vertices;
      }
      else
      {
        v1 = name_it->second;
      }
      // Is name2 new?
      name_it = Name2ID.find(name2_str);
      if(name_it == Name2ID.end())
      {
        Name2ID[name2_str] = nb_vertices;
        v2 = nb_vertices;
        ++nb_vertices;
      }
      else
      {
        v2 = name_it->second;
      }
      // Adds the edge (multiedges are ignored).
      if(v1 > v2)
      {
        std::swap(v1, v2);
      }
      edgelist.insert(std::make_pair(v1, v2));
    }
  }
  // Closes the stream.
  edgelist_file.close();
}


// =================================================================================================
// =================================================================================================
void pgl::edgelist_perco_t::merge_clusters(std::vector<int> &size)
{
  // Variables.
  int v1, v2, v3, v4;
  // Loops over the vertices.
  for(int i(0); i<nb_vertices; ++i)
  {
    // Loops over the neighbors.
    for(int j(0), jj(adjacency_list[i].size()); j<jj; ++j)
    {
      if(get_root(i) != get_root(adjacency_list[i][j]))
      {
        // Adjust the root of vertices.
        v1 = i;
        v2 = adjacency_list[i][j];
        if(size[v2] > size[v1])
          std::swap(v1, v2);
        v3 = get_root(v1);
        v4 = get_root(v2);
        clust_id[v4] = v3;
        size[v3] += size[v4];
      }
    }
  }
}
