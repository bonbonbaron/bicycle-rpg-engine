#include <map>
#include <iostream>

using namespace std;
int main() {
  map<int, int> m { { 1, 2 } };
  if ( m.contains( 1 ) ) {
    cout << "it has it\n";
  }
  else {
    cout << "nope\n";
  }
  return 0;
}
