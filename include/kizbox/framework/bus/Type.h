/*
 * Type.h
 *
 *      Copyright (C) 2015 Overkiz SA.
 */

#ifndef BUSTYPE_H_
#define BUSTYPE_H_

#include <dbus/dbus.h>
#include <stdint.h>
#include <string>
#include <stdio.h>

#include <kizbox/framework/core/Date.h>
#include <kizbox/framework/core/Time.h>
#include <kizbox/framework/core/UniversalUniqueIdentifier.h>
#include <kizbox/framework/bus/Message.h>
#include <kizbox/framework/bus/Object.h>
#include <kizbox/framework/core/Node.h>
#include <kizbox/framework/core/Directory.h>

namespace Overkiz
{

  namespace Bus
  {

    namespace Type
    {

      template <typename T>
      class Wrapper
      {
      public:
        Wrapper() {}

        virtual ~Wrapper() {}

        virtual void serialize(Message::Iterator& iterator, const T& val) const = 0;
        virtual void deserialize(Message::ConstIterator& iterator, T& val) const = 0;

        virtual std::string signature() const = 0;
      };



      template <>
      class Wrapper <std::string>
      {
      public:
        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const std::string& val) const;

        virtual void deserialize(Message::ConstIterator& iterator, std::string& val) const;

        virtual std::string signature() const;
      };

      template <>
      class Wrapper <uint8_t>
      {
      public:
        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const uint8_t& val) const;

        virtual void deserialize(Message::ConstIterator& iterator, uint8_t& val) const;

        virtual std::string signature() const;
      };

      template <>
      class Wrapper <bool>
      {
      public:
        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const bool& val) const;

        virtual void deserialize(Message::ConstIterator& iterator, bool& val) const;

        virtual std::string signature() const;
      private:
      };

      template <>
      class Wrapper <int16_t>
      {
      public:
        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const int16_t& val) const;

        virtual void deserialize(Message::ConstIterator& iterator, int16_t& val) const;

        virtual std::string signature() const;
      private:
      };

      template <>
      class Wrapper <uint16_t>
      {
      public:
        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const uint16_t& val) const;

        virtual void deserialize(Message::ConstIterator& iterator, uint16_t& val) const;

        virtual std::string signature() const;
      private:
      };

      template <>
      class Wrapper <int32_t>
      {
      public:
        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const int32_t& val) const;

        virtual void deserialize(Message::ConstIterator& iterator, int32_t& val) const;

        virtual std::string signature() const;
      private:
      };

      template <>
      class Wrapper <uint32_t>
      {
      public:
        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const uint32_t& val) const;

        virtual void deserialize(Message::ConstIterator& iterator, uint32_t& val) const;

        virtual std::string signature() const;
      private:
      };



      template <>
      class Wrapper <int64_t>
      {
      public:
        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const int64_t& val) const;

        virtual void deserialize(Message::ConstIterator& iterator, int64_t& val) const;

        virtual std::string signature() const;
      private:
      };

      template <>
      class Wrapper <uint64_t>
      {
      public:
        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const uint64_t& val) const;

        virtual void deserialize(Message::ConstIterator& iterator, uint64_t& val) const;

        virtual std::string signature() const;
      private:
      };



      template <>
      class Wrapper <double>
      {
      public:
        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const double& val) const;

        virtual void deserialize(Message::ConstIterator& iterator, double& val) const;

        virtual std::string signature() const;
      private:
      };



      template <>
      class Wrapper <Overkiz::UniversalUniqueIdentifier>
      {
      public:

        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const Overkiz::UniversalUniqueIdentifier& val) const;

        virtual void deserialize(Message::ConstIterator& iterator, Overkiz::UniversalUniqueIdentifier& val) const;

        virtual std::string signature() const;

      };


      template <>
      class Wrapper <Overkiz::Date::Relative::Week>
      {
      public:

        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const Overkiz::Date::Relative::Week& val) const;

        virtual void deserialize(Message::ConstIterator& iterator, Overkiz::Date::Relative::Week& val) const;

        virtual std::string signature() const;

      };


      template <>
      class Wrapper< Overkiz::Time::Elapsed >
      {
      public:

        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Overkiz::Bus::Message::Iterator & iterator, const Overkiz::Time::Elapsed & value) const;

        virtual void deserialize(Overkiz::Bus::Message::ConstIterator & iterator, Overkiz::Time::Elapsed & value) const;

        std::string signature() const;

      };




      template <typename T, template <typename> class TW, template <typename, typename> class C, typename A = std::allocator<T> >
      class SequenceContainerWrapper : public Wrapper<C<T,A> >
      {
      public:
        SequenceContainerWrapper() {}

        virtual ~SequenceContainerWrapper() {}

        virtual void serialize(Message::Iterator& iterator, const C<T, A>& val) const
        {
          TW<T> wrapper;
          Message::Iterator iter = iterator.begin(DBUS_TYPE_ARRAY, wrapper.signature().c_str());

          for(typename C<T, A>::const_iterator i = val.begin(); i != val.end(); i++)
          {
            wrapper.serialize(iter, *i);
          }

          iterator.end(iter);
        }



        virtual void deserialize(Message::ConstIterator& iterator, C<T, A>& val) const
        {
          if(iterator.type() != DBUS_TYPE_ARRAY)
          {
            throw;
          }

          for(Message::ConstIterator iter = iterator.begin(); iter != iterator.end(); ++iter)
          {
            TW<T> wrapper;
            T entry;
            wrapper.deserialize(iter, entry);
            val.insert(val.end(), entry);
          }
        }

        virtual std::string signature() const
        {
          TW<T> wrapper;
          std::string sign(DBUS_TYPE_ARRAY_AS_STRING);
          sign += wrapper.signature();
          return sign;
        }



      private:
      };

      template <typename T, template <typename> class TW, int N >
      class FixedSequenceContainerWrapper : public Wrapper< T >
      {
      public:
        FixedSequenceContainerWrapper() {}

        virtual ~FixedSequenceContainerWrapper() {}

        virtual void serialize(Message::Iterator& iterator, const T & val) const
        {
          TW<T> wrapper;
          const T* valPtr = &val;
          Message::Iterator iter = iterator.begin(DBUS_TYPE_ARRAY, wrapper.signature().c_str());

          for(int i = 0; i < N; i++)
          {
            wrapper.serialize(iter, valPtr[i]);
          }

          iterator.end(iter);
        }



        virtual void deserialize(Message::ConstIterator& iterator, T & val) const
        {
          if(iterator.type() != DBUS_TYPE_ARRAY)
          {
            throw;
          }

          TW<T> wrapper;
          T* valPtr = &val;
          int i = 0;

          for(Message::ConstIterator iter = iterator.begin(); iter != iterator.end(), i < N; ++iter, ++i)
          {
            wrapper.deserialize(iter, valPtr[i]);
          }
        }

        virtual std::string signature() const
        {
          TW<T> wrapper;
          std::string sign(DBUS_TYPE_ARRAY_AS_STRING);
          sign += wrapper.signature();
          return sign;
        }
      };


      template <typename Type, template <typename> class TypeWrapper, template <typename, typename, typename> class Container, typename Compare = std::less<Type>, typename Allocator = std::allocator<Type> >
      class AssociativeContainerWrapper : public Wrapper<Container<Type, Compare, Allocator> >
      {
        AssociativeContainerWrapper() {}

        virtual ~AssociativeContainerWrapper() {}

        virtual void serialize(Message::Iterator& iterator, const Container<Type, Compare, Allocator>& val) const
        {
          TypeWrapper<Type> wrapper;
          Message::Iterator iter = iterator.begin(DBUS_TYPE_ARRAY, wrapper.signature().c_str());

          for(typename Container<Type, Compare, Allocator>::const_iterator i = val.begin(); i != val.end(); i++)
          {
            wrapper.serialize(iter, *i);
          }

          iterator.end(iter);
        }



        virtual void deserialize(Message::ConstIterator& iterator, Container<Type, Compare, Allocator>& val) const
        {
          if(iterator.type() != DBUS_TYPE_ARRAY)
          {
            throw;
          }

          for(Message::ConstIterator iter = iterator.begin(); iter != iterator.end(); ++iter)
          {
            TypeWrapper<Type> wrapper;
            Type entry;
            wrapper.deserialize(iter, entry);
            val.insert(entry, val.end());
          }
        }

        virtual std::string signature() const
        {
          TypeWrapper<Type> wrapper;
          std::string sign(DBUS_TYPE_ARRAY_AS_STRING);
          sign += wrapper.signature();
          return sign;
        }
      };



      template <typename K, typename T, template <typename> class KW, template <typename> class TW>
      class PairWrapper : public Wrapper<std::pair<K,T> >
      {
      public:

        PairWrapper() {}

        virtual ~PairWrapper() {}

        virtual void serialize(Message::Iterator& iterator, const std::pair<K, T>& val) const
        {
          Message::Iterator iter = iterator.begin(DBUS_TYPE_DICT_ENTRY);
          KW<K> keyWrapper;
          TW<T> typeWrapper;
          keyWrapper.serialize(iter, val.first);
          typeWrapper.serialize(iter, val.second);
          iterator.end(iter);
        }

        virtual void deserialize(Message::ConstIterator& iterator, std::pair<K, T>& val) const
        {
          if(iterator.type() != DBUS_TYPE_DICT_ENTRY)
          {
            throw;
          }

          Message::ConstIterator iter = iterator.begin();
          KW<K> keyWrapper;
          TW<T> typeWrapper;
          keyWrapper.deserialize(iter, val.first);
          ++iter;
          typeWrapper.deserialize(iter, val.second);
        }

        virtual std::string signature() const
        {
          KW<K> keyWrapper;
          TW<T> typeWrapper;
          std::string sign(DBUS_DICT_ENTRY_BEGIN_CHAR_AS_STRING);
          sign += keyWrapper.signature();
          sign += typeWrapper.signature();
          sign += DBUS_DICT_ENTRY_END_CHAR_AS_STRING;
          return sign;
        }

      };

      template <typename Key, typename Type, template <typename> class KeyWrapper, template <typename> class TypeWrapper, template <typename, typename, typename, typename> class Container, typename Compare = std::less<Type>, typename Allocator = std::allocator<std::pair<const Key,Type> > >
      class AssociativePairContainerWrapper : public Wrapper<Container<Key, Type, Compare, Allocator> >
      {
      public:
        AssociativePairContainerWrapper() {}

        virtual ~AssociativePairContainerWrapper() {}

        virtual void serialize(Message::Iterator& iterator, const Container<Key, Type, Compare, Allocator>& val) const
        {
          PairWrapper<Key, Type, KeyWrapper, TypeWrapper> wrapper;
          Message::Iterator arrayIter = iterator.begin(DBUS_TYPE_ARRAY, wrapper.signature().c_str());

          for(typename Container<Key, Type, Compare, Allocator>::const_iterator i = val.begin(); i != val.end(); i++)
          {
            wrapper.serialize(arrayIter, *i);
          }

          iterator.end(arrayIter);
        }


        virtual void deserialize(Message::ConstIterator& iterator, Container<Key, Type, Compare, Allocator>& val) const
        {
          if(iterator.type() != DBUS_TYPE_ARRAY)
          {
            throw;
          }

          for(Message::ConstIterator arrayIter = iterator.begin(); arrayIter != iterator.end(); ++arrayIter)
          {
            PairWrapper<Key, Type, KeyWrapper, TypeWrapper> wrapper;
            std::pair<Key, Type> entry;
            wrapper.deserialize(arrayIter, entry);
            val.insert(entry);
          }
        }

        virtual std::string signature() const
        {
          Wrapper<Key> keyWrapper;
          Wrapper<Type> typeWrapper;
          std::string sign(DBUS_TYPE_ARRAY_AS_STRING);
          sign += DBUS_DICT_ENTRY_BEGIN_CHAR_AS_STRING;
          sign += keyWrapper.signature();
          sign += typeWrapper.signature();
          sign += DBUS_DICT_ENTRY_END_CHAR_AS_STRING;
          return sign;
        }

      private:
      };

      class VariantHandler
      {
      public:
        VariantHandler() {}

        virtual ~VariantHandler() {}

        virtual void serialize(Message::Iterator& iterator) const = 0;
        virtual void deserialize(Message::ConstIterator& iterator, const std::string& sign) = 0;
        virtual std::string signature() const = 0;
      };


      template <>
      class Wrapper <VariantHandler>
      {
      public:
        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const VariantHandler& val) const;

        virtual void deserialize(Message::ConstIterator& iterator, VariantHandler& val) const;

        virtual std::string signature() const;
      private:

      };


      template <typename T>
      class StructWrapper
      {
      public:
        StructWrapper() {}

        virtual ~StructWrapper() {}

        virtual void serialize(Message::Iterator& iterator, const T& val) const
        {
          Message::Iterator iter = iterator.begin(DBUS_TYPE_STRUCT);
          Wrapper<T> wrapper;
          wrapper.serialize(iter, val);
          iterator.end(iter);
        }

        virtual void deserialize(Message::ConstIterator& iterator, T& val) const
        {
          if(iterator.type() != DBUS_TYPE_STRUCT)
          {
            throw;
          }

          Message::ConstIterator iter = iterator.begin();
          Wrapper<T> wrapper;
          wrapper.deserialize(iter, val);
        }

        virtual std::string signature() const
        {
          Wrapper<T> wrapper;
          std::string sign(DBUS_STRUCT_BEGIN_CHAR_AS_STRING);
          sign += wrapper.signature() + DBUS_STRUCT_END_CHAR_AS_STRING;
          return sign;
        }


      private:
      };



      template <>
      class Wrapper <Object::Server>
      {
      public:
        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const Object::Server& val) const;

        virtual void deserialize(Message::ConstIterator& , Object::Server&) const;

        virtual std::string signature() const;
      private:
      };

      template <>
      class Wrapper <Object::Proxy>
      {
      public:
        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& , const Object::Proxy&) const;

        virtual void deserialize(Message::ConstIterator& iterator, Object::Proxy& val) const;

        virtual std::string signature() const;
      private:
      };


      template <>
      class Wrapper <FileSystem::Node>
      {
      public:
        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const FileSystem::Node & val) const;

        virtual void deserialize(Message::ConstIterator& iterator, FileSystem::Node & val) const;

        virtual std::string signature() const;
      };

      template <>
      class Wrapper <FileSystem::Directory>
      {
      public:
        Wrapper();

        virtual ~Wrapper();

        virtual void serialize(Message::Iterator& iterator, const FileSystem::Directory & val) const;

        virtual void deserialize(Message::ConstIterator& iterator, FileSystem::Directory & val) const;

        virtual std::string signature() const;
      };

    }
  }
}

#endif /* BUSTYPE_H_ */
