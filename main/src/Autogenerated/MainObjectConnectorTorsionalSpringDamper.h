/** ***********************************************************************************************
* @class        MainObjectConnectorTorsionalSpringDamperParameters
* @brief        Parameter class for MainObjectConnectorTorsionalSpringDamper
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2021-09-16  17:25:12 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINOBJECTCONNECTORTORSIONALSPRINGDAMPERPARAMETERS__H
#define MAINOBJECTCONNECTORTORSIONALSPRINGDAMPERPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include <pybind11/functional.h> //! AUTO: for function handling ... otherwise gives a python error (no compilation error in C++ !)
#include "Autogenerated/CObjectConnectorTorsionalSpringDamper.h"

#include "Autogenerated/VisuObjectConnectorTorsionalSpringDamper.h"

//! AUTO: Parameters for class MainObjectConnectorTorsionalSpringDamperParameters
class MainObjectConnectorTorsionalSpringDamperParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectConnectorTorsionalSpringDamper
* @brief        An torsional spring-damper element acting on relative rotations around Z-axis of local joint0 coordinate system; connects to orientation-based markers; if other rotation axis than the local joint0 Z axis shall be used, the joint rotationMarker0 / rotationMarker1 may be used. The joint perfectly extends a RevoluteJoint with a spring-damper, which can also be used to represent feedback control in an elegant and efficient way, by chosing appropriate user functions. It also allows to measure continuous / infinite rotations by making use of a NodeGeneric which compensates \f$\pm \pi\f$ jumps in the measured rotation (OutputVariableType.Rotation).
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

//! AUTO: MainObjectConnectorTorsionalSpringDamper
class MainObjectConnectorTorsionalSpringDamper: public MainObjectConnector // AUTO: 
{
protected: // AUTO: 
    CObjectConnectorTorsionalSpringDamper* cObjectConnectorTorsionalSpringDamper; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectConnectorTorsionalSpringDamper* visualizationObjectConnectorTorsionalSpringDamper; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectConnectorTorsionalSpringDamper()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectConnectorTorsionalSpringDamper* GetCObjectConnectorTorsionalSpringDamper() { return cObjectConnectorTorsionalSpringDamper; }
    //! AUTO: Get const pointer to computational class
    const CObjectConnectorTorsionalSpringDamper* GetCObjectConnectorTorsionalSpringDamper() const { return cObjectConnectorTorsionalSpringDamper; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectConnectorTorsionalSpringDamper(CObjectConnectorTorsionalSpringDamper* pCObjectConnectorTorsionalSpringDamper) { cObjectConnectorTorsionalSpringDamper = pCObjectConnectorTorsionalSpringDamper; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectConnectorTorsionalSpringDamper* GetVisualizationObjectConnectorTorsionalSpringDamper() { return visualizationObjectConnectorTorsionalSpringDamper; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectConnectorTorsionalSpringDamper* GetVisualizationObjectConnectorTorsionalSpringDamper() const { return visualizationObjectConnectorTorsionalSpringDamper; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectConnectorTorsionalSpringDamper(VisualizationObjectConnectorTorsionalSpringDamper* pVisualizationObjectConnectorTorsionalSpringDamper) { visualizationObjectConnectorTorsionalSpringDamper = pVisualizationObjectConnectorTorsionalSpringDamper; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectConnectorTorsionalSpringDamper; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectConnectorTorsionalSpringDamper = (CObjectConnectorTorsionalSpringDamper*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectConnectorTorsionalSpringDamper; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectConnectorTorsionalSpringDamper = (VisualizationObjectConnectorTorsionalSpringDamper*)pVisualizationObject; }

    //! AUTO:  provide requested nodeType for objects; used for automatic checks in CheckSystemIntegrity()
    virtual Node::Type GetRequestedNodeType() const override
    {
        return Node::GenericData;
    }

    //! AUTO:  Get type name of node (without keyword 'Object'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "ConnectorTorsionalSpringDamper";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectConnectorTorsionalSpringDamper->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(d["markerNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "nodeNumber")) { cObjectConnectorTorsionalSpringDamper->GetParameters().nodeNumber = EPyUtils::GetNodeIndexSafely(d["nodeNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        cObjectConnectorTorsionalSpringDamper->GetParameters().stiffness = py::cast<Real>(d["stiffness"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectConnectorTorsionalSpringDamper->GetParameters().damping = py::cast<Real>(d["damping"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetMatrix3DSafely(d, "rotationMarker0", cObjectConnectorTorsionalSpringDamper->GetParameters().rotationMarker0); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetMatrix3DSafely(d, "rotationMarker1", cObjectConnectorTorsionalSpringDamper->GetParameters().rotationMarker1); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "offset")) { cObjectConnectorTorsionalSpringDamper->GetParameters().offset = py::cast<Real>(d["offset"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "torque")) { cObjectConnectorTorsionalSpringDamper->GetParameters().torque = py::cast<Real>(d["torque"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "activeConnector")) { cObjectConnectorTorsionalSpringDamper->GetParameters().activeConnector = py::cast<bool>(d["activeConnector"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "springTorqueUserFunction")) { if (EPyUtils::CheckForValidFunction(d["springTorqueUserFunction"])) 
            { cObjectConnectorTorsionalSpringDamper->GetParameters().springTorqueUserFunction = py::cast<std::function<Real(const MainSystem&,Real,Index,Real,Real,Real,Real,Real)>>((py::function)d["springTorqueUserFunction"]); /* AUTO:  read out dictionary and cast to C++ type*/}
            else {cObjectConnectorTorsionalSpringDamper->GetParameters().springTorqueUserFunction = 0;  /*AUTO: otherwise assign with zero!*/ }} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectConnectorTorsionalSpringDamper->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawSize")) { visualizationObjectConnectorTorsionalSpringDamper->GetDrawSize() = py::cast<float>(d["VdrawSize"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectConnectorTorsionalSpringDamper->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["markerNumbers"] = EPyUtils::GetArrayMarkerIndex(cObjectConnectorTorsionalSpringDamper->GetParameters().markerNumbers); //! AUTO: cast variables into python (not needed for standard types) 
        d["nodeNumber"] = (NodeIndex)cObjectConnectorTorsionalSpringDamper->GetParameters().nodeNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["stiffness"] = (Real)cObjectConnectorTorsionalSpringDamper->GetParameters().stiffness; //! AUTO: cast variables into python (not needed for standard types) 
        d["damping"] = (Real)cObjectConnectorTorsionalSpringDamper->GetParameters().damping; //! AUTO: cast variables into python (not needed for standard types) 
        d["rotationMarker0"] = EXUmath::Matrix3DToStdArray33(cObjectConnectorTorsionalSpringDamper->GetParameters().rotationMarker0); //! AUTO: cast variables into python (not needed for standard types) 
        d["rotationMarker1"] = EXUmath::Matrix3DToStdArray33(cObjectConnectorTorsionalSpringDamper->GetParameters().rotationMarker1); //! AUTO: cast variables into python (not needed for standard types) 
        d["offset"] = (Real)cObjectConnectorTorsionalSpringDamper->GetParameters().offset; //! AUTO: cast variables into python (not needed for standard types) 
        d["torque"] = (Real)cObjectConnectorTorsionalSpringDamper->GetParameters().torque; //! AUTO: cast variables into python (not needed for standard types) 
        d["activeConnector"] = (bool)cObjectConnectorTorsionalSpringDamper->GetParameters().activeConnector; //! AUTO: cast variables into python (not needed for standard types) 
        if (cObjectConnectorTorsionalSpringDamper->GetParameters().springTorqueUserFunction)
            {d["springTorqueUserFunction"] = (std::function<Real(const MainSystem&,Real,Index,Real,Real,Real,Real,Real)>)cObjectConnectorTorsionalSpringDamper->GetParameters().springTorqueUserFunction;}
        else
            {d["springTorqueUserFunction"] = 0;}
 //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectConnectorTorsionalSpringDamper->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawSize"] = (float)visualizationObjectConnectorTorsionalSpringDamper->GetDrawSize(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectConnectorTorsionalSpringDamper->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { return py::cast(EPyUtils::GetArrayMarkerIndex(cObjectConnectorTorsionalSpringDamper->GetParameters().markerNumbers));} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { return py::cast((NodeIndex)cObjectConnectorTorsionalSpringDamper->GetParameters().nodeNumber);} //! AUTO: get parameter
        else if (parameterName.compare("stiffness") == 0) { return py::cast((Real)cObjectConnectorTorsionalSpringDamper->GetParameters().stiffness);} //! AUTO: get parameter
        else if (parameterName.compare("damping") == 0) { return py::cast((Real)cObjectConnectorTorsionalSpringDamper->GetParameters().damping);} //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker0") == 0) { return py::cast(EXUmath::Matrix3DToStdArray33(cObjectConnectorTorsionalSpringDamper->GetParameters().rotationMarker0));} //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker1") == 0) { return py::cast(EXUmath::Matrix3DToStdArray33(cObjectConnectorTorsionalSpringDamper->GetParameters().rotationMarker1));} //! AUTO: get parameter
        else if (parameterName.compare("offset") == 0) { return py::cast((Real)cObjectConnectorTorsionalSpringDamper->GetParameters().offset);} //! AUTO: get parameter
        else if (parameterName.compare("torque") == 0) { return py::cast((Real)cObjectConnectorTorsionalSpringDamper->GetParameters().torque);} //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { return py::cast((bool)cObjectConnectorTorsionalSpringDamper->GetParameters().activeConnector);} //! AUTO: get parameter
        else if (parameterName.compare("springTorqueUserFunction") == 0) { return py::cast((std::function<Real(const MainSystem&,Real,Index,Real,Real,Real,Real,Real)>)cObjectConnectorTorsionalSpringDamper->GetParameters().springTorqueUserFunction);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectConnectorTorsionalSpringDamper->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { return py::cast((float)visualizationObjectConnectorTorsionalSpringDamper->GetDrawSize());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectConnectorTorsionalSpringDamper->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectConnectorTorsionalSpringDamper::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { cObjectConnectorTorsionalSpringDamper->GetParameters().markerNumbers = EPyUtils::GetArrayMarkerIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { cObjectConnectorTorsionalSpringDamper->GetParameters().nodeNumber = EPyUtils::GetNodeIndexSafely(value); /* AUTO:  read out dictionary, check if correct index used and store (converted) Index to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("stiffness") == 0) { cObjectConnectorTorsionalSpringDamper->GetParameters().stiffness = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("damping") == 0) { cObjectConnectorTorsionalSpringDamper->GetParameters().damping = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker0") == 0) { EPyUtils::SetMatrix3DSafely(value, cObjectConnectorTorsionalSpringDamper->GetParameters().rotationMarker0); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("rotationMarker1") == 0) { EPyUtils::SetMatrix3DSafely(value, cObjectConnectorTorsionalSpringDamper->GetParameters().rotationMarker1); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("offset") == 0) { cObjectConnectorTorsionalSpringDamper->GetParameters().offset = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("torque") == 0) { cObjectConnectorTorsionalSpringDamper->GetParameters().torque = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { cObjectConnectorTorsionalSpringDamper->GetParameters().activeConnector = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("springTorqueUserFunction") == 0) { if (py::isinstance<py::function>(value)) {cObjectConnectorTorsionalSpringDamper->GetParameters().springTorqueUserFunction = py::cast<std::function<Real(const MainSystem&,Real,Index,Real,Real,Real,Real,Real)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/} else
            if (!EPyUtils::IsPyTypeInteger(value) || (py::cast<int>(value) != 0)) {PyError(STDstring("Failed to convert PyFunction: must be either valid python function or 0, but got ")+EXUstd::ToString(value)); }; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectConnectorTorsionalSpringDamper->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { visualizationObjectConnectorTorsionalSpringDamper->GetDrawSize() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectConnectorTorsionalSpringDamper->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectConnectorTorsionalSpringDamper::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};



#endif //#ifdef include once...
