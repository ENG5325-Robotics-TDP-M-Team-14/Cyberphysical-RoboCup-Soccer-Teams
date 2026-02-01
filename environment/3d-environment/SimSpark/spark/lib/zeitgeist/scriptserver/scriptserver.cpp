/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id$

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include <any>
#include <sstream>
#include <salt/fileclasses.h>
#include <zeitgeist/core.h>
#include <zeitgeist/corecontext.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <sys/stat.h>
#include "private/gcvalue.h"
#include "rubywrapper.h"
#include "scriptserver.h"

#ifdef HAVE_CONFIG_H
#include <sparkconfig.h>
#endif

using namespace std;
using namespace zeitgeist;

std::shared_ptr<CoreContext> gMyPrivateContext;

//
// Helper functions for built-in commands
//

static void
getParameterList(VALUE args, ParameterList& params)
{
    int argc = RARRAY_LEN(args);

    for (int i = 0; i<argc; ++i)
    {
        VALUE argument = rb_ary_entry(args, i);
        std::any var;

        // do type conversion
        switch (TYPE(argument))
        {
        case T_STRING:
        {
            char *c = STR2CSTR(argument);
            var = c;
            //printf("string: '%s'\n",std::any_cast<char*>(var));
        }
        break;
        case T_FIXNUM:
        {
            int i = FIX2INT(argument);
            var = i;
            //printf("int: '%d'\n", std::any_cast<int>(var));
        }
        break;
        case T_FLOAT:
        {
            float f = (float)NUM2DBL(argument);
            var = f;
            //printf("float: '%f'\n", std::any_cast<float>(var));
        }
        break;
        case T_TRUE:
        {
            var = true;
            //printf("bool: 'true'\n");
        }
        break;
        case T_FALSE:
        {
            var = false;
            //printf("bool: 'false'\n");
        }
        break;
        }

        params.AddValue(var);
    }
}

/**
 * Constructs the ZeitgeistObject corresponding to a given leaf.
 * Do not call this method outside the Ruby thread!
 */
GCValue
getZeitgeistObject(std::shared_ptr<Leaf> leaf)
{
    GCValue v;

    if (leaf.get() != 0)
    {
        stringstream ss;
        ss << "ZeitgeistObject.new(" << (unsigned long) leaf.get() <<")";
        int error;
        v = UnsafeRubyWrapper::RbEvalStringWrap(ss.str(), error);
    }

    return v;
}

//
// Built-in commands
//

static VALUE
selectObject(VALUE /*self*/, VALUE path)
{
    std::shared_ptr<Leaf> leaf = gMyPrivateContext->Select(STR2CSTR(path));
    return getZeitgeistObject(leaf).Get();
}

static VALUE
selectCall(VALUE /*self*/, VALUE functionName, VALUE args)
{
    ParameterList in;
    getParameterList(args, in);

    Class::TCmdProc cmd =
        gMyPrivateContext->GetSelection()->GetClass()->GetCmdProc
        (STR2CSTR(functionName));

    GCValue out;

    if (cmd != 0)
    {
        out = cmd(static_cast<Object*>(gMyPrivateContext->GetSelection().get()), in);
    } else
    {
        gMyPrivateContext->GetCore()->GetLogServer()->Error()
            << "(ScriptServer) ERROR: Unknown function '"
            << STR2CSTR(functionName) << "'" << endl;
    }

    return out.Get();
}

static VALUE
thisCall(VALUE /*self*/, VALUE objPointer, VALUE functionName, VALUE args)
{
    ParameterList in;
    getParameterList(args, in);

    Object *obj = (Object*)NUM2ULONG(objPointer);
    Class::TCmdProc cmd =
        obj->GetClass()->GetCmdProc(STR2CSTR(functionName));

    GCValue out;

    if (cmd != 0)
    {
        out = cmd(obj, in);
    } else
    {
        gMyPrivateContext->GetCore()->GetLogServer()->Error()
            << "(ScriptServer) ERROR: Unknown function '"
            << STR2CSTR(functionName) << "'" << endl;
    }

    return out.Get();
}

static VALUE
importBundle(VALUE /*self*/, VALUE path)
{
    gMyPrivateContext->GetCore()->ImportBundle(STR2CSTR(path));
    return Qnil;
}

static VALUE
run (VALUE /*self*/, VALUE file)
{
    gMyPrivateContext->GetCore()->GetScriptServer()->Run(STR2CSTR(file));
    return Qnil;
}

static VALUE
newObject(VALUE /*self*/, VALUE className, VALUE pathStr)
{
    std::shared_ptr<Leaf> leaf =
        gMyPrivateContext->New(STR2CSTR(className), STR2CSTR(pathStr));
    return getZeitgeistObject(leaf).Get();
}

static VALUE
deleteObject(VALUE /*self*/, VALUE name)
{
    gMyPrivateContext->Delete(STR2CSTR(name));
    return Qnil;
}

static VALUE
getObject(VALUE /*self*/, VALUE path)
{
    std::shared_ptr<Leaf> leaf = gMyPrivateContext->Get(STR2CSTR(path));
    return getZeitgeistObject(leaf).Get();
}

static VALUE
listObjects(VALUE /*self*/)
{
    gMyPrivateContext->ListObjects();
    return Qnil;
}

static VALUE
pushd(VALUE /*self*/)
{
    gMyPrivateContext->Push();
    return Qnil;
}

static VALUE
popd(VALUE /*self*/)
{
    gMyPrivateContext->Pop();
    return Qnil;
}

static VALUE
dirs(VALUE /*self*/)
{
    gMyPrivateContext->Dir();
    return Qnil;
}

ScriptServer::ScriptServer() : mRubyWrapper(new RubyWrapper())
{
    // register built-in commands
    mRubyWrapper->DefineGlobalFunction("selectObject", selectObject);
    mRubyWrapper->DefineGlobalFunction("selectCall",   selectCall);
    mRubyWrapper->DefineGlobalFunction("thisCall",     thisCall);
    mRubyWrapper->DefineGlobalFunction("importBundle", importBundle);
    mRubyWrapper->DefineGlobalFunction("run",          run);
    mRubyWrapper->DefineGlobalFunction("new",          newObject);
    mRubyWrapper->DefineGlobalFunction("delete",       deleteObject);
    mRubyWrapper->DefineGlobalFunction("get",          getObject);
    mRubyWrapper->DefineGlobalFunction("ls",           listObjects);
    mRubyWrapper->DefineGlobalFunction("pushd",        pushd);
    mRubyWrapper->DefineGlobalFunction("popd",         popd);
    mRubyWrapper->DefineGlobalFunction("dirs",         dirs);

    mRelPathPrefix = string("..") + salt::RFile::Sep() + ".." + salt::RFile::Sep();
}

ScriptServer::~ScriptServer()
{
}

void
ScriptServer::UpdateCachedAllNodes()
{
    GetLog()->Debug() << "(ScriptServer) updating cached script variables\n";
    GetCore()->GetRoot()->UpdateCached();
}

bool
ScriptServer::Run(std::shared_ptr<salt::RFile> file)
{
    if (file.get() == 0)
    {
        return false;
    }

    std::unique_ptr<char[]> buffer(new char[file->Size() + 1]);
    file->Read(buffer.get(), file->Size());
    buffer[file->Size()] = 0;

    bool ok = Eval(buffer.get());
    UpdateCachedAllNodes();

    return ok;
}

bool
ScriptServer::Run(const string &fileName)
{
    return Run(GetFile()->OpenResource(fileName));
}

bool
ScriptServer::Eval(const string &command)
{
    int error;
    mRubyWrapper->RbEvalStringWrap(command,error);
    return (error == 0);
}

bool
ScriptServer::Eval(const std::string &command, ScriptValue& value)
{
    int error;
    value = mRubyWrapper->RbEvalStringWrap(command,error);
    return (error == 0);
}

void
ScriptServer::CreateVariable(const string &varName, int value)
{
    // create a string with: "createVariable 'varName', value"
    stringstream s;
    s << "createVariable('" << varName << "', " << value << ")";
    Eval(s.str());
}

void
ScriptServer::CreateVariable(const string &varName, float value)
{
    // create a string with: "createVariable 'ns', 'varName', value"
    stringstream s;
    s << "createVariable('" << varName << "', " << value << ")";
    Eval(s.str());
}

void
ScriptServer::CreateVariable(const string &varName, const string &value)
{
    // create a string with: "createVariable 'ns', 'varName', 'value'"
    stringstream s;
    s << "createVariable('" << varName << "', '" << value << "')";
    Eval(s.str());
}

bool
ScriptServer::ParseVarName(const string& varName, string& nameSpace, string& name)
{
    stringstream  ss(varName);
    string current;
    vector<string> tokens;

    // segment varName
    while(! ss.eof())
    {
        getline(ss, current,'.');
        if (current.size())
        {
            tokens.push_back(current);
        }
    }

    if (tokens.size() != 2)
    {
        return false;
    }

    nameSpace = tokens[0];
    name = tokens[1];

    return (
        (nameSpace.size() >= 1) &&
        (nameSpace[0] >= 'A') &&
        (nameSpace[0] <= 'Z') &&
        (name.size() >= 1) &&
        (name[0] >= 'A') &&
        (name[0] <= 'Z')
        );
}

bool
ScriptServer::ExistsVariable(const string &varName)
{
    return (! GetVariable(varName).IsNil());
}

ScriptValue
ScriptServer::GetVariable(const string &varName)
{
    string nameSpace;
    string name;

    if (! ParseVarName(varName,nameSpace,name))
    {
        return ScriptValue();
    }

    ScriptValue v;
    if (nameSpace != "")
    {
        v = mRubyWrapper->CallMethod(nameSpace, name);
    } else
    {
        v = mRubyWrapper->RbConstGet(name);
    }

    return v;
}

bool
ScriptServer::GetVariable(const string &varName, int &value)
{
    return GetVariable(varName).GetInt(value);
}

bool
ScriptServer::GetVariable(const std::string &varName, float &value)
{
    return GetVariable(varName).GetFloat(value);
}

bool
ScriptServer::GetVariable(const string &varName, bool &value)
{
    return GetVariable(varName).GetBool(value);
}

bool
ScriptServer::GetVariable(const string &varName, string &value)
{
    return GetVariable(varName).GetString(value);
}

std::shared_ptr<CoreContext>
ScriptServer::GetContext() const
{
    return gMyPrivateContext;
}

bool
ScriptServer::SetupDotDir()
{
    string dotDir;
    if (GetDotDirName(dotDir) && CreateDotDir(dotDir))
    {
        GetFile()->AddResourceLocation(dotDir);
        return true;
    }
    return false;
}

bool
ScriptServer::ConstructInternal()
{
    if (! Leaf::ConstructInternal())
    {
        return false;
    }

    gMyPrivateContext = GetCore()->CreateContext();
    return true;
}

void
ScriptServer::SetInitRelPathPrefix(const std::string &relPathPrefix)
{
    mRelPathPrefix = relPathPrefix;
}

ScriptServer::ERunScriptErrorType
ScriptServer::RunInitScriptInternal(const string &sourceDir, const string &name,
                                    bool copy, const string& destDir)
{
    // run the init script in the sourceDir
    string sourcePath = sourceDir + salt::RFile::Sep() + name;
    GetLog()->Debug() << "(ScriptServer) Running " << sourcePath << "... " << endl;

    std::shared_ptr<salt::StdFile> file(new(salt::StdFile));
    if (! file->Open(sourcePath.c_str()))
    {
        GetLog()->Error() << "(ScriptServer) Script not found " << sourcePath << endl;
        return eNotFound;
    } else if (! Run(file))
    {
        GetLog()->Error() << "(ScriptServer) Error in script " << sourcePath << endl;
        return eError;
    } else
    {
        GetLog()->Debug() << "(ScriptServer) Script ended OK " << sourcePath << endl;
    }

    // copy it to the destDir
    if (! copy)
    {
        return eOK;
    }

    string destPath = destDir + salt::RFile::Sep() + name;

    GetLog()->Debug() << "Copying " << sourcePath
                       << " to " << destPath << endl;

    stringstream s;
#ifdef WIN32
    s << "copy \"" << sourcePath << "\" \"" << destPath << '"';
#else
    s << "cp \"" << sourcePath << "\" \"" << destPath << '"';
#endif
    system(s.str().c_str());

    return eOK;
}

bool
ScriptServer::GetDotDirName(string& dotDir)
{
    if (mDotName == "")
    {
        GetLog()->Warning() << "(ScriptServer) WARNING: Dot directory name unset.\n";
        return false;
    }

    const char* envName =
#ifdef WIN32
        "USERPROFILE";
#else
        "HOME";
#endif

    char* home = getenv(envName);
    if (!home)
    {
        GetLog()->Warning() << "(ScriptServer) WARNING: $HOME is unset.\n";
        return false;
    }

    dotDir = string(home) + salt::RFile::Sep() + mDotName;
    return true;
}

bool
ScriptServer::CreateDotDir(const string& dotDir)
{
    char cwd[PATH_MAX+1];
#if WIN32
    if (GetCurrentDirectory(PATH_MAX, cwd) == 0)
#else
    if (getcwd(cwd,sizeof(cwd)) == NULL)
#endif
    {
        GetLog()->Error()
            << "(ScriptServer) ERROR: Cannot get current directory\n";
        return false;
    }

#if WIN32
    if (SetCurrentDirectory(dotDir.c_str()))
#else
    if (chdir(dotDir.c_str()) == 0)
#endif
    {
        // dot dir exists; change back to original directory
        chdir(cwd);
        return true;
    }

    // dot dir is not existent, try to create it
#if WIN32
    if (! CreateDirectory(dotDir.c_str(), 0))
#else
    if (mkdir(dotDir.c_str(),0777) != 0)
#endif
    {
        GetLog()->Error() << "(ScriptServer) ERROR: Cannot create directory '"
                          << dotDir << "'\n";
        return false;
    }

    GetLog()->Debug() << "(ScriptServer) Created Directory '"
                       << dotDir << "'\n";

    return true;
}

bool
ScriptServer::RunInitScript(const string &fileName, const string &relPath,
                            EInitScriptType type)
{
    GetLog()->Debug() << "(ScriptServer) Attempting to run init script '"
                       << fileName << "'\n";

    string dotDir;
    bool validDotDir =
        (type == IS_USERLOCAL) &&
        GetDotDirName(dotDir) &&
        CreateDotDir(dotDir);

    // get the (OS specific) path to the script directory
    string pkgdatadir = salt::RFile::BundlePath();
#if __APPLE__
#if USE_COREFOUNDATION
    pkgdatadir += "Contents/Resources/";
#endif
#endif

    ERunScriptErrorType result = eNotFound;

    // Trying directory given in mRelPathPrefix
    if (!mRelPathPrefix.empty())
    {
        result = RunInitScriptInternal(mRelPathPrefix, fileName, validDotDir, dotDir);
        if (result == eOK)
        {
            GetLog()->Debug() << "(ScriptServer) : Ran init script '"
                              << mRelPathPrefix << salt::RFile::Sep() << fileName << "'\n";
            return true;
        }
        else if (result == eError)
        {
            GetLog()->Error() << "(ScriptServer) ERROR: Found error in init script '"
                              << mRelPathPrefix << salt::RFile::Sep() << fileName << "'\n";
            return false;
        }
    }


    if (validDotDir)
    {
        // Trying dot-dir in home directory
        result = RunInitScriptInternal(dotDir, fileName, false);
        if (result == eOK)
        {
            GetLog()->Debug() << "(ScriptServer) : Ran init script '"
                              << dotDir << salt::RFile::Sep() << fileName << "'\n";
            return true;
        }
        else if (result == eError)
        {
            GetLog()->Error() << "(ScriptServer) ERROR: Found error in init script '"
                              << dotDir << salt::RFile::Sep() << fileName << "'\n";
            return false;
        }

        GetLog()->Debug() << "(ScriptServer) : Did not find init script '"
                           << dotDir << salt::RFile::Sep() << fileName << "'\n";
    }



    // Trying package data directory
    result = RunInitScriptInternal(pkgdatadir,  fileName, validDotDir, dotDir);
    if (result == eOK)
    {
        GetLog()->Debug() << "(ScriptServer) : Ran init script '"
                          << pkgdatadir << salt::RFile::Sep() << fileName << "'\n";
        return true;
    }
    else if (result == eError)
    {
        GetLog()->Error() << "(ScriptServer) ERROR: Found error in init script '"
                          << pkgdatadir << salt::RFile::Sep() << fileName << "'\n";
        return false;
    }
    GetLog()->Debug() << "(ScriptServer) : Did not find init script '"
                      << pkgdatadir << salt::RFile::Sep() << fileName << "'\n";


    // Trying relative path to cwd
    result = RunInitScriptInternal(mRelPathPrefix+relPath, fileName, validDotDir, dotDir);
    if (result == eOK)
    {
        GetLog()->Debug() << "(ScriptServer) : Ran init script '"
                          <<  mRelPathPrefix+relPath << salt::RFile::Sep() << fileName << "'\n";
        return true;
    }
    else if (result == eError)
    {
        GetLog()->Error() << "(ScriptServer) ERROR: Found error in init script '"
                          << mRelPathPrefix+relPath << salt::RFile::Sep() << fileName << "'\n";
    }

    GetLog()->Error() << "(ScriptServer) ERROR: Cannot locate init script '"
                          << fileName << "'\n";
    return (result == eOK);
}
