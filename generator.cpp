#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <set>

using namespace std;

int main(int argc, char *argv[]) {
  srand((unsigned int)time(nullptr));
  if (argc < 3)
    exit(1);
  long n = atol(argv[1]);
  int max_degree = atoi(argv[2]);
  vector<long> vertices(n);
  vector<long> edges;
  for (long u = 0; u < vertices.size(); ++u) {
    vertices[u] = edges.size();
    set<long> used;
    int t = rand() % (max_degree + 1);
    for (int i = 0; i < t; ++i) {
      long v;
      do {
        v = rand() % n;
      } while (v == u || used.find(v) != used.end());
      used.insert(v);
    }
    edges.insert(edges.end(), used.begin(), used.end());
  }
  printf("%ld %ld\n", n, edges.size());
  for (long u : vertices)
    printf(" %ld", u);
  puts("");
  for (long u : edges)
    printf(" %ld", u);
  puts("");
  return 0;
}