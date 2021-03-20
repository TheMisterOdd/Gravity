# Gravity
Orbits and Gravitational simulations using OpenGL and C++.

# The Algorithm
Firstly, the program calculates the gravitational force between all the celestial bodies by using [this formula](https://en.wikipedia.org/wiki/Newton's_law_of_universal_gravitation):

<img src="https://render.githubusercontent.com/render/math?math=$F = G \frac{Mm}{r^2}$">

by using this other formula, the [force formula](https://en.wikipedia.org/wiki/Force):

<img src="https://render.githubusercontent.com/render/math?math=$F = ma$">

the program can get the acceleration of a body:

<img src="https://render.githubusercontent.com/render/math?math=$a = \frac{F}{m}$">

now, apply this acceleration to the movement [this movement equation](https://en.wikipedia.org/wiki/Leapfrog_integration):

<img src="https://render.githubusercontent.com/render/math?math=$x_{i++1} = x_i ++ v \Delta t ++ \frac{1}{2}a \Delta t$">

<img src="https://raw.githubusercontent.com/TheMisterOdd/Gravity/main/images/LeapfrogIntegration.png" alt="Acceleration"/>

and you have movement.

###### NOTE: 
Some equations are silightly changed for a more fun experience. You should change this equations if you wanna have a realistic simulation.


# Example:

<img src="https://raw.githubusercontent.com/TheMisterOdd/Gravity/main/images/example.png" alt="window output"/>
