#ifndef ZEITGEIST_RUBYAPI_H
#define ZEITGEIST_RUBYAPI_H

//
// Both <ruby.h> and "config.h" define PACKAGE_ constants.
// To suppress compiler warnings about redefinitions they
// are #undef'ed in this wrapper
//

#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#ifndef __GNUC__
#define EXTERN extern __declspec(dllimport)
#endif

#ifdef WIN32
#include <winsock2.h>

#ifndef __MINGW32__
// disable compiler warning about type cast from VALUE to RBasic*
#pragma warning (disable : 4312)
#endif
#endif

#include <ruby.h>

#ifdef WIN32
#undef bind
#undef listen
#undef accept
#undef connect
#undef close
#undef recv
#undef socket
#undef send
#undef read
#undef write

#ifndef __MINGW32__
// reenable compiler warning
#pragma warning (default : 4312)
#endif
#endif

#undef EXTERN

#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#endif // ZEITGEIST_RUBYAPI_H
