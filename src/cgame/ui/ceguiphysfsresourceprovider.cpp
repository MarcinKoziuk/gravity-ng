/*
 * cgame/ui/ceguiphysfsresourceprovider.cpp
 *
 * Copied and adapted from: http://cegui.org.uk/wiki/Loading_Resources_From_PhysFS
 *
 * Copyright (c) 2016
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */
/***********************************************************************
filename:   CEGUIPhysFSResourceProvider.cpp
created:    8/7/2004
author:     James '_mental_' O'Sullivan
with minor changes by Marek 'Wansti' Moeckel
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
*
*   Permission is hereby granted, free of charge, to any person obtaining
*   a copy of this software and associated documentation files (the
*   "Software"), to deal in the Software without restriction, including
*   without limitation the rights to use, copy, modify, merge, publish,
*   distribute, sublicense, and/or sell copies of the Software, and to
*   permit persons to whom the Software is furnished to do so, subject to
*   the following conditions:
*
*   The above copyright notice and this permission notice shall be
*   included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
*   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
*   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
*   OTHER DEALINGS IN THE SOFTWARE.
***************************************************************************/

#include <gravity/cgame/ui/ceguiphysfsresourceprovider.hpp>

#include <CEGUI/String.h>
#include <CEGUI/Exceptions.h>

#include <stdio.h>
#include <physfs.h>

#if defined(__WIN32__) || defined(_WIN32)
#   include <io.h>
#   include <windows.h>
#   include <string>

using namespace CEGUI;

//----------------------------------------------------------------------------//
static std::wstring Utf8ToUtf16(const std::string& utf8text)
{
	const int textLen = MultiByteToWideChar(CP_UTF8, 0, utf8text.c_str(),
		utf8text.size() + 1, 0, 0);

	if (textLen == 0)
		CEGUI_THROW(CEGUI::InvalidRequestException(
			"Utf8ToUtf16 - MultiByteToWideChar failed"));

	std::wstring wideStr(textLen, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8text.c_str(), utf8text.size() + 1,
		&wideStr[0], wideStr.size());
	return wideStr;
}

//----------------------------------------------------------------------------//
static CEGUI::String Utf16ToString(const wchar_t* const utf16text)
{
	const int len = WideCharToMultiByte(CP_UTF8, 0, utf16text, -1,
		0, 0, 0, 0);
	if (!len)
		CEGUI_THROW(CEGUI::InvalidRequestException(
			"Utf16ToUtf8 - WideCharToMultiByte failed"));

	CEGUI::utf8* buff = new CEGUI::utf8[len + 1];
	WideCharToMultiByte(CP_UTF8, 0, utf16text, -1,
		reinterpret_cast<char*>(buff), len, 0, 0);
	const CEGUI::String result(buff);
	delete[] buff;

	return result;
}

//----------------------------------------------------------------------------//
#else
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <dirent.h>
#   include <fnmatch.h>
#endif

namespace Gravity
{

using namespace CEGUI;

//----------------------------------------------------------------------------//
void PhysFSResourceProvider::loadRawDataContainer(const String& filename,
	RawDataContainer& output,
	const String& resourceGroup)
{
	// Make sure PhysFS has been initialized before this point!
	const String final_filename(getFinalFilename(filename, resourceGroup));
	if (!PHYSFS_exists(final_filename.c_str()))
		CEGUI_THROW(InvalidRequestException("PhysFSResourceProvider::load: "
			"Filename supplied for data loading must be valid"));

	PHYSFS_file* file = PHYSFS_openRead(final_filename.c_str());

	if (file == NULL)
		CEGUI_THROW(InvalidRequestException("PhysFSResourceProvider::load: " +
			final_filename + " does not exist. " + PHYSFS_getLastError()));

	const size_t size = PHYSFS_fileLength(file);
	unsigned char* const buffer = new unsigned char[size];

	const size_t size_read = PHYSFS_read(file, buffer, 1, size);
	PHYSFS_close(file);

	if (size_read != size)
	{
		delete[] buffer;
		CEGUI_THROW(GenericException(
			"PhysFSResourceProvider::loadRawDataContainer: "
			"A problem occurred while reading file: " + final_filename));
	}

	output.setData(buffer);
	output.setSize(size);
}

//----------------------------------------------------------------------------//
void PhysFSResourceProvider::unloadRawDataContainer(RawDataContainer& data)
{
	uint8* const ptr = data.getDataPtr();
	delete[] ptr;
	data.setData(0);
	data.setSize(0);
}

//----------------------------------------------------------------------------//
void PhysFSResourceProvider::setResourceGroupDirectory(
	const String& resourceGroup,
	const String& directory)
{
	if (directory.length() == 0)
		return;

#if defined(_WIN32) || defined(__WIN32__)
	// while we rarely use the unportable '\', the user may have
	const String separators("\\/");
#else
	const String separators("/");
#endif

	if (String::npos == separators.find(directory[directory.length() - 1]))
		d_resourceGroups[resourceGroup] = directory + '/';
	else
		d_resourceGroups[resourceGroup] = directory;
}

//----------------------------------------------------------------------------//
const String& PhysFSResourceProvider::getResourceGroupDirectory(
	const String& resourceGroup)
{
	return d_resourceGroups[resourceGroup];
}

//----------------------------------------------------------------------------//
void PhysFSResourceProvider::clearResourceGroupDirectory(
	const String& resourceGroup)
{
	ResourceGroupMap::iterator iter = d_resourceGroups.find(resourceGroup);

	if (iter != d_resourceGroups.end())
		d_resourceGroups.erase(iter);
}

//----------------------------------------------------------------------------//
String PhysFSResourceProvider::getFinalFilename(
	const String& filename,
	const String& resourceGroup) const
{
	String final_filename;

	// look up resource group directory
	ResourceGroupMap::const_iterator iter =
		d_resourceGroups.find(resourceGroup.empty() ?
			d_defaultResourceGroup :
			resourceGroup);

	// if there was an entry for this group, use it's directory as the
	// first part of the filename
	if (iter != d_resourceGroups.end())
		final_filename = (*iter).second;

	// append the filename part that we were passed
	final_filename += filename;

	// return result
	return final_filename;
}

//----------------------------------------------------------------------------//
size_t PhysFSResourceProvider::getResourceGroupFileNames(
	std::vector<String>& out_vec,
	const String& file_pattern,
	const String& resource_group)
{
	// look-up resource group name
	ResourceGroupMap::const_iterator iter =
		d_resourceGroups.find(resource_group.empty() ? d_defaultResourceGroup :
			resource_group);
	// get directory that's set for the resource group
	const String dir_name(
		iter != d_resourceGroups.end() ? (*iter).second : "./");

	size_t entries = 0;

	// Win32 code.
#if defined(__WIN32__) || defined(_WIN32)
	intptr_t f;
	struct _wfinddata_t fd;

	if ((f = _wfindfirst(Utf8ToUtf16((dir_name + file_pattern).c_str()).c_str(), &fd)) != -1)
	{
		do
		{
			if ((fd.attrib & _A_SUBDIR))
				continue;

			out_vec.push_back(Utf16ToString(fd.name));
			++entries;
		} while (_wfindnext(f, &fd) == 0);

		_findclose(f);
	}

	// Everybody else
#else
	DIR* dirp;

	if ((dirp = opendir(dir_name.c_str())))
	{
		struct dirent* dp;

		while ((dp = readdir(dirp)))
		{
			const String filename(dir_name + dp->d_name);
			struct stat s;

			if ((stat(filename.c_str(), &s) == 0) &&
				S_ISREG(s.st_mode) &&
				(fnmatch(file_pattern.c_str(), dp->d_name, 0) == 0))
			{
				out_vec.push_back(dp->d_name);
				++entries;
			}
		}

		closedir(dirp);
	}
#endif

	return entries;
}

//----------------------------------------------------------------------------//

} // namespace Gravity
