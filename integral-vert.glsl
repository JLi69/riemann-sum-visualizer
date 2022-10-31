#version 430

layout(location = 0) in vec4 pos;

out vec4 fragCoord;
uniform vec2 uWinSize;

void main()
{
	gl_Position = pos;
	fragCoord = vec4(pos.x * uWinSize.x, pos.y * uWinSize.y, 0.0, 1.0);
}
