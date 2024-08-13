#version 330 core
out vec4 FragColor;
in vec2 tc;
uniform sampler2D frameTexture;
// main function
void main()
{
    vec4 col = texture(frameTexture, tc);
    FragColor = vec4(col);
}