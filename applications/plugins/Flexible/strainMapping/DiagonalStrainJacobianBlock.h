/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
*                (c) 2006-2011 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Plugins                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef FLEXIBLE_DiagonalStrainJacobianBlock_H
#define FLEXIBLE_DiagonalStrainJacobianBlock_H

#include "../BaseJacobian.h"
#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/Mat.h>
#include "../types/DeformationGradientTypes.h"
#include "../types/StrainTypes.h"

#include "PrincipalStrechesJacobianBlock.h" // to access helper methods

#include <sofa/helper/MatEigen.h>


namespace sofa
{

namespace defaulttype
{


//////////////////////////////////////////////////////////////////////////////////
////  macros
//////////////////////////////////////////////////////////////////////////////////
#define F221(type)  DefGradientTypes<2,2,0,type>
#define F321(type)  DefGradientTypes<3,2,0,type>
#define F331(type)  DefGradientTypes<3,3,0,type>
#define F332(type)  DefGradientTypes<3,3,1,type>
#define D221(type)  DiagonalizedStrainTypes<2,2,0,type>
#define D331(type)  DiagonalizedStrainTypes<3,3,0,type>
#define D332(type)  DiagonalizedStrainTypes<3,3,1,type>

//////////////////////////////////////////////////////////////////////////////////
////  helpers
//////////////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


/** Template class used to implement one jacobian block for DiagonalStrainMapping */
template<class TIn, class TOut>
class DiagonalStrainJacobianBlock : public BaseJacobianBlock<TIn,TOut>
{
public:

    typedef TIn In;
    typedef TOut Out;

    typedef BaseJacobianBlock<In,Out> Inherit;
    typedef typename Inherit::InCoord InCoord;
    typedef typename Inherit::InDeriv InDeriv;
    typedef typename Inherit::OutCoord OutCoord;
    typedef typename Inherit::OutDeriv OutDeriv;
    typedef typename Inherit::MatBlock MatBlock;
    typedef typename Inherit::KBlock KBlock;
    typedef typename Inherit::Real Real;

    typedef typename In::Frame Frame;  ///< Matrix representing a deformation gradient
    typedef typename Out::StrainVec StrainVec;  ///< Vec representing a strain
    enum { material_dimensions = In::material_dimensions };
    enum { spatial_dimensions = In::spatial_dimensions };
    enum { strain_size = Out::strain_size };
    enum { frame_size = spatial_dimensions*material_dimensions };

    typedef Mat<material_dimensions,material_dimensions,Real> MaterialMaterialMat;
    typedef Mat<spatial_dimensions,material_dimensions,Real> SpatialMaterialMat;

    /**
    Mapping:   \f$ E = Ut.F.V\f$
    where:  U/V are the spatial and material rotation parts of F and E is diagonal
    Jacobian:    \f$  dE = Ut.dF.V \f$ Note that dE is not diagonal
    */

    static const bool constantJ = false;

    SpatialMaterialMat U;  ///< Spatial Rotation
    MaterialMaterialMat V; ///< Material Rotation



    void addapply( OutCoord& result, const InCoord& data )
    {
        Vec<material_dimensions,Real> F_diag = computeSVD( data.getF(), U, V );

        for( int i=0 ; i<material_dimensions ; ++i )
            result.getStrain()[i] += F_diag[i];
    }

    void addmult( OutDeriv& result,const InDeriv& data )
    {
        result.getStrain() += StrainMatToVoigt( U.multTranspose( data.getF() * V ) );
    }

    void addMultTranspose( InDeriv& result, const OutDeriv& data )
    {
        result.getF() += U * StressVoigtToMat( data.getStrain() ).multTransposed( V );
    }

    // TODO how?????
    MatBlock getJ()
    {
        MatBlock B = MatBlock();
        //typedef Eigen::Map<Eigen::Matrix<Real,Out::deriv_total_size,In::deriv_total_size,Eigen::RowMajor> > EigenMap;
        //EigenMap eB(&B[0][0]);
        //eB = assembleJ( U.multTransposed( V ) );
        return B;
    }


    // requires derivative of R. Not Yet implemented..
    KBlock getK(const OutDeriv& /*childForce*/)
    {
        return KBlock();
    }
    void addDForce( InDeriv& /*df*/, const InDeriv& /*dx*/, const OutDeriv& /*childForce*/, const double& /*kfactor*/ )
    {
    }
};




} // namespace defaulttype
} // namespace sofa



#endif // FLEXIBLE_DiagonalStrainJacobianBlock_H
