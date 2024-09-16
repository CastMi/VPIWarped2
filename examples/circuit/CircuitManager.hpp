#include "warped.hpp"
#include "logic.hpp"
#include <array>
#include <utility>
#include <boost/logic/tribool.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

struct myVertex {
   std::string name;
};

struct MyEdge {
   boost::tribool value;
};

typedef boost::adjacency_list<boost::setS, boost::vecS,
            boost::bidirectionalS, Port, MyEdge> Simulatedgraph;
typedef boost::graph_traits<Simulatedgraph>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<Simulatedgraph>::edge_descriptor edge_descriptor;
typedef boost::graph_traits<Simulatedgraph>::edge_iterator edge_iterator;
typedef boost::graph_traits<Simulatedgraph>::in_edge_iterator in_edge_iterator;
typedef boost::graph_traits<Simulatedgraph>::out_edge_iterator out_edge_iterator;

class Manager {

   Manager() : circuit() {}
   
   public:
      void addWire(); 
      void addComponent();

   private:
      Simulatedgraph circuit;
};

