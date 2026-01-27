#include <bicycle/Grid.h>
#include <bicycle/bicycle.h>

int main() {
  bicycle::init();
  bicycle::push<Grid>( 1,2, 30, 14 );
  return bicycle::run();
}
