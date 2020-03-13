#version 450 core // Minimal GL version su	pport expected from the GPU

in vec3 fPosition; // Shader input, linearly interpolated by default from the previous stage (here the vertex shader)
in vec3 fNormal;
in vec3 fColor;

out vec4 color; // Shader output: the color response attached to this fragment

struct LightSource {
	vec3 position;
	vec3 color;
	float intensity;
};
uniform LightSource lightSource;

struct Material {
	vec3 albedo;
	int shineness;
	float kd;
	float ks;
};

uniform Material material;

vec3 specularIllumination() {
	vec3 n = normalize (fNormal);
	vec3 wi = normalize (lightSource.position - fPosition);
	vec3 wo = normalize (-fPosition);
	vec3 fd = material.kd * material.albedo;
	vec3 wh = normalize (wi + wo);
	vec3 fs = vec3 (1.0) * material.ks * pow (max(0.0, dot(wh, n)), material.shineness);
	vec3 Li = lightSource.color * lightSource.intensity;
	vec3 radiance = vec3(Li * (fd + fs) * max(0.0, dot(n, wi)));
	return radiance;
}

vec3 diffuseIllumination() {
	vec3 wi = normalize (lightSource.position - fPosition);
	float lambertianTerm = max (0.0, dot (fNormal, wi));
	vec3 radiance = vec3 (
		lambertianTerm * lightSource.color.x * lightSource.intensity,
		lambertianTerm * lightSource.color.y * lightSource.intensity,
		lambertianTerm * lightSource.color.z * lightSource.intensity
	);
	return radiance;
}

void main() {
	vec3 sd = diffuseIllumination();
	color = vec4(sd, 1.0);
}