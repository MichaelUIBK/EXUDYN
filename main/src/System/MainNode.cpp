/** ***********************************************************************************************
* @brief		Implementation for MainNode
*
* @author		Gerstmayr Johannes
* @date			2019-07-23 (generated)
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

#include "Main/MainSystem.h"
//#include "Pymodules/PybindUtilities.h"



//! GetOutputVariable with type and return value; copies values==>slow!; can be scalar or vector-valued! maps to CNode GetOutputVariable(...)
py::object MainNode::GetOutputVariable(OutputVariableType variableType, ConfigurationType configuration) const
{
	Vector value;
	//check if type is valid:
	if ((Index)GetCNode()->GetOutputVariableTypes() & (Index)variableType)
	{
		GetCNode()->GetOutputVariable(variableType, configuration, value);

		//now check if it is scalar or a vector-valued:
		if (value.NumberOfItems() == 1) { return py::float_(value[0]); }
		else { return py::array_t<Real>(value.NumberOfItems(), value.GetDataPointer()); }
	}
	else
	{
		PyError(STDstring("Invalid OutputVariableType in MainNode::GetOutputVariable: '") + GetOutputVariableTypeString(variableType) + "'");
		return py::int_(EXUstd::InvalidIndex);
		//return py::object();
	}
}
