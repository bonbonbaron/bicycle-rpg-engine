#include <bicycle/bicycle.h>
#include <cursesw.h>
#include "Battle.h"

using namespace std;
int main() {
  bicycle::init_ncurses();
  bicycle::push<BG::Battle>();
  return bicycle::run();
}
