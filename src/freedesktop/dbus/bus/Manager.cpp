/*
 * Manager.cpp
 *
 *      Copyright (C) 2015 Overkiz SA.
 */

#include <kizbox/application/freedesktop/dbus/bus/Manager.h>
#include <kizbox/framework/bus/Type.h>

namespace Overkiz
{

  namespace FreeDesktop
  {

    namespace DBus
    {
      namespace Bus
      {
        Manager::Manager() : Overkiz::Bus::Object::Proxy("/org/freedesktop/DBus", "org.freedesktop.DBus")
        {
        }

        Manager::~Manager()
        {
        }

        uint32_t Manager::requestName(const std::string& name, uint32_t flags)
        {
          Overkiz::Bus::Type::Wrapper<std::string> strWrapper;
          Overkiz::Bus::Type::Wrapper<uint32_t> uint32Wrapper;
          Overkiz::Bus::Message::Method::Call call;
          call.setInterface("org.freedesktop.DBus");
          call.setMember("RequestName");
          Overkiz::Bus::Message::Iterator it = call.begin();
          strWrapper.serialize(it, name);
          uint32Wrapper.serialize(it, flags);
          Overkiz::Bus::Message::SendReturn ret = send(call);

          if(ret->isException())
          {
            throw *ret;
          }

          Overkiz::Bus::Message::ConstIterator retIt = ret->begin();
          uint32_t retVal;
          uint32Wrapper.deserialize(retIt, retVal);
          return retVal;
        }

        uint32_t Manager::releaseName(const std::string& name)
        {
          Overkiz::Bus::Type::Wrapper<std::string> strWrapper;
          Overkiz::Bus::Type::Wrapper<uint32_t> uint32Wrapper;
          Overkiz::Bus::Message::Method::Call call;
          call.setInterface("org.freedesktop.DBus");
          call.setMember("ReleaseName");
          Overkiz::Bus::Message::Iterator it = call.begin();
          strWrapper.serialize(it, name);
          Overkiz::Bus::Message::SendReturn ret = send(call);

          if(ret->isException())
          {
            throw *ret;
          }

          Overkiz::Bus::Message::ConstIterator retIt = ret->begin();
          uint32_t retVal;
          uint32Wrapper.deserialize(retIt, retVal);
          return retVal;
        }

        std::vector<std::string> Manager::listQueudOwners(const std::string& name) const
        {
          Overkiz::Bus::Type::Wrapper<std::string> strWrapper;
          Overkiz::Bus::Type::SequenceContainerWrapper<std::string, Overkiz::Bus::Type::Wrapper, std::vector> arrayWrapper;
          Overkiz::Bus::Message::Method::Call call;
          call.setInterface("org.freedesktop.DBus");
          call.setMember("ListQueudOwners");
          Overkiz::Bus::Message::Iterator it = call.begin();
          strWrapper.serialize(it, name);
          Overkiz::Bus::Message::SendReturn ret = send(call);

          if(ret->isException())
          {
            throw *ret;
          }

          Overkiz::Bus::Message::ConstIterator retIt = ret->begin();
          std::vector<std::string> retVal;
          arrayWrapper.deserialize(retIt, retVal);
          return retVal;
        }

        std::string Manager::hello()
        {
          Overkiz::Bus::Type::Wrapper<std::string> strWrapper;
          Overkiz::Bus::Message::Method::Call call;
          call.setInterface("org.freedesktop.DBus");
          call.setMember("Hello");
          Overkiz::Bus::Message::SendReturn ret = send(call);

          if(ret->isException())
          {
            throw *ret;
          }

          Overkiz::Bus::Message::ConstIterator retIt = ret->begin();
          std::string retVal;
          strWrapper.deserialize(retIt, retVal);
          return retVal;
        }

        std::vector<std::string> Manager::listNames() const
        {
          Overkiz::Bus::Type::SequenceContainerWrapper<std::string, Overkiz::Bus::Type::Wrapper, std::vector> arrayWrapper;
          Overkiz::Bus::Message::Method::Call call;
          call.setInterface("org.freedesktop.DBus");
          call.setMember("ListNames");
          Overkiz::Bus::Message::SendReturn ret = send(call);

          if(ret->isException())
          {
            throw *ret;
          }

          Overkiz::Bus::Message::ConstIterator retIt = ret->begin();
          std::vector<std::string> retVal;
          arrayWrapper.deserialize(retIt, retVal);
          return retVal;
        }

        std::vector<std::string> Manager::listActivableNames() const
        {
          Overkiz::Bus::Type::SequenceContainerWrapper<std::string, Overkiz::Bus::Type::Wrapper, std::vector> arrayWrapper;
          Overkiz::Bus::Message::Method::Call call;
          call.setInterface("org.freedesktop.DBus");
          call.setMember("ListActivableNames");
          Overkiz::Bus::Message::SendReturn ret = send(call);

          if(ret->isException())
          {
            throw *ret;
          }

          Overkiz::Bus::Message::ConstIterator retIt = ret->begin();
          std::vector<std::string> retVal;
          arrayWrapper.deserialize(retIt, retVal);
          return retVal;
        }

        bool Manager::nameHasOwner(const std::string& name) const
        {
          Overkiz::Bus::Type::Wrapper<std::string> strWrapper;
          Overkiz::Bus::Type::Wrapper<bool> boolWrapper;
          Overkiz::Bus::Message::Method::Call call;
          call.setInterface("org.freedesktop.DBus");
          call.setMember("NameHasOwner");
          Overkiz::Bus::Message::Iterator it = call.begin();
          strWrapper.serialize(it, name);
          Overkiz::Bus::Message::SendReturn ret = send(call);

          if(ret->isException())
          {
            throw *ret;
          }

          Overkiz::Bus::Message::ConstIterator retIt = ret->begin();
          bool retVal;
          boolWrapper.deserialize(retIt, retVal);
          return retVal;
        }

        void Manager::nameOwnerChanged(const std::string& name, const std::string& oldOwner, const std::string& newOwner) {}

        void Manager::nameLost(const std::string& name) {}

        void Manager::nameAcquired(const std::string& name) {}

        uint32_t Manager::startServiceByName(const std::string& name, uint32_t flags)
        {
          Overkiz::Bus::Type::Wrapper<std::string> strWrapper;
          Overkiz::Bus::Type::Wrapper<uint32_t> uint32Wrapper;
          Overkiz::Bus::Message::Method::Call call;
          call.setInterface("org.freedesktop.DBus");
          call.setMember("StartServiceByName");
          Overkiz::Bus::Message::Iterator it = call.begin();
          strWrapper.serialize(it, name);
          uint32Wrapper.serialize(it, flags);
          Overkiz::Bus::Message::SendReturn ret = send(call);

          if(ret->isException())
          {
            throw *ret;
          }

          Overkiz::Bus::Message::ConstIterator retIt = ret->begin();
          uint32_t retVal;
          uint32Wrapper.deserialize(retIt, retVal);
          return retVal;
        }

        void Manager::updateActivationEnvironment(const std::map<std::string, std::string>& env)
        {
          Overkiz::Bus::Type::AssociativePairContainerWrapper<std::string, std::string, Overkiz::Bus::Type::Wrapper, Overkiz::Bus::Type::Wrapper, std::map> mapWrapper;
          Overkiz::Bus::Message::Method::Call call;
          call.setInterface("org.freedesktop.DBus");
          call.setMember("UpdateActivationEnvironment");
          Overkiz::Bus::Message::Iterator it = call.begin();
          mapWrapper.serialize(it, env);
          Overkiz::Bus::Message::SendReturn ret = send(call);

          if(ret->isException())
          {
            throw *ret;
          }
        }

        std::string Manager::getNameOwner(const std::string& name) const
        {
          Overkiz::Bus::Type::Wrapper<std::string> strWrapper;
          Overkiz::Bus::Message::Method::Call call;
          call.setInterface("org.freedesktop.DBus");
          call.setMember("GetNameOwner");
          Overkiz::Bus::Message::Iterator it = call.begin();
          strWrapper.serialize(it, name);
          Overkiz::Bus::Message::SendReturn ret = send(call);

          if(ret->isException())
          {
            throw *ret;
          }

          Overkiz::Bus::Message::ConstIterator retIt = ret->begin();
          std::string retVal;
          strWrapper.deserialize(retIt, retVal);
          return retVal;
        }

        uint32_t Manager::getConnectionUnixUser(const std::string& busName) const
        {
          Overkiz::Bus::Type::Wrapper<std::string> strWrapper;
          Overkiz::Bus::Type::Wrapper<uint32_t> uint32Wrapper;
          Overkiz::Bus::Message::Method::Call call;
          call.setInterface("org.freedesktop.DBus");
          call.setMember("GetConnectionUnixUser");
          Overkiz::Bus::Message::Iterator it = call.begin();
          strWrapper.serialize(it, busName);
          Overkiz::Bus::Message::SendReturn ret = send(call);

          if(ret->isException())
          {
            throw *ret;
          }

          Overkiz::Bus::Message::ConstIterator retIt = ret->begin();
          uint32_t retVal;
          uint32Wrapper.deserialize(retIt, retVal);
          return retVal;
        }

        uint32_t Manager::getConnectionUnixProcessId(const std::string& busName) const
        {
          Overkiz::Bus::Type::Wrapper<std::string> strWrapper;
          Overkiz::Bus::Type::Wrapper<uint32_t> uint32Wrapper;
          Overkiz::Bus::Message::Method::Call call;
          call.setInterface("org.freedesktop.DBus");
          call.setMember("GetConnectionUnixProcessId");
          Overkiz::Bus::Message::Iterator it = call.begin();
          strWrapper.serialize(it, busName);
          Overkiz::Bus::Message::SendReturn ret = send(call);

          if(ret->isException())
          {
            throw *ret;
          }

          Overkiz::Bus::Message::ConstIterator retIt = ret->begin();
          uint32_t retVal;
          uint32Wrapper.deserialize(retIt, retVal);
          return retVal;
        }

        void Manager::addMatch(const std::string& rule)
        {
          Overkiz::Bus::Type::Wrapper<std::string> strWrapper;
          Overkiz::Bus::Message::Method::Call call;
          call.setInterface("org.freedesktop.DBus");
          call.setMember("AddMatch");
          Overkiz::Bus::Message::Iterator it = call.begin();
          strWrapper.serialize(it, rule);
          Overkiz::Bus::Message::SendReturn ret = send(call);

          if(ret->isException())
          {
            throw *ret;
          }
        }

        void Manager::removeMatch(const std::string& rule)
        {
          Overkiz::Bus::Type::Wrapper<std::string> strWrapper;
          Overkiz::Bus::Message::Method::Call call;
          call.setInterface("org.freedesktop.DBus");
          call.setMember("RemoveMatch");
          Overkiz::Bus::Message::Iterator it = call.begin();
          strWrapper.serialize(it, rule);
          Overkiz::Bus::Message::SendReturn ret = send(call);

          if(ret->isException())
          {
            throw *ret;
          }
        }

        std::string Manager::getId() const
        {
          Overkiz::Bus::Type::Wrapper<std::string> strWrapper;
          Overkiz::Bus::Message::Method::Call call;
          call.setInterface("org.freedesktop.DBus");
          call.setMember("GetId");
          Overkiz::Bus::Message::SendReturn ret = send(call);

          if(ret->isException())
          {
            throw *ret;
          }

          Overkiz::Bus::Message::ConstIterator retIt = ret->begin();
          std::string retVal;
          strWrapper.deserialize(retIt, retVal);
          return retVal;
        }

        void Manager::signal(const Overkiz::Bus::Message::Signal& signal)
        {
          if(!strcmp(signal.getInterface(), "org.freedesktop.DBus"))
          {
            Overkiz::Bus::Type::Wrapper<std::string> strWrapper;

            if(!strcmp(signal.getMember(), "NameOwnerChanged"))
            {
              std::string sign = strWrapper.signature() + strWrapper.signature() + strWrapper.signature();

              if(!strcmp(sign.c_str(), signal.getSignature()))
              {
                std::string name;
                std::string oldOwner;
                std::string newOwner;
                Overkiz::Bus::Message::ConstIterator it = signal.begin();
                strWrapper.deserialize(it, name);
                ++it;
                strWrapper.deserialize(it, oldOwner);
                ++it;
                strWrapper.deserialize(it, newOwner);
                nameOwnerChanged(name, oldOwner, newOwner);
              }
            }
            else if(!strcmp(signal.getMember(), "NameLost"))
            {
              if(!strcmp(strWrapper.signature().c_str(), signal.getSignature()))
              {
                std::string name;
                Overkiz::Bus::Message::ConstIterator it = signal.begin();
                strWrapper.deserialize(it, name);
                nameLost(name);
              }
            }
            else if(!strcmp(signal.getMember(), "NameAcquired"))
            {
              if(!strcmp(strWrapper.signature().c_str(), signal.getSignature()))
              {
                std::string name;
                Overkiz::Bus::Message::ConstIterator it = signal.begin();
                strWrapper.deserialize(it, name);
                nameAcquired(name);
              }
            }
          }
        }
      }
    }
  }

}
