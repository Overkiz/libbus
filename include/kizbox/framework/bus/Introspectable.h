/*
 * Introspectable.h
 *
 *      Copyright (C) 2015 Overkiz SA.
 */

#ifndef INTROSPECTABLE_H_
#define INTROSPECTABLE_H_

#include <string>

namespace Overkiz
{
  namespace Bus
  {
#define INTROSPECTION_HEAD \
  "<!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\"\n " \
  "\"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\">\n"

#define INTROSPECTABLE_INTERFACE "org.freedesktop.DBus.Introspectable"

#define INTROSPECT_METHOD "Introspect"

    class IntrospectableInterface
    {
    public:
      IntrospectableInterface() {}
      virtual ~IntrospectableInterface() {}

      virtual void introspect(std::string& definition) = 0;
    private:


    };

  }
}

#endif /* INTROSPECTABLE_H_ */
