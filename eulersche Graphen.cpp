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

    /* Die Knoten sind lediglich Zahlen von 0 bis n-1,
     * brauchen also keinen eigenen Datentyp. Wir spei-
     * chern die Kanten in folgender Weise:
     * Für jeden Knoten v gibt es eine Liste von Paaren.
     * Die erste Zahl aus dem Paar gibt einen Nachbarn w
     * von v an. Es steht auch v in der Liste von w. Hier
     * kommt die zweite Zahl im Paar ins Spiel:
     * In der Liste von v gibt die Zahl, die im Paar mit
     * w steht, an, an welcher Stelle in der Liste von w
     * das Tupel mit der Zahl v steht. Genauso gibt die
     * Zahl, die mit v im Paar in der Liste von w steht
     * an, an welcher Stelle w in der Liste von v steht. */
  
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
    //Get new vertex and its position by getting the last neighbour of the current vertex
    int new_vertex, new_vertex_pos;
    tie(new_vertex, new_vertex_pos) = nachbarliste[current_vertex].back();
    nachbarliste[current_vertex].pop_back(); //Remove neighbour from the current vertex's neighbour list
    current_vertex = new_vertex; 

    //Remove neighbour from the new vertex's neighbour list and replace it by the pair in the back of this list
    nachbarliste[new_vertex][new_vertex_pos] = nachbarliste[new_vertex].back(); 
    // In the partner of the pair we just moved, update the position part.
    get<1>(nachbarliste[get<0>(nachbarliste[new_vertex][new_vertex_pos])]
                       [get<1>(nachbarliste[new_vertex][new_vertex_pos])]) = new_vertex_pos;
    //Remove the element in the back of the neighbour list (it was just moved to another place)
    nachbarliste[new_vertex].pop_back();
    cycle.push_back(current_vertex);
  } while (current_vertex != starting_point);
  vector<int> actual_cycle {starting_point};
  for (int i=1; i < cycle.size(); i++) {
    vector<int> smaller_cycle = find_eulerzyklus(nachbarliste, cycle[i]);
    for (int j=0; j < smaller_cycle.size(); j++) {
      actual_cycle.push_back(smaller_cycle[j]);
    }
  }
  return actual_cycle;
}

int main(int argc, char *argv[]) {
  if (argc < 2){
    cout << "Please call this program with a filename as its argument, e.g. \'./a.out Test_Instanzen_Eulertour/inst_1.\'" << endl;
    return 0;
  }
  string filename = argv[1];
  Graph g(filename);

  //Check if the given Graph has a euler circuit
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
    } else {
      cout <<"nicht eulersch"; // Nicht zusammenhängend
    }
    cout << endl;
  }
  return 0;
}
