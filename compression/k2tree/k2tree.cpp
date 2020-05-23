#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

typedef vector<vector<int>> vvi;
typedef vector<vector<bool>> vvb;

int log_k(int x, int k) {
  int temp = 1;
  int res = 0;
  while (temp < x) {
    res++;
    temp *= k;
  }
  return res;
}

int pow(int base, int e) {
  int res = 1;
  for (int i = 0; i < e; i++) {
    res *= base;
  }
  return res;
}

bool isAllZero(vector<bool> C) {
  for (auto e : C) {
    if (e) return false;
  }
  return true;
}

class K2tree {
 public:
  int nodes = 0;
  int edges = 0;
  int k = 2;
  int h = 0;

  int build(vvi adjacency_list, vector<vector<int>::iterator>& cursors,
            vvb& levels, int n, int l, int p, int q) {
    // n: nodes
    // l: level
    // p: row index
    // q: column index
    cout << n << " " << l << " " << p << " " << q << endl;
    vector<bool> C;
    auto element = false;
    if (l == this->h) {  // last level
      for (int i = 0; i < this->k; i++) {
        for (int j = 0; j < this->k; j++) {
          element = (p + i) < nodes &&
                    cursors[p + i] != adjacency_list[p + i].end() &&
                    (q + j) == (*cursors[p + i]);
          C.push_back(element);
          if (element) {
            cursors[p + i]++;
          }
        }
      }
    } else {
      for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
          C.push_back(build(adjacency_list, cursors, levels, n / this->k, l + 1,
                            p + i * (n / k), q + j * (n / k)));
        }
      }
    }
    if (isAllZero(C)) {
      return 0;
    } else {
      levels[l - 1].insert(levels[l - 1].end(), C.begin(), C.end());
      return 1;
    }
  }

  K2tree(vvi adjacency_list, int k) {
    vector<vector<int>::iterator> cursors;
    auto max_col = 0;
    edges = 0;
    for (auto v : adjacency_list) {
      cursors.push_back(v.begin());

      edges += v.size();
      for (auto e : v) {
        max_col = max(max_col, e);
      }
    }

    this->nodes = adjacency_list.size();

    this->k = k;
    this->h = max(1, log_k(max(nodes, max_col + 1), this->k));
    vvb levels(this->h);

    cout << "h: " << (this->h) << endl;

    build(adjacency_list, cursors, levels, pow(this->k, this->h), 1, 0, 0);
  }
};

vvi load_dataset(string dataset_path) {
  auto fp = fopen(dataset_path.c_str(), "r");

  int nodes = 0;
  long long edges = 0;
  fread(&nodes, sizeof(int), 1, fp);
  fread(&edges, sizeof(long long), 1, fp);

  cout << "Nodes: " << nodes << " Edges:" << edges << endl;
  int cur_node = 0;

  int temp = 0;

  vvi graph(nodes + 1);
  int cnt = 0;
  while (fread(&temp, sizeof(int), 1, fp)) {
    if (temp < 0) {
      cur_node = -temp;
    } else {
      graph[cur_node].push_back(temp);
    }
    cnt++;
  }
  cout << "read " << cnt << " numbers" << endl;
  fclose(fp);
  return graph;
}

int main(int argc, char const* argv[]) {

  string file = "../../webgraph_datasets/cnr-2000";
  auto adjacency_list = load_dataset(file);

  K2tree(adjacency_list, 2);

  return 0;
}
