# Gravity
Orbits and Gravitational simulations using OpenGL and C++.

# The Algorithm
Firstly, the program calculates the gravitational force between all the celestial bodies by using [this formula](https://en.wikipedia.org/wiki/Newton's_law_of_universal_gravitation):

<img src="https://raw.githubusercontent.com/TheMisterOdd/Gravity/main/images/GravitationalForce.png" alt="GravitationalForce"/>

by using this other formula, the [force formula](https://en.wikipedia.org/wiki/Force):

<img src="https://raw.githubusercontent.com/TheMisterOdd/Gravity/main/images/Force.png" alt="GravitationalForce"/>

the program can get the acceleration of a body:

<img src="https://raw.githubusercontent.com/TheMisterOdd/Gravity/main/images/Acceleration.png" alt="Acceleration"/>

now, apply this acceleration to the movement [this movement equation](https://en.wikipedia.org/wiki/Leapfrog_integration):

<img src="https://raw.githubusercontent.com/TheMisterOdd/Gravity/main/images/LeapfrogIntegration.png" alt="Acceleration"/>

and you have movement.

###### NOTE: 
Some equations are silightly changed for a more fun experience. You should change this equations if you wanna have a realistic simulation.


# Example:

<img src="https://raw.githubusercontent.com/TheMisterOdd/Gravity/main/images/example.png" alt="window output"/>
