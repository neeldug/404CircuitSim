#include <vector>
#include <map>
#include <string>

class Node {
private:
    std::string name;
    float voltage;
    
    std::vector<Component *> comps;
};

class Component {
private:
    std::string name;
    float value;
    std::vector<Node *> nodes;
    
};

class Component::Circuit {
    
    std::map<std::string,Node *> nodes;
    
    std::map<std::string, Component *> comps;

    

};

namespace Component{
	
    class Capacitor;
    class Inductor;
    class Resistor;
    class Transistor;
    class Mosfet; 
}