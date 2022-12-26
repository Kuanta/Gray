#version 420 core
#define PI 3.14
#define MAX_NUM_LIGHTS 100
out vec4 FragColor;
layout (location = 0) out vec4 colors;
layout (location = 1) out vec4 shadowMask;
in vec2 TexCoords;
layout(binding=0) uniform sampler2D gPositions;
layout(binding=1) uniform sampler2D gNormals;
layout(binding=2) uniform sampler2D gColors;
layout(binding=3) uniform sampler2D gPbr;

struct Material
{
	vec3 color;
	float shininess;
	float metalness;
	float roughness;
};

//Light types
const int AMBIENT_LIGHT = 0;
const int DIRECTIONAL_LIGHT = 1;
const int POINT_LIGHT = 2;
const int SOFTNESS_FACTOR = 1;
struct Light
{
	mat4 lightMatrix;  //For directional
	vec4 color;		//4*4 = 16
	vec4 position;  //4*4 = 32
	vec4 direction;	//4*4 = 48
	int type;	//4
	float intensity;	//4 
	float constant;	//4
	float linear;	//4
	float quadratic;	//4
	int castShadow;
	float far_plane;
	float attenuationFactor;
	int softnessFactor;
	float dummy1;
	float dummy2;
	float dummy3;
	//Total: 48+20 = 68 (Must be a multiple of 16, so ve set it to 16*5=80, LOSS OF 12 BYTES)
};

float geomSchlick(Material material, vec3 normal, float dotProd, float roughness);
float geomSmith(Material material, vec3 normal, vec3 view, vec3 halfway);
vec3 schlickFresnel(Material material, vec3 lightDir, vec3 halfWay);
float ggxDistribution(Material material, float nDotH);
vec3 calcRadiance(Material material, Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
float shadowCalculation();
float calculateDirectionalShadow(Light currLight, vec3 fragPos, int lightIndex);
float calculatePointShadow(Light light, vec3 fragPos, vec3 fragNormal, int lightIndex);

layout (std140, binding=1) uniform Lights
{
	Light allLights[MAX_NUM_LIGHTS]; //144*MAX_NUM_LIGHTS
	int numLights; //Current number of lights in the scene //4
	//Total: 144*MAX_NUM_LIGHTS+4
};

//Uniform matrices
layout (std140, binding=0) uniform Matrices
{
	mat4 view;		//16*4 bytes
	mat4 projection;	//16*4 bytes
	vec3 eyePosition;	//3*4
						//Total: 16*8+12
};

//Shadows

layout(binding=5) uniform sampler2DArrayShadow dynamicShadowMaps;
layout(binding=6) uniform samplerCubeArray dynamicPointShadowMaps;
layout(binding=7) uniform sampler2DArrayShadow shadowMaps; //TODO: The binding point may change in future
layout(binding=8) uniform samplerCubeArray pointShadowMaps;

void main()
{

    vec3 fragPos = texture(gPositions, TexCoords).rgb;
    vec3 normal = texture(gNormals, TexCoords).rgb;
	if(length(normal) > 0)
	{
		normal = normalize(normal);
	}
    vec3 diffuse = texture(gColors, TexCoords).rgb;
    vec3 eyeVec = normalize(eyePosition - fragPos);

	
	if(normal.x + normal.y + normal.z == 0)
	{
		//This is skybox
		colors = vec4(diffuse,1);
		shadowMask= vec4(0);
		return;
	}
    Material mat;
	vec3 pbrProps =  texture(gPbr, TexCoords).rgb;
    mat.color = diffuse;

	mat.metalness = pbrProps.g;
    mat.roughness = pbrProps.r;

    vec3 illumination = vec3(0.0);
	int directLightIndex = 0;
	int pointLightIndex = 0;
	float shadow = 0;
	float _shadowMask = 0;
    for (int i=0; i<numLights; i++)
    {
		shadow = 0;
        Light currLight = allLights[i];
		if(currLight.castShadow == 1)  //Light must cast shadow and object must receive it
		{
			if(currLight.type == 0)
			{
				shadow = 1;
			}
			else if(currLight.type == 1) //Direct light
			{
				shadow = calculateDirectionalShadow(currLight, fragPos, directLightIndex);
			}
			else if(currLight.type == 2)
			{
				//Check attenuation of point light
				float attenuation = pow(length(currLight.position.xyz-fragPos) / currLight.attenuationFactor,2);
				if(attenuation <= 0.1f)
				{
					continue;
				}
				shadow = calculatePointShadow(currLight, fragPos, normal, pointLightIndex);
			}
			
		}else
		{
			//Ambient
			shadow = 1.0f;
		}
		shadow = max(0.0f,min(shadow, 1.0f));
		_shadowMask += shadow;
        vec3 _illumination = shadow*calcRadiance(mat, currLight, normal, fragPos, eyeVec);
        illumination += _illumination;

		//Increment Counters 
		if(currLight.type == 1)
		{
			directLightIndex++;
		}else if(currLight.type == 2)
		{
			pointLightIndex++;
		}
    }
	_shadowMask = min(_shadowMask / numLights, 1.0f);
    //illumination = min(max(illumination,0.0f), 1.0f);
	illumination = illumination / (illumination + vec3(1.0));
	illumination = pow(illumination, vec3(1.0/1.5));
    //FragColor = vec4(illumination,1);
	colors = vec4(illumination,1);
	shadowMask.rgb = vec3(_shadowMask);

}

//Light Calc. Funcs
vec3 schlickFresnel(Material material, vec3 lightDir, vec3 halfWay)
{
	vec3 F0;
	F0 = vec3(0.04);
	F0 = mix(F0, material.color, material.metalness);
	return F0+(1-F0)*pow(max(1.0-dot(lightDir, halfWay), 0.0f),5);
}
float geomSchlick(Material material, vec3 normal, float dotProd, float roughness)
{
	float k = (material.roughness+1.0)*(material.roughness+1.0)/8.0;
	//float dotProd = max(dot(normal, view), 0);
	return dotProd/(dotProd*(1.0-k)+k);
}
float geomSmith(Material material, vec3 normal, vec3 view, vec3 halfway){
	float NdotV = max(dot(normal, view), 0.0f);
	float NdotL = max(dot(normal, halfway), 0.0f);
	float ggx2 = geomSchlick(material, normal, NdotV, material.roughness);
	float ggx1 = geomSchlick(material, normal, NdotL, material.roughness);
	return ggx2;
}

float ggxDistribution(Material material, float nDotH)
{
	float alpha2 = pow(material.roughness,4);
	float d = (nDotH*nDotH)*(alpha2-1)+1;
	return alpha2 / (PI*d*d);
}

vec3 calcRadiance(Material material, Light light, vec3 normal, vec3 fragPos, vec3 viewDir){

	//Shadow Calculation

	vec3 lightDir;
	if(light.type == AMBIENT_LIGHT)
	{
		return min(max(light.color.xyz*material.color*light.intensity,0.0f),1.0f);
	}else if(light.type == DIRECTIONAL_LIGHT)
	{
		lightDir = -1*normalize(light.direction.xyz);
	}
	else if(light.type == POINT_LIGHT)
	{
		lightDir = normalize(light.position.xyz - fragPos);
	}else{
		return vec3(0);
	}
	float lightIntensity = light.intensity;
	vec3 halfway = normalize(lightDir.xyz+viewDir);
	vec3 fresnel = schlickFresnel(material, lightDir, halfway);
	float geo = geomSmith(material, normal, viewDir, halfway);
	float ndf = ggxDistribution(material, max(dot(normal, halfway), 0));
	float denom = 4*(max(dot(normal, lightDir), 0)*max(dot(normal, viewDir), 0));
	denom = max(denom, 0.01);
	vec3 specular = fresnel*geo*ndf/denom;
	vec3 diffuse = material.color;
	vec3 incomingRadiance = light.color.rgb;
	if(light.type == POINT_LIGHT)
	{
		//It isn't a directional light
		float attenuation = pow(length(light.position.xyz-fragPos) / light.attenuationFactor,2);  //TODO: A better attenuation
		incomingRadiance = (incomingRadiance / attenuation);
	}
	incomingRadiance *= lightIntensity;
	vec3 kS = fresnel;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - material.metalness;
	float nLDot = max(dot(normal , lightDir), 0);
	return (kD*diffuse/PI + specular) * incomingRadiance * nLDot; //diffuse is the material color
}

float calculateDirectionalShadow(Light light, vec3 fragPos, int directLightIndex)
{
	float staticShadow = 0;
	float dynamicShadow = 0;
	float bias = 0.0005;
	float shadow = 0.0f;
	//Check Shadow
	vec4 projCoords_ = (light.lightMatrix*vec4(fragPos,1.0));
	vec3 projCoords = projCoords_.xyz/projCoords_.w;
	projCoords = projCoords*0.5 + 0.5;

	float currentDepth = projCoords.z;

	//For static
	vec2 texelSizeStatic = 1.0  / textureSize(shadowMaps, 0).xy;
	for(int r=-light.softnessFactor;r<=light.softnessFactor;r++)
	{
		for(int c=-light.softnessFactor; c<=light.softnessFactor; c++)
		{
			float pcfDepth = texture(shadowMaps, vec4(projCoords.xyz + vec3(r,c,0)*vec3(texelSizeStatic,0), directLightIndex));
			staticShadow += currentDepth - bias > pcfDepth ? 0.0 : 1.0;
		}
	}
	staticShadow = staticShadow / (pow(light.softnessFactor * 2 + 1, 2));
	//float closestDepth = texture(shadowMaps, vec4(projCoords.xyz, directLightIndex));
	
	//For dynamic
	vec2 texelSizeDynamic = 1.0/textureSize(dynamicShadowMaps, 0).xy;
	for(int r=-light.softnessFactor;r<=light.softnessFactor;r++)
	{
		for(int c=-light.softnessFactor; c<=light.softnessFactor; c++)
		{
			float pcfDepth = texture(dynamicShadowMaps, vec4(projCoords.xyz + vec3(r,c,0)*vec3(texelSizeDynamic,0), directLightIndex));
			dynamicShadow += currentDepth - bias > pcfDepth ? 0.0 : 1.0;
		}
	}
	dynamicShadow = dynamicShadow / (pow(light.softnessFactor * 2 + 1, 2));
	shadow = staticShadow + dynamicShadow;

	if(projCoords.z >= 1.0f)
	{
		shadow = 1;
	}
	return shadow;
}
float calculatePointShadow(Light currLight, vec3 fragPos, vec3 fragNormal, int lightIndex)
{
	float shadow = 0.0f;
	vec3 fragToLight = fragPos - currLight.position.xyz;

	//Static
	float samples = currLight.softnessFactor*2 + 1;
	float offset = 0.1f;

	float currentDepth = length(fragToLight);

	// float closestDepth = texture(pointShadowMaps, vec4(fragToLight, lightIndex)).r;
	// float closestDepthDynamic = texture(dynamicPointShadowMaps, vec4(fragToLight, lightIndex)).r;
	// closestDepth *= currLight.far_plane;  //Far plane
	// closestDepthDynamic *= currLight.far_plane;
	// closestDepth = min(closestDepth, closestDepthDynamic);  //TODO: IS this true? Or should i do min?
	float bias = 0.5f;


	
	// //FragColor = vec4(vec3(closestDepth/currLight.far_plane), 1.0); 
	if(dot(fragToLight, fragNormal) > 0 || length(fragToLight) > currLight.far_plane)
	{
		return 1.0f;
	}
	
	for(float x=-offset; x < offset; x += offset/(samples*0.5))
	{
		for(float y=-offset; y < offset; y += offset/(samples*0.5))
		{
			for(float z=-offset; z < offset; z += offset/(samples*0.5))
			{
				float closestDepth = texture(pointShadowMaps, vec4(fragToLight + vec3(x,y,z), lightIndex)).r;
				float closestDepthDynamic = texture(dynamicPointShadowMaps, vec4(fragToLight + vec3(x,y,z), lightIndex)).r;
				closestDepth *= currLight.far_plane;  //Far plane
				closestDepthDynamic *= currLight.far_plane;
				closestDepth = min(closestDepth, closestDepthDynamic);

				
				//FragColor = vec4(vec3(closestDepth/currLight.far_plane), 1.0); 
				if(currentDepth - bias <= closestDepth)
				{
					shadow += 1.0;
				}
			}
		}
	}
	return shadow / (samples*samples*samples);
}