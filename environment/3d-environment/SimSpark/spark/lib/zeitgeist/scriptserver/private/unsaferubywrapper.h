#ifndef ZEITGEIST_UNSAFERUBYWRAPPER_H
#define ZEITGEIST_UNSAFERUBYWRAPPER_H

#include <string>
#include "gcvalue.h"
#include "rubyapi.h"

namespace zeitgeist
{
/**
 * UnsafeRubyWrapper provides a wrapped access to the Ruby C API.
 * These functions are not thread safe. Use RubyWrapper instead,
 * if you can't ensure that these functions are only executed
 * on the Ruby thread.
 */
class UnsafeRubyWrapper
{
public:
    /** RbArguments is a structure that describes a ruby function
        call.
        \param receiver is the ruby object that receives the function call
        \param id is the ruby id of the receiver member function
        \param n is the number of parameters passed
        \param argv is a pointer to an array containnig the function
        parameters
    */
    struct RbArguments
    {
        VALUE receiver;
        ID id;
        int n;
        VALUE *argv;

        RbArguments(VALUE r, ID id, int n, VALUE *argv) :
            receiver(r), id(id), n(n), argv(argv) {};
    };

    /** calls a method on the given class */
    static GCValue CallMethod(const std::string& className, const std::string& methodName);

    /** defines a global function with the given name */
    template<typename... Args> static void DefineGlobalFunction(const std::string& name, VALUE (*func) (Args...))
    {
        rb_define_global_function(name.c_str(), func, sizeof...(Args) - 1);
    }

    /** calls a safe rb_eval_string variant and prints any ruby error
        messages along with a backtrace to stdout. The error code
        returned from ruby is stored in the 'error' parameter.
     */
    static VALUE RbEvalStringWrap(const std::string& str, int& error);

    /** a functor for the rb_protect function, used to safely excecute
        ruby code */
    static VALUE RbFuncallWrap(VALUE arg);

    /** queries ruby for a string that describes the last error */
    static std::string RbGetError();

    /** prints the last ruby error to stdout along with a backtrace */
    static void RbPrintError();
};
}

#endif // ZEITGEIST_UNSAFERUBYWRAPPER_H
