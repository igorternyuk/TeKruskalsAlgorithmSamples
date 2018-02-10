#include <iostream>
#include "kruskals.hpp"

std::set<std::pair<int, std::pair<int,int>>> edges;

int main()
{
    Kruskals kr;
    kr.read_edges_set_from_file("input.txt");
    kr.print_edges();
    kr.find_mst2();
    kr.print_mst();
    kr.write_mst_to_file("output2.txt");
    return 0;
}
