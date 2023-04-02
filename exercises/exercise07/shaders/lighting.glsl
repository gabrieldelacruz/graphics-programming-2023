
uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 LightDirection;
uniform vec4 LightAttenuation;

float ComputeDistanceAttenuation(vec3 position)
{
	// Compute distance attenuation, reading the range from LightAttenuation.x (fade start) and LightAttenuation.y (fade end)
	return smoothstep(LightAttenuation.y, LightAttenuation.x, distance(position, LightPosition));
}

float ComputeAngularAttenuation(vec3 lightDir)
{
	float angle = acos(dot(LightDirection, lightDir));
	vec2 attAngle = LightAttenuation.zw;
	return smoothstep(attAngle.y, attAngle.x, angle);
}

float ComputeAttenuation(vec3 position, vec3 lightDir)
{
	float attenuation = 1.0f;
	if (LightAttenuation.y > 0)
	{
		attenuation *= ComputeDistanceAttenuation(position);
	}
	if (LightAttenuation.w > 0)
	{
		attenuation *= ComputeAngularAttenuation(lightDir);
	}
	return attenuation;
}

vec3 ComputeLightDirection(vec3 position)
{
	return LightAttenuation.y >= 0 ? GetDirection(position, LightPosition) : LightDirection;
}

vec3 ComputeLight(SurfaceData data, vec3 viewDir, vec3 position)
{
	vec3 lightDir = ComputeLightDirection(position);

	vec3 light = vec3(0);
	light += ComputeDiffuseLighting(data, lightDir);
	light += ComputeSpecularLighting(data, lightDir, viewDir);

	float attenuation = ComputeAttenuation(position, lightDir);
	return light * LightColor * attenuation;
}

vec3 ComputeLighting(vec3 position, SurfaceData data, vec3 viewDir, bool indirect)
{
	vec3 light = vec3(0);
	
	if (indirect)
	{
		light += ComputeDiffuseIndirectLighting(data);
		light += ComputeSpecularIndirectLighting(data, viewDir);
	}

	light += ComputeLight(data, viewDir, position);

	return light;
}
