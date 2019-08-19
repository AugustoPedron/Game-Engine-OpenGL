#version 330 core

#define NUMBER_OF_POINT_LIGHT 4

struct Material{
	float shininess;
};

struct DirectionalLight{
	vec3 direction;
	vec3 position;
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
in vec4 FragPosLightSpace;

out vec4 color;

uniform Material material;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D shadowMap;
uniform vec3 viewPos;
uniform DirectionalLight dirLight;
uniform SpotLight spotLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHT];
uniform int enableSpot = 0;
uniform int enableDir = 0;
uniform int enablePoint = 0;

vec3 CalcDirLight( DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir );
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir );
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir );
float ShadowCalculation(vec4 fragPosLightSpace, vec3 Normal, vec3 LightDir);

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

	color = vec4( result, 1.0f );
}

vec3 CalcDirLight( DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir ){
	//vec3 lightDir = normalize( -light.direction );
	vec3 lightDir = normalize( light.position - fragPos );
	float diff = max( dot ( normal, lightDir ), 0.0 );
	vec3 halfwayDir = normalize( lightDir + viewDir );
	float spec = pow( max( dot( normal, halfwayDir ), 0.0 ), material.shininess );
	vec3 ambient = light.ambient * vec3( texture( texture_diffuse, TexCoords ));
	vec3 diffuse = light.diffuse * diff * vec3( texture( texture_diffuse, TexCoords ));
	vec3 specular = light.specular * spec * vec3( texture( texture_specular, TexCoords ));
	float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);  

	return (ambient + (1.0 - shadow) * (diffuse + specular));
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
	float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);  

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + (1.0 - shadow) * (diffuse + specular));
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
	float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);  

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + (1.0 - shadow) * (diffuse + specular));
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 Normal, vec3 LightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
	if(projCoords.z > 1.0){
		return 0.0;
	}
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	float bias = max(0.05 * (1.0 - dot(Normal, LightDir)), 0.005);  
    	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;
    return shadow;
} 
