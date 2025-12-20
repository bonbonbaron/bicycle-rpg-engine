#include "Config.h"
#include "GameState.h"

Config::Config () : _root( CONFIG_FILEPATH.data() ) {
  auto& gs = GameState::getInstance();
  // TODO read stuff hither

}
