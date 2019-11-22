#version 450

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D TexQuad;

void main() {
    FragColor = texture(TexQuad, TexCoord);
    //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
