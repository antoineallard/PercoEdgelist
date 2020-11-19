/*
 *
 *  This tutorial explains the use of the edgelist_perco_t class of the PGL
 *    to simulate bond percolation on a given undirected graph structure.
 *
 *  Compilation: g++ -O3 -std=c++11 tutorial_edgelist_perco_t.cpp
 *
 *  Author:  Antoine Allard
 *  WWW:     antoineallard.info
 *  Date:    August 2019
 *
 *
 *  Copyright (C) 2019 Antoine Allard
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */


// Portable graph library
#include "../src/edgelist_perco_t.hpp"


int main(int argc, char const *argv[])
{
  // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
  // Loading a graph from a edgelist
  // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=

  // A graph can be imported from a file containing its edgelist (one edge per
  //  line). The edgelist file consists in a simple text file with the following
  //  convention:
  //
  // # lines beginning with "#" are ignored (comments).
  // # note that vertices' name must be separated by at least one white space.
  // # there may be white space at the beginning of a line.
  // [name of a vertex]  [name of a vertex]  [ignores the rest]
  // [name of a vertex]  [name of a vertex]  [ignores the rest]
  // [name of a vertex]  [name of a vertex]  [ignores the rest]
  // # comments can be inserted between edges
  // [name of a vertex]  [name of a vertex]  [ignores the rest]
  // [name of a vertex]  [name of a vertex]  [ignores the rest]
  // ...
  //
  // where the vertices's name will be imported as std::string and can therefore
  //   be virtually anything as long as they do not include white spaces (i.e.,
  //   there is not need for the vertices to be identified by contiguous
  //   integers).

  // IMPORTANT: this class only considers simple undirected graphs without
  //   self-loops. Any multiple edges (e.g., if the graph is originally
  //   directed) or self-loops will be ignored.

  // Loads the graph from a edgelist file.
  pgl::edgelist_perco_t g("../assets/edgelists/example04.edge");


  // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
  // Generates a random instances of the original graph
  // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=

  // Generates an instance of the original graph in which each edge has been
  //   removed with probability 1 - 0.6.
  g.bond_percolate(0.6);


  // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
  // Measures various properties of the random instance.
  // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=

  // Number of vertices in the graph.
  int nb_vertices = g.get_nb_vertices();
  // Number of components.
  int nb_components = g.get_nb_components();
  // Size of the largest component.
  int size_largest_perco_component = g.get_size_largest_perco_component();
  // Size of the second largest component.
  int size_second_largest_perco_component = g.get_size_second_largest_perco_component();
  // Returns the numerical ID of a randomly chosen vertex.
  int v = g.get_random_vertex();
  // Size of the component to which vertex "v" belongs.
  int component_size = g.get_component_size(v);
  // Size of the component to which a random vertex belongs.
  int size_random_perco_component = g.get_size_random_perco_component();





  return 0;
}
