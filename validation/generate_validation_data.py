"""
This scripts uses graph_tool to generate the data which will be compared to
  the ones obtained with edgelist_perco_t, namely
  - get_nb_components
  - get_size_largest_perco_component
  - get_size_second_largest_perco_component

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
nb_vertices = 250

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

with open("validation_raw_results.dat", "w" ) as f:

    # g.set_fast_edge_removal(True)
    f.write('#   nb_vertices        nb_edges        size_1st        size_2nd         nb_comp\n')

    for i in range(25):

        g2 = g.copy()
        g2.set_fast_edge_removal(fast=True)
        edges = g2.get_edges()
        nb_edges = len(edges)
        np.random.shuffle(edges)

        ncomp = np.zeros(nb_edges + 1)
        size1 = np.zeros(nb_edges + 1)
        size2 = np.zeros(nb_edges + 1)

        comps = sorted(gt.label_components(g2)[1], reverse=True)
        ncomp[0] = len(comps)
        size1[0] = comps[0]
        if ncomp[0] > 1:
             size2[0] = comps[1]

        for e in np.arange(0, nb_edges, 1):
            g2.remove_edge(g2.edge(edges[e][0], edges[e][1]))
            comps = sorted(gt.label_components(g2)[1], reverse=True)
            ncomp[e+1] = len(comps)
            size1[e+1] = comps[0]
            if ncomp[e+1] > 1:
                 size2[e+1] = comps[1]

        np.savetxt(f, np.column_stack((np.ones((nb_edges + 1, 1)) * nb_vertices, range(nb_edges, -1, -1), size1, size2, ncomp)),
                   delimiter=' ', encoding='utf-8',
                   fmt='%15d %15d %15d %15d %15d ')
