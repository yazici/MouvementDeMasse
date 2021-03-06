#version 120

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat3 NormalMatrix;

attribute vec3 position_att;
attribute vec3 normal_att;
attribute vec2 texCoord_att;

varying vec4 eyeVec;
varying vec3 normal;
varying vec2 texCoord;
varying float height;

void main(void)
{
    eyeVec = ViewMatrix * vec4(position_att, 1.0);
    gl_Position = ProjectionMatrix * eyeVec;

    normal = NormalMatrix * normal_att;
    height = position_att.z;

    texCoord = texCoord_att;
}
