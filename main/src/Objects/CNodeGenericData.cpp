/** ***********************************************************************************************
* @brief		Implementation for CNodeGenericData
*
* @author		Gerstmayr Johannes
* @date			2019-05-02 (generated)
* @pre			...
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
* @note			Bug reports, support and further information:
* 				- email: johannes.gerstmayr@uibk.ac.at
* 				- weblink: https://github.com/jgerstmayr/EXUDYN
* 				
*
* *** Example code ***
*
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CNodeGenericData.h"


//! provide according output variable in "value"
void CNodeGenericData::GetOutputVariable(OutputVariableType variableType, ConfigurationType configuration, Vector& value) const
{
	switch (variableType)
	{
	case OutputVariableType::Coordinates:
	{
		if (IsConfigurationInitialCurrentVisualization(configuration))//((Index)configuration & ((Index)ConfigurationType::Current + (Index)ConfigurationType::Initial + (Index)ConfigurationType::Visualization))
		{
			value = GetCoordinateVector(configuration);
		}
		else
		{
			PyError("CNodeGenericData::GetOutputVariable: invalid configuration");
		}
		break;
	}
	default:
		SysError("CNodeGenericData::GetOutputVariable failed"); //error should not occur, because types are checked!
	}
}

