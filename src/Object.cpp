/*
 * Object.cpp
 *
 *      Copyright (C) 2015 Overkiz SA.
 */

#include <kizbox/framework/bus/Connection.h>
#include <kizbox/framework/bus/Message.h>

#include "Object.h"

namespace Overkiz
{
  namespace Bus
  {
    namespace Object
    {

      Base::Base()
      {
      }

      Base::Base(const std::string& newPath) :
        path(newPath)
      {
        if(!dbus_validate_path(newPath.c_str(), NULL))
        {
          throw Message::Exception::SyntaxException();
        }
      }

      Base::~Base()
      {
      }

      const std::string& Base::getPath() const
      {
        return path;
      }

      void Base::setPath(const std::string& newPath)
      {
        if(!dbus_validate_path(newPath.c_str(), NULL))
        {
          throw Message::Exception::SyntaxException();
        }

        if(!connection.empty())
        {
          Overkiz::Bus::Connection::AlreadySetException e;
          throw e;
        }

        path = newPath;
      }

      Proxy::Proxy() :
        Base()
      {
      }

      Proxy::Proxy(const std::string& newPath, const char *newHost) :
        Base(newPath),
        host(newHost) //, timeout (this)
      {
        if((!dbus_validate_bus_name(newHost, NULL)) && (!dbus_validate_interface(newHost, NULL)))
        {
          throw Message::Exception::SyntaxException();
        }
      }

      Proxy::~Proxy()
      {
        if(!connection.empty())
        {
          try
          {
            connection->remove(this);
          }
          catch(Overkiz::Exception& e)
          {
            connection->log(Overkiz::Log::Priority::OVK_ERROR, "Exception : %s received", e.getId());
          }
        }
      }

      const char *Proxy::getHost() const
      {
        return host.c_str();
      }

      void Proxy::setHost(const char *newHost)
      {
        if((!dbus_validate_bus_name(newHost, NULL)) && (!dbus_validate_interface(newHost, NULL)))
        {
          throw Message::Exception::SyntaxException();
        }

        host = newHost;
      }

      void Proxy::start(bool system)
      {
        enable();
        Shared::Pointer< Connection > threadConnection = Connection::get(system);

        if(!connection.empty())
        {
          if(connection != threadConnection)
          {
            connection->remove(this);
          }
        }

        connection = threadConnection;
        connection->add(this);
      }

      void Proxy::stop()
      {
        if(!connection.empty())
        {
          connection->remove(this);
          connection = Shared::Pointer< Connection >();
        }

        disable();
      }

      Message::SendReturn Proxy::send(Message::Method::Call& method) const
      {
        if(connection.empty())
        {
          Overkiz::Bus::Connection::EmptyException e;
          throw e;
        }

        method.setPath(path.c_str());
        method.setDestination(host.c_str());
        return connection->send(method);
      }

      uint32_t Proxy::send(Message::Method::Call& method, const Shared::Pointer< ReturnHandler > & handler) const
      {
        if(connection.empty())
        {
          Overkiz::Bus::Connection::EmptyException e;
          throw e;
        }

        method.setPath(path.c_str());
        method.setDestination(host.c_str());
        return connection->send(method, handler);
      }

      void Proxy::signal(const Message::Signal& signal)
      {
      }

      void Proxy::introspect(std::string& def) const
      {
        Bus::Message::Method::Call message;
        message.setInterface("org.freedesktop.DBus.Introspectable");
        message.setMember("Introspect");
        Bus::Message::SendReturn reply = send(message);

        if(reply->isException())
        {
          throw *reply;
        }

        Message::ConstIterator iter = reply->begin();
        Type::Wrapper< std::string > strWrapper;
        strWrapper.deserialize(iter, def);
      }

      uint32_t Proxy::introspect(const Shared::Pointer< ReturnHandler > & handler) const
      {
        Bus::Message::Method::Call message;
        message.setInterface("org.freedesktop.DBus.Introspectable");
        message.setMember("Introspect");
        return connection->send(message, handler);
      }

      void Proxy::entry()
      {
        for(std::list< Message::Signal >::iterator i = signals.begin(); i != signals.end(); i = signals.erase(i))
        {
          signal(*i);
        }
      }

      Server::Server() :
        Base()
      {
      }

      Server::Server(const std::string& newPath) :
        Base(newPath)
      {
      }

      Server::~Server()
      {
        if(!connection.empty())
        {
          connection->remove(this);
        }
      }

      const char *Server::getHost() const
      {
        if(connection.empty())
        {
          Overkiz::Bus::Connection::EmptyException e;
          throw e;
        }

        return connection->getName();
      }

      bool Server::method(const Message::Method::Call& method)
      {
        return false;
      }

      void Server::introspect(std::string& def) const
      {
      }

      void Server::start(bool system)
      {
        enable();
        Shared::Pointer< Connection > threadConnection = Connection::get(system);

        if(!connection.empty())
        {
          if(connection != threadConnection)
          {
            connection->remove(this);
          }
        }

        connection = threadConnection;
        connection->add(this);
      }

      void Server::stop()
      {
        if(!connection.empty())
        {
          connection->remove(this);
          connection = Shared::Pointer< Connection >();
        }

        disable();
      }

      void Server::send(Message::Method::Return& method) const
      {
        if(connection.empty())
        {
          Overkiz::Bus::Connection::EmptyException e;
          throw e;
        }

        return connection->send(method);
      }

      void Server::send(Message::Signal& signal) const
      {
        if(connection.empty())
        {
          Overkiz::Bus::Connection::EmptyException e;
          throw e;
        }

        signal.setPath(path.c_str());
        return connection->send(signal);
      }

      void Server::entry()
      {
        for(std::list< Message::Method::Call >::iterator i = calls.begin(); i != calls.end(); i = calls.erase(i))
        {
          if(method(*i) == false)
          {
            Message::Method::Return ret(*i, Message::Exception::UnknownMethod());
            send(ret);
          }
        }
      }

      ReturnHandler::ReturnHandler()
      {
        enable();
        serial = 0;
      }

      ReturnHandler::~ReturnHandler()
      {
        disable();
      }

      void ReturnHandler::entry()
      {
        sendReturn(serial, ret);
      }

    }
  }
}
