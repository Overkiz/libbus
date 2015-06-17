#
# Framework
#
lib_LTLIBRARIES = libBus.la

libBus_la_SOURCES =   Message.cpp \
                      Object.cpp \
                      Connection.cpp \
                      Type.cpp \
                      freedesktop/dbus/bus/Manager.cpp
libBus_la_CXXFLAGS =  -Werror -std=c++0x \
                      -I$(top_builddir)/include \
                      -I$(top_builddir)/include/kizbox/framework/bus \
                      -I$(top_srcdir)/include \
                      -I$(top_srcdir)/include/kizbox/framework/bus
libBus_la_LIBADD =    -lCore -ldbus-1
libBus_la_LDFLAGS =   -version-info $(FRAMEWORK_LT_VERSION_INFO)