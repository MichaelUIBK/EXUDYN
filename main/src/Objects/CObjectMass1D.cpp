/** ***********************************************************************************************
* @brief        CObjectMass1D implementation
*
* @author       Gerstmayr Johannes
* @date         2019-06-15 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CObjectMass1D.h"
#include "Utilities/RigidBodyMath.h"


//! Computational function: compute mass matrix
void CObjectMass1D::ComputeMassMatrix(EXUmath::MatrixContainer& massMatrixC, const ArrayIndex& ltg, Index objectNumber) const
{
	Matrix& massMatrix = massMatrixC.GetInternalDenseMatrix();
	massMatrix.SetScalarMatrix(1, parameters.physicsMass);
}

//! Computational function: compute left-hand-side (LHS) of second order ordinary differential equations (ODE) to "ode2Lhs"
void CObjectMass1D::ComputeODE2LHS(Vector& ode2Lhs, Index objectNumber) const
{
	ode2Lhs.SetNumberOfItems(1);
	ode2Lhs.SetAll(0.);
}

//! Flags to determine, which access (forces, moments, connectors, ...) to object are possible
AccessFunctionType CObjectMass1D::GetAccessFunctionTypes() const
{
	return (AccessFunctionType)((Index)AccessFunctionType::TranslationalVelocity_qt + 
		(Index)AccessFunctionType::AngularVelocity_qt + 
		(Index)AccessFunctionType::DisplacementMassIntegral_q);
}

//! provide Jacobian at localPosition in "value" according to object access
void CObjectMass1D::GetAccessFunctionBody(AccessFunctionType accessType, const Vector3D& localPosition, Matrix& value) const
{
	switch (accessType)
	{
	case AccessFunctionType::TranslationalVelocity_qt:
	{
		//[fx,0,0] = A^T*F = F^T*A; global force F acts on local x-coordinate
		//Vector3D v = parameters.referenceRotation * Vector3D({ 1.,0.,0. });
		//value.SetMatrix(3, 1, { v[0], v[1], v[2] }); //a 3D Vector (e.g. 3D ForceVector) acts on 1 coordinate, rotated by reference rotation
		value.SetMatrix(3, 1, { parameters.referenceRotation(0,0), parameters.referenceRotation(1,0), parameters.referenceRotation(2,0) }); //a 3D Vector (e.g. 3D ForceVector) acts on 1 coordinate, rotated by reference rotation
		break;
	}
	case AccessFunctionType::AngularVelocity_qt:
	{
		//no action (but needed for application of rigid-body joints, similar to ground joints):
		value.SetMatrix(3, 1, { 0, 0, 0 }); //a 3D torque vector acts on 1 coordinate
		break;
	}
	case AccessFunctionType::DisplacementMassIntegral_q:
	{
		Vector3D v = parameters.referenceRotation * Vector3D({ parameters.physicsMass,0.,0. });
		value.SetMatrix(3, 1, { v[0], v[1], v[2] }); //a 3D gravity Vector acts on 1 coordinate, rotated by reference rotation
		break;
	}
	default:
		SysError("CObjectMass1D:GetAccessFunctionBody illegal accessType");
	}
}

//! provide according output variable in "value"
void CObjectMass1D::GetOutputVariableBody(OutputVariableType variableType, const Vector3D& localPosition, ConfigurationType configuration, Vector& value, Index objectNumber) const
{
	switch (variableType)
	{
	case OutputVariableType::Position: value.CopyFrom(GetPosition(localPosition, configuration)); break;
	case OutputVariableType::Displacement:	value.CopyFrom(GetDisplacement(localPosition, configuration)); break;
	case OutputVariableType::Velocity: value.CopyFrom(GetVelocity(localPosition, configuration)); break;
	case OutputVariableType::AngularVelocity: value.CopyFrom(Vector3D({ 0,0,0 })); break;
	case OutputVariableType::AngularVelocityLocal: value.CopyFrom(Vector3D({ 0,0,0 })); break;
	case OutputVariableType::RotationMatrix: {
		Matrix3D rot = parameters.referenceRotation;
		value.SetVector(9, rot.GetDataPointer());
		break;
	}
	case OutputVariableType::Rotation: {
		Vector3D rot = RigidBodyMath::RotationMatrix2RotXYZ(parameters.referenceRotation);
		value.SetVector(3, rot.GetDataPointer());
		break;
	}
	default:
		SysError("CObjectMass1D::GetOutputVariableBody failed"); //error should not occur, because types are checked!
	}
}

//  return the (global) position of "localPosition" according to configuration type
Vector3D CObjectMass1D::GetPosition(const Vector3D& localPosition, ConfigurationType configuration) const
{
	return parameters.referenceRotation*(((CNodeODE2*)GetCNode(0))->GetPosition(configuration) + localPosition) + parameters.referencePosition;
}

//  return the (global) position of "localPosition" according to configuration type
Vector3D CObjectMass1D::GetVelocity(const Vector3D& localPosition, ConfigurationType configuration) const
{
	return parameters.referenceRotation*(((CNodeODE2*)GetCNode(0))->GetVelocity(configuration));
}

//! return the (global) position of "localPosition" according to configuration type
Vector3D CObjectMass1D::GetDisplacement(const Vector3D& localPosition, ConfigurationType configuration) const
{
	//this also works for NodePointGround:
	return parameters.referenceRotation*(((CNodeODE2*)GetCNode(0))->GetPosition(configuration) - ((CNodeODE2*)GetCNode(0))->GetPosition(ConfigurationType::Reference));
}

