/** ***********************************************************************************************
* @brief        CObjectBeamGeometricallyExact2D implementation
*
* @author       Gerstmayr Johannes
* @date         2021-03-21 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Main/CSystemData.h"
//#include "Objects/CObjectANCFCable2DBase.h"
#include "Autogenerated/CObjectBeamGeometricallyExact2D.h"


Vector2D CObjectBeamGeometricallyExact2D::ComputeShapeFunctions(Real x) const
{
	Real lElem = parameters.physicsLength;
	return Vector2D({ (lElem*0.5 - x) / lElem, (lElem*0.5 + x) / lElem });
}

Vector2D CObjectBeamGeometricallyExact2D::ComputeShapeFunctions_x(Real x) const
{
	Real lElem = parameters.physicsLength;
	return Vector2D({ -1 / lElem , 1 / lElem });
}

//! map element coordinates (position or velocity level) given by nodal vectors q0 and q1 onto compressed shape function vector to compute position, etc.
Vector3D CObjectBeamGeometricallyExact2D::MapCoordinates(const Vector2D& SV, const LinkedDataVector& q0, const LinkedDataVector& q1) const
{
	return Vector3D({ SV[0] * q0[0] + SV[1] * q1[0], 
					  SV[0] * q0[1] + SV[1] * q1[1], 
					  SV[0] * q0[2] + SV[1] * q1[2]});
}

Matrix2D CObjectBeamGeometricallyExact2D::GetRotationMatrix2D(Real theta) const
{
	return Matrix2D(2, 2, {cos(theta), -sin(theta), sin(theta), cos(theta)}); //rotation about z-axis, stored in 2D matrix
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! Computational function: compute mass matrix
void CObjectBeamGeometricallyExact2D::ComputeMassMatrix(EXUmath::MatrixContainer& massMatrixC, const ArrayIndex& ltg, Index objectNumber) const
{
	Matrix& massMatrix = massMatrixC.GetInternalDenseMatrix();
	if (massMatrixComputed) //advantage due to precomputed mass matrix comparatively small (will improve in case of sparse matrices)
	{
		massMatrix.CopyFrom(precomputedMassMatrix); //just assignement; ConstSizeMatrix is directly assigned to Matrix (no double copy)
	}
	else
	{
		massMatrix.SetScalarMatrix(nODE2Coordinates, 0.); //set 6x6 matrix

		Real a = -0.5*parameters.physicsLength;
		Real b = 0.5*parameters.physicsLength;
		Real rhoA = parameters.physicsMassPerLength;
		Real rhoJ = parameters.physicsCrossSectionInertia;

		Index cnt = 0;
		Index ns = 2;
		for (auto item : EXUmath::gaussRuleOrder3Points)
		{
			Real x = 0.5*(b - a)*item + 0.5*(b + a);
			Vector2D SV = ComputeShapeFunctions(x);
			Real factInt = (0.5*(b - a)*EXUmath::gaussRuleOrder3Weights[cnt++]); //integration factor

			for (Index i = 0; i < ns; i++)
			{
				for (Index j = 0; j < ns; j++)
				{
					massMatrix(i * 3 + 0, j * 3 + 0) += SV[i] * SV[j] * rhoA * factInt;
					massMatrix(i * 3 + 1, j * 3 + 1) += SV[i] * SV[j] * rhoA * factInt;
					massMatrix(i * 3 + 2, j * 3 + 2) += SV[i] * SV[j] * rhoJ * factInt;
				}
			}
		}
		precomputedMassMatrix.CopyFrom(massMatrix); //assignement operator would cause double copy!
		massMatrixComputed = true;
	}
	//pout << "M=" << massMatrix << "\n";
}

//! compute strains and variation of strains for given interpolated derivatives of displacement u1_x, u2_x, angle theta (incl. reference config.!), shape vector SV and shape vector derivatives SV_x
void CObjectBeamGeometricallyExact2D::ComputeGeneralizedStrains(Real u1_x, Real u2_x, Real theta, const Vector2D& SV, const Vector2D& SV_x, const Vector2D& referenceSlopeVector,
	Real& gamma1, Real& gamma2, CSVector6D& deltaGamma1, CSVector6D& deltaGamma2) const
{
	Real cosTheta = cos(theta);
	Real sinTheta = sin(theta);
	const Vector2D& RS = referenceSlopeVector;
	gamma1 = cosTheta * (RS[0] + u1_x - cosTheta) + sinTheta * (RS[1] + u2_x - sinTheta);

	Real termGamma1 = (-sinTheta * (RS[0] + u1_x - cosTheta) + cosTheta * sinTheta + cosTheta * (RS[1] + u2_x - sinTheta) - sinTheta * cosTheta);

	deltaGamma1 = CSVector6D({ cosTheta*SV_x[0], sinTheta*SV_x[0], termGamma1*SV[0],
			cosTheta*SV_x[1], sinTheta*SV_x[1], termGamma1*SV[1] });

	gamma2 = -sinTheta * (RS[0] + u1_x - cosTheta) + cosTheta * (RS[1] + u2_x - sinTheta);

	Real termGamma2 = (-cosTheta * (RS[0] + u1_x - cosTheta) - sinTheta * sinTheta - sinTheta * (RS[1] + u2_x - sinTheta) - cosTheta * cosTheta);

	deltaGamma2 = CSVector6D({ -sinTheta * SV_x[0], cosTheta * SV_x[0], termGamma2*SV[0],
			-sinTheta * SV_x[1], cosTheta * SV_x[1], termGamma2*SV[1] });
}

//! Computational function: compute left-hand-side (LHS) of second order ordinary differential equations (ODE) to 'ode2Lhs'
void CObjectBeamGeometricallyExact2D::ComputeODE2LHS(Vector& ode2Lhs, Index objectNumber) const
{
	ode2Lhs.SetNumberOfItems(nODE2Coordinates);
	ode2Lhs.SetAll(0.);

	//could be speed up by only computing relevant components!
	//const int ns = 2; //number of shape functions
	CSVector3D qNode0( ((CNodeODE2*)GetCNode(0))->GetCurrentCoordinateVector(),0 ); //displacement coordinates node 0
	CSVector3D qNode1( ((CNodeODE2*)GetCNode(1))->GetCurrentCoordinateVector(),0 ); //displacement coordinates node 1

	CSVector3D qNode0Ref( ((CNodeODE2*)GetCNode(0))->GetReferenceCoordinateVector(), 0 );
	CSVector3D qNode1Ref( ((CNodeODE2*)GetCNode(1))->GetReferenceCoordinateVector(), 0 );

	//compute elastic forces at midpoint:
	Real L = parameters.physicsLength;
	Real x = 0; //midpoint, beam goes from -L/2 ... L/2
	Vector2D SV = ComputeShapeFunctions(x);
	Vector2D SV_x = ComputeShapeFunctions_x(x);

	Real u1_x = SV_x[0] * qNode0[0] + SV_x[1] * qNode1[0];
	Real u2_x = SV_x[0] * qNode0[1] + SV_x[1] * qNode1[1];

	Real theta   = SV[0] * (qNode0[2]+ qNode0Ref[2]) + SV[1] * (qNode1[2] + qNode1Ref[2]); //rotations need also reference values
	Real theta_x = SV_x[0] * qNode0[2] + SV_x[1] * qNode1[2];

	Vector2D refSlopeVector({ SV_x[0] * qNode0Ref[0] + SV_x[1] * qNode1Ref[0] ,
		                      SV_x[0] * qNode0Ref[1] + SV_x[1] * qNode1Ref[1]}); //reference slope vector, r'=d r / dx needed in reference configuration needed for computation of strains
	Real gamma1, gamma2;
	CSVector6D deltaGamma1, deltaGamma2;
	ComputeGeneralizedStrains(u1_x, u2_x, theta, SV, SV_x, refSlopeVector, gamma1, gamma2, deltaGamma1, deltaGamma2);

	Real fact = L; //integration factor

	ode2Lhs.MultAdd(parameters.physicsAxialStiffness*fact*gamma1, deltaGamma1);
	ode2Lhs.MultAdd(parameters.physicsShearStiffness*fact*gamma2, deltaGamma2);
	ode2Lhs[2+0] += parameters.physicsBendingStiffness*fact*theta_x*SV_x[0];
	ode2Lhs[2+3] += parameters.physicsBendingStiffness*fact*theta_x*SV_x[1];

	//pout << "refSlope=" << refSlopeVector << ", "
	//	<< "u1_x=" << u1_x << ", "
	//	<< "u2_x=" << u2_x << ", "
	//	<< "theta=" << theta << ", "
	//	<< "theta_x=" << theta_x << ", "
	//	<< "gamma1=" << gamma1 << ", "
	//	<< "gamma2=" << gamma2 << ", \n    "
	//	<< "deltaGamma1=" << deltaGamma1 << ", "
	//	<< "deltaGamma2=" << deltaGamma2 << ", "
	//	<< "f=" << ode2Lhs << "\n";
}

//! Flags to determine, which access (forces, moments, connectors, ...) to object are possible
AccessFunctionType CObjectBeamGeometricallyExact2D::GetAccessFunctionTypes() const
{
	return (AccessFunctionType)((Index)AccessFunctionType::TranslationalVelocity_qt + (Index)AccessFunctionType::AngularVelocity_qt + (Index)AccessFunctionType::DisplacementMassIntegral_q);
}

//! provide Jacobian at localPosition in 'value' according to object access
void CObjectBeamGeometricallyExact2D::GetAccessFunctionBody(AccessFunctionType accessType, const Vector3D& localPosition, Matrix& value) const
{
	//Real L = parameters.physicsLength;

	switch (accessType)
	{
	case AccessFunctionType::TranslationalVelocity_qt:
	{
		//const Index dim = 2;  //2D finite element
		//const Index ns = 2;   //number of shape functions

		Real x = localPosition[0]; //only x-coordinate
		Vector2D SV = ComputeShapeFunctions(x);
		value.SetNumberOfRowsAndColumns(3, nODE2Coordinates); //3D velocity, 6 coordinates qt

		if (localPosition[1] == 0)
		{
			value.SetAll(0.);
			value(0, 0) = SV[0];
			value(1, 1) = SV[0];
			value(0, 3) = SV[1];
			value(1, 4) = SV[1];
		}
		else
		{
			CHECKandTHROWstring("CObjectBeamGeometricallyExact2D::GetAccessFunctionBody for markers: only implemented if localPosition[1]==0");
				//	//pout << "inside ..." << "\n";
				//	//value.SetAll(0.);
				//	Real y = localPosition[1];
				//	Vector4D SV_x = ComputeShapeFunctions_x(x, L);
				//	Vector2D r_x = ComputeSlopeVector(x, ConfigurationType::Current);
				//	Real norm = r_x.GetL2Norm();
				//	Real normInv = 0;
				//	Vector2D n({ -r_x[1], r_x[0] });
				//	if (norm != 0.)
				//	{
				//		normInv = 1. / norm;
				//	}
				//	else
				//	{
				//		CHECKandTHROWstring("CObjectANCFCable2DBase::GetPosition(...): slope vector has length 0!");
				//	}
				//	n *= normInv;
				//	//p = r(localPosition[0]) + localPosition[1] * n; n=1/sqrt(rx^T*rx)*[-rx[1],rx[0]]
				//	//dp/dq = S + (ry^T*S_x)/(rx^T*rx) (3/2) * n + 1/sqrt(rx^T*rx) * S_x^perpendicular
				//	Real norm3 = norm * norm * norm; //could be SIMPLIFIED, because n also contains 1/norm ....

				//	//pout << "  slope=" << r_x << ", norm=" << norm << ", n=" << n << "\n";
				//	//
				//	for (Index i = 0; i < 4; i++)
				//	{
				//		Vector2D Svec[2]; //SV_x
				//		Svec[0] = Vector2D({ SV_x[i],0 });
				//		Svec[1] = Vector2D({ 0, SV_x[i] });
				//		Vector2D SvecP[2];
				//		SvecP[0] = Vector2D({ 0., SV_x[i] });
				//		SvecP[1] = Vector2D({ -SV_x[i], 0. });

				//		for (Index j = 0; j < 2; j++)
				//		{
				//			Real u = -y * (r_x*Svec[j]) / norm3;
				//			value(0, i * 2 + j) = u * n[0] + y * normInv * SvecP[j][0];
				//			value(1, i * 2 + j) = u * n[1] + y * normInv * SvecP[j][1];

				//			value(j, i * 2 + j) += SV[i];
				//		}

				//	}
		}

		break;
	}
	case AccessFunctionType::AngularVelocity_qt:
	{
		//const Index ns = 2;   //number of shape functions

		Real x = localPosition[0]; //only x-coordinate

		value.SetNumberOfRowsAndColumns(3, nODE2Coordinates); //3D velocity, 6 coordinates qt
		value.SetAll(0.); //last row not necessary to set to zero ... 
		
		Vector2D SV = ComputeShapeFunctions(x);
		value(2, 2) = SV[0];
		value(2, 5) = SV[1];

		break;
	}
	case AccessFunctionType::DisplacementMassIntegral_q:
	{
		value.SetNumberOfRowsAndColumns(3, nODE2Coordinates); //3D velocity, 6 coordinates qt
		value.SetAll(0.);

		Real L = parameters.physicsLength;
		Real rhoA = parameters.physicsMassPerLength;

		Vector2D SV = rhoA * L * ComputeShapeFunctions(0.); //0=midpoint at axis

		value(0, 0) = SV[0];
		value(1, 1) = SV[0];
		value(0, 3) = SV[1];
		value(1, 4) = SV[1];
		break;
	}
	default:
		SysError("CObjectBeamGeometricallyExact2D:GetAccessFunctionBody illegal accessType");
	}

}

//! provide according output variable in 'value'
void CObjectBeamGeometricallyExact2D::GetOutputVariableBody(OutputVariableType variableType, const Vector3D& localPosition, ConfigurationType configuration, Vector& value, Index objectNumber) const
{
	//CHECKandTHROWstring("CObjectBeamGeometricallyExact2D::GetOutputVariableBody not implemented!");

	switch (variableType)
	{
	case OutputVariableType::Position:	value.CopyFrom(GetPosition(localPosition, configuration)); break;
	case OutputVariableType::Displacement:	value.CopyFrom(GetPosition(localPosition, configuration) - GetPosition(localPosition, ConfigurationType::Reference)); break;
	case OutputVariableType::Velocity:	value.CopyFrom(GetVelocity(localPosition, configuration)); break;
	//case OutputVariableType::StrainLocal:	value.SetVector({ ComputeAxialStrain(localPosition[0], configuration) }); break;
	//case OutputVariableType::CurvatureLocal:	value.SetVector({ ComputeCurvature(localPosition[0], configuration) }); break;
	//case OutputVariableType::ForceLocal: {
	//	Real physicsBendingStiffness, physicsAxialStiffness, bendingDamping, axialDamping, physicsReferenceAxialStrain, physicsReferenceCurvature;
	//	GetMaterialParameters(physicsBendingStiffness, physicsAxialStiffness, bendingDamping, axialDamping, physicsReferenceAxialStrain, physicsReferenceCurvature);

	//	Real force = physicsAxialStiffness * (ComputeAxialStrain(localPosition[0], configuration) - physicsReferenceAxialStrain);
	//	if (axialDamping != 0) { force += axialDamping * ComputeAxialStrain_t(localPosition[0], configuration); }

	//	value.SetVector({ force }); break;
	//}
	//case OutputVariableType::TorqueLocal: {
	//	Real physicsBendingStiffness, physicsAxialStiffness, physicsReferenceAxialStrain, physicsReferenceCurvature, bendingDamping, axialDamping;
	//	GetMaterialParameters(physicsBendingStiffness, physicsAxialStiffness, bendingDamping, axialDamping, physicsReferenceAxialStrain, physicsReferenceCurvature);

	//	Real torque = physicsBendingStiffness * (ComputeCurvature(localPosition[0], configuration) - physicsReferenceCurvature);
	//	if (bendingDamping != 0) { torque += bendingDamping * ComputeCurvature_t(localPosition[0], configuration); }
	//	value.SetVector({ torque }); break;
	//}
	default:
		SysError("CObjectANCFCable2D::GetOutputVariableBody failed"); //error should not occur, because types are checked!
	}
}

//! return the (global) position of 'localPosition' according to configuration type
Vector3D CObjectBeamGeometricallyExact2D::GetPosition(const Vector3D& localPosition, ConfigurationType configuration) const
{
	Real x = localPosition[0]; //only x-coordinate
	Vector2D SV = ComputeShapeFunctions(x);

	Vector3D u = MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(configuration));
	if (configuration != ConfigurationType::Reference)
	{
		u += MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Reference), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(ConfigurationType::Reference));
	}
	//include off-axis position:
	if (localPosition[1] != 0.)
	{
		Vector2D p = GetRotationMatrix2D(u[2]) * Vector2D({ 0,localPosition[1] });
		u[0] += p[0];
		u[1] += p[1];
	}

	return Vector3D({ u[0], u[1],0. });

}

//! return the (global) displacemnet of 'localPosition' according to configuration type
Vector3D CObjectBeamGeometricallyExact2D::GetDisplacement(const Vector3D& localPosition, ConfigurationType configuration) const
{
	return GetPosition(localPosition, configuration) - GetPosition(localPosition, ConfigurationType::Reference);
}

//! return the (global) velocity of 'localPosition' according to configuration type
Vector3D CObjectBeamGeometricallyExact2D::GetVelocity(const Vector3D& localPosition, ConfigurationType configuration) const
{
	Real x = localPosition[0]; //only x-coordinate
	Vector2D SV = ComputeShapeFunctions(x);

	Vector3D u = MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(configuration));
	if (configuration != ConfigurationType::Reference)
	{
		u += MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Reference), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(ConfigurationType::Reference));
	}

	Vector3D v = MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector_t(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector_t(configuration));

	//include off-axis position:
	if (localPosition[1] != 0.)
	{
		//add velocity due to rotation of cross section
		Vector2D vRot = GetRotationMatrix2D(u[2]) * Vector2D({ -v[2]*localPosition[1] , 0}); //omega x locPos
		v[0] += vRot[0];
		v[1] += vRot[1];
	}

	return Vector3D({ v[0],v[1],0. });

}

//! return configuration dependent rotation matrix of node; returns always a 3D Matrix, independent of 2D or 3D object; for rigid bodies, the argument localPosition has no effect
Matrix3D CObjectBeamGeometricallyExact2D::GetRotationMatrix(const Vector3D& localPosition, ConfigurationType configuration) const
{
	Real x = localPosition[0]; //only x-coordinate
	Vector2D SV = ComputeShapeFunctions(x);

	Vector3D u = MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(configuration));
	if (configuration != ConfigurationType::Reference)
	{
		u += MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Reference), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(ConfigurationType::Reference));
	}

	return Matrix3D(3, 3, { cos(u[2]), -sin(u[2]), 0, 
		                    sin(u[2]),  cos(u[2]), 0,
		                    0,          0,         1}); //rotation about z-axis, stored in 3D matrix
}

//! return configuration dependent angular velocity of node; returns always a 3D Vector, independent of 2D or 3D object; for rigid bodies, the argument localPosition has no effect
Vector3D CObjectBeamGeometricallyExact2D::GetAngularVelocity(const Vector3D& localPosition, ConfigurationType configuration) const
{
	Real x = localPosition[0]; //only x-coordinate
	Vector2D SV = ComputeShapeFunctions(x);

	Vector3D v = MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector_t(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector_t(configuration));
	return Vector3D({0,0,v[2]});
}





