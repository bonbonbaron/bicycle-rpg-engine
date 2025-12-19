#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <cassert>
#include <cursesw.h>
#include <bicycle/Constellation.h>

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

struct Effect {
  std::string name;  // TODO is this needed?
  int amount{};    // generic enough to affect any stat positively or negatively
  TargetAttribute tgtAttr{};  // defaults to HP
  EffectType type{};  // defaults to normal
};

struct Spell : Effect {
  unsigned cost;
};

struct Item : Effect {
  unsigned qty;
};

enum class EquipType { HELMET, BODY, LEGS, SHIELD, WEAPON };

struct Equipment : Effect {
  EquipType type;
};

struct EquipmentSet {
  Equipment helmet, armor, legs, shield, weapon;
};

struct Stats {
  int hp{};
  int mp{};
  int strength{};
  int defense{};
  int speed{};
  EffectType type{};
  unsigned level{};

  // Natural stats + equipment stats
  auto operator+( const Stats& rhs ) -> Stats {
    Stats out;
    out.hp = this->hp + rhs.hp;
    out.mp = this->mp + rhs.mp;
    out.strength = this->strength + rhs.strength;
    out.defense = this->defense + rhs.defense;
    out.speed = this->speed + rhs.speed;
    out.type = rhs.type;  // Equipment type strength overrides natural type strength.
    return out;
  }
};

struct Character;
struct Action;

class Character : public std::enable_shared_from_this<Character> {
  public:
    Character();
    Character( std::string name,
        const Stats& stats,
        const Stats& maxStats);
    std::string name;
    bool good{};
    Stats stats{};
    Stats maxStats{};
    std::shared_ptr<Action> action{};
    std::vector<Item> items{};
    std::vector<Spell> spells{};
    // Functions
    void fight( std::shared_ptr<Battle>&& battle );
    void spell( std::shared_ptr<Battle>&& battle );
    void item( std::shared_ptr<Battle>&& battle );
};


struct Action {
  std::shared_ptr<Character> src;
  std::shared_ptr<Character> dst;
  std::shared_ptr<Effect> effect;

  bool operator<( const Action& rhs ) const {
    return src->stats.speed > rhs.src->stats.speed;  // ">" sorts in descending order
  }

  void operator=( const Action& rhs ) {
    src = rhs.src;
    dst = rhs.dst;
    effect = rhs.effect;
  }

  void execute() {
    assert( effect != nullptr );
    switch ( effect->tgtAttr ) {
      case TargetAttribute::HP: 
        dst->stats.hp += effect->amount;
        dst->stats.hp = std::max<int>( dst->stats.hp, 0 );
        break;
      case TargetAttribute::MP: 
        dst->stats.mp += dst->stats.mp, effect->amount;
        dst->stats.mp = std::max<int>( dst->stats.mp, 0 );
        break;
      case TargetAttribute::STRENGTH: 
        dst->stats.strength += dst->stats.strength, effect->amount;
        dst->stats.strength = std::max<int>( dst->stats.strength, 0 );
        break;
      case TargetAttribute::DEFENSE: 
        dst->stats.defense += dst->stats.defense, effect->amount;
        dst->stats.defense = std::max<int>( dst->stats.defense, 0 );
        break;
      case TargetAttribute::SPEED: 
        dst->stats.speed += dst->stats.speed, effect->amount;
        dst->stats.speed = std::max<int>( dst->stats.speed, 0 );
        break;
      case TargetAttribute::TYPE:
        dst->stats.type = effect->type;
        break;
    }
  };
};

