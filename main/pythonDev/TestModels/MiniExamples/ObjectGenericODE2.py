#+++++++++++++++++++++++++++++++++++++++++++
# Mini example for class ObjectGenericODE2
#+++++++++++++++++++++++++++++++++++++++++++

import sys
sys.path.append('../TestModels')

import exudyn as exu
from exudyn.itemInterface import *
from exudyn.utilities import *

from modelUnitTests import ExudynTestStructure, exudynTestGlobals
import numpy as np

#create an environment for mini example
SC = exu.SystemContainer()
mbs = SC.AddSystem()

oGround=mbs.AddObject(ObjectGround(referencePosition= [0,0,0]))
nGround = mbs.AddNode(NodePointGround(referenceCoordinates=[0,0,0]))

testError=1 #set default error, if failed
exu.Print("start mini example for class ObjectGenericODE2")
try: #puts example in safe environment
    #set up a mechanical system with two nodes; it has the structure: |~~M0~~M1
    nMass0 = mbs.AddNode(NodePoint(referenceCoordinates=[0,0,0]))
    nMass1 = mbs.AddNode(NodePoint(referenceCoordinates=[1,0,0]))

    mass = 0.5 * np.eye(3)      #mass of nodes
    stif = 5000 * np.eye(3)     #stiffness of nodes
    damp = 50 * np.eye(3)      #damping of nodes
    Z = 0. * np.eye(3)          #matrix with zeros
    #build mass, stiffness and damping matrices (:
    M = np.block([[mass,         0.*np.eye(3)],
                  [0.*np.eye(3), mass        ] ])
    K = np.block([[2*stif, -stif],
                  [ -stif,  stif] ])
    D = np.block([[2*damp, -damp],
                  [ -damp,  damp] ])
    
    oGenericODE2 = mbs.AddObject(ObjectGenericODE2(nodeNumbers=[nMass0,nMass1], 
                                                   massMatrix=M, 
                                                   stiffnessMatrix=K,
                                                   dampingMatrix=D))
    
    mNode1 = mbs.AddMarker(MarkerNodePosition(nodeNumber=nMass1))
    mbs.AddLoad(Force(markerNumber = mNode1, loadVector = [10, 0, 0])) #static solution=10*(1/5000+1/5000)=0.0004

    #assemble and solve system for default parameters
    mbs.Assemble()
    
    exu.SolveDynamic(mbs, solverType = exudyn.DynamicSolverType.TrapezoidalIndex2)

    #check result at default integration time
    exudynTestGlobals.testResult = mbs.GetNodeOutput(nMass1, exu.OutputVariableType.Position)[0]

except BaseException as e:
    exu.Print("An error occured in test example for ObjectGenericODE2:", e)
else:
    exu.Print("example for ObjectGenericODE2 completed, test error =", testError)

