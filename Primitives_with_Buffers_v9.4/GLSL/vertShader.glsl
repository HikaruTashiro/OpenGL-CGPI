#version 430

layout (location = 0) in vec2 position;
uniform vec4 received_color;
out vec4 send_color;
void main(void)
{
    gl_Position = vec4(position,0.0,1.0);
    send_color = received_color;
}
