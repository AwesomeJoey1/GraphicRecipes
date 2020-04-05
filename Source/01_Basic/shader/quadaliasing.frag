#version 450

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D TexQuad;

void main() {
    FragColor = texture(TexQuad, TexCoord);
}
