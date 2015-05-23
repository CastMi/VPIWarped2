#include <vector>
#include <memory>

#include "warped.hpp"
#include "logic.hpp"
#include <boost/logic/tribool.hpp>
#include <boost/logic/tribool_io.hpp>

// default computation time of a port
#define MY_COMP_TIME 3

WARPED_REGISTER_POLYMORPHIC_SERIALIZABLE_CLASS(PortEvent)

std::vector<std::shared_ptr<warped::Event> > AndPort::createInitialEvents() {

      std::vector<std::shared_ptr<warped::Event> > events;
      
      /*
       * Print the inputs in order to understand whether the netlist is well defined.
       */
      std::cout << name_ << " has " << upstreams.size() << " input(s):" << std::endl;
      for (std::map<std::string, boost::tribool>::iterator it = upstreams.begin(); it != upstreams.end() ; it++)
         std::cout << it->first << " -> " << it->second << std::endl;
      
      /*
       * Determine the initial output value
       */
      boost::tribool exit(true);
      for (std::map<std::string, boost::tribool>::iterator it = upstreams.begin(); it != upstreams.end(); it++) {
         if(!it->second)
         {
            exit = false;
            break;
         }
         else if(boost::indeterminate(it->second))
         {
            exit = boost::indeterminate;
            continue;
         }
      }
      
      /*
       * Indeterminate means that the port is not able to set the initial output,
       * therefore we don't need to send useless messages.
       */
      if(boost::indeterminate(exit))
         return events;

      output = exit;
      for (std::unordered_set<std::string>::iterator it = downstreams.begin(); it != downstreams.end(); it++) {
         std::cout << name_ << " sends a message to " << *it << " with value " << output << std::endl;
         events.emplace_back(new PortEvent { *it, output, MY_COMP_TIME });
      }
      return events;
   }

std::vector<std::shared_ptr<warped::Event> > AndPort::receiveEvent(const warped::Event& event) {
   std::vector<std::shared_ptr<warped::Event> > response_events;
   auto received_event = static_cast<const PortEvent&>(event);
   
   std::cout << name_ << " received a message from " << received_event.sender_name_ << " with value " << received_event.newinput_ << std::endl;

   /*
    * Be sure that the sender is in my upstreams before setting the input.
    */
   if(upstreams.find(event.sender_name_) != upstreams.end())
      upstreams[event.sender_name_] = received_event.newinput_;
   else
   {
      return response_events;
   }

   boost::tribool exit(true);
   for (std::map<std::string, boost::tribool>::iterator it = upstreams.begin(); it != upstreams.end(); it++) {
      if(!it->second)
      {
         exit = false;
         break;
      }
      if(boost::indeterminate(it->second))
      {
         exit = boost::indeterminate;
         continue;
      }
   }

   output = exit;
   for (std::unordered_set<std::string>::iterator it = downstreams.begin(); it != downstreams.end() ; it++) {
      std::cout << name_ << " sends a message to " << *it << " with value " << output << std::endl;
      response_events.emplace_back(new PortEvent { *it, output, event.timestamp() + MY_COMP_TIME });
   }
   return response_events;
}

   std::vector<std::shared_ptr<warped::Event> > OrPort::createInitialEvents() {

      std::vector<std::shared_ptr<warped::Event> > events;
      /*
       * Print the inputs in order to understand if the netlist is well defined.
       */
      std::cout << name_ << " has " << upstreams.size() << " input(s):" << std::endl;
      for (std::map<std::string, boost::tribool>::iterator it = upstreams.begin(); it != upstreams.end() ; it++)
         std::cout << it->first << " -> " << it->second << std::endl;

      /*
       * Determine the initial output value
       */
      boost::tribool exit(false);
      for (std::map<std::string, boost::tribool>::iterator it = upstreams.begin(); it != upstreams.end() ; it++) {
         if(it->second)
         {
            exit = true;
            break;
         }
         if(boost::indeterminate(it->second))
         {
            exit = boost::indeterminate;
            continue;
         }
      }
      
      /*
       * Indeterminate means that the port is not able to set the initial output,
       * therefore we don't need to send useless messages.
       */
      if(boost::indeterminate(exit))
         return events;
      
      output = exit;
      for (std::unordered_set<std::string>::iterator it = downstreams.begin(); it != downstreams.end() ; it++) {
         std::cout << name_ << " sends a message to " << *it << " with value " << output << std::endl;
         events.emplace_back(new PortEvent { *it, output, MY_COMP_TIME });
      }
      return events;
   }

std::vector<std::shared_ptr<warped::Event> > OrPort::receiveEvent(const warped::Event& event) {
   std::vector<std::shared_ptr<warped::Event> > response_events;
   auto received_event = static_cast<const PortEvent&>(event);
   
   std::cout << name_ << " received a message from " << received_event.sender_name_ << " with value " << received_event.newinput_ << std::endl;
   
   /*
    * Be sure that the sender is in my upstreams before setting the input.
    */
   if(upstreams.find(event.sender_name_) != upstreams.end())
      upstreams[event.sender_name_] = received_event.newinput_;
   else
   {
      return response_events;
   }
  
   /*
    * Determine the output value
    */
   boost::tribool exit(false);
   for (std::map<std::string, boost::tribool>::iterator it = upstreams.begin(); it != upstreams.end(); it++) {
      if(it->second)
      {
         exit = true;
         break;
      }
      else if(boost::indeterminate(it->second))
      {
         exit = boost::indeterminate;
      }
   }
   
   output = exit;
   for (std::unordered_set<std::string>::iterator it = downstreams.begin(); it != downstreams.end() ; it++) {
      std::cout << name_ << " sends a message to " << *it << " with value " << output << std::endl;
      response_events.emplace_back(new PortEvent { *it, output, event.timestamp() + MY_COMP_TIME });
   }
   return response_events;
}

std::vector<std::shared_ptr<warped::Event> > FakePort::createInitialEvents() {

      std::vector<std::shared_ptr<warped::Event> > events;
      return events;
   }

std::vector<std::shared_ptr<warped::Event> > FakePort::receiveEvent(const warped::Event& event) {
   auto received_event = static_cast<const PortEvent&>(event);

   /*
    * This is a FakePort, It only prints the received messages.
    */
   std::cout << name_ << " received a message from " << received_event.sender_name_ << " with value " << received_event.newinput_ << std::endl;
   
   std::vector<std::shared_ptr<warped::Event> > response_events;
   return response_events;
}

