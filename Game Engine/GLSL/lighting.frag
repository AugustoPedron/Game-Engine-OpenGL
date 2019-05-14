#version 330 core
#define NUMBER_OF_POINT_LIGHT 4

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D shadowMap;
	float shininess;
};

struct DirectionalLight{
	vec3 position;
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

out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
//in vec4 FragPosLightSpace;

uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight dirLight;
uniform SpotLight spotLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHT];
uniform int enableSpot = 0;
uniform int enableDir = 0;
uniform int enablePoint = 0;

vec3 CalcDirLight( DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir );
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir );
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir );
float ShadowCalculation(vec4 fragPosLightSpace);

void main( )
{
	vec3 result;
	vec3 norm = normalize( Normal );
	vec3 viewDir = normalize( viewPos - FragPos );
	float gamma = 2.2;

	if(enableDir == 1){
		result += CalcDirLight( dirLight, norm, FragPos, viewDir );
	}

	if(enablePoint == 1){
		for( int i = 0; i < NUMBER_OF_POINT_LIGHT; i++ ){
			result += CalcPointLight( pointLights[i], norm, FragPos, viewDir );
		}
	}

	if(enableSpot == 1){
		result += CalcSpotLight ( spotLight, norm, FragPos, viewDir );
	}
	//è il colore risultante da tutte le operazioni per il pixel in elaborazione
	color = vec4( pow( result.rgb, vec3 ( 1.0/gamma)), 1.0f );
}

vec3 CalcDirLight( DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir ){
	//vec3 lightDir = normalize( -light.direction );
	vec3 lightDir = normalize( light.position - fragPos );
	float diff = max( dot ( normal, lightDir ), 0.0 );
	vec3 halfwayDir = normalize( lightDir + viewDir );
	float spec = pow( max( dot( normal, halfwayDir ), 0.0 ), material.shininess );
	vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ));
	vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ));
	vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ));
	//float shadow = ShadowCalculation(FragPosLightSpace);  
	float shadow = 0.0f;

	return ambient + (1.0 - shadow) * (diffuse + specular);
}

vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir ){
	vec3 lightDir = normalize( light.position - fragPos );
	float diff = max( dot ( normal, lightDir ), 0.0 );

	float distance = length( light.position - fragPos );
	float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ));

	vec3 halfwayDir = normalize( lightDir + viewDir );
	float spec = pow( max( dot( normal, halfwayDir ), 0.0 ), material.shininess );
	vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ));
	vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ));
	vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ));
	//float shadow = ShadowCalculation(FragPosLightSpace);  
	float shadow = 0.0f;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + (1.0 - shadow) * (diffuse + specular);
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
	vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ));
	vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ));
	vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ));
	//float shadow = ShadowCalculation(FragPosLightSpace);  
	float shadow = 0.0f;

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return ambient + (1.0 - shadow) * (diffuse + specular);
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(material.shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
} 
