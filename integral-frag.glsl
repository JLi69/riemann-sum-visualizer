#version 430

#define LEFT 0
#define RIGHT 1
#define MID 2
#define TRAPEZOID 3
#define HIDE 4

in vec4 fragCoord;

//Demonstrates the left rectangle approximation of a Riemann Sum

//Change this to change the interval
uniform float uInterval = 0.5;
//Change this change the max
uniform float uMax = 20.0;
//Change this to change the min
uniform float uMin = -20.0;
//Scale view
uniform float uScale = 40.0;
//center of the screen
uniform vec2 uCenter = vec2(0.0, 0.0);

uniform int uApproximationMethod = LEFT;

//Change this to change the function
float f(float x)
{
	return sin(x) * x;
}

out vec4 fragColor;

void main()
{
	fragColor = vec4(0.0, 0.0, 0.0, 1.0);

	vec2 trans = (fragCoord.xy) * 1.0 / uScale + uCenter;
    fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    if(trans.x < uMax && trans.x > uMin)
	{
        if(f(trans.x) > 0.0 && trans.y > 0.0 && f(trans.x) > trans.y)
            fragColor = vec4(0.0, 0.8, 0.0, 1.0);
        else if(f(trans.x) < 0.0 && trans.y < 0.0 && f(trans.x) < trans.y)
            fragColor = vec4(0.0, 0.8, 0.0, 1.0);
	} 

	float x;
	float x1, x2;
	float y;	
	switch(uApproximationMethod)
	{
	case LEFT:
		//Check if it's within the left rectangle
    	x = floor(trans.x / uInterval) * uInterval;
    	if(trans.x < uMax && trans.x > uMin)
		{ 
			if(f(trans.x) > 0.0 && trans.y > 0.0 && f(x) > trans.y)
    	        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    	    else if(f(trans.x) < 0.0 && trans.y < 0.0 && f(x) < trans.y)
    	        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
		break;
	case RIGHT:
		//Check if it's within the right rectangle
    	x = ceil(trans.x / uInterval) * uInterval;
    	if(trans.x < uMax && trans.x > uMin)
		{ 
			if(f(trans.x) > 0.0 && trans.y > 0.0 && f(x) > trans.y)
    	        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    	    else if(f(trans.x) < 0.0 && trans.y < 0.0 && f(x) < trans.y)
    	        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}		
		break;
	case MID:
		//Check if it's within the rectangle
    	x1 = floor(trans.x / uInterval) * uInterval;		
    	x2 = ceil(trans.x / uInterval) * uInterval;
		if(trans.x < uMax && trans.x > uMin)
		{ 
			if(f(trans.x) > 0.0 && trans.y > 0.0 && f((x1 + x2) * 0.5) > trans.y)
    	        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    	    else if(f(trans.x) < 0.0 && trans.y < 0.0 && f((x1 + x2) * 0.5) < trans.y)
    	        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}		
		break;
	case TRAPEZOID:
		//Check if it's within the trapezoid
    	x1 = floor(trans.x / uInterval) * uInterval;		
    	x2 = ceil(trans.x / uInterval) * uInterval;
		y = (f(x2) - f(x1)) / (x2 - x1) * (trans.x - x1) + f(x1);
		if(trans.x < uMax && trans.x > uMin)
		{ 
			if(f(trans.x) > 0.0 && trans.y > 0.0 && y > trans.y)
    	        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    	    else if(f(trans.x) < 0.0 && trans.y < 0.0 && y < trans.y)
    	        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}	
	default:
		break;
	}
	
	//Draw axes
	if(abs(trans.x) <= 2.0 * 1.0 / uScale || abs(trans.y) <= 2.0 * 1.0 / uScale)
		fragColor = vec4(1.0, 1.0, 1.0, 1.0);

	//Draw line tickers
	if(abs(trans.x - round(trans.x)) <= 2.0 * 1.0 / uScale && abs(trans.y) <= 8.0 * 1.0 / uScale)
		fragColor = vec4(1.0, 1.0, 1.0, 1.0);
	if(abs(trans.y - round(trans.y)) <= 2.0 * 1.0 / uScale && abs(trans.x) <= 8.0 * 1.0 / uScale)
		fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
