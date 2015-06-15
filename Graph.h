#ifndef GRAPH_H
#define GRAPH_H

#include <array>
#include <vector>
#include <memory>
#include <unordered_map>
#include <cassert>

template <typename V, typename E>
struct Edge;

/** \brief A vertex of a graph.
 *	\tparam V is the type of vertex data.
 *	\tparam E is the type of edge data.
 *
 *  \detail This struct is a container of edges and data specific to the vertex.
 *			Although the vertex's edges are publicly accessible, they are meant
 *			to be removed and added only through the Graph class template.
 */
template <typename V, typename E>
struct Vertex
{
	/** \brief The Vertex constructor.
	 *	\param data is the vertex's data.
	 */
	Vertex(const V& data)
	: data(data)
	{
		;
	}

	std::vector<std::shared_ptr<Edge<V, E>>> edges;

	V data;
};

/** \brief An edge of a graph.
 *	\tparam V is the type of vertex data.
 *	\tparam E is the type of edge data.
 *
 *  \detail This struct is a container of vertices and data specific to the edge.
 *			Although the edge's vertices are publicly accessible and not const,
 *			this is only to allow for altering vertex data.
 */
template <typename V, typename E>
struct Edge
{
	/** \brief The Edge constructor.
	 *
	 *  \param is the data
	 */
	Edge(const std::array<std::shared_ptr<Vertex<V,E>>,2>& vertices,
		 const E& data)
	: vertices(vertices), data(data)
	{
		;
	}

	std::array<std::shared_ptr<Vertex<V, E>>, 2> vertices;

	E data;
};

/** \brief The Graph class.
 *	\tparam Key is the type used to represent the vertex relative to other vertices.
 *	\tparam Hash is the type of hash used for looking up vertices.
 *	\tparam V is the type of vertex data.
 *	\tparam E is the type of edge data.
 *
 *  \detail This class implements the adjacency list structure of a graph. That
 *			is, the graph contains a set of vertices which, in turn, contain a
 *			set of edges. As such, this class is better suited for representing
 *			a sparse graph.
 *			No copy-constructor, assignment operator, or move-constructor exist.
 *			Any copies will result in a shallow copy.
 */
template <typename Key, typename Hash, typename V, typename E>
class Graph
{
	/** \param os is the stream to which the graph is output.
	 *	\param graph is the graph which is output.
	 *	\return the stream after outputting the graph to it.
	 *
	 *  \detail This function is useful for debugging; it outputs the addresses of
	 *			the graph, its vertices, and the vertices' edges.
	 */
	friend std::ostream& operator<<(std::ostream& os, const Graph& graph)
	{
		os << "Graph " << &graph << ":\n";
		for (auto& vertex : graph.vertices)
		{
			os << "  Vertex " << vertex.second << ":\n";
			for (auto& edge : vertex.second->edges)
			{
				os << "    Edge " << edge << ": " << edge->vertices.at(0) << " to " << edge->vertices.at(1) << "\n";
			}
		}

		return os;
	}

public:
	/** \brief The default constructor.
	 *
	 *	\detail Initializes the size of the graph to 0.
	 */
	Graph()
	: size(0)
	{
		;
	}
	/** \brief A constructor.
	*	\param vertex_count is the expected size of the graph.
	*
	*	\detail Initializes the size of the graph to 0 and then reserves memory
	*			for the expected number of vertices.
	*/
	Graph(size_t vertex_count)
	: size(0)
	{
		vertices.reserve(vertex_count);
	}
	/** \brief The (empty) destructor.
     */
	~Graph()
	{
		;
	}

	/** \brief Adds a vertex to the graph.
	 *	\param key is the key at which to store the vertex.
	 *	\param data is the data held by the vertex.
	 *
	 *  \detail This function does not check for pre-existing vertices.
	 */
	void addVertex(const Key& key,
				   const V& data)
	{
		std::shared_ptr<Vertex<V, E>> new_vertex(new Vertex<V, E>(data));
		std::pair<Key, std::shared_ptr<Vertex<V, E>>> new_pair(key, new_vertex);

		vertices.insert(new_pair);

		++size;
	}
	/** \brief Adds an edge to the graph.
	 *	\param key_0 is the key corresponding to the origin vertex.
	 *	\param key_1 is the key corresponding to the destination vertex.
	 *	\param data is the data held by the edge.
	 *
	 *  \detail This function does not check for the existence of vertices
	 *			at key_0 and key_1. However, it does assert that the keys
	 *			are not equal.
	 */
	void addEdge(const Key& key_0,
				 const Key& key_1,
				 const E& data)
	{
		assert(key_0 != key_1);

		std::shared_ptr<Vertex<V, E>> vertex_0 = vertices.at(key_0);
		std::shared_ptr<Vertex<V, E>> vertex_1 = vertices.at(key_1);

		std::array<std::shared_ptr<Vertex<V, E>>, 2> new_edge_vertices = { vertex_0, vertex_1 };

		std::shared_ptr<Edge<V, E>> new_edge(new Edge<V, E>(new_edge_vertices, data));

		vertex_0->edges.push_back(new_edge);
		vertex_1->edges.push_back(new_edge);
	}
	
	/** \brief Retrieve the vertex at the given input.
	 *	\param key is the key corresponding to desired vertex.
	 *	\return the vertex at the given input.
	 *
	 *	\detail This function does not check for the existence of the vertex.
	 */
	Vertex<V, E>& getVertex(const Key& key)
	{
		return *vertices.at(key);
	}
	/** \brief Retrieve the edge connecting the vertices at the given input.
	 *	\param key_0 is the key corresponding to the origin vertex.
	 *	\param key_1 is the key corresponding to the destination vertex.
	 *	\return the edge connecting the vertices at the given input.
	 *
	 *  \detail This function does not check for the existence of vertices
	 *	at key_0 and key_1. However, it does assert that the keys are not equal
	 *	and that the edge exists.
	 */
	Edge<V, E>& getEdge(const Key& key_0,
						const Key& key_1)
	{
		assert(key_0 != key_1);

		std::shared_ptr<Vertex<V, E>> vertex_0 = vertices.at(key_0);
		std::shared_ptr<Vertex<V, E>> vertex_1 = vertices.at(key_1);

		auto edge_iterator = vertex_0->edges.begin();

		while (edge_iterator != vertex_0->edges.end()
			&& (*edge_iterator)->vertices.at(0) != vertex_1
			&& (*edge_iterator)->vertices.at(1) != vertex_1)
		{
			++edge_iterator;
		}

		assert(edge_iterator != vertex_0->edges.end());

		return *(*edge_iterator);
	}
	/** \brief Retrieve the key of the input vertex.
	 *	\param vertex is the vertex of the desired key.
	 *	\return the key of the input vertex.
	 *
	 *  \detail This function asserts that the vertex exists in the graph.
	 */
	Key getKey(const Vertex<V, E>& vertex)
	{
		auto vertex_iterator = vertices.begin();
		
		while (vertex_iterator != vertices.end()
			&& vertex_iterator->second.get() != &vertex)
		{
			++vertex_iterator;
		}

		assert(vertex_iterator != vertices.end());

		return vertex_iterator->first;
	}
	/** \brief Retrieve the size of the graph.
	 *	\return the number of vertices in the graph.
	 */
	size_t getSize() const
	{
		return size;
	}

	/** \brief Remove the vertex at the given input.
	 *	\param key is the key corresponding to the desired vertex.
	 *
	 *  \detail This function does not check for the existence of the vertex.
	 *			The edges of the vertex are also removed.
	 */
	void removeVertex(const Key& key)
	{
		std::shared_ptr<Vertex<V, E>> old_vertex = vertices.at(key);
		std::shared_ptr<Vertex<V, E>> connected_vertex;

		std::shared_ptr<Edge<V, E>> old_edge;

		while (old_vertex->edges.size() > 0)
		{
			old_edge = *(--old_vertex->edges.end());

			if (old_vertex == old_edge->vertices.at(0))
				connected_vertex = old_edge->vertices.at(1);
			else
				connected_vertex = old_edge->vertices.at(0);

			old_vertex->edges.pop_back();

			auto old_edge_iterator = std::find(connected_vertex->edges.begin(), connected_vertex->edges.end(), old_edge);
			*old_edge_iterator = *(--connected_vertex->edges.end());
			connected_vertex->edges.pop_back();
		}

		vertices.erase(key);

		--size;
	}
	/** \brief Remove the edge conntecting the vertices at the given input.
	 *	\param key_0 is the key corresponding to the origin vertex.
	 *	\param key_1 is the key corresponding to the destination vertex.
	 *
	 *  \detail This function does not check for the existence of vertices
	 *	at key_0 and key_1. However, it does assert that the keys are not equal
	 *	and that the edge exists.
	 */
	void removeEdge(const Key& key_0,
					const Key& key_1)
	{
		assert(key_0 != key_1);

		std::shared_ptr<Vertex<V, E>> vertex_0 = vertices.at(key_0);
		std::shared_ptr<Vertex<V, E>> vertex_1 = vertices.at(key_1);

		auto current_edge_iterator = vertex_0->edges.begin();

		while (current_edge_iterator != vertex_0->edges.end()
			&& (*current_edge_iterator)->vertices.at(0) != vertex_1
			&& (*current_edge_iterator)->vertices.at(1) != vertex_1)
		{
			++current_edge_iterator;
		}

		assert(current_edge_iterator != vertex_0->edges.end());

		std::shared_ptr<Edge<V, E>> old_edge = *current_edge_iterator;

		*current_edge_iterator = *(--vertex_0->edges.end());
		vertex_0->edges.pop_back();

		auto old_edge_iterator = std::find(vertex_1->edges.begin(), vertex_1->edges.end(), old_edge);
		*old_edge_iterator = *(--vertex_1->edges.end());
		vertex_1->edges.pop_back();
	}

private:
	size_t size;
	std::unordered_map<Key, std::shared_ptr<Vertex<V, E>>, Hash> vertices;
};

#endif GRAPH_H