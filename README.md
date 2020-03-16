# Cloth-Simulation

This project implements the basic cloth simulation in C++ and uses modern OpenGL to render the scene.

## Build and Run the Code

### In MacOS:

  Clone the repo and open 'Cloth-Simulation.xcodeproj' in Xcode. 
  
  Then under 'Product', click 'Run'.
  
## User Control

### Select scenes:

'1': activate scene 1 (curtain)

'2': activate scene 2 (flag)

'3': activate scene 3 (parachute)

### Move objects:

'w': move objects in

's': move objects out

'a': move objects left

'd': move objects right

'o': move objects up

'l': move objects down

### Adjust wind speed:

'up': increase wind speed in negative z direction

'down': increase wind speed in positive z direction

'left': increase wind speed in negative x direction

'right': increase wind speed in positive x direction

## Algorithms and Techniques

The basic cloth simulation uses mass-spring and particle system that follows Newton's law. 

Simple forward Euler integration was used to update velocity and position of each particle. Cloth and ground collision was implemented so that cloth can slide on the plane.
