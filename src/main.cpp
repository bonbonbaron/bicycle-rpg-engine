#include <iostream>
#include <bicycle/Graph.h>
#include <bicycle/ConditionRegistry.h>
#include <string_view>
#include <memory>

using namespace std;
using enum ActionState;

struct Stats {
  int age;
  int num;
  std::string buddy;
};

F_( sayHi, std::cout << "tree1: Hi there!\n"; return SUCCESS; );
F_( sayHo, std::cout << "tree2: Ho there!\n"; return FAILED; );
//F_( sayHe, std::cout << "He there!\n"; arg.get<int>("hello") = 45; return SUCCESS; );
F_( sayHe, std::cout << "tree2: He there!\n"; return SUCCESS; );
F_( chooseTgt, 
    std::cout << "both trees: Thinking...\n"; 
    std::cout << " both trees: Choosing my target\n"; 
    return FAILED; );
F_( eatTgt, std::cout << "both trees: Eating my target\n"; return SUCCESS; );

static void registerPortTypes() {
  PORT( age, int );
  PORT( hi, Stats );
  PORT( hello, int );
  PORT( there, int );
  PORT( time, int );
}

// TODO automate this, perhaps replacing F_ macro with template
static void registerActions() {
  ACT( sayHi, "hello", "there" );
  ACT( sayHo );
  ACT( sayHe );
  ACT( chooseTgt );
  ACT( eatTgt );
}

static auto imnotgay ()-> bool {
 cout << "I SWEAR TO GAWD\n"; 
 return true; 
}

// TODO automate this, perhaps replacing F_ macro with template
static void registerConditions() {
  COND( imnotgay );
}

// Now to figure out how to get references to talk directly through.
int main() {
  registerPortTypes();
  registerActions();
  registerConditions();
  std::string_view fp{ "./config/node/n1.yml" };
  auto root = YAML::LoadFile( fp.data() );
  auto n = root.as<bicycle::Node>();
  n.run();
  return 0;
}
