/*
 * Manager.h
 *
 *      Copyright (C) 2015 Overkiz SA.
 */

#ifndef OVERKIZ_FREEDESKTOP_DBUS_MANAGER_H_
#define OVERKIZ_FREEDESKTOP_DBUS_MANAGER_H_

#include <string>
#include <vector>
#include <map>

namespace Overkiz
{

  namespace FreeDesktop
  {

    namespace DBus
    {
      class Manager
      {
      public:

        /*!
         * \enum ServiceStatus
         *
         * Enumeration defining the status of the service.
         */
        enum ServiceStatus
        {
          STATUS_SUCCESS = DBUS_START_REPLY_SUCCESS, //!< The service was successfully started.
          STATUS_ALREADY_RUNNING = DBUS_START_REPLY_ALREADY_RUNNING, //!< A connection already owns the given name.
        };

        Manager() {}

        virtual ~Manager() {}

        virtual uint32_t requestName(const std::string& name, uint32_t flags) = 0;

        virtual uint32_t releaseName(const std::string& name) = 0;

        virtual std::vector<std::string> listQueudOwners(const std::string& name) const = 0;

        virtual std::string hello() = 0;

        virtual std::vector<std::string> listNames() const = 0;

        virtual std::vector<std::string> listActivableNames() const = 0;

        virtual bool nameHasOwner(const std::string& name) const = 0;

        virtual void nameOwnerChanged(const std::string& name, const std::string& oldOwner, const std::string& newOwner) = 0;

        virtual void nameLost(const std::string& name) = 0;

        virtual void nameAcquired(const std::string& name) = 0;

        virtual uint32_t startServiceByName(const std::string& name, uint32_t flags) = 0;

        virtual void updateActivationEnvironment(const std::map<std::string, std::string>& env) = 0;

        virtual std::string getNameOwner(const std::string& name) const = 0;

        virtual uint32_t getConnectionUnixUser(const std::string& busName) const = 0;

        virtual uint32_t getConnectionUnixProcessId(const std::string& busName) const = 0;

        virtual void addMatch(const std::string& rule) = 0;

        virtual void removeMatch(const std::string& rule) = 0;

        virtual std::string getId() const = 0;
      };
    }
  }

}

#endif /* OVERKIZ_FREEDESKTOP_DBUS_MANAGER_H_ */
