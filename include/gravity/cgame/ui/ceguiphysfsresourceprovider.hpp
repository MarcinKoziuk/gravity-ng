/*
 * cgame/ui/ceguiphysfsresourceprovider.hpp
 *
 * Copied and adapted from: http://cegui.org.uk/wiki/Loading_Resources_From_PhysFS
 *
 * Copyright (c) 2016
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */
/***********************************************************************
filename:       CEGUIPhysFSResourceProvider.h
created:        8/7/2004
author:         James '_mental_' O'Sullivan
with minor changes by Marek 'Wansti' Moeckel

purpose:        Load GUI resources via PhysFS
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#ifndef GRAVITY_CGAME_UI_CEGUIPHYSFSRESOURCEPROVIDER_HPP
#define GRAVITY_CGAME_UI_CEGUIPHYSFSRESOURCEPROVIDER_HPP

#include <CEGUI/Base.h>
#include <CEGUI/ResourceProvider.h>

#include <map>

#if defined(_MSC_VER)
#       pragma warning(push)
#       pragma warning(disable : 4251)
#endif

namespace Gravity
{

//! CEGUI Resource Provider for the PhysFS File System.
/*! This is an auxiliary class (based on the DefaultResourceProvider from CEGUI)
* that provides file access via PhysFS. This is used to load GUI resources
* via PhysFS.
* Check the CEGUI documentation for further information on Resource Providers.
* PhysFS must be initialized before this class can be used! Currently, the
* Resource Provider blindly assumes that this has been done which might cause
* errors.
*/
class PhysFSResourceProvider : public CEGUI::ResourceProvider
{
public:
	/*************************************************************************
	Construction and Destruction
	*************************************************************************/
	PhysFSResourceProvider() {}
	~PhysFSResourceProvider(void) {}

	/*!
	\brief
	Set the directory associated with a given resource group identifier.

	\param resourceGroup
	The resource group identifier whose directory is to be set.

	\param directory
	The directory to be associated with resource group identifier
	\a resourceGroup

	\return
	Nothing.
	*/
	void setResourceGroupDirectory(const CEGUI::String& resourceGroup, const CEGUI::String& directory);

	/*!
	\brief
	Return the directory associated with the specified resource group
	identifier.

	\param resourceGroup
	The resource group identifier for which the associated directory is to
	be returned.

	\return
	String object describing the directory currently associated with resource
	group identifier \a resourceGroup.

	\note
	This member is not defined as being const because it may cause
	creation of an 'empty' directory specification for the resourceGroup
	if the resourceGroup has not previously been accessed.
	*/
	const CEGUI::String& getResourceGroupDirectory(const CEGUI::String& resourceGroup);

	/*!
	\brief
	clears any currently set directory for the specified resource group
	identifier.

	\param resourceGroup
	The resource group identifier for which the associated directory is to
	be cleared.
	*/
	void clearResourceGroupDirectory(const CEGUI::String& resourceGroup);

	void loadRawDataContainer(const CEGUI::String& filename, CEGUI::RawDataContainer& output, const CEGUI::String& resourceGroup);
	void unloadRawDataContainer(CEGUI::RawDataContainer& data);
	size_t getResourceGroupFileNames(std::vector<CEGUI::String>& out_vec,
		const CEGUI::String& file_pattern,
		const CEGUI::String& resource_group);

protected:
	/*!
	\brief
	Return the final path and filename, taking into account the given
	resource group identifier that should be used when attempting to
	load the data.
	*/
	CEGUI::String getFinalFilename(const CEGUI::String& filename, const CEGUI::String& resourceGroup) const;

	typedef std::map<CEGUI::String, CEGUI::String, CEGUI::StringFastLessCompare> ResourceGroupMap;
	ResourceGroupMap    d_resourceGroups;
};

} // namespace Gravity

#if defined(_MSC_VER)
#       pragma warning(pop)
#endif

#endif  /* GRAVITY_CGAME_UI_CEGUIPHYSFSRESOURCEPROVIDER_HPP */