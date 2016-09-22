/*
 * Message.h
 *
 *      Copyright (C) 2015 Overkiz SA.
 */

#ifndef BUSMESSAGE_H_
#define BUSMESSAGE_H_

#include <dbus/dbus.h>
#include <stdbool.h>
#include <stdint.h>
#include <string>
#include <list>
#include <stdint.h>
#include <string.h>

#include <kizbox/framework/core/Coroutine.h>
#include <kizbox/framework/core/Exception.h>
#include <kizbox/framework/core/Time.h>

namespace Overkiz
{

  namespace Bus
  {

    class Connection;

    namespace Message
    {

      namespace Exception
      {

        class SyntaxException : public Overkiz::Exception
        {
        public:

          virtual ~SyntaxException()
          {
          }

          const char * getId() const
          {
            return "com.kizbox.Framework.Bus.Connection.SyntaxException";
          }

        };

        class MalformedMessage : public Overkiz::Exception
        {
        public:

          MalformedMessage()
          {
          }

          virtual ~MalformedMessage()
          {
          }

          const char * getId() const
          {
            return DBUS_ERROR_INVALID_ARGS;
          }

        };

        class UnknownMethod : public Overkiz::Exception
        {
        public:

          UnknownMethod()
          {
          }

          virtual ~UnknownMethod()
          {
          }

          const char * getId() const
          {
            return DBUS_ERROR_UNKNOWN_METHOD;
          }

        };

        class MismatchSignature : public Overkiz::Exception
        {
        public:

          MismatchSignature(const std::string & matchSignature)
          {
            matchedSignatures.push_back(matchSignature);
          }

          MismatchSignature(const std::initializer_list< std::string > & matchedSignatures = std::initializer_list< std::string >()) :
            matchedSignatures(matchedSignatures)
          {
          }

          virtual ~MismatchSignature()
          {
          }

          const std::list< std::string > & getDesiredType() const
          {
            return matchedSignatures;
          }

          void add(const std::string & signature)
          {
            matchedSignatures.push_back(signature);
          }

          const char * getId() const
          {
            return "com.kizbox.Framework.Bus.Error.MismatchSignature";
          }

        protected:

          std::list< std::string > matchedSignatures;

        };

        class MismatchType : public Overkiz::Exception
        {
        public:

          MismatchType(int type, int desiredType) :
            type(type)
          {
            desiredTypes.push_back(desiredType);
          }

          MismatchType(int type, const std::initializer_list< int > & desiredTypes = std::initializer_list< int >()) :
            type(type),
            desiredTypes(desiredTypes)
          {
          }

          virtual ~MismatchType()
          {
          }

          int getArgumentType() const
          {
            return type;
          }

          const std::list< int > & getDesiredType() const
          {
            return desiredTypes;
          }

          void addDesiredType(int desiredType)
          {
            desiredTypes.push_back(desiredType);
          }

          const char * getId() const
          {
            return "com.kizbox.Framework.Bus.Message.MismatchedType";
          }

        private:

          int type;
          std::list< int > desiredTypes;

        };

        class NilMessageReference : public Overkiz::Exception
        {
        public:

          NilMessageReference()
          {
          }

          virtual ~NilMessageReference()
          {
          }

          const char * getId() const
          {
            return "com.kizbox.Framework.Bus.Message.NilReference";
          }

        };

      }

      class ConstIterator
      {
      public:
        ConstIterator();

        virtual ~ConstIterator();

        ConstIterator& operator ++ ();

        int type();

        const char *signature();

        ConstIterator begin();

        ConstIterator end();

        void get(const char *& val);

        void get(std::string& val);

        void get(uint8_t& val);

        void get(bool& val);

        void get(int16_t& val);

        void get(uint16_t& val);

        void get(int32_t& val);

        void get(uint32_t& val);

        void get(int64_t& val);

        void get(uint64_t& val);

        void get(double& val);

        bool operator != (const ConstIterator& it) const;

      private:
        DBusMessageIter iterator;
        std::string sign;
        friend class Base;
      };

      class Iterator
      {
      public:
        Iterator();

        virtual ~Iterator();

        bool operator != (const Iterator& it) const;

        Iterator begin(int type, const char *signature);

        Iterator begin(int type);

        void end(Iterator& it);

        void push(const char * val, int type);

        void push(uint8_t val);

        void push(bool val);

        void push(int16_t val);

        void push(uint16_t val);

        void push(int32_t val);

        void push(uint32_t val);

        void push(int64_t val);

        void push(uint64_t val);

        void push(double val);

      private:
        DBusMessageIter iterator;

        friend class Base;
      };

      class Base
      {
      public:
        Base();
        Base(const Base& msg);
        virtual ~Base();

        Iterator begin();
        ConstIterator begin() const;

        Iterator end();
        ConstIterator end() const;

        Base& operator = (const Base& msg);

        const char *getSender() const;

        void setSender(const char *sender);

        const char *getDestination() const;

        void setDestination(const char *);

        const char *getSignature() const;

        virtual void check() const;

      protected:
        DBusMessage *message;

      private:
        Base(DBusMessage *msg);

        friend class Overkiz::Bus::Connection;
      };

      namespace Method
      {
        class Call : public Base
        {
        public:
          Call();
          Call(const Call& message);

          virtual ~Call();

          const char *getPath() const;

          void setPath(const char *path);

          const char *getInterface() const;

          void setInterface(const char *iface);

          const char *getMember() const;

          void setMember(const char *member);

          void check() const;

          void setTimeout(const Overkiz::Time::Elapsed & timeout);

          const Overkiz::Time::Elapsed &getTimeout() const;

        private:
          Call(DBusMessage *msg);

          Overkiz::Time::Elapsed _timeout;

          friend class Overkiz::Bus::Connection;
          friend class Return;
        };

        class Return : public Base, public Overkiz::Exception
        {
        public:
          Return();
          Return(const Return& message);
          Return(const Call& message);
          Return(const Call& message, const Overkiz::Exception& exception);

          virtual ~Return();

          bool isException() const;

          const char *getId() const;

        private:
          Return(DBusMessage *msg);

          friend class Overkiz::Bus::Connection;
        };

      }

      class Signal : public Base
      {
      public:
        Signal();

        Signal(const Signal& message);

        virtual ~Signal();

        const char *getPath() const;

        void setPath(const char *path);

        const char *getInterface() const;

        void setInterface(const char *iface);

        const char *getMember() const;

        void setMember(const char *member);

        void check() const;

      private:
        Signal(DBusMessage *msg);

        friend class Overkiz::Bus::Connection;
      };

      class SendReturn
      {
      public:
        SendReturn(const Message::Method::Return& ret);
        virtual ~SendReturn();

        const Message::Method::Return& operator * () const;

        const Message::Method::Return *operator -> () const;

      private:
        const Message::Method::Return ret;
      };

    }

  }
}

#endif /* BUSMESSAGE_H_ */
