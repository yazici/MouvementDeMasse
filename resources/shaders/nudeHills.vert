#version 120

uniform mat4 Projection;
uniform mat4 ModelView;
uniform mat3 NormalMatrix;

attribute vec3 position_att;
attribute vec3 normal_att;
attribute vec4 color_att;

varying vec4 eyeVec;
varying vec3 normal;
varying vec4 color;

void main(void)
{
    eyeVec = ModelView * vec4(position_att, 1.0);
    gl_Position = Projection * eyeVec;

    normal = NormalMatrix * normal_att;

    color = color_att;
}
