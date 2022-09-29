#version 430
in vec4 send_color;
out vec4 color;
void main(void)
{
    color = send_color;  
}

