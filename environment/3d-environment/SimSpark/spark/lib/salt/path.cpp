/* -*- mode: c++ -*-
   
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#include "path.h"
#include <filesystem>

using namespace std;
using namespace salt;

Path::Path(const std::string &path)
{
	Set(path);
}

void Path::Set(const std::string &path)
{
	if (path[0] == '/' || path[0] == '\\')
	{
		mIsAbsolute = true;
	}
	else
	{
		mIsAbsolute = false;
	}

	Tokenize(path);
}

std::string Path::GetCleanPath(const std::string &sep) const
{
	string path;

	if (mIsAbsolute)
	{
		path += sep;
	}

	unsigned int count = 0;
	for (TStringList::const_iterator i = mPathComponents.begin(); i != mPathComponents.end(); ++i)
	{
		path += (*i);
		++count;
		if ( count != mPathComponents.size() )
		{
			path += sep;
		}
	}

	return path;
}

bool Path::IsAbsolute() const
{
	return mIsAbsolute;
}

bool Path::IsEmpty() const
{
	return mPathComponents.empty();
}

const std::string& Path::Front() const
{
	return mPathComponents.front();
}

void Path::PopFront()
{
	mIsAbsolute = false;
	mPathComponents.pop_front();
}

const std::string& Path::Back() const
{
	return mPathComponents.back();
}

void Path::PopBack()
{
	mPathComponents.pop_back();
}

void Path::Tokenize(const std::string &path)
{
	std::filesystem::path fsPath = std::filesystem::path(path);
	for (auto part = fsPath.begin(); part != fsPath.end(); part++)
	{
		std::string str = part->generic_string();
		if (!str.empty() && str != "/")
		{
			mPathComponents.push_back(str);
		}
	}
}
