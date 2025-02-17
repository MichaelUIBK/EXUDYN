#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN example
#
# Details:  Test with user-defined load function and user-defined spring-damper function (Duffing oscillator)
#
# Author:   Johannes Gerstmayr
# Date:     2019-11-15
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

import sys
sys.path.append('../TestModels')            #for modelUnitTest as this example may be used also as a unit test

import exudyn as exu
from exudyn.itemInterface import *

from modelUnitTests import ExudynTestStructure, exudynTestGlobals #for testing
import numpy as np

SC = exu.SystemContainer()
mbs = SC.AddSystem()
exu.Print('EXUDYN version='+exu.GetVersionString())

L=0.5
mass = 1.6          #mass in kg
spring = 4000       #stiffness of spring-damper in N/m
damper = 4          #damping constant in N/(m/s)
load0 = 80

omega0=np.sqrt(spring/mass)
f0 = 0.*omega0/(2*np.pi)
f1 = 1.*omega0/(2*np.pi)

exu.Print('resonance frequency = '+str(omega0))
tEnd = 50     #end time of simulation
steps = 5000  #number of steps

#tEnd = 0.05     #end time of simulation
#steps = 5  #number of steps


#user function for spring force
def springForce(mbs, t, itemIndex, u, v, k, d, offset, mu, muPropZone):
    return 0.1*k*u+k*u**3+v*d

#linear frequency sweep in time interval [0, t1] and frequency interval [f0,f1];
def Sweep(t, t1, f0, f1):
    k = (f1-f0)/t1
    return np.sin(2*np.pi*(f0+k*0.5*t)*t) #take care of factor 0.5 in k*0.5*t, in order to obtain correct frequencies!!!

#user function for load
def userLoad(mbs, t, load):
    #return load*np.sin(0.5*omega0*t) #gives resonance
    #exu.Print(t)
    return load*Sweep(t, tEnd, f0, f1)
    #return load*Sweep(t, tEnd, f1, f0) #backward sweep

#node for 3D mass point:
n1=mbs.AddNode(Point(referenceCoordinates = [L,0,0]))

#ground node
nGround=mbs.AddNode(NodePointGround(referenceCoordinates = [0,0,0]))

#add mass point (this is a 3D object with 3 coordinates):
massPoint = mbs.AddObject(MassPoint(physicsMass = mass, nodeNumber = n1))

#marker for ground (=fixed):
groundMarker=mbs.AddMarker(MarkerNodeCoordinate(nodeNumber= nGround, coordinate = 0))
#marker for springDamper for first (x-)coordinate:
nodeMarker  =mbs.AddMarker(MarkerNodeCoordinate(nodeNumber= n1, coordinate = 0))

#Spring-Damper between two marker coordinates
mbs.AddObject(CoordinateSpringDamper(markerNumbers = [groundMarker, nodeMarker], 
                                     stiffness = spring, damping = damper, 
                                     springForceUserFunction = springForce)) 

#add load:
loadC = mbs.AddLoad(LoadCoordinate(markerNumber = nodeMarker, 
                           load = load0, loadUserFunction=userLoad))

writeSensorFile = False
if exudynTestGlobals.useGraphics:
    writeSensorFile = True

mbs.AddSensor(SensorLoad(loadNumber=loadC, writeToFile = writeSensorFile, 
                         fileName="solution/userFunctionLoad.txt"))
#mbs.AddSensor(SensorNode(nodeNumber=n1, writeToFile = writeSensorFile, fileName="solution/userFunctionNode.txt"))
mbs.AddSensor(SensorNode(nodeNumber=n1, writeToFile = writeSensorFile, 
                         outputVariableType=exu.OutputVariableType.Coordinates, 
                         fileName="solution/userFunctionNode.txt"))
    
#exu.Print(mbs)
mbs.Assemble()

simulationSettings = exu.SimulationSettings()
simulationSettings.solutionSettings.solutionWritePeriod = 2e-3  #output interval
simulationSettings.timeIntegration.numberOfSteps = steps
simulationSettings.timeIntegration.endTime = tEnd

simulationSettings.timeIntegration.generalizedAlpha.spectralRadius = 1

simulationSettings.displayStatistics = True
simulationSettings.timeIntegration.verboseMode = 1

#exu.StartRenderer()              #start graphics visualization
#mbs.WaitForUserToContinue()    #wait for pressing SPACE bar to continue

#start solver:
exu.SolveDynamic(mbs, simulationSettings)

#SC.WaitForRenderEngineStopFlag()#wait for pressing 'Q' to quit
#exu.StopRenderer()               #safely close rendering window!

#evaluate final (=current) output values
u = mbs.GetNodeOutput(n1, exu.OutputVariableType.Position)
exu.Print('displacement=',u[0])

exudynTestGlobals.testError = u[0] - (0.5062872273010898) #2019-12-18: 0.5062872273010898; #2019-12-15: 0.5062872272996835; 2019-12-13:0.5062872273014417; 2019-12-01: 0.5152217339585201
exudynTestGlobals.testResult = u[0]

#+++++++++++++++++++++++++++++++++++++++++++++++++++++

if exudynTestGlobals.useGraphics:
    import matplotlib.pyplot as plt
    import matplotlib.ticker as ticker
    data = np.loadtxt('coordinatesSolution.txt', comments='#', delimiter=',')
    plt.plot(data[:,0], data[:,1], 'r-') #numerical solution
    
    ax=plt.gca() # get current axes
    ax.grid(True, 'major', 'both')
    ax.xaxis.set_major_locator(ticker.MaxNLocator(10)) 
    ax.yaxis.set_major_locator(ticker.MaxNLocator(10)) 
    plt.tight_layout()
    plt.show() 
