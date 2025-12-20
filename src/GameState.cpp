#include <yaml-cpp/yaml.h>
#include "GameState.h"
#include "Dice.h"
#include <iostream>

GameState::GameState() {}

auto GameState::getParty() -> Party& {
  return _party;
}

auto GameState::getArea() -> Area {
  return _area;
}

auto GameState::getMonsters() -> std::vector<Character> {
  return {};
}

void GameState::configure() {
  YAML::Node cfg = YAML::LoadFile( "./config/dummy.yml" ); // TODO make generalizable
  std::cout << cfg["title"];
}


