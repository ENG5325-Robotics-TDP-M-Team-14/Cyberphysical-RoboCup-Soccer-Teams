#include "unsaferubywrapper.h"
#include <iostream>

using namespace zeitgeist;

VALUE UnsafeRubyWrapper::RbFuncallWrap(VALUE arg)
{
    RbArguments &a = *reinterpret_cast<RbArguments*>(arg);
    return rb_funcall2(a.receiver, a.id, a.n, a.argv);
}

VALUE UnsafeRubyWrapper::RbEvalStringWrap(const std::string& str, int& error)
{
  VALUE v = rb_eval_string_protect(str.c_str(), &error);

  if (error)
    {
      RbPrintError();
      return Qnil;
    }

  return v;
}

std::string UnsafeRubyWrapper::RbGetError()
{
  VALUE mes = rb_inspect(rb_gv_get("$!"));
  return RSTRING_PTR(mes);
}

void UnsafeRubyWrapper::RbPrintError()
{
  std::cout << RbGetError().c_str() << std::endl;
  rb_backtrace();
}

GCValue UnsafeRubyWrapper::CallMethod(const std::string& className, const std::string& methodName)
{
  // get namespace class
  GCValue ns = rb_const_get(rb_cObject, rb_intern(className.c_str()));
  GCValue v;

  if (!ns.IsNil())
  {
    // get member variable of namespace object
    ID var = rb_intern(methodName.c_str());

    int error;
    RbArguments arg(ns.Get(), var, 0, 0);
    v = rb_protect(
      RbFuncallWrap,
      reinterpret_cast<VALUE>(&arg), &error
    );

    if (error)
    {
      std::cerr << "(RubyWrapper) Ruby ERROR: "
        << RbGetError() << std::endl;
        v = Qnil;
    }
  }

  return v;
}
