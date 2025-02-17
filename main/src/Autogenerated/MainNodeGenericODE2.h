/** ***********************************************************************************************
* @class        MainNodeGenericODE2Parameters
* @brief        Parameter class for MainNodeGenericODE2
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2021-08-11  16:20:58 (last modified)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINNODEGENERICODE2PARAMETERS__H
#define MAINNODEGENERICODE2PARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "System/ItemIndices.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CNodeGenericODE2.h"

#include "Autogenerated/VisuNodeGenericODE2.h"

//! AUTO: Parameters for class MainNodeGenericODE2Parameters
class MainNodeGenericODE2Parameters // AUTO: 
{
public: // AUTO: 
    Vector initialCoordinates;                    //!< AUTO: initial displacement coordinates; must be consistent with numberOfODE2Coordinates
    Vector initialCoordinates_t;                  //!< AUTO: initial velocity coordinates; must be consistent with numberOfODE2Coordinates
    //! AUTO: default constructor with parameter initialization
    MainNodeGenericODE2Parameters()
    {
        initialCoordinates = Vector();
        initialCoordinates_t = Vector();
    };
};


/** ***********************************************************************************************
* @class        MainNodeGenericODE2
* @brief        A node containing a number of \hac{ODE2} variables; use e.g. for scalar dynamic equations (Mass1D) or for the ALECable element. Note that referenceCoordinates and all initialCoordinates(\_t) must be initialized, because no default values exist.
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

//! AUTO: MainNodeGenericODE2
class MainNodeGenericODE2: public MainNode // AUTO: 
{
protected: // AUTO: 
    CNodeGenericODE2* cNodeGenericODE2; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationNodeGenericODE2* visualizationNodeGenericODE2; //pointer to computational object (initialized in object factory) AUTO:
    MainNodeGenericODE2Parameters parameters; //! AUTO: contains all parameters for MainNodeGenericODE2

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainNodeGenericODE2()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CNodeGenericODE2* GetCNodeGenericODE2() { return cNodeGenericODE2; }
    //! AUTO: Get const pointer to computational class
    const CNodeGenericODE2* GetCNodeGenericODE2() const { return cNodeGenericODE2; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCNodeGenericODE2(CNodeGenericODE2* pCNodeGenericODE2) { cNodeGenericODE2 = pCNodeGenericODE2; }

    //! AUTO: Get pointer to visualization class
    VisualizationNodeGenericODE2* GetVisualizationNodeGenericODE2() { return visualizationNodeGenericODE2; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationNodeGenericODE2* GetVisualizationNodeGenericODE2() const { return visualizationNodeGenericODE2; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationNodeGenericODE2(VisualizationNodeGenericODE2* pVisualizationNodeGenericODE2) { visualizationNodeGenericODE2 = pVisualizationNodeGenericODE2; }

    //! AUTO: Get const pointer to computational base class object
    virtual CNode* GetCNode() const { return cNodeGenericODE2; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCNode(CNode* pCNode) { cNodeGenericODE2 = (CNodeGenericODE2*)pCNode; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationNode* GetVisualizationNode() const { return visualizationNodeGenericODE2; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationNode(VisualizationNode* pVisualizationNode) { visualizationNodeGenericODE2 = (VisualizationNodeGenericODE2*)pVisualizationNode; }

    //! AUTO: Write (Reference) access to parameters
    virtual MainNodeGenericODE2Parameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const MainNodeGenericODE2Parameters& GetParameters() const { return parameters; }

    //! AUTO:  Get type name of node (without keyword 'Node'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "GenericODE2";
    }

    //! AUTO:  return internally stored initial coordinates (displacements) of node
    virtual LinkedDataVector GetInitialVector() const override
    {
        return parameters.initialCoordinates;
    }

    //! AUTO:  return internally stored initial coordinates (velocities) of node
    virtual LinkedDataVector GetInitialVector_t() const override
    {
        return parameters.initialCoordinates_t;
    }

    //! AUTO:  Check consistency prior to CSystem::Assemble(); needs to find all possible violations such that Assemble() would fail
    virtual bool CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const override;


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cNodeGenericODE2->GetParameters().referenceCoordinates = py::cast<std::vector<Real>>(d["referenceCoordinates"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cNodeGenericODE2->GetParameters().numberOfODE2Coordinates = py::cast<Index>(d["numberOfODE2Coordinates"]); /* AUTO:  read out dictionary and cast to C++ type*/
        GetParameters().initialCoordinates = py::cast<std::vector<Real>>(d["initialCoordinates"]); /* AUTO:  read out dictionary and cast to C++ type*/
        GetParameters().initialCoordinates_t = py::cast<std::vector<Real>>(d["initialCoordinates_t"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationNodeGenericODE2->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["nodeType"] = (std::string)GetTypeName();
        d["referenceCoordinates"] = (std::vector<Real>)cNodeGenericODE2->GetParameters().referenceCoordinates; //! AUTO: cast variables into python (not needed for standard types) 
        d["numberOfODE2Coordinates"] = (Index)cNodeGenericODE2->GetParameters().numberOfODE2Coordinates; //! AUTO: cast variables into python (not needed for standard types) 
        d["initialCoordinates"] = (std::vector<Real>)GetParameters().initialCoordinates; //! AUTO: cast variables into python (not needed for standard types) 
        d["initialCoordinates_t"] = (std::vector<Real>)GetParameters().initialCoordinates_t; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationNodeGenericODE2->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { return py::cast((std::vector<Real>)cNodeGenericODE2->GetParameters().referenceCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates") == 0) { return py::cast((std::vector<Real>)GetParameters().initialCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates_t") == 0) { return py::cast((std::vector<Real>)GetParameters().initialCoordinates_t);} //! AUTO: get parameter
        else if (parameterName.compare("numberOfODE2Coordinates") == 0) { return py::cast((Index)cNodeGenericODE2->GetParameters().numberOfODE2Coordinates);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationNodeGenericODE2->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("NodeGenericODE2::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { cNodeGenericODE2->GetParameters().referenceCoordinates = py::cast<std::vector<Real>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates") == 0) { GetParameters().initialCoordinates = py::cast<std::vector<Real>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates_t") == 0) { GetParameters().initialCoordinates_t = py::cast<std::vector<Real>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("numberOfODE2Coordinates") == 0) { cNodeGenericODE2->GetParameters().numberOfODE2Coordinates = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationNodeGenericODE2->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("NodeGenericODE2::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};



#endif //#ifdef include once...
