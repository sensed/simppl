#include "simppl/interface.h"
#include "simppl/stub.h"


using simppl::dbus::in;
using simppl::dbus::out;


namespace a
{

namespace b
{

INTERFACE(Test)
{
   Request<in<int>> eval;
   
   Test()
    : INIT(eval)
   {
      // NOOP
   }
};

}

}


// ---------------------------------------------------------------------


int main()
{
   simppl::dbus::CallState cs(nullptr);
   
   // one out
   {
      typedef simppl::dbus::ClientRequest<in<int>, in<double>, out<std::string>> request_type;
      static_assert(std::is_same<request_type::return_type, std::string>::value, "hey, not same");
      static_assert(request_type::is_oneway == false, "shall NOT be oneway");
      
      request_type r("hallo", nullptr);
      std::string rc = r(42, 3.1415);
      r.f_(cs, "Hallo");
   }
   
   // one out, different order
   {
      typedef simppl::dbus::ClientRequest<in<int>, out<std::string>, in<double>> request_type;
      static_assert(std::is_same<request_type::return_type, std::string>::value, "hey, not same");
      
      request_type r("hallo", nullptr);
      std::string rc = r(42, 3.1415);
   }
   
   // no out at all
   {
      typedef simppl::dbus::ClientRequest<in<int>, in<double>, simppl::dbus::Oneway> request_type;
      static_assert(std::is_same<request_type::return_type, void>::value, "hey, not same");
      static_assert(request_type::is_oneway == true, "shall be oneway");
      
      request_type r("hallo", nullptr);
      r(42, 3.1415);
   }

   // multiple out
   {
      typedef simppl::dbus::ClientRequest<in<int>, in<double>, out<int>, out<std::string>> request_type;
      static_assert(std::is_same<request_type::return_type, std::tuple<int, std::string>>::value, "hey, not same");
      
      request_type r("hallo", nullptr);
      std::tuple<int, std::string> rc = r(42, 3.1415);
      r.f_(cs, 42, "Hallo");
   }
   
   // just one in 
   {
      typedef simppl::dbus::ClientRequest<in<int>> request_type;
      static_assert(std::is_same<request_type::return_type, void>::value, "hey, not same");
      
      request_type r("hallo", nullptr);
      r(42);
      r.f_(cs);
   }
   
   // no in 
   {
      typedef simppl::dbus::ClientRequest<out<int>> request_type;
      static_assert(std::is_same<request_type::return_type, int>::value, "hey, not same");
      
      request_type r("hallo", nullptr);
      int i = r();
      r.f_(cs, 42);
   }
   
   // no in, no out 
   {
      typedef simppl::dbus::ClientRequest<> request_type;
      static_assert(std::is_same<request_type::return_type, void>::value, "hey, not same");
      static_assert(request_type::is_oneway == false, "shall NOT be oneway");
      
      request_type r("hallo", nullptr);
      r();
      r.f_(cs);
   }

   // no in, no out 
   {
      typedef simppl::dbus::ClientRequest<simppl::dbus::Oneway> request_type;
      static_assert(std::is_same<request_type::return_type, void>::value, "hey, not same");
      static_assert(request_type::is_oneway == true, "shall be oneway");
      
      request_type r("hallo", nullptr);
      r();
      r.f_(cs);
   }
   
   // one in, one out 
   {
      typedef simppl::dbus::ClientRequest<in<int>, out<std::string>> request_type;
      static_assert(std::is_same<request_type::return_type, std::string>::value, "hey, not same");
      
      request_type r("hallo", nullptr);
      std::string rc = r(42);
      r.f_(cs, "Hallo");
      
      r.async(42);
   }

   // one in, one out 
   {
      typedef simppl::dbus::ClientRequest<out<std::string>, in<int>> request_type;
      static_assert(std::is_same<request_type::return_type, std::string>::value, "hey, not same");
      
      request_type r("hallo", nullptr);
      std::string rc = r(42);
      r.f_(cs, "Hallo");
   }
   
   std::tuple<> t;
 
   return 0;
}
