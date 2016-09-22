/*
 * Message.cpp
 *
 *      Copyright (C) 2015 Overkiz SA.
 */

#include <stdio.h>

#include "Connection.h"
#include "Message.h"

#define DBUS_METHOD_RETURN_DEFAULT_TIMEOUT 5
#define DBUS_METHOD_RETURN_UPPER_TIMEOUT 28

namespace Overkiz
{
  namespace Bus
  {
    namespace Message
    {
      ConstIterator::ConstIterator()
      {
        memset(&iterator, 0, sizeof(DBusMessageIter));
      }

      ConstIterator::~ConstIterator()
      {
      }

      ConstIterator& ConstIterator::operator ++ ()
      {
        if(dbus_message_iter_next(&iterator) == FALSE)
        {
          memset(&iterator, 0, sizeof(DBusMessageIter));
        }
        else
        {
          char *tmp = dbus_message_iter_get_signature(&iterator);
          sign = tmp;
          dbus_free(tmp);
        }

        return *this;
      }

      int ConstIterator::type()
      {
        return dbus_message_iter_get_arg_type(&iterator);
      }

      const char *ConstIterator::signature()
      {
        return sign.c_str();
      }

      ConstIterator ConstIterator::begin()
      {
        int type = dbus_message_iter_get_arg_type(&iterator);

        if(type != DBUS_TYPE_ARRAY && type != DBUS_TYPE_DICT_ENTRY && type != DBUS_TYPE_VARIANT && type != DBUS_TYPE_STRUCT)
        {
          Exception::MismatchType e(type,
          {
            DBUS_TYPE_ARRAY,
            DBUS_TYPE_DICT_ENTRY,
            DBUS_TYPE_VARIANT,
            DBUS_TYPE_STRUCT
          });
          throw e;
        }

        ConstIterator it;
        dbus_message_iter_recurse(&iterator, &it.iterator);

        if(dbus_message_iter_get_arg_type(&it.iterator) == DBUS_TYPE_INVALID)
        {
          memset(&it.iterator, 0, sizeof(DBusMessageIter));
        }
        else
        {
          char *sign = dbus_message_iter_get_signature(&it.iterator);
          it.sign = sign;
          dbus_free(sign);
        }

        return it;
      }

      ConstIterator ConstIterator::end()
      {
        ConstIterator it;
        memset(&it.iterator, 0, sizeof(DBusMessageIter));
        return it;
      }

      void ConstIterator::get(const char *& val)
      {
        int type = dbus_message_iter_get_arg_type(&iterator);

        if((type != DBUS_TYPE_STRING) && (type != DBUS_TYPE_OBJECT_PATH) && (type != DBUS_TYPE_SIGNATURE))
        {
          Exception::MismatchType e(type,
          {
            DBUS_TYPE_STRING,
            DBUS_TYPE_OBJECT_PATH,
            DBUS_TYPE_SIGNATURE
          });
          throw e;
        }

        dbus_message_iter_get_basic(&iterator, &val);
      }

      void ConstIterator::get(std::string& val)
      {
        int type = dbus_message_iter_get_arg_type(&iterator);

        if((type != DBUS_TYPE_STRING) && (type != DBUS_TYPE_OBJECT_PATH) && (type != DBUS_TYPE_SIGNATURE))
        {
          Exception::MismatchType e(type,
          {
            DBUS_TYPE_STRING,
            DBUS_TYPE_OBJECT_PATH,
            DBUS_TYPE_SIGNATURE
          });
          throw e;
        }

        const char* ptr;
        dbus_message_iter_get_basic(&iterator, &ptr);
        val = ptr;
      }

      void ConstIterator::get(uint8_t& val)
      {
        int type = dbus_message_iter_get_arg_type(&iterator);

        if(type != DBUS_TYPE_BYTE)
        {
          Exception::MismatchType e(type, DBUS_TYPE_BYTE);
          throw e;
        }

        dbus_message_iter_get_basic(&iterator, &val);
      }

      void ConstIterator::get(bool& val)
      {
        int type = dbus_message_iter_get_arg_type(&iterator);

        if(type != DBUS_TYPE_BOOLEAN)
        {
          Exception::MismatchType e(type, DBUS_TYPE_BOOLEAN);
          throw e;
        }

        uint32_t tmp;
        dbus_message_iter_get_basic(&iterator, &tmp);
        val = tmp;
      }

      void ConstIterator::get(int16_t& val)
      {
        int type = dbus_message_iter_get_arg_type(&iterator);

        if(type != DBUS_TYPE_INT16)
        {
          Exception::MismatchType e(type, DBUS_TYPE_INT16);
          throw e;
        }

        dbus_message_iter_get_basic(&iterator, &val);
      }

      void ConstIterator::get(uint16_t& val)
      {
        int type = dbus_message_iter_get_arg_type(&iterator);

        if(type != DBUS_TYPE_UINT16)
        {
          Exception::MismatchType e(type, DBUS_TYPE_UINT16);
          throw e;
        }

        dbus_message_iter_get_basic(&iterator, &val);
      }

      void ConstIterator::get(int32_t& val)
      {
        int type = dbus_message_iter_get_arg_type(&iterator);

        if(type != DBUS_TYPE_INT32)
        {
          Exception::MismatchType e(type, DBUS_TYPE_INT32);
          throw e;
        }

        dbus_message_iter_get_basic(&iterator, &val);
      }

      void ConstIterator::get(uint32_t& val)
      {
        int type = dbus_message_iter_get_arg_type(&iterator);

        if(type != DBUS_TYPE_UINT32)
        {
          Exception::MismatchType e(type, DBUS_TYPE_UINT32);
          throw e;
        }

        dbus_message_iter_get_basic(&iterator, &val);
      }

      void ConstIterator::get(int64_t& val)
      {
        int type = dbus_message_iter_get_arg_type(&iterator);

        if(type != DBUS_TYPE_INT64)
        {
          Exception::MismatchType e(type, DBUS_TYPE_INT64);
          throw e;
        }

        dbus_message_iter_get_basic(&iterator, &val);
      }

      void ConstIterator::get(uint64_t& val)
      {
        int type = dbus_message_iter_get_arg_type(&iterator);

        if(type != DBUS_TYPE_UINT64)
        {
          Exception::MismatchType e(type, DBUS_TYPE_UINT64);
          throw e;
        }

        dbus_message_iter_get_basic(&iterator, &val);
      }

      void ConstIterator::get(double& val)
      {
        int type = dbus_message_iter_get_arg_type(&iterator);

        if(type != DBUS_TYPE_DOUBLE)
        {
          Exception::MismatchType e(type, DBUS_TYPE_DOUBLE);
          throw e;
        }

        dbus_message_iter_get_basic(&iterator, &val);
      }

      bool ConstIterator::operator != (const ConstIterator& it) const
      {
        return memcmp(&it.iterator, &iterator, sizeof(DBusMessageIter));
      }

      Iterator::Iterator()
      {
        memset(&iterator, 0, sizeof(DBusMessageIter));
      }

      Iterator::~Iterator()
      {
      }

      bool Iterator::operator != (const Iterator& it) const
      {
        return memcmp(&it.iterator, &iterator, sizeof(DBusMessageIter));
      }

      Iterator Iterator::begin(int type, const char *signature)
      {
        if(type != DBUS_TYPE_ARRAY && type != DBUS_TYPE_VARIANT)
        {
          Exception::MismatchType e(type,
          {
            DBUS_TYPE_ARRAY,
            DBUS_TYPE_VARIANT
          });
          throw e;
        }

        Iterator it;
        dbus_bool_t ret = dbus_message_iter_open_container(&iterator, type, signature, &it.iterator);

        if(ret == FALSE)
        {
          throw;
        }

        return it;
      }

      Iterator Iterator::begin(int type)
      {
        if(type != DBUS_TYPE_DICT_ENTRY && type != DBUS_TYPE_STRUCT)
        {
          Exception::MismatchType e(type,
          {
            DBUS_TYPE_DICT_ENTRY,
            DBUS_TYPE_STRUCT
          });
          throw e;
        }

        Iterator it;
        dbus_bool_t ret = dbus_message_iter_open_container(&iterator, type, NULL, &it.iterator);

        if(ret == FALSE)
        {
          throw;
        }

        return it;
      }

      void Iterator::end(Iterator& it)
      {
        dbus_bool_t ret = dbus_message_iter_close_container(&iterator, &it.iterator);

        if(ret == FALSE)
        {
          throw;
        }
      }

      void Iterator::push(const char * val, int type)
      {
        if(!dbus_validate_utf8(val, NULL))
        {
          throw Message::Exception::SyntaxException();
        }

        if(type != DBUS_TYPE_STRING && type != DBUS_TYPE_OBJECT_PATH && type != DBUS_TYPE_SIGNATURE)
        {
          Exception::MismatchType e(type,
          {
            DBUS_TYPE_STRING,
            DBUS_TYPE_OBJECT_PATH,
            DBUS_TYPE_SIGNATURE
          });
          throw e;
        }

        dbus_bool_t ret = dbus_message_iter_append_basic(&iterator, type, &val);

        if(ret == false)
        {
          throw;
        }
      }

      void Iterator::push(uint8_t val)
      {
        dbus_bool_t ret = dbus_message_iter_append_basic(&iterator, DBUS_TYPE_BYTE, &val);

        if(ret == false)
        {
          throw;
        }
      }

      void Iterator::push(bool val)
      {
        uint32_t tmp = val;
        dbus_bool_t ret = dbus_message_iter_append_basic(&iterator, DBUS_TYPE_BOOLEAN, &tmp);

        if(ret == false)
        {
          throw;
        }
      }

      void Iterator::push(int16_t val)
      {
        dbus_bool_t ret = dbus_message_iter_append_basic(&iterator, DBUS_TYPE_INT16, &val);

        if(ret == false)
        {
          throw;
        }
      }

      void Iterator::push(uint16_t val)
      {
        dbus_bool_t ret = dbus_message_iter_append_basic(&iterator, DBUS_TYPE_UINT16, &val);

        if(ret == false)
        {
          throw;
        }
      }

      void Iterator::push(int32_t val)
      {
        dbus_bool_t ret = dbus_message_iter_append_basic(&iterator, DBUS_TYPE_INT32, &val);

        if(ret == false)
        {
          throw;
        }
      }

      void Iterator::push(uint32_t val)
      {
        dbus_bool_t ret = dbus_message_iter_append_basic(&iterator, DBUS_TYPE_UINT32, &val);

        if(ret == false)
        {
          throw;
        }
      }

      void Iterator::push(int64_t val)
      {
        dbus_bool_t ret = dbus_message_iter_append_basic(&iterator, DBUS_TYPE_INT64, &val);

        if(ret == false)
        {
          throw;
        }
      }

      void Iterator::push(uint64_t val)
      {
        dbus_bool_t ret = dbus_message_iter_append_basic(&iterator, DBUS_TYPE_UINT64, &val);

        if(ret == false)
        {
          throw;
        }
      }

      void Iterator::push(double val)
      {
        dbus_bool_t ret = dbus_message_iter_append_basic(&iterator, DBUS_TYPE_DOUBLE, &val);

        if(ret == false)
        {
          throw;
        }
      }

      Base::Base()
      {
        message = NULL;
      }

      Base::Base(DBusMessage *msg)
      {
        if(msg)
        {
          message = dbus_message_ref(msg);
        }
        else
        {
          message = NULL;
        }
      }

      Base::Base(const Base& msg)
      {
        if(msg.message)
        {
          message = dbus_message_ref(msg.message);
        }
        else
        {
          message = NULL;
        }
      }

      Base::~Base()
      {
        if(message)
        {
          dbus_message_unref(message);
        }
      }

      Iterator Base::begin()
      {
        Iterator i;
        dbus_message_iter_init_append(message, &i.iterator);
        return i;
      }

      ConstIterator Base::begin() const
      {
        if(message == NULL)
        {
          throw Exception::NilMessageReference();
        }

        ConstIterator i;
        dbus_message_iter_init(message, &i.iterator);

        if(dbus_message_iter_get_arg_type(&i.iterator) == DBUS_TYPE_INVALID)
        {
          memset(&i.iterator, 0, sizeof(DBusMessageIter));
        }
        else
        {
          char *sign = dbus_message_iter_get_signature(&i.iterator);
          i.sign = sign;
          dbus_free(sign);
        }

        return i;
      }

      Iterator Base::end()
      {
        Iterator i;
        memset(&i.iterator, 0, sizeof(i.iterator));
        return i;
      }

      ConstIterator Base::end() const
      {
        ConstIterator i;
        memset(&i.iterator, 0, sizeof(i.iterator));
        return i;
      }

      Base& Base::operator = (const Base& msg)
      {
        if(message != NULL)
        {
          dbus_message_unref(message);
        }

        if(msg.message != NULL)
        {
          message = dbus_message_ref(msg.message);
        }

        return *this;
      }

      const char *Base::getSender() const
      {
        return dbus_message_get_sender(message);
      }

      void Base::setSender(const char *sender)
      {
        dbus_message_set_sender(message, sender);
      }

      const char *Base::getDestination() const
      {
        return dbus_message_get_destination(message);
      }

      void Base::setDestination(const char *destination)
      {
        dbus_message_set_destination(message, destination);
      }

      const char *Base::getSignature() const
      {
        return dbus_message_get_signature(message);
      }

      void Base::check() const
      {
        DBusError error;
        dbus_error_init(&error);
        dbus_signature_validate(dbus_message_get_signature(message), &error);

        if(dbus_error_is_set(&error))
        {
          Connection::get()->log(Overkiz::Log::Priority::OVK_ERROR, "Message Error %s: %s", error.name, error.message);
          dbus_error_free(&error);
          throw Exception::MalformedMessage();
        }

        dbus_error_free(&error);
      }

      namespace Method
      {
        Call::Call()
        {
          message = dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL);
          _timeout = Time::Elapsed(DBUS_METHOD_RETURN_DEFAULT_TIMEOUT, 0);
        }

        Call::Call(const Call& message) :
          Base(message)
        {
          _timeout = message.getTimeout();
        }

        Call::Call(DBusMessage *msg)
        {
          if(msg && dbus_message_get_type(msg) == DBUS_MESSAGE_TYPE_METHOD_CALL)
          {
            message = dbus_message_ref(msg);
          }
          else
          {
            message = NULL;
          }

          _timeout = Time::Elapsed(DBUS_METHOD_RETURN_DEFAULT_TIMEOUT, 0);
        }

        Call::~Call()
        {
        }

        const char *Call::getPath() const
        {
          return dbus_message_get_path(message);
        }

        void Call::setPath(const char *path)
        {
          dbus_message_set_path(message, path);
        }

        const char *Call::getInterface() const
        {
          return dbus_message_get_interface(message);
        }

        void Call::setInterface(const char *iface)
        {
          dbus_message_set_interface(message, iface);
        }

        const char *Call::getMember() const
        {
          return dbus_message_get_member(message);
        }

        void Call::setMember(const char *member)
        {
          if(!dbus_validate_member(member, NULL))
          {
            throw Message::Exception::SyntaxException();
          }

          dbus_message_set_member(message, member);
        }

        void Call::check() const
        {
          Base::check();

          if(dbus_message_get_member(message) == NULL || dbus_message_get_interface(message) == NULL || dbus_message_get_path(message) == NULL
             || dbus_message_get_destination(message) == NULL)
          {
            Connection::get()->log(Overkiz::Log::Priority::OVK_ERROR, "Call error, missing member or interface or path or destination");
            throw Exception::MalformedMessage();
          }
        }

        // Be careful, timeout upper limit is set by dbus global settings !
        void Call::setTimeout(const Time::Elapsed & timeout)
        {
          if(_timeout.seconds > DBUS_METHOD_RETURN_UPPER_TIMEOUT)
          {
            Connection::get()->log(Overkiz::Log::Priority::OVK_WARNING, "Call timeout exeeds upper value.");
            _timeout = Time::Elapsed(DBUS_METHOD_RETURN_UPPER_TIMEOUT, 0);
          }
          else
          {
            _timeout = timeout;
          }
        }

        const Time::Elapsed &Call::getTimeout() const
        {
          return _timeout;
        }

        Return::Return() :
          Base()
        {
          message = NULL;
        }

        Return::Return(DBusMessage *msg)
        {
          if(msg &&
             (dbus_message_get_type(msg) == DBUS_MESSAGE_TYPE_METHOD_RETURN ||
              dbus_message_get_type(msg) == DBUS_MESSAGE_TYPE_ERROR))
          {
            message = dbus_message_ref(msg);
          }
          else
          {
            message = NULL;
          }
        }

        Return::Return(const Return& message) :
          Base(message)
        {
        }

        Return::Return(const Call& msg)
        {
          message = dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_RETURN);
          dbus_message_set_reply_serial(message, dbus_message_get_serial(msg.message));
          dbus_message_set_destination(message, dbus_message_get_sender(msg.message));
        }

        Return::Return(const Call& msg, const Overkiz::Exception& exception)
        {
          message = dbus_message_new(DBUS_MESSAGE_TYPE_ERROR);
          dbus_message_set_reply_serial(message, dbus_message_get_serial(msg.message));
          dbus_message_set_error_name(message, exception.getId());
          dbus_message_set_destination(message, dbus_message_get_sender(msg.message));
        }

        Return::~Return()
        {
        }

        bool Return::isException() const
        {
          if(message)
            return dbus_message_get_type(message) == DBUS_MESSAGE_TYPE_ERROR;

          return false;
        }

        const char *Return::getId() const
        {
          const char* ret = NULL;

          if(message)
          {
            ret = dbus_message_get_error_name(message);
          }

          if(!ret)
            return DBUS_ERROR_INCONSISTENT_MESSAGE;

          return ret;
        }
      }

      Signal::Signal()
      {
        message = dbus_message_new(DBUS_MESSAGE_TYPE_SIGNAL);
      }

      Signal::Signal(DBusMessage *msg)
      {
        if(msg && dbus_message_get_type(msg) == DBUS_MESSAGE_TYPE_SIGNAL)
        {
          message = dbus_message_ref(msg);
        }
        else
        {
          message = NULL;
        }
      }

      Signal::Signal(const Signal& message) :
        Base(message)
      {
      }

      Signal::~Signal()
      {
      }

      const char *Signal::getPath() const
      {
        return dbus_message_get_path(message);
      }

      void Signal::setPath(const char *path)
      {
        if(!dbus_validate_path(path, NULL))
        {
          throw Message::Exception::SyntaxException();
        }

        dbus_message_set_path(message, path);
      }

      const char *Signal::getInterface() const
      {
        return dbus_message_get_interface(message);
      }

      void Signal::setInterface(const char *iface)
      {
        if(!dbus_validate_interface(iface, NULL))
        {
          throw Message::Exception::SyntaxException();
        }

        dbus_message_set_interface(message, iface);
      }

      const char *Signal::getMember() const
      {
        return dbus_message_get_member(message);
      }

      void Signal::setMember(const char *member)
      {
        if(!dbus_validate_member(member, NULL))
        {
          throw Message::Exception::SyntaxException();
        }

        dbus_message_set_member(message, member);
      }

      void Signal::check() const
      {
        Base::check();

        if(dbus_message_get_member(message) == NULL || dbus_message_get_interface(message) == NULL || dbus_message_get_path(message) == NULL)
        {
          Connection::get()->log(Overkiz::Log::Priority::OVK_ERROR, "Signal error, missing member or interface or path");
          throw Exception::MalformedMessage();
        }
      }

      SendReturn::SendReturn(const Message::Method::Return& reply) :
        ret(reply)
      {
      }

      SendReturn::~SendReturn()
      {
      }

      const Message::Method::Return& SendReturn::operator * () const
      {
        return ret;
      }

      const Message::Method::Return *SendReturn::operator -> () const
      {
        return &ret;
      }
    }
  }
}
