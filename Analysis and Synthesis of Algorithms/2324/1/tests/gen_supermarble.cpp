#include <iostream>

using namespace std;

// Dimensions
int _N;
int _M;
int _P; // # pieces

//-----------------------------------------------------------------------------

void printUsage(char *progname) {
  cerr << "Usage: " << progname << " <N> <M> <P> <seed>" << endl;
  cerr << "  N: number of rows" << endl;
  cerr << "  M: number of columns" << endl;
  cerr << "  P: number of pieces (dimensions 1 to N x 1 to M)" << endl;
  cerr << "  seed: random seed number (optional)" << endl;
  exit(1);
}

void parseArgs(int argc, char *argv[]) {
  int seed = 0;

  if (argc < 3 || argc > 5) {
    cerr << "ERROR: Wrong number of arguments" << endl;
    printUsage(argv[0]);
  }

  sscanf(argv[1], "%d", &_N);
  if (_N < 1) {
    cerr << "ERROR: N rows must be >= 1" << endl;
    printUsage(argv[0]);
  }

  sscanf(argv[2], "%d", &_M);
  if (_M < 1) {
    cerr << "ERROR: M columns must be >= 1" << endl;
    printUsage(argv[0]);
  }

  sscanf(argv[3], "%d", &_P);
  if (_P < 1) {
    cerr << "ERROR: P pieces must be >= 1" << endl;
    printUsage(argv[0]);
  }

  if (argc == 5) {
    sscanf(argv[4], "%d", &seed);
    srand(seed);
  } else { // pseudo-random seed
    srand((unsigned int)time(NULL));
  }
}

inline int randomValue(int max) {
  return rand() % max; // [0, max - 1]
}

int main(int argc, char *argv[]) {
  parseArgs(argc, argv);

  // Print dimensions
  cout << _N << " " << _M << endl;
  // Print number of pieces
  cout << _P << endl;

  // Print matrix
  for (int i = 0; i < _P; i++) {
    int x = randomValue(_N) + 1;
    int y = randomValue(_M) + 1;
    int v = randomValue(2 * x * y) + 1;
    cout << x << " " << y << " " << v << endl;
  }

  return 0;
}
