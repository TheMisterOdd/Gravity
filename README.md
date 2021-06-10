# Gravity
Orbits and Gravitational simulations using OpenGL and C.

# The Algorithm
Firstly, the program calculates the gravitational force between all the celestial bodies by using [this formula](https://en.wikipedia.org/wiki/Newton's_law_of_universal_gravitation):

<img src="https://render.githubusercontent.com/render/math?math=$F = G \frac{Mm}{r^2}$">

by using this other formula, the [force formula](https://en.wikipedia.org/wiki/Force):

<img src="https://render.githubusercontent.com/render/math?math=$F = ma$">

the program can get the acceleration of a body:

<img src="https://render.githubusercontent.com/render/math?math=$a = \frac{F}{m}$">
<img src="https://render.githubusercontent.com/render/math?math=$a = \frac{GM}{r^2}$">

now, apply this acceleration to [this movement equation](https://en.wikipedia.org/wiki/Leapfrog_integration):

<img src="https://render.githubusercontent.com/render/math?math=$x_{i %2B 1} = x_i %2B v \Delta t %2B \frac{1}{2}a \Delta t^2$">


# Example:

<img src="https://raw.githubusercontent.com/TheMisterOdd/Gravity/main/images/example.png" alt="window output"/>
