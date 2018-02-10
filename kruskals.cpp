#include "kruskals.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

bool operator<(const Edge &a, const Edge &b)
{
    return a.weight < b.weight;
}

Kruskals::Kruskals()
{}

Kruskals::Kruskals(const std::vector<Edge> &edges):
    m_edges{edges}
{}

void Kruskals::add_edge(int from, int to, int weight)
{
     m_edges.push_back({from, to, weight});
     remove_edge_duplicates();
     m_total_cost = 0;
     m_has_mst_changed = true;
}

void Kruskals::remove_edge(int from, int to)
{
    auto it = m_edges.begin();
    for(;it != m_edges.end(); ++it)
    {
        if(it->from == from && it->to == to)
        {
            it = m_edges.erase(it);
            m_total_cost = 0;
            m_has_mst_changed = true;
            break;
        }
    }
}

void Kruskals::read_edges_set_from_file(const std::string &path_to_file)
{
    std::ifstream fi(path_to_file);
    if(fi.is_open())
    {
        m_edges.clear();
        size_t counter{0};
        while(!fi.eof())
        {
            std::string line;
            std::getline(fi, line);
            std::stringstream ss;
            ss << line;
            if(counter == 0)
            {
                m_vertex_num = std::atoi(line.c_str());
                ++counter;
                continue;
            }
            int from, to, weight;
            ss >> from >> to >> weight;
            m_edges.push_back({from, to, weight});
            ++counter;
        }
        fi.close();
        remove_edge_duplicates();
        m_total_cost = 0;
        m_has_mst_changed = true;
    }
}

const std::vector<Edge> &Kruskals::find_mst1()
{
    if(m_has_mst_changed)
    {
        m_mst.clear();
        m_mst.reserve(m_edges.size());
        m_total_cost = 0;
        m_tree_id.clear();
        m_tree_id.reserve(m_vertex_num);
        for(size_t i{0}; i < m_vertex_num; ++i)
            m_tree_id.push_back(i);
        for(auto it = m_edges.begin(); it != m_edges.end(); ++it)
        {
            auto from = it->from;
            auto to = it->to;
            if(m_tree_id[from] != m_tree_id[to])
            {
                m_total_cost += it->weight;
                m_mst.push_back(*it);
                auto id_old = m_tree_id[to];
                auto id_new = m_tree_id[from];
                for(size_t i{0}; i < m_vertex_num; ++i)
                {
                    if(m_tree_id[i] == id_old)
                        m_tree_id[i] = id_new;
                }
            }
        }
        m_has_mst_changed = false;
    }
    return m_mst;
}

const std::vector<Edge> &Kruskals::find_mst2()
{
    if(m_has_mst_changed)
    {
        m_mst.clear();
        m_mst.reserve(m_edges.size());
        m_total_cost = 0;
        m_dsu_parent.clear();
        m_dsu_parent.resize(m_vertex_num);
        m_dsu_rank.clear();
        m_dsu_rank.resize(m_vertex_num);
        for(size_t i{0}; i < m_vertex_num; ++i)
            make_set(i);
        for(const auto &e: m_edges)
        {
            auto from = e.from;
            auto to = e.to;
            auto weight = e.weight;
            if(find_set(from) != find_set(to))
            {
                m_total_cost += weight;
                m_mst.push_back(e);
                unite_sets(from, to);
            }
        }
    }
    return m_mst;
}

void Kruskals::write_mst_to_file(const std::string &path_to_file)
{
    std::ofstream fo(path_to_file);
    if(fo.is_open())
    {
        for(const auto &e: m_mst)
            fo << "(" << e.from << "," << e.to << ") ";
        fo << std::endl << "Total cost: " << m_total_cost << std::endl;
        fo.close();
    }
}

void Kruskals::print_edges() const
{
    for(auto it = m_edges.begin(); it != m_edges.end(); ++it)
    {
        std::cout << "(" << it->from << "," << it->to << ",w=" <<
        it->weight << ")" << std::endl;
    }
}

void Kruskals::print_mst() const
{
    for(const auto &e: m_mst)
        std::cout << "(" << e.from << "," << e.to << ") ->";
    std::cout << std::endl << "Total cost: " << m_total_cost << std::endl;
}

void Kruskals::remove_edge_duplicates()
{
    std::sort(m_edges.begin(), m_edges.end());
    auto predicate = [](const auto &a, const auto &b){
           return a.from == b.from && b.to == b.to;
       };
    m_edges.erase(std::unique(m_edges.begin(), m_edges.end(), predicate),
                  m_edges.end());
}

void Kruskals::make_set(int v)
{
    m_dsu_parent[v] = v;
    m_dsu_rank[v] = 0;
}

int Kruskals::find_set(int v)
{
    return (v == m_dsu_parent[v]) ? v : m_dsu_parent[v] = find_set(m_dsu_parent[v]);
}

void Kruskals::unite_sets(int a, int b)
{
    a = find_set(a);
    b = find_set(b);
    if(a != b)
    {
        if(m_dsu_rank[a] < m_dsu_rank[b])
            std::swap(a,b);
        m_dsu_parent[b] = a;
        if(m_dsu_rank[a] == m_dsu_rank[b])
            ++m_dsu_rank[a];
    }
}
