/** ***********************************************************************************************
* @brief        Implementation for MarkerNodeRigid
*
* @author       Gerstmayr Johannes
* @date         2019-06-13 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CMarkerNodeRigid.h"


void CMarkerNodeRigid::GetPosition(const CSystemData& cSystemData, Vector3D& position, ConfigurationType configuration) const
{
	position = ((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetPosition(configuration);
}

void CMarkerNodeRigid::GetVelocity(const CSystemData& cSystemData, Vector3D& velocity, ConfigurationType configuration) const
{
	velocity = ((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetVelocity(configuration);
}

void CMarkerNodeRigid::GetRotationMatrix(const CSystemData& cSystemData, Matrix3D& rotationMatrix, ConfigurationType configuration) const
{

	rotationMatrix = ((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetRotationMatrix(configuration);
}

void CMarkerNodeRigid::GetAngularVelocity(const CSystemData& cSystemData, Vector3D& angularVelocity, ConfigurationType configuration) const
{
	angularVelocity = ((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetAngularVelocity(configuration);
}

void CMarkerNodeRigid::GetAngularVelocityLocal(const CSystemData& cSystemData, Vector3D& angularVelocity, ConfigurationType configuration) const
{
	angularVelocity = ((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetAngularVelocityLocal(configuration);
}


void CMarkerNodeRigid::ComputeMarkerData(const CSystemData& cSystemData, bool computeJacobian, MarkerData& markerData) const
{
	//A marker 'NodeRigid' must be attached to the ODE2 part of a node
	markerData.position = ((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetPosition(ConfigurationType::Current);
	markerData.velocity = ((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetVelocity(ConfigurationType::Current);

	markerData.orientation = ((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetRotationMatrix(ConfigurationType::Current);
	markerData.angularVelocityLocal = ((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetAngularVelocityLocal(ConfigurationType::Current);

	markerData.velocityAvailable = true;

	if (computeJacobian)
	{
		((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetPositionJacobian(markerData.positionJacobian);
		((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetRotationJacobian(markerData.rotationJacobian);
	}

}

