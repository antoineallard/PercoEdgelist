## Percolation on graph

The `edgelist_perco_t` class offers many methods to perform and analyze bond percolation on **simple unweighted undirected graphs without self-loops**.


The class can be imported directly and is available under the namepace `pgl`.
```c++
// Importing the class
#include "src/edgelist_perco_t.hpp"
```

The currently available methods can be distinguished into three categories
* [Importing a graph from an edgelist file](#importing-a-graph-from-an-edgelist-file)
* [Generating one instance of a bond percolated graph](#generating-one-instance-of-a-bond-percolated-edgelist)
* [Properties of the percolated graph](#properties-of-the-percolated-edgelist)

Note that further examples on how to use `edgelist_perco_t` can be found in a notebook (and related scripts) used to validate the class.


#### Importing a graph from an edgelist file

A graph can be imported from a file containing its edgelist (one edge per line). The edgelist file consists in a simple text file with the following convention

```python
# lines beginning with "#" are ignored (comments).
# note that vertices' name must be separated by at least one white space.
# there may be white space at the beginning of a line.
[name of vertex1]  [name of vertex2]  [remaining information will be ignored]
[name of vertex2]  [name of vertex3]  [remaining information will be ignored]
[name of vertex4]  [name of vertex5]  [remaining information will be ignored]
# comments can be inserted between edges
[name of vertex5]  [name of vertex6]  [remaining information will be ignored]
[name of vertex7]  [name of vertex6]  [remaining information will be ignored]
...
```
Note that the vertices' name will be imported as `std::string` and can therefore be virtually anything as long as they do not include white spaces (i.e., there is not need for the vertices to be identified by contiguous integers).

**IMPORTANT**: this class only considers **simple unweighted undirected graphs without self-loops**. Any multiple edges or self-loops in the edgelist file will be ignored.

```c++
// The graph is loaded at the initialization of the class
pgl::edgelist_perco_t g("<path-to-edgelist-file>");
```


#### Generating one instance of a bond percolated edgelist

This method creates a provisional edgelist in which each edge has been kept with the probability passed as an argument.  The connected components are then identified using a [weighted union-find with path compression algorithm][1].

```c++
// Generates an instance of the original graph in which each edge has been
//   removed with probability 1 - 0.6.
int number_of_remaining_edges = g.bond_percolate(0.6);
// and returns the number of edges that have not been removed.
```


#### Properties of the percolated edgelist

Once a percolated instance has been generated, various information about its components can be extracted

```c++
// Size of the connected component to which vertex "v" belongs.
int component_size = g.get_component_size(v);

// Number of connected components.
int nb_components = g.get_nb_components();

// Number of vertices in the graph.
int nb_vertices = g.get_nb_vertices();

// Returns the numerical ID of a randomly chosen vertex.
int v = g.get_random_vertex();

// Size of the largest connected component.
int size_largest_perco_component = g.get_size_largest_perco_component();

// Size of the component to which a random vertex belongs.
int size_random_perco_component = g.get_size_random_perco_component();

// Size of the second largest connected component.
int size_second_largest_perco_component = g.get_size_second_largest_perco_component();
```

[1]: https://www.cs.princeton.edu/~rs/AlgsDS07/01UnionFind.pdf
