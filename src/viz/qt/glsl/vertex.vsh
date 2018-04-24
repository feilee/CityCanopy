attribute vec4 vertex;
attribute vec3 normal;
attribute vec3 color;
varying vec3 vert;
varying vec3 vertNormal;
varying vec3 color_out;
uniform mat4 projMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;
uniform highp vec3 lightPos;
void main() {
   color_out = color;
   vert = vec3(mvMatrix * vertex);
   vertNormal = normalize(normalMatrix * normal);
   gl_Position = projMatrix * mvMatrix * vertex;
}
