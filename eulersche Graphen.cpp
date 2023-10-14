//
//  safe space
//  eulerkreis
//
//  Created by Juri Kaganskiy on 13.10.23.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <tuple>

using namespace std;

class Graph {
  public:
    int vertex_count;
    vector<vector<tuple<int, int> > > neighbor_lists;
    
    Graph(string filename){
      ifstream filestream(filename);
      filestream >> vertex_count;
      for(int i = 0; i < vertex_count; i++){
        vector<tuple<int, int> > neighbours;
        neighbor_lists.push_back(neighbours);
      }
      int start, end;
    
      while (filestream >> start >> end) {
        tuple<int, int> a {end, neighbor_lists[end].size()};
        tuple<int, int> b {start, neighbor_lists[start].size()};
        neighbor_lists[start].push_back(a);
        neighbor_lists[end].push_back(b);
      }
         
    }
};

vector<int> find_eulerzyklus(vector<vector<tuple<int, int> > > &nachbarliste, int starting_point) {
  vector<int> cycle;
  if (nachbarliste[starting_point].size() == 0) {
    vector<int> v {starting_point};
    return v;
  }
  cycle.push_back(starting_point);
  int current_vertex = starting_point;
  do {
    int a, b;
    tie(a, b) = nachbarliste[current_vertex].back();
    nachbarliste[current_vertex].pop_back();

    nachbarliste[a][b] = nachbarliste[a].back();

    get<1>(nachbarliste[get<0>(nachbarliste[a][b])][get<1>(nachbarliste[a][b])]) = b;
    current_vertex = a;
    nachbarliste[current_vertex].pop_back();
    cycle.push_back(current_vertex);
  } while (current_vertex != starting_point);
  vector<int> actual_cycle {starting_point};
  for (int i=1; i < cycle.size(); i++) {
    vector<int> smaller_cycle = find_eulerzyklus(nachbarliste, cycle[i]);
    for (int j=0; j<smaller_cycle.size(); j++) {
      actual_cycle.push_back(smaller_cycle[j]);
    }
  }
  return actual_cycle;
}

int main() {
  Graph g("ex1.txt");
  bool ist_eulersch = true;
  int number_of_edges = 0;
  for (int i=0; i<g.neighbor_lists.size(); i++){
    if (g.neighbor_lists[i].size()%2 != 0) {
      ist_eulersch = false;
    }
    number_of_edges = number_of_edges + (int)g.neighbor_lists[i].size();
  }
  number_of_edges = (int)(number_of_edges/2);
  if (!ist_eulersch) {
    cout << "nicht eulersch" << endl;
  } else {
    vector<int> EulerZyklus = find_eulerzyklus(g.neighbor_lists, 0);
    if (number_of_edges == EulerZyklus.size()-1) {
      cout << "Es gibt einen Euler-Zyklus" << endl;
      for (int i=0; i<EulerZyklus.size(); i++) {
        cout << EulerZyklus[i] << " ";
      }
    }
  }
  return 0;
}
