#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <set>

using namespace std;

typedef int NodeId;

int main(int argc, char *argv[]) {
  srand((unsigned int)time(nullptr));
  if (argc < 3)
    exit(1);
  int n = atoi(argv[1]);
  int max_degree = atoi(argv[2]);
  vector<NodeId> vertices((size_t)n);
  vector<NodeId> edges;
  for (NodeId u = 0; u < n; ++u) {
    vertices[u] = (NodeId)edges.size();
    set<NodeId> used;
    int t = rand() % (max_degree + 1);
    for (int i = 0; i < t; ++i) {
      NodeId v;
      do {
        v = rand() % n;
      } while (v == u || used.find(v) != used.end());
      used.insert(v);
    }
    edges.insert(edges.end(), used.begin(), used.end());
  }
  printf("%u %u\n", n, (NodeId)edges.size());
  for (NodeId u : vertices)
    printf(" %u", u);
  puts("");
  for (NodeId u : edges)
    printf(" %u", u);
  puts("");
  return 0;
}