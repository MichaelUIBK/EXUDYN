/** ***********************************************************************************************
* @class        CMarkerObjectODE2CoordinatesParameters
* @brief        Parameter class for CMarkerObjectODE2Coordinates
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2021-11-16  08:25:45 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef CMARKEROBJECTODE2COORDINATESPARAMETERS__H
#define CMARKEROBJECTODE2COORDINATESPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"


//! AUTO: Parameters for class CMarkerObjectODE2CoordinatesParameters
class CMarkerObjectODE2CoordinatesParameters // AUTO: 
{
public: // AUTO: 
    Index objectNumber;                           //!< AUTO: body number to which marker is attached to
    //! AUTO: default constructor with parameter initialization
    CMarkerObjectODE2CoordinatesParameters()
    {
        objectNumber = EXUstd::InvalidIndex;
    };
};


/** ***********************************************************************************************
* @class        CMarkerObjectODE2Coordinates
* @brief        A Marker attached to all coordinates of an object (currently only body is possible), e.g. to apply special constraints or loads on all coordinates. The measured coordinates INCLUDE reference + current coordinates.
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

//! AUTO: CMarkerObjectODE2Coordinates
class CMarkerObjectODE2Coordinates: public CMarker // AUTO: 
{
protected: // AUTO: 
    CMarkerObjectODE2CoordinatesParameters parameters; //! AUTO: contains all parameters for CMarkerObjectODE2Coordinates

public: // AUTO: 

    // AUTO: access functions
    //! AUTO: Write (Reference) access to parameters
    virtual CMarkerObjectODE2CoordinatesParameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const CMarkerObjectODE2CoordinatesParameters& GetParameters() const { return parameters; }

    //! AUTO:  general access to object number
    virtual Index GetObjectNumber() const override
    {
        return parameters.objectNumber;
    }

    //! AUTO:  return marker type (for node treatment in computation)
    virtual Marker::Type GetType() const override
    {
        return (Marker::Type)(Marker::Body + Marker::Object + Marker::Coordinates + Marker::JacobianDerivativeAvailable);
    }

    //! AUTO:  return dimension of connector, which an attached connector would have; for coordinate markers, it gives the number of coordinates used by the marker
    virtual Index GetDimension(const CSystemData& cSystemData) const override;

    //! AUTO:  return position of marker
    virtual void GetPosition(const CSystemData& cSystemData, Vector3D& position, ConfigurationType configuration = ConfigurationType::Current) const override
    {
        position = Vector3D({0,0,0});
    }

    //! AUTO:  Compute marker data (e.g. position and positionJacobian) for a marker
    virtual void ComputeMarkerData(const CSystemData& cSystemData, bool computeJacobian, MarkerData& markerData) const override;

    //! AUTO:  fill in according data for derivative of jacobian times vector v, e.g.: d(Jpos.T @ v)/dq
    virtual void ComputeMarkerDataJacobianDerivative(const CSystemData& cSystemData, const Vector& v, MarkerData& markerData) const override;

    //! AUTO:  return the \hac{ODE2} coordinate vectors (and derivative) of the attached object
    void GetObjectODE2Coordinates(const CSystemData& cSystemData, Vector& objectCoordinates, Vector& objectCoordinates_t) const;

};



#endif //#ifdef include once...
