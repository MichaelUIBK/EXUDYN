/** ***********************************************************************************************
* @class        CMarkerNodeRigidParameters
* @brief        Parameter class for CMarkerNodeRigid
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2021-08-11  16:20:59 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef CMARKERNODERIGIDPARAMETERS__H
#define CMARKERNODERIGIDPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"


//! AUTO: Parameters for class CMarkerNodeRigidParameters
class CMarkerNodeRigidParameters // AUTO: 
{
public: // AUTO: 
    Index nodeNumber;                             //!< AUTO: node number to which marker is attached to
    //! AUTO: default constructor with parameter initialization
    CMarkerNodeRigidParameters()
    {
        nodeNumber = EXUstd::InvalidIndex;
    };
};


/** ***********************************************************************************************
* @class        CMarkerNodeRigid
* @brief        A rigid-body (position+orientation) node-marker attached to a rigid-body node.
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

//! AUTO: CMarkerNodeRigid
class CMarkerNodeRigid: public CMarker // AUTO: 
{
protected: // AUTO: 
    CMarkerNodeRigidParameters parameters; //! AUTO: contains all parameters for CMarkerNodeRigid

public: // AUTO: 

    // AUTO: access functions
    //! AUTO: Write (Reference) access to parameters
    virtual CMarkerNodeRigidParameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const CMarkerNodeRigidParameters& GetParameters() const { return parameters; }

    //! AUTO:  general access to node number
    virtual Index GetNodeNumber() const override
    {
        return parameters.nodeNumber;
    }

    //! AUTO:  return marker type (for node treatment in computation)
    virtual Marker::Type GetType() const override
    {
        return (Marker::Type)(Marker::Node + Marker::Position+ Marker::Orientation);
    }

    //! AUTO:  return dimension of connector, which an attached connector would have; for coordinate markers, it gives the number of coordinates used by the marker
    virtual Index GetDimension(const CSystemData& cSystemData) const override
    {
        return 3;
    }

    //! AUTO:  return position of marker
    virtual void GetPosition(const CSystemData& cSystemData, Vector3D& position, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return velocity of marker
    virtual void GetVelocity(const CSystemData& cSystemData, Vector3D& velocity, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent rotation matrix of node; returns always a 3D Matrix
    virtual void GetRotationMatrix(const CSystemData& cSystemData, Matrix3D& rotationMatrix, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent angular velocity of node; returns always a 3D Vector
    virtual void GetAngularVelocity(const CSystemData& cSystemData, Vector3D& angularVelocity, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent local (=body-fixed) angular velocity of node; returns always a 3D Vector
    virtual void GetAngularVelocityLocal(const CSystemData& cSystemData, Vector3D& angularVelocity, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  Compute marker data (e.g. position and positionJacobian) for a marker
    virtual void ComputeMarkerData(const CSystemData& cSystemData, bool computeJacobian, MarkerData& markerData) const override;

};



#endif //#ifdef include once...
