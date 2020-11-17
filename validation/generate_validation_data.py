"""
This scripts used graph_tool to generate the data which will be compared to
  the ones obtained with edgelist_perco_t.

Copyright (C) 2020  Antoine Allard (antoineallard.info)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
"""
import graph_tool.all as gt
import numpy as np

# Number of vertices
nb_vertices = 2500

# Expected average degree
expected_average_degree = 5

# Sampler for the degree of the vertices
def sample_deg():
    return np.random.poisson(expected_average_degree)


# Generates a graph
g = gt.random_graph(nb_vertices, sample_deg, directed=False)

# Saves the graph as an edgelist.
with open("validation_edgelist.dat", "ab") as f:
    np.savetxt(f, g.get_edges(),
               fmt="%15s", delimiter=" ",
               header=" SourceVertex    TargetVertex")

# Gets the edge list
edges = g.get_edges()
nb_edges = len(edges)
with open("validation_raw_results.dat", "w" )as f:

    for i in range(200):
        np.random.shuffle(g.get_edges())
        sizes1, comp = gt.edge_percolation(g, edges)
        sizes2, comp = gt.edge_percolation(g, edges, second=True)

        np.savetxt(f, np.column_stack((range(1, nb_edges + 1), sizes1, sizes2)),
                   delimiter=' ', encoding='utf-8',
                   fmt='%20d %20d %20d ',
                   header='          nb_edges         size_largest  size_second_largest')
