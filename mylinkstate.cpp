#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>
#include <iomanip>
#include <algorithm>
#include <chrono>
#define INF INT_MAX
#define MAX 1073741824
using namespace std;


// Implementation of priority queue
class PriorityQueue{

private:
  vector<pair<int, int>> arr;

  void heapUp(int index) {
    int p = (index - 1) / 2;
    if (p <= 0){
      return;
    }
    if (arr[index].first < arr[p].second){
      swap (arr[index], arr[p]);
      heapUp(p);
    }
  }

  void heapDown(int index) {
    if (index >= arr.size()){
      return;
    }
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int minI = index;
    if (left < arr.size() && arr[left].second < arr[minI].second){
      minI = left;
    }
    if (right < arr.size() && arr[right].second < arr[minI].second){
      minI = right;
    }
    if (minI != index){
      swap (arr[minI], arr[index]);
      heapDown(minI);
    }
  }

public:
PriorityQueue(){}

void push(pair<int, int> p){
  arr.push_back(p);
  heapUp(arr.size() - 1);
}

pair<int, int> pop() {
  pair<int, int> p = arr[0];
  arr[0] = arr[arr.size() - 1];
  arr.pop_back();
  heapDown(0);
  return p;
}

void updateKey(pair<int, int> p) {
  int index = -1;
  for (int i = 0; i < arr.size(); ++i){
    if (arr[i].first == p.first){
      index = i;
      break;
    }
  }
  
  if (index == -1){
    return;
  }
  if (p.second < arr[index].second){
    arr[index].second = p.second;
    heapUp(index);
  }
  else if (p.second > arr[index].second){
    arr[index].second = p.second;
    heapDown(index);
  }
}

bool empty(){
  return (arr.size() == 0);
}

};


// convert vector to string
string vect_str(vector<int>& vect){
  stringstream txt;
  txt << "{";
  int N = vect.size();
  for (int i = 0; i < N; ++i){
    if (i != N - 1){
      txt << vect[i] + 1 << ",";
    }
    else{
      txt << vect[i] + 1;
    }
  }
  txt << "}";
  return txt.str();
}

// prints a separator to format output
void sep(int x){
  for (int i = 0; i < x; ++i) cout << "-";
  cout << endl;
}

// driver function
int main(int argc, char* argv[]){
  if (argc < 3 || argc > 4){
    cout << "Error: Invalid number of arguments" << endl;
    exit (EXIT_FAILURE);
  }
  char* filename = argv[1];
  int sourceNode = atoi(argv[2]) - 1;
  bool printImmediate = false;
  
  if (argc == 4){
    printImmediate = (atoi(argv[3]) == 1);
  }

  // read the input file
  fstream inFile(filename, ios::in);
  if (!inFile.is_open()){
    cout << "Failed to open " << filename << endl;
    exit (EXIT_FAILURE);
  }

  // start runtime
  auto start = chrono::steady_clock::now();
  
  // read the number of vertices
  int N, source, dest, weight;
  inFile >> N;

  // store the network
  vector<vector<pair<int, int>>> adj;
  
  for (int i = 0; i < N; ++i){
    vector<pair<int, int>> vect;
    adj.push_back(vect);
  }

  // read the file
  while (!inFile.eof()){
    inFile >> source >> dest >> weight;
    
    // check if there is a link
    if (weight < MAX){
      adj[source - 1].push_back(make_pair(dest - 1, weight));
    }
  }
        
  // close file
  inFile.close();
  
  // display the information
  int dist[N];
  int parent[N];
  for (int i = 0; i < N; ++i){
    dist[i] = INF;
    parent[i] = -1;
  }
  
  // left align the content
  cout << left;
  
  // compute the information
  vector<int> popped;
  int step = 0;
  
  PriorityQueue pq;
  pq.push(make_pair(sourceNode, 0));
  dist[sourceNode] = 0;
  parent[sourceNode] = 1; 

  // print table header
  if (printImmediate){
    sep(10 + 50 + (N - 1) * 10);
    cout << setw(10) << "Step" << setw(50) << "N'";
    for (int i = 0; i < N; ++i){
      if (i != sourceNode){
        cout << setw(10) << (i + 1);
      }
    }
    cout << endl;
    sep(10 + 50 + (N - 1) * 10);
  }

  // Dijkstra's algorithm
  while (!pq.empty()){
    pair<int, int> item = pq.pop();
    popped.push_back(item.first);
    
    int u = item.first;
    int d = item.second;
    
    vector<pair<int, int>>&vect = adj[u];
    for (pair<int, int>& p : vect){
      int v = p.first;
      int w = p.second;
      
      if (dist[v] == INF){
        dist[v] = dist[u] + w;
        parent[v] = u;
        pq.push(make_pair(v, dist[v]));
      }
      else if (dist[u] + w < dist[v]){
        dist[v] = dist[u] + w;
        parent[v] = u;
        pq.updateKey(make_pair(v, dist[v]));
      }
    }
    
    // print
    if (printImmediate){
      cout << setw(10) << step++ << setw(50) << vect_str(popped);
      
      for (int i = 0; i < N; ++i){
        if (i != sourceNode){
          if (find(popped.begin(), popped.end(), i) != popped.end()) {
            cout << setw(10) << "";
          } 
          else{
            cout << setw(10) << ( (dist[i] != INF ? to_string(dist[i]) : "INF") + "," + to_string(parent[i] + 1));
          }
        }
      }
      cout << endl;
    }
  }
    // print immediate
  if (printImmediate){
    cout << endl << endl;
  }
    
  cout << "FORWARDING TABLE:" << endl;
  sep(40);
    
  cout << setw(20) << "Destination" << " | " << setw(20) << "Link" << endl;
  sep(40);
  for (int i = 0; i < N; ++i){
    if (i != sourceNode){
      if (parent[i] == sourceNode){
        cout << setw(20) << (i + 1) << " | " << setw(20) << ("(" + to_string(sourceNode + 1) + "," + to_string(parent[parent[i]]) + ")") << endl;
      } 
      else{
        cout << setw(20) << (i + 1) << " | " << setw(20) << ("(" + to_string(sourceNode + 1) + "," + to_string(parent[i]) + ")") << endl;
      }
    }
  }
  // get the time after processing
  auto end = chrono::steady_clock::now();
    
  // compute the difference
  cout << "\nExecution time: " << chrono::duration <double, milli> (end - start).count() << "ms" << endl;
  return 0;
}