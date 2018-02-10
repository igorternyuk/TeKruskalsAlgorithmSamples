#ifndef KRUSKALS1_H
#define KRUSKALS1_H

#include <vector>
#include <set>
#include <string>

struct Edge
{
    int from, to, weight;
    friend bool operator<(const Edge &a, const Edge &b);
};

bool operator<(const Edge &a, const Edge &b);

class Kruskals
{
public:
    explicit Kruskals();
    explicit Kruskals(const std::vector<Edge>& edges);
    void add_edge(int from, int to, int weight);
    void remove_edge(int from, int to);
    void read_edges_set_from_file(const std::string &path_to_file);
    const std::vector<Edge>& find_mst1();
    const std::vector<Edge>& find_mst2();
    void write_mst_to_file(const std::string &path_to_file);
    void print_edges() const;
    void print_mst() const;

private:
   size_t m_vertex_num;
   std::vector<Edge> m_edges;
   std::vector<int> m_tree_id;
   std::vector<int> m_dsu_parent;
   std::vector<int> m_dsu_rank;
   std::vector<Edge> m_mst;
   int m_total_cost {0};
   bool m_has_mst_changed {true};
   void remove_edge_duplicates();
   void make_set(int v);
   int find_set(int v);
   void unite_sets(int a, int b);
};

#endif // KRUSKALS1_H
