#include <vector>
#include <memory>

#include "warped.hpp"
#include "logic.hpp"
#include <boost/logic/tribool_io.hpp>

#include "tclap/ValueArg.h"

int main(int argc, const char** argv) {

   /*
    * TCLAP is a library to parse command line arguments.
    * Similar to getopt for C.
    */
   std::vector<TCLAP::Arg*> args;
   warped::Simulation airport_sim {"Circuit Simulation", argc, argv, args};

   std::vector<AndPort> andportstuff;
   std::vector<OrPort> orportstuff;
   std::vector<FakePort> fakeportstuff;

   // create the And ports
   for (unsigned int i = 0; i < 1; i++) {
      andportstuff.emplace_back(AndPort::object_name(i));
   }
   // create the Or ports
   for (unsigned int i = 0; i < 3; i++) {
      orportstuff.emplace_back(OrPort::object_name(i));
   }
   // create the Fake ports
   for (unsigned int i = 0; i < 2; i++) {
      fakeportstuff.emplace_back(FakePort::object_name(i));
   }


   // create the netlist
   // Start first layer
   // first or
   orportstuff[0].addUpstream("input1", true);
   orportstuff[0].addUpstream("input2", false);
   orportstuff[0].addDownStream(orportstuff[2].name_);
   orportstuff[0].addDownStream(andportstuff[0].name_);
   // second or
   orportstuff[1].addUpstream("input3", false);
   orportstuff[1].addUpstream("input4", false);
   orportstuff[1].addDownStream(orportstuff[2].name_);
   orportstuff[1].addDownStream(andportstuff[0].name_);
   // end first layer && start second layer
   // last or
   orportstuff[2].addUpstream(orportstuff[0].name_);
   orportstuff[2].addUpstream(orportstuff[1].name_);
   orportstuff[2].addDownStream(fakeportstuff[0].name_);
   // the and
   andportstuff[0].addUpstream(orportstuff[0].name_);
   andportstuff[0].addUpstream(orportstuff[1].name_);
   andportstuff[0].addDownStream(fakeportstuff[1].name_);

   // Create a vector to pass to the simulator
   std::vector<warped::SimulationObject*> object_pointers;
   for (auto& o : andportstuff) {
      object_pointers.push_back(&o);
   }
   for (auto& o : orportstuff) {
      object_pointers.push_back(&o);
   }
   for (auto& o : fakeportstuff) {
      object_pointers.push_back(&o);
   }
   airport_sim.simulate(object_pointers);

   /*
    * Results
    */
   std::cout << "The output of " << orportstuff[0].name_ << " shuld be 1. The real value is: " << orportstuff[0].getPortOutput() << std::endl;
   std::cout << "The output of " << orportstuff[1].name_ << " shuld be 0. The real value is: " << orportstuff[1].getPortOutput() << std::endl;
   std::cout << "The output of " << orportstuff[2].name_ << " shuld be 1. The real value is: " << orportstuff[2].getPortOutput() << std::endl;
   std::cout << "The output of " << andportstuff[0].name_ << " shuld be 0. The real value is: " << andportstuff[2].getPortOutput() << std::endl;
   return 0;
}

