#version 330 core
//parametri che vengono passati per il caricamento delle mesh
layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;
layout ( location = 2 ) in vec2 texCoords;

//valori di output per il fragment shader. i nomi in out qua devono essere uguali ai nomi di in nel fragment shader perchè il compilatore li possa collegare
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

//valori che vengono passati in fase di definizione degli attributi
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main( )
{
	gl_Position = projection * view * model * vec4( position, 1.0f );
	FragPos = vec3( model * vec4( position, 1.0 ));
	Normal = ( transpose ( inverse ( mat3( model) ))) * normal;
	TexCoords = texCoords;
	FragPosLightSpace = lightSpaceMatrix * vec4( FragPos, 1.0 );
}