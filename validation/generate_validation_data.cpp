/*
 *
 *  This code generates the text files used to validate the edgelist_perco_t
 *    class of the AGL to analyze the structure of undirected, unweighted and simple graph.
 *
 *  Compilation requires the c++11 standard.
 *    Example: g++ -O3 -std=c++11 generate_validation_data.cpp -o generate_validation_data
 *
 *  Author:  Antoine Allard
 *  WWW:     antoineallard.info
 *  Date:    November 2020
 *
 *
 *  Copyright (C) 2020 Antoine Allard
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


 // Standard template library
 #include <iomanip>
 #include <fstream>
 #include <string>
 // Portable graph library
 #include "../src/edgelist_perco_t.hpp"


 int main(int argc, char const *argv[])
 {
  // Opens the file stream.
  std::ofstream output_file("validation_edgelist_perco_t.dat");

  // column width in the output file.
  int width = 15;

  // Parameters of the simulations
  int nb_simulations = 25000;
  double dt = 0.01;

  // Writes the header.
  output_file << "#";
  output_file << std::setw(width - 1) << "edge_prob" << " ";
  output_file << std::setw(width) << "nb_vertices" << " ";
  output_file << std::setw(width) << "nb_edges" << " ";
  output_file << std::setw(width) << "size_1st" << " ";
  output_file << std::setw(width) << "size_2nd" << " ";
  output_file << std::setw(width) << "nb_comp" << " ";
  output_file << std::endl;

  // Loads the random edgelist.
  agl::edgelist_perco_t g("validation_edgelist.dat");

  // Performs several bond percolation simulations for each shuffled graph.
  for(int m(0), nb_edges; m<nb_simulations; ++m)
  {
    for(double t(0), tt(1.00001); t<=tt; t+=dt)
    {
      // Generates a bond percolated instance.
      nb_edges = g.bond_percolate(t);

      // Outputs the raw results.
      output_file << std::setw(width) << t << " ";
      output_file << std::setw(width) << g.get_nb_vertices() << " ";
      output_file << std::setw(width) << nb_edges << " ";
      output_file << std::setw(width) << g.get_size_largest_perco_component() << " ";
      output_file << std::setw(width) << g.get_size_second_largest_perco_component() << " ";
      output_file << std::setw(width) << g.get_nb_components() << " ";
      output_file << std::endl;
    }
  }

  // Closes the file stream.
  output_file.close();


  return 0;
 }
