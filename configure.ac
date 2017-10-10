#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([com.kizos.libbus], [1.4.1])
AC_CANONICAL_HOST
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([foreign -Wall subdir-objects])

# Checks for programs.
AM_PROG_AR
AC_PROG_CXX
LT_INIT

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h string.h])
AC_HEADER_STDBOOL

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([getpagesize memset])

# Add trailling spaces
test -n "${CPPFLAGS}" && CPPFLAGS+=" "
test -n "${CXXFLAGS}" && CXXFLAGS+=" "
test -n "${LIBS}"     && LIBS+=" "
test -n "${LDFLAGS}"  && LDFLAGS+=" "



#
# Kizbox framework
#
# The following variables set up some preprocessor macros you may use into you c/c++ source code or
# into the .in files.
#

FRAMEWORK_NAME='Bus'
FRAMEWORK_LT_VERSION_INFO='4:1:0'
APPLICATION_NAME="Test/${FRAMEWORK_NAME}"
APPLICATION_NAME_LC="$(echo ${APPLICATION_NAME} | sed 's/.*/\L&/')"

KIZBOX_FRAMEWORK_HOST_PREFIX='com.kizbox.Framework'
KIZBOX_FRAMEWORK_PATH_PREFIX='/com/kizbox/Framework'
FRAMEWORK_HOST="${KIZBOX_FRAMEWORK_HOST_PREFIX}.${FRAMEWORK_NAME}"
FRAMEWORK_PATH="${KIZBOX_FRAMEWORK_PATH_PREFIX}/${FRAMEWORK_NAME}"

OVERKIZ_APPLICATION_HOST_PREFIX="com.overkiz.Application"
OVERKIZ_APPLICATION_PATH_PREFIX="/com/overkiz/Application"
APPLICATION_HOST="${OVERKIZ_APPLICATION_HOST_PREFIX}.${APPLICATION_NAME}"
APPLICATION_PATH="${OVERKIZ_APPLICATION_PATH_PREFIX}/${APPLICATION_NAME}"

# Variables you may use as @MY_VAR@ into .in files:
AC_SUBST([FRAMEWORK_NAME])
AC_SUBST([FRAMEWORK_LT_VERSION_INFO])

AC_SUBST([KIZBOX_FRAMEWORK_HOST_PREFIX])
AC_SUBST([KIZBOX_FRAMEWORK_PATH_PREFIX])
AC_SUBST([FRAMEWORK_HOST])
AC_SUBST([FRAMEWORK_PATH])

AC_SUBST([APPLICATION_NAME])
AC_SUBST([APPLICATION_NAME_LC])
AC_SUBST([OVERKIZ_APPLICATION_HOST_PREFIX])
AC_SUBST([OVERKIZ_APPLICATION_PATH_PREFIX])
AC_SUBST([APPLICATION_HOST])
AC_SUBST([APPLICATION_PATH])

# C/C++ macros you may use into your source code:
AC_DEFINE_UNQUOTED([FRAMEWORK_NAME],                  ["${FRAMEWORK_NAME}"],                  [Defines framework name])
AC_DEFINE_UNQUOTED([KIZBOX_FRAMEWORK_HOST_PREFIX],    ["${KIZBOX_FRAMEWORK_HOST_PREFIX}"],    [Defines the host kizbox framework prefix])
AC_DEFINE_UNQUOTED([KIZBOX_FRAMEWORK_PATH_PREFIX],    ["${KIZBOX_FRAMEWORK_PATH_PREFIX}"],    [Defines the path kizbox framework prefix])
AC_DEFINE_UNQUOTED([FRAMEWORK_HOST],                  ["${FRAMEWORK_HOST}"],                  [Defines the host framework])
AC_DEFINE_UNQUOTED([FRAMEWORK_PATH],                  ["${FRAMEWORK_PATH}"],                  [Defines the path framework])

AC_DEFINE_UNQUOTED([APPLICATION_NAME],                ["${APPLICATION_NAME}"],                [Defines application name])
AC_DEFINE_UNQUOTED([APPLICATION_NAME_LC],             ["${APPLICATION_NAME_LC}"],             [Defines application name in lower case for log])
AC_DEFINE_UNQUOTED([OVERKIZ_APPLICATION_HOST_PREFIX], ["${OVERKIZ_APPLICATION_HOST_PREFIX}"], [Defines the host overkiz application prefix])
AC_DEFINE_UNQUOTED([OVERKIZ_APPLICATION_PATH_PREFIX], ["${OVERKIZ_APPLICATION_PATH_PREFIX}"], [Defines the path overkiz application prefix])
AC_DEFINE_UNQUOTED([APPLICATION_HOST],                ["${APPLICATION_HOST}"],                [Defines the host application])
AC_DEFINE_UNQUOTED([APPLICATION_PATH],                ["${APPLICATION_PATH}"],                [Defines the path application])

# Here you may add some application constants
# ...



#
# Pkg-config
#
# The following variables set up the pkg-config file.
#

PKGCONFIG_DESCRIPTION="Framework ${FRAMEWORK_NAME}"
PKGCONFIG_URL="https://github.com/Overkiz/libbus"
PKGCONFIG_REQUIRES=
PKGCONFIG_PRIVATE_REQUIRES=
PKGCONFIG_CONFLICTS=
PKGCONFIG_LIBDIRS="-L\${libdir}"
PKGCONFIG_LIBS="-l${FRAMEWORK_NAME}"
PKGCONFIG_PRIVATE_LIBS=
PKGCONFIG_INCDIRS="-I\${includedir}"
PKGCONFIG_DEFS=

AC_SUBST([PKGCONFIG_DESCRIPTION])
AC_SUBST([PKGCONFIG_URL])
AC_SUBST([PKGCONFIG_REQUIRES])
AC_SUBST([PKGCONFIG_PRIVATE_REQUIRES])
AC_SUBST([PKGCONFIG_CONFLICTS])
AC_SUBST([PKGCONFIG_LIBDIRS])
AC_SUBST([PKGCONFIG_LIBS])
AC_SUBST([PKGCONFIG_PRIVATE_LIBS])
AC_SUBST([PKGCONFIG_INCDIRS])
AC_SUBST([PKGCONFIG_DEFS])



# Checks for packages.

AC_ARG_ENABLE(
  [pkg-config],
  [AS_HELP_STRING([--disable-pkg-config],					[Disable pkg-config [default=no]])],
  [
    case "${enableval}" in
    yes) pkgconfig='true';;
    no)  pkgconfig='false';;
    *)   AC_MSG_ERROR([bad value ${enableval} for --disable-pkg-config]);;
    esac
  ],
  [
    pkgconfig='true'
  ]
  )
AM_CONDITIONAL([PKGCONFIG], [test x${pkgconfig} = xtrue])



AC_ARG_ENABLE(
  [pkg-config-check],
  [AS_HELP_STRING([--disable-pkg-config-check],				[Disable pkg-config check [default=no]])],
  [
    case "${enableval}" in
    yes) pkgconfigcheck='true';;
    no)  pkgconfigcheck='false';;
    *)   AC_MSG_ERROR([bad value ${enableval} for --disable-pkg-config-check]);;
    esac
  ],
  [
    pkgconfigcheck='true'
  ]
  )
if test "${pkgconfigcheck}" = 'true' ; then
  PKG_CHECK_MODULES(
    [libcore],
    [com.kizos.libcore >= 1.6],
    [
      test -n "${PKGCONFIG_REQUIRES}" && PKGCONFIG_REQUIRES+=", "
      PKGCONFIG_REQUIRES+="com.kizos.libcore >= 1.6"

      CPPFLAGS+=" $(pkg-config --cflags-only-other com.kizos.libcore) "
    ]
    )



  PKG_CHECK_MODULES(
    [libdbus_1],
    [dbus-1],
    [
      test -n "${PKGCONFIG_REQUIRES}" && PKGCONFIG_REQUIRES+=", "
      PKGCONFIG_REQUIRES+="dbus-1"

      CXXFLAGS+=" $(pkg-config --cflags-only-I dbus-1) "
    ]
    )
fi



AC_MSG_CHECKING([whether system bus])
AC_ARG_ENABLE(
  [system-bus],
  AS_HELP_STRING([--disable-system-bus],					[Disable system bus as default bus [default=no]]),
  [
    case "${enableval}" in
    yes) systembus='true';;
    no)  systembus='false';;
    *)   AC_MSG_ERROR([bad value ${withval} for --disable-system-bus]);;
    esac
  ],
  [
    systembus='true'
  ]
  )
AC_MSG_RESULT([${systembus}])
SYSTEM_BUS=${systembus}
AC_SUBST([SYSTEM_BUS])



AC_MSG_CHECKING([whether assertions are enabled])
AC_ARG_ENABLE(
  [assertions],
  [AC_HELP_STRING([--enable-assertions],					[Enable assertions [default=no]])],
  [
    case "${enableval}" in
    yes)  assert='true';;
    no)   assert='false';;
    *)    AC_MSG_ERROR([bad value ${enableval} for --enable-assertions]);;
    esac
  ],
  [
    assert='false'
  ]
  )
if test "${assert}" = 'true'; then
  AC_MSG_RESULT([yes])
else
  AC_MSG_RESULT([no])
  AC_DEFINE([NDEBUG],										[],										[Disable assertions])
fi



AC_CONFIG_FILES(
  [
    Makefile
    include/Makefile
    include/kizbox/framework/bus/Connection.h
    include/kizbox/framework/bus/Object.h
    src/Makefile
    pkg-config.pc
  ]
  )

AC_OUTPUT
