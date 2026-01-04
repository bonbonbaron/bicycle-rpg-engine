#include <algorithm>
#include <iostream>
#include <vector>
#include <execution>
#include <string>

struct Guy {
  std::string name;
  int hp;
};

int main() {
  std::vector<int> v{ {
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
    1, 2, 3, 4, 5,
  } };
  std::vector<Guy> g { {
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
    { "Bill", 45 },
  }};
  std::for_each( std::execution::par, v.begin(), v.end(), []( auto item ){ std::cout << item * 5 << "\n"; } );
  std::for_each( std::execution::par, g.begin(), g.end(), []( Guy& item ){ std::cout << item.name << "'s HP is " << item.hp << "\n"; } );
  return 0;
}
