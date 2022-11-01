Riemann Sum Visualizer
----------------------

This is a simple program that I made for my AP Calculus class when my teacher
introduced integrals and talked about Riemann Sums.

The program can display left, right, midpoint, and trapezoid Riemann Sums.

Controls
--------

The program is entirely controlled through the keyboard.

(I) -- Increase the interval size
(D) -- Decrease the interval size
(CTRL +) -- Zoom in
(CTRL -) -- Zoom out
(K) -- decrease the maximum value for which the function is displayed
(L) -- increase the maximum value for which the function is displayed
(H) -- decrease the minimum value for which the function is displayed
(J) -- increase the minimum value for which the function is displayed
(1) -- switch to left mode
(2) -- switch to right mode
(3) -- switch to midpoint mode
(4) -- switch to trapezoid mode
(5) -- hide the red highlight
(Arrow Keys) -- move around

To edit the function that is displayed, open "integral-frag.glsl" and change the 
function labelled "float f(float x)" to have a different return value.

Libraries used
--------------

GLFW
glad

License
-------

Code is public domain, do whatever you want with it.
