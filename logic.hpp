#ifndef LOGIC_HPP_DEFINED
#define LOGIC_HPP_DEFINED

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <unordered_set>

#include "warped.hpp"

#include <boost/logic/tribool.hpp>
#include <boost/logic/tribool_io.hpp>

WARPED_DEFINE_OBJECT_STATE_STRUCT(PortState) {
   boost::tribool exit;
};

class Port : public warped::SimulationObject {
   public:
      Port (const std::string& name)
         :   SimulationObject(name), 
         state_(),
         upstreams(),
         downstreams(),
         output(boost::indeterminate)
   {}
      Port (const std::string& name, boost::tribool value)
         :   SimulationObject(name), 
         state_(),
         upstreams(),
         downstreams(),
         output(value)
   {}

      virtual std::vector<std::shared_ptr<warped::Event> > createInitialEvents() = 0;
      virtual std::vector<std::shared_ptr<warped::Event> > receiveEvent(const warped::Event&) = 0;
      warped::ObjectState& getState() { return this->state_; }
      void addUpstream( std::pair<std::string, boost::tribool> value ) { upstreams.insert(value); }
      void addUpstream( std::string name, boost::tribool value ) { upstreams.emplace(name, value); }
      void addUpstream( std::string name ) { upstreams.emplace(name, boost::indeterminate); }
      void addDownStream( std::string name ) { downstreams.emplace(name); }
      boost::tribool getPortOutput() { return output; }

      PortState state_;

   protected:
      // inputs
      std::map<std::string, boost::tribool> upstreams;
      std::unordered_set<std::string> downstreams;
      // my output
      boost::tribool output;

};

class PortEvent : public warped::Event {
   public:
      PortEvent() = default;
      PortEvent(const std::string& receiver_name, boost::tribool signal, 
            const unsigned int timestamp)
         : receiver_name_(receiver_name),
         newinput_(signal),
         ts_(timestamp)
   {}

      const std::string& receiverName() const { return receiver_name_; }
      unsigned int timestamp() const { return ts_; }

      std::string receiver_name_;
      // the new value
      boost::tribool newinput_;
      unsigned int ts_;

      WARPED_REGISTER_SERIALIZABLE_MEMBERS(cereal::base_class<warped::Event>(this), receiver_name_, ts_)
};

class AndPort : public Port {
   public:
      AndPort (const std::string& name, boost::tribool value)
         :   Port(name, value)
   {}
      AndPort (const std::string& name)
         :   Port(name)
   {}

      virtual std::vector<std::shared_ptr<warped::Event> > createInitialEvents() override;
      virtual std::vector<std::shared_ptr<warped::Event> > receiveEvent(const warped::Event&) override;

      static inline std::string object_name(const unsigned int object_index) { return std::string("AndPort_") + std::to_string(object_index); }
};

class OrPort : public Port {
   public:
      OrPort (const std::string& name, boost::tribool value)
         :   Port(name, value)
   {}
      OrPort (const std::string& name)
         :   Port(name)
   {}

      virtual std::vector<std::shared_ptr<warped::Event> > createInitialEvents() override;
      virtual std::vector<std::shared_ptr<warped::Event> > receiveEvent(const warped::Event&) override;

      static inline std::string object_name(const unsigned int object_index) { return std::string("OrPort_") + std::to_string(object_index); }
};

class FakePort : public Port {
   public:
      FakePort (const std::string& name, boost::tribool value)
         :   Port(name, value)
   {}
      FakePort (const std::string& name)
         :   Port(name) 
   {}

      virtual std::vector<std::shared_ptr<warped::Event> > createInitialEvents() override;
      virtual std::vector<std::shared_ptr<warped::Event> > receiveEvent(const warped::Event&) override;

      static inline std::string object_name(const unsigned int object_index) { return std::string("FakePort_") + std::to_string(object_index); }
};

#endif
