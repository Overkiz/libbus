/*
 * Type.cpp
 *
 *      Copyright (C) 2015 Overkiz SA.
 */

#include "Type.h"

namespace Overkiz
{
  namespace Bus
  {
    namespace Type
    {

      Wrapper<std::string>::Wrapper() {}

      Wrapper<std::string>::~Wrapper() {}

      void Wrapper<std::string>::serialize(Message::Iterator& iterator, const std::string& val) const
      {
        const char *str = val.c_str();
        iterator.push(str, DBUS_TYPE_STRING);
      }

      void Wrapper<std::string>::deserialize(Message::ConstIterator& iterator, std::string& val) const
      {
        iterator.get(val);
      }

      std::string Wrapper<std::string>::signature() const
      {
        return DBUS_TYPE_STRING_AS_STRING;
      }


      Wrapper<uint8_t>::Wrapper() {}

      Wrapper<uint8_t>::~Wrapper() {}

      void Wrapper<uint8_t>::serialize(Message::Iterator& iterator, const uint8_t& val) const
      {
        iterator.push(val);
      }

      void Wrapper<uint8_t>::deserialize(Message::ConstIterator& iterator, uint8_t& val) const
      {
        iterator.get(val);
      }

      std::string Wrapper<uint8_t>::signature() const
      {
        return DBUS_TYPE_BYTE_AS_STRING;
      }

      Wrapper<bool>::Wrapper() {}

      Wrapper<bool>::~Wrapper() {}

      void Wrapper<bool>::serialize(Message::Iterator& iterator, const bool& val) const
      {
        iterator.push(val);
      }

      void Wrapper<bool>::deserialize(Message::ConstIterator& iterator, bool& val) const
      {
        iterator.get(val);
      }

      std::string Wrapper<bool>::signature() const
      {
        return DBUS_TYPE_BOOLEAN_AS_STRING;
      }

      Wrapper<int16_t>::Wrapper() {}

      Wrapper<int16_t>::~Wrapper() {}

      void Wrapper<int16_t>::serialize(Message::Iterator& iterator, const int16_t& val) const
      {
        iterator.push(val);
      }

      void Wrapper<int16_t>::deserialize(Message::ConstIterator& iterator, int16_t& val) const
      {
        iterator.get(val);
      }

      std::string Wrapper<int16_t>::signature() const
      {
        return DBUS_TYPE_INT16_AS_STRING;
      }


      Wrapper<uint16_t>::Wrapper() {}

      Wrapper<uint16_t>::~Wrapper() {}

      void Wrapper<uint16_t>::serialize(Message::Iterator& iterator, const uint16_t& val) const
      {
        iterator.push(val);
      }

      void Wrapper<uint16_t>::deserialize(Message::ConstIterator& iterator, uint16_t& val) const
      {
        iterator.get(val);
      }

      std::string Wrapper<uint16_t>::signature() const
      {
        return DBUS_TYPE_UINT16_AS_STRING;
      }

      Wrapper<int32_t>::Wrapper() {}

      Wrapper<int32_t>::~Wrapper() {}

      void Wrapper<int32_t>::serialize(Message::Iterator& iterator, const int32_t& val) const
      {
        iterator.push(val);
      }

      void Wrapper<int32_t>::deserialize(Message::ConstIterator& iterator, int32_t& val) const
      {
        iterator.get(val);
      }

      std::string Wrapper<int32_t>::signature() const
      {
        return DBUS_TYPE_INT32_AS_STRING;
      }

      Wrapper<uint32_t>::Wrapper() {}

      Wrapper<uint32_t>::~Wrapper() {}

      void Wrapper<uint32_t>::serialize(Message::Iterator& iterator, const uint32_t& val) const
      {
        iterator.push(val);
      }

      void Wrapper<uint32_t>::deserialize(Message::ConstIterator& iterator, uint32_t& val) const
      {
        iterator.get(val);
      }

      std::string Wrapper<uint32_t>::signature() const
      {
        return DBUS_TYPE_UINT32_AS_STRING;
      }


      Wrapper<int64_t>::Wrapper() {}

      Wrapper<int64_t>::~Wrapper() {}

      void Wrapper<int64_t>::serialize(Message::Iterator& iterator, const int64_t& val) const
      {
        iterator.push(val);
      }

      void Wrapper<int64_t>::deserialize(Message::ConstIterator& iterator, int64_t& val) const
      {
        iterator.get(val);
      }

      std::string Wrapper<int64_t>::signature() const
      {
        return DBUS_TYPE_INT64_AS_STRING;
      }

      Wrapper<uint64_t>::Wrapper() {}

      Wrapper<uint64_t>::~Wrapper() {}

      void Wrapper<uint64_t>::serialize(Message::Iterator& iterator, const uint64_t& val) const
      {
        iterator.push(val);
      }

      void Wrapper<uint64_t>::deserialize(Message::ConstIterator& iterator, uint64_t& val) const
      {
        iterator.get(val);
      }

      std::string Wrapper<uint64_t>::signature() const
      {
        return DBUS_TYPE_UINT64_AS_STRING;
      }

      Wrapper<double>::Wrapper() {}

      Wrapper<double>::~Wrapper() {}

      void Wrapper<double>::serialize(Message::Iterator& iterator, const double& val) const
      {
        iterator.push(val);
      }

      void Wrapper<double>::deserialize(Message::ConstIterator& iterator, double& val) const
      {
        iterator.get(val);
      }

      std::string Wrapper<double>::signature() const
      {
        return DBUS_TYPE_DOUBLE_AS_STRING;
      }

      Wrapper<Overkiz::UniversalUniqueIdentifier>::Wrapper() {}

      Wrapper<Overkiz::UniversalUniqueIdentifier>::~Wrapper() {}

      void Wrapper<Overkiz::UniversalUniqueIdentifier>::serialize(Message::Iterator& iterator, const Overkiz::UniversalUniqueIdentifier& val) const
      {
        std::string string;
        val.get(string);
        Wrapper<std::string> wstring;
        wstring.serialize(iterator, string);
      }

      void Wrapper<Overkiz::UniversalUniqueIdentifier>::deserialize(Message::ConstIterator& iterator, Overkiz::UniversalUniqueIdentifier& val) const
      {
        std::string string;
        Wrapper<std::string> wstring;
        wstring.deserialize(iterator, string);
        val.build(string);
      }

      std::string Wrapper<Overkiz::UniversalUniqueIdentifier>::signature() const
      {
        Wrapper<std::string> wstring;
        return wstring.signature();
      }



      Wrapper<Overkiz::Date::Relative::Week>::Wrapper() {}

      Wrapper<Overkiz::Date::Relative::Week>::~Wrapper() {}


      void Wrapper<Overkiz::Date::Relative::Week>::serialize(Message::Iterator& iterator, const Overkiz::Date::Relative::Week& val) const
      {
        uint32_t uinteger32;
        Wrapper<uint32_t> wuinteger32;
        uinteger32 = val.day;
        wuinteger32.serialize(iterator, uinteger32);
        uinteger32 = val.hour;
        wuinteger32.serialize(iterator, uinteger32);
        uinteger32 = val.minute;
        wuinteger32.serialize(iterator, uinteger32);
        uinteger32 = val.second;
        wuinteger32.serialize(iterator, uinteger32);
        uinteger32 = val.nanosecond;
        wuinteger32.serialize(iterator, uinteger32);
      }

      void Wrapper<Overkiz::Date::Relative::Week>::deserialize(Message::ConstIterator& iterator, Overkiz::Date::Relative::Week& val) const
      {
        uint32_t uinteger32;
        Wrapper<uint32_t> wuinteger32;
        wuinteger32.deserialize(iterator, uinteger32);
        ++ iterator;
        val.day = uinteger32;
        wuinteger32.deserialize(iterator, uinteger32);
        ++ iterator;
        val.hour = uinteger32;
        wuinteger32.deserialize(iterator, uinteger32);
        ++ iterator;
        val.minute = uinteger32;
        wuinteger32.deserialize(iterator, uinteger32);
        ++ iterator;
        val.second = uinteger32;
        wuinteger32.deserialize(iterator, uinteger32);
        //          ++ iterator;
        val.nanosecond = uinteger32;
      }

      std::string Wrapper<Overkiz::Date::Relative::Week>::signature() const
      {
        Wrapper<uint32_t> uinteger32;
        return uinteger32.signature()
               + uinteger32.signature()
               + uinteger32.signature()
               + uinteger32.signature()
               + uinteger32.signature();
      }

      Wrapper<Overkiz::Time::Elapsed>::Wrapper() {}

      Wrapper<Overkiz::Time::Elapsed>::~Wrapper() {}

      void Wrapper<Overkiz::Time::Elapsed>::serialize(Overkiz::Bus::Message::Iterator & iterator, const Overkiz::Time::Elapsed & value) const
      {
        int32_t integer32;
        integer32 = value.seconds;
        iterator.push(integer32);
        integer32 = value.nanoseconds;
        iterator.push(integer32);
      }

      void Wrapper<Overkiz::Time::Elapsed>::deserialize(Overkiz::Bus::Message::ConstIterator & iterator, Overkiz::Time::Elapsed & value) const
      {
        int32_t seconds;
        iterator.get(seconds);
        value.seconds = seconds;
        ++ iterator;
        int32_t nanoseconds;
        iterator.get(nanoseconds);
        value.nanoseconds = nanoseconds;
      }

      std::string Wrapper<Overkiz::Time::Elapsed>::signature() const
      {
        Overkiz::Bus::Type::Wrapper< int32_t > wrapperInteger32;
        return wrapperInteger32.signature()
               + wrapperInteger32.signature();
      }

      Wrapper<VariantHandler>::Wrapper() {}

      Wrapper<VariantHandler>::~Wrapper() {}

      void Wrapper<VariantHandler>::serialize(Message::Iterator& iterator, const VariantHandler& val) const
      {
        Message::Iterator iter = iterator.begin(DBUS_TYPE_VARIANT, val.signature().c_str());
        val.serialize(iter);
        iterator.end(iter);
      }

      void Wrapper<VariantHandler>::deserialize(Message::ConstIterator& iterator, VariantHandler& val) const
      {
        if(iterator.type() != DBUS_TYPE_VARIANT)
        {
          throw;
        }

        Message::ConstIterator iter = iterator.begin();
        val.deserialize(iter, iter.signature());
      }

      std::string Wrapper<VariantHandler>::signature() const
      {
        return DBUS_TYPE_VARIANT_AS_STRING;
      }

      Wrapper<Object::Server>::Wrapper() {}

      Wrapper<Object::Server>::~Wrapper() {}

      void Wrapper<Object::Server>::serialize(Message::Iterator& iterator, const Object::Server& val) const
      {
        iterator.push(val.getPath().c_str(), DBUS_TYPE_OBJECT_PATH);
        iterator.push(val.getHost(), DBUS_TYPE_STRING);
      }

      void Wrapper<Object::Server>::deserialize(Message::ConstIterator& , Object::Server&) const
      {
        throw;
      }

      std::string Wrapper<Object::Server>::signature() const
      {
        return DBUS_TYPE_OBJECT_PATH_AS_STRING
               DBUS_TYPE_STRING_AS_STRING;
      }

      Wrapper<Object::Proxy>::Wrapper() {}

      Wrapper<Object::Proxy>::~Wrapper() {}


      void Wrapper<Object::Proxy>::serialize(Message::Iterator& , const Object::Proxy&) const
      {
        throw;
      }

      void Wrapper<Object::Proxy>::deserialize(Message::ConstIterator& iterator, Object::Proxy& val) const
      {
        if(iterator.type() != DBUS_TYPE_OBJECT_PATH)
        {
          throw;
        }

        iterator.get(val.path);
        ++iterator;

        if(iterator.type() != DBUS_TYPE_STRING)
        {
          throw;
        }

        iterator.get(val.host);
      }

      std::string Wrapper<Object::Proxy>::signature() const
      {
        return DBUS_TYPE_OBJECT_PATH_AS_STRING
               DBUS_TYPE_STRING_AS_STRING;
      }

      Wrapper<FileSystem::Node>::Wrapper() {}

      Wrapper<FileSystem::Node>::~Wrapper() {}

      void Wrapper<FileSystem::Node>::serialize(Message::Iterator& iterator, const FileSystem::Node & val) const
      {
        Wrapper<std::string> stringWrapper;
        Wrapper<uint32_t> uint32_tWrapper;
        stringWrapper.serialize(iterator, val.getName());
        stringWrapper.serialize(iterator, val.getPath());
        uint32_tWrapper.serialize(iterator, val.getMode());
      }

      void Wrapper<FileSystem::Node>::deserialize(Message::ConstIterator& iterator, FileSystem::Node & val) const
      {
        Wrapper<std::string> stringWrapper;
        Wrapper<uint32_t> uint32_tWrapper;
        std::string tmpString;
        uint32_t tmpUint32_t;
        stringWrapper.deserialize(iterator, tmpString);
        ++iterator;
        val.getName() = tmpString;
        stringWrapper.deserialize(iterator, tmpString);
        ++iterator;
        val.getPath() = tmpString;
        uint32_tWrapper.deserialize(iterator, tmpUint32_t);
        val.getMode() = tmpUint32_t;
      }

      std::string Wrapper<FileSystem::Node>::signature() const
      {
        Wrapper<std::string> stringWrapper;
        Wrapper<uint32_t> uint32_tWrapper;
        return (stringWrapper.signature() + stringWrapper.signature() + uint32_tWrapper.signature());
      }

      Wrapper<FileSystem::Directory>::Wrapper() {}

      Wrapper<FileSystem::Directory>::~Wrapper() {}

      void Wrapper<FileSystem::Directory>::serialize(Message::Iterator& iterator, const FileSystem::Directory & val) const
      {
        Wrapper<std::string> stringWrapper;
        Wrapper<uint32_t>  uint32_tWrapper;
        stringWrapper.serialize(iterator, val.getName());
        stringWrapper.serialize(iterator, val.getPath());
        uint32_tWrapper.serialize(iterator, val.getMode());
      }

      void Wrapper<FileSystem::Directory>::deserialize(Message::ConstIterator& iterator, FileSystem::Directory & val) const
      {
        Wrapper<std::string> stringWrapper;
        Wrapper<uint32_t>  uint32_tWrapper;
        std::string tmpString;
        uint32_t tmpUint32_t;
        stringWrapper.deserialize(iterator, tmpString);
        ++iterator;
        val.getName() = tmpString;
        stringWrapper.deserialize(iterator, tmpString);
        ++iterator;
        val.getPath() = tmpString;
        uint32_tWrapper.deserialize(iterator, tmpUint32_t);
        val.getMode() = tmpUint32_t;
      }

      std::string Wrapper<FileSystem::Directory>::signature() const
      {
        Wrapper<std::string> stringWrapper;
        Wrapper<uint32_t>  uint32_tWrapper;
        return (stringWrapper.signature() + stringWrapper.signature() + uint32_tWrapper.signature());
      }
    }
  }
}
