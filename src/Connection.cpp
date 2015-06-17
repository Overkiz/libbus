/*
 * Connection.cpp
 *
 *      Copyright (C) 2015 Overkiz SA.
 */

#include <dbus/dbus.h>
#include <kizbox/framework/core/Poller.h>
#include <kizbox/framework/core/Log.h>

#include "Connection.h"
#include "Object.h"

namespace Overkiz
{

  namespace Bus
  {
    Connection::Connection(bool system) :
      logger(LOG_GET_REF)
    {
      DBusError error;
      dbus_error_init(&error);
      dbus_threads_init_default();
      connection = dbus_bus_get(system ? DBUS_BUS_SYSTEM : DBUS_BUS_SESSION, &error);

      if(connection == NULL || dbus_error_is_set(&error))
      {
        OVK_CRITICAL("Error %s: %s", error.name, error.message);
        dbus_error_free(&error);
        throw Overkiz::Bus::Connection::EmptyException();
      }

      dbus_error_free(&error);

      if(dbus_connection_add_filter(connection, process, this, NULL) == false)
      {
        dbus_connection_unref(connection);
        throw Overkiz::Bus::Connection::AddFilterException();
      }

      dbus_connection_set_watch_functions(connection, BusWatcher::add, BusWatcher::remove, BusWatcher::toggled, NULL, NULL);
      dbus_connection_set_timeout_functions(connection, TimeoutWatcher::add, TimeoutWatcher::remove, TimeoutWatcher::toggled, NULL, NULL);
      dbus_connection_set_dispatch_status_function(connection, dispatch, this, NULL);
      dbus_connection_set_wakeup_main_function(connection, wakeup, this, NULL);
      setStackSize(2 * 4096);
    }

    Connection::~Connection()
    {
      dbus_connection_remove_filter(connection, process, NULL);
      dbus_connection_unref(connection);
    }

    bool Connection::bind(const std::string& hostname, bool forceReplace)
    {
      bool status = false;
      DBusError error;
      dbus_error_init(&error);
      //Default policy
      int flags = DBUS_NAME_FLAG_DO_NOT_QUEUE | DBUS_NAME_FLAG_ALLOW_REPLACEMENT; //Allow replacement, if someone would force replace

      if(forceReplace)
      {
        flags |= DBUS_NAME_FLAG_REPLACE_EXISTING;
      }

      int ret = -1;
      ret = dbus_bus_request_name(connection, hostname.c_str(), flags, &error);

      if(ret == DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
      {
        // It's ok, now we are the owner
        status = true;
      }
      else if(ret == DBUS_REQUEST_NAME_REPLY_ALREADY_OWNER)
      {
        // This Apps Already own this name
        log(Overkiz::Log::Priority::OVK_ERROR, "Bind %s error : DBUS_REQUEST_NAME_REPLY_ALREADY_OWNER", hostname.c_str());
      }
      else if(ret == DBUS_REQUEST_NAME_REPLY_EXISTS)
      {
        // Name has already an owner
        log(Overkiz::Log::Priority::OVK_ERROR, "Bind %s error : DBUS_REQUEST_NAME_REPLY_EXISTS", hostname.c_str());
      }
      else if(ret == DBUS_REQUEST_NAME_REPLY_IN_QUEUE)
      {
        // Request is in the queue, waiting for release by the owner
        log(Overkiz::Log::Priority::OVK_ERROR, "Bind %s error : DBUS_REQUEST_NAME_REPLY_IN_QUEUE", hostname.c_str());
      }
      else if(ret == -1)
      {
        log(Overkiz::Log::Priority::OVK_ERROR, "Bind %s error %s: %s", hostname.c_str(), error.name, error.message);
        dbus_error_free(&error);
        throw Overkiz::Bus::Connection::RequestNameException(hostname);
      }

      dbus_error_free(&error);
      return status;
    }

    bool Connection::unbind(const std::string& hostname)
    {
      bool status = false;
      DBusError error;
      dbus_error_init(&error);
      int ret = -1;
      ret = dbus_bus_release_name(connection, hostname.c_str(), &error);

      if(ret == DBUS_RELEASE_NAME_REPLY_RELEASED)
      {
        // It's ok, i'm the owner, and i released this name
        status = true;
      }
      else if(ret == DBUS_RELEASE_NAME_REPLY_NOT_OWNER)
      {
        // Oops, someone take my name, i am not the current owner
        log(Overkiz::Log::Priority::OVK_ERROR, "Unbind %s error : DBUS_RELEASE_NAME_REPLY_NOT_OWNER", hostname.c_str());
      }
      else if(ret == DBUS_RELEASE_NAME_REPLY_NON_EXISTENT)
      {
        //Oops, nobody known this name
        log(Overkiz::Log::Priority::OVK_ERROR, "Unbind %s error : DBUS_RELEASE_NAME_REPLY_NON_EXISTENT", hostname.c_str());
      }
      else if(ret == -1)
      {
        log(Overkiz::Log::Priority::OVK_ERROR, "Unbind %s error %s: %s", hostname.c_str(), error.name, error.message);
        dbus_error_free(&error);
        throw Overkiz::Bus::Connection::RequestNameException(hostname);
      }

      dbus_error_free(&error);
      return status;
    }

    Message::Method::Return Connection::send(Message::Method::Call& message)
    {
      message.check();
      Shared::Pointer< Poller > poller = Poller::get();
      Message::Method::Return ret;

      if(Poller::get()->isInterruptible() && poller->status() != Poller::STOPPED)
      {
        DBusPendingCall * pendingCall;

        if(!dbus_connection_send_with_reply(connection, message.message, &pendingCall, message.getTimeout().seconds * 1000))
        {
          log(Overkiz::Log::Priority::OVK_ERROR, "Send error, lack of memory !");
          throw Overkiz::Bus::Connection::LackOfMemoryException();
        }

        if(pendingCall == nullptr)
        {
          throw Overkiz::Bus::Connection::LackOfMemoryException();
        }

        SyncProxyCall * call = new SyncProxyCall();

        if(!dbus_pending_call_set_notify(pendingCall, received, call, NULL))
        {
          throw Overkiz::Bus::Connection::LackOfMemoryException();
        }

        Overkiz::Coroutine::yield();
        ret = call->reply;
      }
      else
      {
        DBusError error;
        dbus_error_init(&error);
        DBusMessage *reply = dbus_connection_send_with_reply_and_block(connection, message.message, message.getTimeout().seconds * 1000, &error);

        //It's necessary to throw an exception only if DBUS_ERROR_NO_MEMORY or DBUS_ERROR_DISCONNECTED is set
        if(!reply && dbus_error_is_set(&error) && memcmp(DBUS_ERROR_NO_REPLY, error.name, sizeof(DBUS_ERROR_NO_REPLY)) != 0)
        {
          log(Overkiz::Log::Priority::OVK_ERROR, "Send error %s: %s", error.name, error.message);
          dbus_error_free(&error);
          throw Overkiz::Bus::Connection::BusErrorException();
        }
        else if(!reply)
        {
          reply = dbus_message_new_error(message.message, error.name, error.message);
        }

        ret.message = reply;
        dbus_error_free(&error);
      }

      return ret;
    }

    uint32_t Connection::send(Message::Method::Call& message, const Shared::Pointer< Object::ReturnHandler > & handler)
    {
      message.check();
      DBusPendingCall * pendingCall;

      if(!dbus_connection_send_with_reply(connection, message.message, &pendingCall, message.getTimeout().seconds * 1000))
      {
        log(Overkiz::Log::Priority::OVK_ERROR, "Send error, lack of memory !");
        throw Overkiz::Bus::Connection::LackOfMemoryException();
      }

      if(pendingCall == nullptr)
      {
        throw Overkiz::Bus::Connection::LackOfMemoryException();
      }

      //Raw copy to generate unique ID
      uint32_t ret;
      memcpy(&ret, &pendingCall, sizeof(ret));
      AsyncProxyCall * call = new AsyncProxyCall(ret, handler);

      if(!dbus_pending_call_set_notify(pendingCall, received, call, NULL))
      {
        throw Overkiz::Bus::Connection::LackOfMemoryException();
      }

      return ret;
    }

    void Connection::send(Message::Method::Return& message) const
    {
      message.check();
      dbus_uint32_t serial;

      if(!dbus_connection_send(connection, message.message, &serial))
      {
        log(Overkiz::Log::Priority::OVK_ERROR, "Send error, lack of memory !");
        throw Overkiz::Bus::Connection::LackOfMemoryException();
      }
    }

    void Connection::send(Message::Signal& message) const
    {
      message.check();
      dbus_uint32_t serial;

      if(!dbus_connection_send(connection, message.message, &serial))
      {
        log(Overkiz::Log::Priority::OVK_ERROR, "Send error, lack of memory !");
        throw Overkiz::Bus::Connection::LackOfMemoryException();
      }
    }

    std::string Connection::buildMatchRule(const std::string & path, const std::string & host)
    {
      return "type='signal',path='" + path + "',sender='" + host + "'";
    }

    void Connection::add(Object::Proxy *proxy)
    {
      std::string ruleDef = buildMatchRule(proxy->getPath(), proxy->getHost());

      //Check if we need to add a filter
      if(proxies.find(ruleDef) == proxies.end())
      {
        //Rule doesn't exists, create it
        DBusError error;
        dbus_error_init(&error);
        dbus_bus_add_match(connection, ruleDef.c_str(), &error);

        if(dbus_error_is_set(&error))
        {
          log(Overkiz::Log::Priority::OVK_ERROR, "Add Error %s: %s (%s)", error.name, error.message, ruleDef.c_str());
          dbus_error_free(&error);
          throw Overkiz::Bus::Connection::BusErrorException();
        }

        dbus_error_free(&error);
      }

      //Add this proxy in the proxies list
      std::list< Object::Proxy * >& objs = proxies[ruleDef];

      for(std::list< Object::Proxy * >::iterator i = objs.begin(); i != objs.end(); ++i)
      {
        if(*i == proxy)
        {
          //Proxy is already registred, nothing to do
          return;
        }
      }

      objs.push_back(proxy);
    }

    void Connection::add(Object::Server *server)
    {
      servers[server->getPath()] = server;
    }

    void Connection::remove(Object::Proxy *proxy)
    {
      //Check if we need to unsubscribe to this rule
      std::string ruleDef = buildMatchRule(proxy->getPath(), proxy->getHost());
      auto it = proxies.find(ruleDef);

      if(it != proxies.end())
      {
        std::list< Object::Proxy * >& objs = it->second;

        for(std::list< Object::Proxy * >::iterator j = objs.begin(); j != objs.end();)
        {
          if(*j == proxy)
          {
            j = objs.erase(j);
          }
          else
          {
            ++j;
          }
        }

        if(objs.empty())
        {
          proxies.erase(it);
          DBusError error;
          dbus_error_init(&error);
          dbus_bus_remove_match(connection, ruleDef.c_str(), &error);

          if(dbus_error_is_set(&error))
          {
            log(Overkiz::Log::Priority::OVK_ERROR, "Remove Error %s: %s (%s)", error.name, error.message, ruleDef.c_str());
            dbus_error_free(&error);
            throw Overkiz::Bus::Connection::BusErrorException();
          }

          dbus_error_free(&error);
        }
      }
    }

    void Connection::remove(Object::Server *server)
    {
      std::map< std::string, Object::Server * >::iterator i;
      i = servers.find(server->getPath());

      if(i != servers.end())
      {
        servers.erase(i);
      }
    }

    DBusHandlerResult Connection::process(DBusConnection *connec, DBusMessage *msg, void *data)
    {
      Connection *connection = static_cast< Connection * >(data);
      DBusHandlerResult ret = DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

      switch(dbus_message_get_type(msg))
      {
        case DBUS_MESSAGE_TYPE_METHOD_CALL:
        {
          std::map< std::string, Object::Server * >::iterator i;

          if(!strcmp(dbus_message_get_interface(msg), "org.freedesktop.DBus.Introspectable") &&
             strlen(dbus_message_get_interface(msg)) == strlen("org.freedesktop.DBus.Introspectable") &&
             !strcmp(dbus_message_get_member(msg), "Introspect") &&
             strlen(dbus_message_get_member(msg)) == strlen("Introspect"))
          {
            DBusMessage *res = dbus_message_new_method_return(msg);
            std::string path = dbus_message_get_path(msg);
            std::string def =
              "<!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\"\n "
              "\"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\">\n"
              "<node name=\"";
            def += dbus_message_get_path(msg);
            def += "\">\n";
            i = connection->servers.lower_bound(path);

            if(i != connection->servers.end())
            {
              if(i->first == path)
              {
                i->second->introspect(def);
                ++i;
              }

              if(*(path.rbegin()) != '/')
              {
                path += "/";
              }

              std::set< std::string > nodes;
              std::set< std::string >::iterator j;
              size_t pos;

              while(i != connection->servers.end() &&
                    !strncmp(i->first.c_str(), path.c_str(), path.size()))
              {
                std::string name;
                pos = i->first.find('/', path.size());

                if(pos == std::string::npos)
                {
                  name.assign(i->first, path.size(), i->first.size() - path.size());
                }
                else
                {
                  name.assign(i->first, path.size(), pos - path.size());
                }

                nodes.insert(name);
                ++i;
              }

              for(j = nodes.begin(); j != nodes.end(); ++j)
              {
                def += "\t<node name=\"";
                def += *j;
                def += "\"/>\n";
              }
            }

            def += "</node>";
            const char *definition = def.c_str();
            dbus_message_append_args(res,
                                     DBUS_TYPE_STRING, &definition,
                                     DBUS_TYPE_INVALID);
            dbus_uint32_t serial;
            dbus_connection_send(connec, res, &serial);
            ret = DBUS_HANDLER_RESULT_HANDLED;
          }
          else
          {
            i = connection->servers.find(dbus_message_get_path(msg));

            if(i != connection->servers.end())
            {
              Message::Method::Call message(msg);
              Object::Server *server = i->second;
              server->calls.push_back(message);

              if(server->status() == Task::IDLE)
              {
                Poller::get()->resume(server);
              }

              ret = DBUS_HANDLER_RESULT_HANDLED;
            }
          }
        }
        break;

        case DBUS_MESSAGE_TYPE_SIGNAL:
        {
          const char * path = dbus_message_get_path(msg);

          for(auto it = connection->proxies.begin() ; it != connection->proxies.end() ; it++)
          {
            //Check if this filter contains current path
            if(it->first.find(path) == std::string::npos)
              continue;

            Message::Signal message(msg);
            std::list< Object::Proxy * >& objs = it->second;

            for(std::list< Object::Proxy * >::iterator j = objs.begin(); j != objs.end(); ++j)
            {
              Object::Proxy *proxy = *j;
              proxy->signals.push_back(message);

              if(proxy->status() == Task::IDLE)
              {
                Poller::get()->resume(proxy);
              }
            }

            ret = DBUS_HANDLER_RESULT_HANDLED;
          }
        }
        break;

        default:
          break;
      }

      return ret;
    }

    void Connection::dispatch(DBusConnection *connection, DBusDispatchStatus status, void *data)
    {
      Connection *connec = static_cast< Connection * >(data);

      if(status == DBUS_DISPATCH_DATA_REMAINS)
        connec->Event::send();
    }

    void Connection::wakeup(void *data)
    {
      Connection *connec = static_cast< Connection * >(data);
      connec->Event::send();
    }

    void Connection::received(DBusPendingCall *pending, void *data)
    {
      ProxyCall *call = static_cast< ProxyCall * >(data);
      DBusMessage * msg = dbus_pending_call_steal_reply(pending);
      call->completed(msg);
      dbus_message_unref(msg); //Free pending message
      dbus_pending_call_unref(pending); //Free pending call handle
      delete call;
    }

    Shared::Pointer< Connection >& Connection::get(bool system)
    {
      if(system == false)
      {
        if(sessionKey->empty())
        {
          sessionKey = Shared::Pointer< Connection >::create(system);
        }

        return *sessionKey;
      }
      else
      {
        if(systemKey->empty())
        {
          systemKey = Shared::Pointer< Connection >::create(system);
        }

        return *systemKey;
      }
    }

    void Connection::receive(uint64_t numberOfEvents)
    {
      DBusDispatchStatus status;
      status = dbus_connection_dispatch(connection);

      if(status == DBUS_DISPATCH_DATA_REMAINS)
        Event::send();
    }

    Connection::BusWatcher::BusWatcher(DBusWatch *watch) :
      Watcher()
    {
      setStackSize(2 * 4096);
      handler = watch;
    }

    Connection::BusWatcher::BusWatcher(DBusWatch *watch, int fd, uint32_t flags) :
      Watcher(fd, flags)
    {
      setStackSize(2 * 4096);
      handler = watch;
    }

    Connection::BusWatcher::~BusWatcher()
    {
      handler = NULL;
    }

    void Connection::BusWatcher::process(uint32_t evts)
    {
      unsigned int flags = 0;

      if(evts & EPOLLIN)
      {
        flags |= DBUS_WATCH_READABLE;
      }

      if(evts & EPOLLOUT)
      {
        flags |= DBUS_WATCH_WRITABLE;
      }

      if(evts & EPOLLERR)
      {
        flags |= DBUS_WATCH_ERROR;
      }

      if(evts & EPOLLHUP)
      {
        flags |= DBUS_WATCH_HANGUP;
      }

      dbus_watch_handle(handler, flags);
    }

    dbus_bool_t Connection::BusWatcher::add(DBusWatch *watch, void *data)
    {
      int fd = dup(dbus_watch_get_unix_fd(watch));
      unsigned int flags = dbus_watch_get_flags(watch);
      uint32_t watcherFlags = 0;

      if(dbus_watch_get_enabled(watch) == true)
      {
        if(flags & DBUS_WATCH_READABLE)
        {
          watcherFlags |= EPOLLIN;
        }

        if(flags & DBUS_WATCH_WRITABLE)
        {
          watcherFlags |= EPOLLOUT;
        }
      }

      BusWatcher *watcher = new BusWatcher(watch, fd, watcherFlags);
      watcher->setStackSize(4 * 4096);
      dbus_watch_set_data(watch, watcher, NULL);
      watcher->start();
      return true;
    }

    void Connection::BusWatcher::toggled(DBusWatch *watch, void *data)
    {
      unsigned int flags = dbus_watch_get_flags(watch);
      uint32_t watcherFlags = 0;

      if(dbus_watch_get_enabled(watch) == true)
      {
        if(flags & DBUS_WATCH_READABLE)
        {
          watcherFlags |= EPOLLIN;
        }

        if(flags & DBUS_WATCH_WRITABLE)
        {
          watcherFlags |= EPOLLOUT;
        }
      }

      BusWatcher *watcher = static_cast< BusWatcher * >(dbus_watch_get_data(watch));

      if(watcher == NULL)
      {
        throw;
      }

      watcher->modify(watcherFlags);
    }

    void Connection::BusWatcher::remove(DBusWatch *watch, void *data)
    {
      BusWatcher *watcher = static_cast< BusWatcher * >(dbus_watch_get_data(watch));
      delete watcher;
      dbus_watch_set_data(watch, NULL, NULL);
    }

    Connection::TimeoutWatcher::TimeoutWatcher(DBusTimeout *watch) :
      Timer::Monotonic(true)
    {
      handler = watch;
    }

    Connection::TimeoutWatcher::TimeoutWatcher(DBusTimeout *watch, const Time::Monotonic& time) :
      Timer::Monotonic(time, true)
    {
      handler = watch;
    }

    Connection::TimeoutWatcher::~TimeoutWatcher()
    {
    }

    void Connection::TimeoutWatcher::expired(const Time::Monotonic& time)
    {
      dbus_timeout_handle(handler);
    }

    dbus_bool_t Connection::TimeoutWatcher::add(DBusTimeout *watch, void *data)
    {
      TimeoutWatcher *timer = new TimeoutWatcher(watch);
      timer->setStackSize(3 * 4096);

      if(dbus_timeout_get_enabled(watch) == true)
      {
        int interval = dbus_timeout_get_interval(watch);
        Time::Elapsed elapsed;
        elapsed.seconds = interval / 1000;
        elapsed.nanoseconds = (interval % 1000) * 1000000;
        Time::Monotonic timeout(elapsed);
        timer->setTime(timeout, true);
        timer->start();
      }

      dbus_timeout_set_data(watch, timer, NULL);
      return true;
    }

    void Connection::TimeoutWatcher::toggled(DBusTimeout *watch, void *data)
    {
      TimeoutWatcher *timer = static_cast< TimeoutWatcher * >(dbus_timeout_get_data(watch));

      if(dbus_timeout_get_enabled(watch) == true)
      {
        int interval = dbus_timeout_get_interval(watch);
        Time::Elapsed elapsed;
        elapsed.seconds = interval / 1000;
        elapsed.nanoseconds = (interval % 1000) * 1000000;
        Time::Monotonic timeout(elapsed);
        timer->setTime(timeout, true);
        timer->start();
      }
      else
      {
        timer->stop();
      }
    }

    void Connection::TimeoutWatcher::remove(DBusTimeout *watch, void *data)
    {
      TimeoutWatcher *timer = static_cast< TimeoutWatcher * >(dbus_timeout_get_data(watch));
      delete timer;
    }

    Connection::ProxyCall::~ProxyCall()
    {
    }

    Connection::SyncProxyCall::SyncProxyCall()
    {
      coroutine = Overkiz::Coroutine::self();
    }

    Connection::SyncProxyCall::~SyncProxyCall()
    {
    }

    void Connection::SyncProxyCall::completed(DBusMessage * msg)
    {
      dbus_message_ref(msg);
      reply.message = msg;
      Overkiz::Coroutine::resume(coroutine);
    }

    Connection::AsyncProxyCall::AsyncProxyCall(uint32_t i, const Shared::Pointer< Object::ReturnHandler > & hd)
    {
      handler = hd;
      id = i;
    }

    void Connection::AsyncProxyCall::completed(DBusMessage * msg)
    {
      dbus_message_ref(msg);
      reply.message = msg;

      if(!handler.empty())
      {
        handler->serial = id;
        handler->ret = Shared::Pointer< Message::Method::Return >::create(Message::Method::Return(reply.message));

        if((*handler).status() == Task::IDLE)
        {
          Poller::get()->resume(&(*handler));
        }
      }
    }

    Connection::AsyncProxyCall::~AsyncProxyCall()
    {
    }

    const char *Connection::getName() const
    {
      return dbus_bus_get_unique_name(connection);
    }

    void Connection::log(const Overkiz::Log::Priority priority, const char * what, ...) const
    {
      if(!logger)
        return;

      char header[] = "Libbus-> ";
      size_t len = strlen(what) + strlen(header) + 1;
      char buff[len];
      strcpy(buff, header);
      strcat(buff, what);
      va_list arguments;
      va_start(arguments, what);
      logger->vprint(priority, buff, arguments);
      va_end(arguments);
    }

    void Connection::setLogger(Overkiz::Log::Logger * log)
    {
      logger = log;
    }

    Thread::Key< Connection > Connection::sessionKey;
    Thread::Key< Connection > Connection::systemKey;
  }
}
