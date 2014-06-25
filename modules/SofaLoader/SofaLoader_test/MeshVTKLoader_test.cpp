/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
*                (c) 2006-2011 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the Free Software Foundation, Inc., 51  *
* Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.                   *
*******************************************************************************
*                            SOFA :: Applications                             *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#include "Sofa_test.h"

#include "SofaLoader/MeshVTKLoader.h"

namespace sofa {

struct MeshVTKLoader_test : public Sofa_test<>, public component::loader::MeshVTKLoader
{

    MeshVTKLoader_test()
    {
    }

};

TEST_F(MeshVTKLoader_test, detectFileType)
{
    ASSERT_EQ(component::loader::MeshVTKLoader::LEGACY, this->detectFileType(sofa::helper::system::DataRepository.getFile("mesh/liver.vtk").c_str()));
    ASSERT_EQ(component::loader::MeshVTKLoader::XML, this->detectFileType(sofa::helper::system::DataRepository.getFile("mesh/Armadillo_Tetra_4406.vtu").c_str()));
}

}// namespace sofa
