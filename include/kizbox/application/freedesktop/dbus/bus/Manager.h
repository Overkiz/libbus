/*
 * Manager.h
 *
 *      Copyright (C) 2015 Overkiz SA.
 */

#ifndef OVERKIZ_FREEDESKTOP_DBUS_BUS_MANAGER_H_
#define OVERKIZ_FREEDESKTOP_DBUS_BUS_MANAGER_H_

#include <kizbox/framework/bus/Object.h>
#include <kizbox/application/freedesktop/dbus/Manager.h>

namespace Overkiz
{

  namespace FreeDesktop
  {

    namespace DBus
    {
      namespace Bus
      {
        class Manager : public Overkiz::FreeDesktop::DBus::Manager, public Overkiz::Bus::Object::Proxy
        {
        public:
          Manager();

          virtual ~Manager();

          virtual uint32_t requestName(const std::string& name, uint32_t flags);

          virtual uint32_t releaseName(const std::string& name);

          virtual std::vector<std::string> listQueudOwners(const std::string& name) const;

          virtual std::string hello();

          virtual std::vector<std::string> listNames() const;

          virtual std::vector<std::string> listActivableNames() const;

          virtual bool nameHasOwner(const std::string& name) const;

          virtual void nameOwnerChanged(const std::string& name, const std::string& oldOwner, const std::string& newOwner);

          virtual void nameLost(const std::string& name);

          virtual void nameAcquired(const std::string& name);

          virtual uint32_t startServiceByName(const std::string& name, uint32_t flags);

          virtual void updateActivationEnvironment(const std::map<std::string, std::string>& env);

          virtual std::string getNameOwner(const std::string& name) const;

          virtual uint32_t getConnectionUnixUser(const std::string& busName) const;

          virtual uint32_t getConnectionUnixProcessId(const std::string& busName) const;

          virtual void addMatch(const std::string& rule);

          virtual void removeMatch(const std::string& rule);

          virtual std::string getId() const;

        private:

          void signal(const Overkiz::Bus::Message::Signal& signal);
        };
      }
    }
  }

}

#endif /* OVERKIZ_FREEDESKTOP_DBUS_BUS_MANAGER_H_ */
