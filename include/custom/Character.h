#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <cassert>
#include <cursesw.h>
#include <set>

class Battle;

enum class EffectType {
  NORMAL, FIRE, WATER, ICE, DRAGON, LIGHTING, GRASS, ROCK, POISON, SPEED, HOLY, GHOST, PSYCHIC, NONE
};

static const std::map< EffectType, EffectType > weaknesses {
  // Lefthand is weak against righthand side.
  { EffectType::NORMAL,   EffectType::NONE },
    { EffectType::FIRE,     EffectType::WATER },
    { EffectType::WATER,    EffectType::GRASS },
    { EffectType::ICE,      EffectType::FIRE },
    { EffectType::DRAGON,   EffectType::HOLY },
    { EffectType::LIGHTING, EffectType::ROCK },
    { EffectType::GRASS,    EffectType::FIRE },
};

enum class TargetAttribute {
  HP, MP, STRENGTH, DEFENSE, SPEED, TYPE
};

struct Agent {
  std::string name;
  std::string desc;
  int hp{};
  int mp{};
  int strength{};
  int defense{};
  int speed{};
  EffectType type{};

  // Natural stats + equipment stats
  // "+" omits multipliers because its context is equipping
  auto operator+( const Agent& rhs ) -> Agent {
    Agent out;
    out.hp = this->hp + rhs.hp;
    out.mp = this->mp + rhs.mp;
    out.strength = this->strength + rhs.strength;
    out.defense = this->defense + rhs.defense;
    out.speed = this->speed + rhs.speed;
    out.type = rhs.type;  // Equipment type strength overrides natural type strength.
    return out;
  }

  // "+=" omits changing type because its context is fighting
  auto operator+=( const Agent& rhs ) -> Agent {
    Agent out;
    int multiplier{1};
    if ( this->type == weaknesses.at(rhs.type) ) {
      multiplier *= 2;
    }
    else if ( this->type == rhs.type || ( rhs.type == EffectType::HOLY ) ) {
      multiplier *= -1;
    }
    hp = this->hp             + ( rhs.hp * multiplier );
    mp = this->mp             + ( rhs.mp * multiplier );
    strength = this->strength + ( rhs.strength * multiplier );
    defense = this->defense   + ( rhs.defense * multiplier );
    speed = this->speed       + ( rhs.speed * multiplier );
  }
};

struct Commodity : Agent {  // Magic is a commodity if you count MP as a currency.
  unsigned cost;
};

struct Spell : Commodity {
  unsigned successProbability;
};

struct Item : Commodity {
  unsigned qty;
  const unsigned vol;
  const unsigned mass;  // affects character's speed
};

enum class EquipType { HELMET, BODY, LEGS, SHIELD, WEAPON };

struct Equipment : Agent {
  const EquipType type;
};

struct EquipmentSet {
  Equipment helmet, armor, legs, shield, weapon;
};

struct Stats {
  unsigned level{};
  Agent base;
};

struct Character;
struct Action;

struct Bag {
  unsigned volCapacity{};
  unsigned massCapacity{};
  std::vector<Item> items{};
};

class Character : public std::enable_shared_from_this<Character>, Agent {
  public:
    Character();
    Character( const Agent& stats );
    std::string name;
    bool good{};
    Bag bag{};
    // TODO equipment
    std::shared_ptr<Action> action{};
    std::vector<Item> items{};
    std::set<Spell> spells{};
    // Functions
    void fight( std::shared_ptr<Battle>&& battle );
    void spell( std::shared_ptr<Battle>&& battle );
    void item( std::shared_ptr<Battle>&& battle );
};


struct Action {
  std::shared_ptr<Character> src;
  std::shared_ptr<Character> dst;
  std::shared_ptr<Agent> effect;

  bool operator<( const Action& rhs ) const {
    return src->stats.speed > rhs.src->stats.speed;  // ">" sorts in descending order
  }

  void operator=( const Action& rhs ) {
    src = rhs.src;
    dst = rhs.dst;
    effect = rhs.effect;
  }

  void execute() {
    dst->stats += effect;
  };
};

