#include <kizbox/framework/core/Poller.h>
#include <kizbox/framework/core/Shared.h>
#include <kizbox/framework/core/Event.h>
#include <kizbox/framework/core/Timer.h>
#include <kizbox/framework/core/Time.h>
#include <kizbox/framework/core/Pipe.h>
#include <kizbox/framework/core/Shared.h>
#include <kizbox/framework/core/Log.h>
#include <kizbox/framework/bus/Object.h>
#include <kizbox/framework/bus/Connection.h>
#include <kizbox/framework/bus/Type.h>


class Foo: public Overkiz::Event
{
public:
  Foo()
  {
  }

  virtual ~Foo()
  {
  }

private:
  void receive(uint64_t numberOfEvents)
  {
    printf("Event received ! \n");
  }

};

class Bar: public Overkiz::Timer::Monotonic
{
public:
  Bar() :
    _myFoo()
  {
    setStackSize(4 * getpagesize());
  }

  virtual ~Bar()
  {
  }

private:
  void expired(const Overkiz::Time::Monotonic& time)
  {
    OVK_NOTICE("Expired !");
    _myFoo.send();
    exit(EXIT_SUCCESS);
  }

  Foo _myFoo;
};

class Server: public Overkiz::Bus::Object::Server
{
public:
  Server(const std::string& path) :
    Overkiz::Bus::Object::Server(path)
  {
    Overkiz::Bus::Connection::get()->bind("com.overkiz.Application.Hellow.Server");
  }

  bool method(const Overkiz::Bus::Message::Method::Call& call)
  {
    std::string message = "";
    OVK_NOTICE("Method Call received !");
    Overkiz::Bus::Type::Wrapper< std::string > stringWrapper;

    if(strcmp(call.getInterface(),"com.overkiz.Application.Hellow.Server")==0)
    {
      char interface[] = "com.overkiz.Application.Hellow.Proxy";
      char path[] = "/com/overkiz/Application/Hellow/Proxy";
      OVK_NOTICE("Interface checked !");
      std::string ret = "Signal message !";
      Overkiz::Bus::Message::Signal signal = Overkiz::Bus::Message::Signal();
      signal.setPath(path);
      signal.setInterface(interface);
      signal.setMember("Member");
      Overkiz::Bus::Message::Iterator methodIt = signal.begin();
      stringWrapper.serialize(methodIt, ret);
      Overkiz::Bus::Object::Server::send(signal);

      if((strcmp(call.getMember(),"Test")==0)&&(strcmp(call.getSignature(),"s")==0))
      {
        OVK_NOTICE("Member checked !");
        Overkiz::Bus::Message::ConstIterator callIt = call.begin();
        stringWrapper.deserialize(callIt, message);
        OVK_NOTICE(message.c_str());
        ret = "Method Return !";
        Overkiz::Bus::Message::Method::Return reply(call);
        Overkiz::Bus::Message::Iterator methodIt = reply.begin();
        stringWrapper.serialize(methodIt, ret);
        Overkiz::Bus::Object::Server::send(reply);
        return true;
      }
    }

    return false;
  }

  virtual ~Server()
  {
  }


};

class Proxy: public Overkiz::Bus::Object::Proxy
{
public:

  Proxy(const std::string& newPath, const char *newHost)
  {
    this->setPath(newPath);
    this->setHost(newHost);
  }
  void signal(const Overkiz::Bus::Message::Signal& signal)
  {
    Overkiz::Bus::Type::Wrapper< std::string > stringWrapper;
    std::string message = "";
    OVK_NOTICE("Signal received !");
    Overkiz::Bus::Message::ConstIterator callIt = signal.begin();
    stringWrapper.deserialize(callIt, message);
    OVK_NOTICE(message.c_str());
  }
};

class ReturnHandler: public Overkiz::Bus::Object::ReturnHandler
{
  void sendReturn(const uint32_t serial, const Overkiz::Shared::Pointer<Overkiz::Bus::Message::Method::Return> & ret)
  {
    const Overkiz::Bus::Message::Method::Return constRet = *ret;
    Overkiz::Bus::Type::Wrapper< std::string > stringWrapper;
    OVK_NOTICE("Return Handler !");
    std::string message = "";
    Overkiz::Bus::Message::ConstIterator methodIt = constRet.begin();
    stringWrapper.deserialize(methodIt, message);
    OVK_NOTICE(message.c_str());
    exit(EXIT_SUCCESS);
  }

};


int main(int argc, char * argv[])
{
  Overkiz::Shared::Pointer<Overkiz::Poller> & poller = Overkiz::Poller::get(false);
  char member[] = "Test";
  char interface[] = "com.overkiz.Application.Hellow.Server";
  char path[] = "/com/overkiz/Application/Hellow/Server";
  printf("Hello world !\n");
  Overkiz::Bus::Type::Wrapper< std::string > stringWrapper;
  std::string ret = "Message proxy!";
  Proxy * myProxy;
  Overkiz::Shared::Pointer<ReturnHandler> retHandlPoint;
  Server * myServer;

  if(strcmp(argv[1], "server")==0)
  {
    OVK_NOTICE("Server");
    myServer = new Server(path);
    myServer->start();
  }
  else if(strcmp(argv[1], "proxy")==0)
  {
    OVK_NOTICE("Proxy");
    myProxy = new Proxy(path, interface);
    retHandlPoint = Overkiz::Shared::Pointer<ReturnHandler>::create();
    Overkiz::Bus::Message::Method::Call call = Overkiz::Bus::Message::Method::Call();
    call.setMember(member);
    call.setInterface(interface);
    Overkiz::Bus::Message::Iterator methodIt = call.begin();
    stringWrapper.serialize(methodIt, ret);
    myProxy->start();
    myProxy->send(call, retHandlPoint);
  }

  OVK_NOTICE("Polling");
  poller->loop();
  //Warning nothing will be executed here
}
