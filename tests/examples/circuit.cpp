#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Ports

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>
#include <logic.cpp>

struct AndPortFixture{
      AndPort theandport;
  
      AndPortFixture() :
         theandport(AndPort::object_name(0))
      {
         BOOST_TEST_MESSAGE( "AndPort is tested with 2 inputs" );
      }
      ~AndPortFixture() {}
};

struct OrPortFixture{
      OrPort theorport;
  
      OrPortFixture() :
         theorport(OrPort::object_name(0))
      {
         BOOST_TEST_MESSAGE( "OrPort is tested with 2 inputs" );
      }
      ~OrPortFixture() {}
};

BOOST_FIXTURE_TEST_SUITE( PortTestSuite, AndPortFixture )

typedef boost::mpl::list< AndPort, OrPort > portTypes;

BOOST_AUTO_TEST_CASE_TEMPLATE( PortTest, T, portTypes )
{
   T myPort(T::object_name(0));
   BOOST_CHECK( boost::indeterminate(myPort.getPortOutput()) );
   BOOST_CHECK( myPort.getOutputSize() == 0 );
   BOOST_CHECK( myPort.getInputSize() == 0 );
   myPort.addUpstream( "input1", true );
   myPort.addUpstream( "input2", false );
   BOOST_CHECK( myPort.getInputSize() == 2 );
   BOOST_CHECK( myPort.getOutputSize() == 0 );
   BOOST_CHECK( boost::indeterminate(myPort.getPortOutput()) );
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE( AndPortTestSuite, AndPortFixture )

BOOST_AUTO_TEST_CASE( andPortFalseTest1 )
{
   theandport.addUpstream("input1", true);
   theandport.addUpstream("input2", false);
   theandport.createInitialEvents();
   BOOST_CHECK( theandport.getPortOutput() == false );
}

BOOST_AUTO_TEST_CASE( andPortFalseTest2 )
{
   theandport.addUpstream("input1", false);
   theandport.addUpstream("input2", false);
   theandport.createInitialEvents();
   BOOST_CHECK( theandport.getPortOutput() == false );
}

BOOST_AUTO_TEST_CASE( andPortFalseTest3 )
{
   theandport.addUpstream("input1", false);
   theandport.addUpstream("input2", boost::indeterminate);
   theandport.createInitialEvents();
   BOOST_CHECK( theandport.getPortOutput() == false );
}

BOOST_AUTO_TEST_CASE( andPortIndeterminateTest1 )
{
   theandport.addUpstream("input1", true);
   theandport.addUpstream("input2", boost::indeterminate);
   theandport.createInitialEvents();
   BOOST_CHECK( boost::indeterminate(theandport.getPortOutput()) );
}

BOOST_AUTO_TEST_CASE( andPortIndeterminateTest2 )
{
   theandport.addUpstream("input1");
   theandport.addUpstream("input2", boost::indeterminate);
   theandport.createInitialEvents();
   BOOST_CHECK( boost::indeterminate(theandport.getPortOutput()) );
}

BOOST_AUTO_TEST_CASE( andPortTrueTest )
{
   theandport.addUpstream("input1", true);
   theandport.addUpstream("input2", true);
   theandport.createInitialEvents();
   BOOST_CHECK( theandport.getPortOutput() == true );
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE( OrPortTestSuite, OrPortFixture )

BOOST_AUTO_TEST_CASE( orPortFalseTest )
{
   theorport.addUpstream("input1", false);
   theorport.addUpstream("input2", false);
   theorport.createInitialEvents();
   BOOST_CHECK( theorport.getPortOutput() == false );
}

BOOST_AUTO_TEST_CASE( orPortIndeterminateTest1 )
{
   theorport.addUpstream("input1");
   theorport.addUpstream("input2", false);
   theorport.createInitialEvents();
   BOOST_CHECK( boost::indeterminate(theorport.getPortOutput()) );
}

BOOST_AUTO_TEST_CASE( orPortIndeterminateTest2 )
{
   theorport.addUpstream("input1");
   theorport.addUpstream("input2", boost::indeterminate);
   theorport.createInitialEvents();
   BOOST_CHECK( boost::indeterminate(theorport.getPortOutput()) );
}

BOOST_AUTO_TEST_CASE( orPortTrueTest1 )
{
   theorport.addUpstream("input1", true);
   theorport.addUpstream("input2", false);
   theorport.createInitialEvents();
   BOOST_CHECK( theorport.getPortOutput() == true );
}

BOOST_AUTO_TEST_CASE( orPortTrueTest2 )
{
   theorport.addUpstream("input1", true);
   theorport.addUpstream("input2", boost::indeterminate);
   theorport.createInitialEvents();
   BOOST_CHECK( theorport.getPortOutput() == true );
}

BOOST_AUTO_TEST_CASE( orPortTrueTest3 )
{
   theorport.addUpstream("input1", true);
   theorport.addUpstream("input2", true);
   theorport.createInitialEvents();
   BOOST_CHECK( theorport.getPortOutput() == true );
}

BOOST_AUTO_TEST_SUITE_END()
