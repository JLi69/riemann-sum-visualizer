#version 430

in vec4 fragCoord;

//Demonstrates the left rectangle approximation of a Riemann Sum

//Change this to change the interval
#define INTERVAL 0.5
//Change this change the max
#define MAX 3.0
//Change this to change the min
#define MIN -2.0
//Scale view
#define SCALE 80.0

//Change this to change the function
float f(float x)
{
	return x * x + 2.0;
}

out vec4 fragColor;

void main()
{
	fragColor = vec4(0.0, 0.0, 0.0, 1.0);

	vec2 trans = (fragCoord.xy) * 1.0 / SCALE;
    fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    if(trans.x < MAX && trans.x > MIN)
        if(f(trans.x) > 0.0 && trans.y > 0.0 && f(trans.x) > trans.y)
            fragColor += vec4(0.0, 1.0, 0.0, 1.0);
        else if(f(trans.x) < 0.0 && trans.y < 0.0 && f(trans.x) < trans.y)
            fragColor += vec4(0.0, 1.0, 0.0, 1.0);
    
    //Check if it's within the left rectangle
    float x = floor(trans.x / INTERVAL) * INTERVAL;
    if(trans.x < MAX && trans.x > MIN)
        if(f(trans.x) > 0.0 && trans.y > 0.0 && f(x) > trans.y)
            fragColor += vec4(1.0, 0.0, 0.0, 1.0);
        else if(f(trans.x) < 0.0 && trans.y < 0.0 && f(x) < trans.y)
            fragColor += vec4(1.0, 0.0, 0.0, 1.0);

}
