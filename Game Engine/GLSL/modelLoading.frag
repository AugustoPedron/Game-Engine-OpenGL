#version 330 core

#define NUMBER_OF_POINT_LIGHT 4

struct Material{
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	float shininess;
};

struct DirectionalLight{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight dirLight;
uniform SpotLight spotLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHT];
uniform int enableSpot = 0;
uniform int enableDir = 0;
uniform int enablePoint = 0;

vec3 CalcDirLight( DirectionalLight light, vec3 normal, vec3 viewDir );
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir );
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir );

void main( )
{
	vec3 result;
	vec3 norm = normalize( Normal );
	vec3 viewDir = normalize( viewPos - FragPos );
	float gamma = 2.2;

	if(enableDir){
		result += CalcDirLight( dirLight, norm, viewDir );
	}

	if(enablePoint){
		for( int i = 0; i < NUMBER_OF_POINT_LIGHT; i++ ){
			result += CalcPointLight( pointLights[i], norm, FragPos, viewDir );
		}
	}

	if(enableSpot){
		result += CalcSpotLight ( spotLight, norm, FragPos, viewDir );
	}

	color = vec4( result, 1.0f );
}

vec3 CalcDirLight( DirectionalLight light, vec3 normal, vec3 viewDir ){
	vec3 lightDir = normalize( -light.direction );
	float diff = max( dot ( normal, lightDir ), 0.0 );
	vec3 halfwayDir = normalize( lightDir + viewDir );
	float spec = pow( max( dot( normal, halfwayDir ), 0.0 ), material.shininess );
	vec3 ambient = light.ambient * vec3( texture( texture_diffuse, TexCoords ));
	vec3 diffuse = light.diffuse * diff * vec3( texture( texture_diffuse, TexCoords ));
	vec3 specular = light.specular * spec * vec3( texture( texture_specular, TexCoords ));

	return ambient + diffuse + specular;
}

vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir ){
	vec3 lightDir = normalize( light.position - fragPos );
	float diff = max( dot ( normal, lightDir ), 0.0 );

	float distance = length( light.position - fragPos );
	float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ));

	vec3 halfwayDir = normalize( lightDir + viewDir );
	float spec = pow( max( dot( normal, halfwayDir ), 0.0 ), material.shininess );
	vec3 ambient = light.ambient * vec3( texture( texture_diffuse, TexCoords ));
	vec3 diffuse = light.diffuse * diff * vec3( texture( texture_diffuse, TexCoords ));
	vec3 specular = light.specular * spec * vec3( texture( texture_specular, TexCoords ));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir ){
	vec3 lightDir = normalize( light.position - fragPos );
	float diff = max( dot ( normal, lightDir ), 0.0 );

	float distance = length( light.position - fragPos );
	float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ));

	float theta = dot( lightDir, normalize( -light.direction ));
	float epsilon = ( light.cutOff - light.outerCutOff );
	float intensity = clamp(( theta - light.outerCutOff ) / epsilon, 0.0, 1.0);

	vec3 halfwayDir = normalize( lightDir + viewDir );
	float spec = pow( max( dot( normal, halfwayDir ), 0.0 ), material.shininess );
	vec3 ambient = light.ambient * vec3( texture( texture_diffuse, TexCoords ));
	vec3 diffuse = light.diffuse * diff * vec3( texture( texture_diffuse, TexCoords ));
	vec3 specular = light.specular * spec * vec3( texture( texture_specular, TexCoords ));

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return ambient + diffuse + specular;
}