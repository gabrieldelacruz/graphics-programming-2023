
uniform vec3 AmbientColor;

struct SurfaceData
{
	vec3 normal;
	vec3 reflectionColor;
	float ambientReflectance;
	float diffuseReflectance;
	float specularReflectance;
	float specularExponent;
};

vec3 ComputeDiffuseIndirectLighting(SurfaceData data)
{
	return data.ambientReflectance * data.reflectionColor * AmbientColor;
}

vec3 ComputeSpecularIndirectLighting(SurfaceData data, vec3 viewDir)
{
	return vec3(0);
}

vec3 CombineIndirectLighting(vec3 diffuse, vec3 specular, SurfaceData data, vec3 viewDir)
{
	return diffuse + specular;
}

vec3 ComputeDiffuseLighting(SurfaceData data, vec3 lightDir)
{
	float diffuseIntensity = ClampedDot(data.normal, lightDir);
	return diffuseIntensity * data.diffuseReflectance * data.reflectionColor;
}

vec3 ComputeSpecularLighting(SurfaceData data, vec3 lightDir, vec3 viewDir)
{
   vec3 halfDir = GetHalfVector(lightDir, viewDir);
   float specularIntensity = ClampedDot(halfDir, data.normal);
   
   // Alternative version: Same result, with different specularExponent values
   //vec3 reflectDir = reflect(lightDir, data.normal);
   //float specularIntensity = ClampedDot(reflectDir, viewDir);

   specularIntensity = pow(specularIntensity, data.specularExponent);

   return vec3(specularIntensity * data.specularReflectance);
}

vec3 CombineLighting(vec3 diffuse, vec3 specular, SurfaceData data, vec3 lightDir, vec3 viewDir)
{
	return diffuse + specular;
}
