#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform int is_spot;
uniform vec3 object_color;

uniform vec3 point_light_pos[9];

// Output
layout(location = 0) out vec4 out_color;

vec3 point_light_contribution(vec3 light_pos)
{
	vec3 color;
	//calculele componentelor difuze si speculare din modelul Phong de iluminare pentru lumina punctiforma.
	vec3 L = normalize( light_pos - world_position );
    vec3 V = normalize( eye_position - world_position );
    vec3 H = normalize( L + V );
    vec3 R = reflect (-L, world_normal);

	vec3 N = normalize(world_normal);
    // TODO(student): Define ambient light component
    float ambient_light = 0.25;

    // TODO(student): Compute diffuse light component
    float diffuse_light = material_kd * max (dot(world_normal,L), 0);

    // TODO(student): Compute specular light component
    int primesteLumina = dot(world_normal, L) > 0 ? 1 : 0;
    float specular_light = material_ks * 1 * primesteLumina * pow(max(dot(V, R), 0), material_shininess);

    float light = 0;

	if (is_spot == 1)
	{
		float cut_off = radians(30.0f);
		float spot_light = dot(-L, light_direction);
		float spot_light_limit = cos(cut_off);
		
		if (spot_light > spot_light_limit) {	 
			float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
			float light_att_factor = pow(linear_att, 2);
			light = ambient_light + light_att_factor * (diffuse_light + specular_light);
		} else {
			light = ambient_light;
		}
	} else {
		light = ambient_light + 0.4 * diffuse_light + specular_light;
	}
    
	color = object_color * light;
	return color;
}

void main()
{
    /* vec3 L = normalize( light_position - world_position );
    vec3 V = normalize( eye_position - world_position );
    vec3 H = normalize( L + V );
    vec3 R = reflect (-L, world_normal);

    // TODO(student): Define ambient light component
    float ambient_light = 0.25;

    // TODO(student): Compute diffuse light component
    float diffuse_light = material_kd * max (dot(world_normal,L), 0);

    // TODO(student): Compute specular light component
    int primesteLumina = dot(world_normal, L) > 0 ? 1 : 0;
    float specular_light = material_ks * 1 * primesteLumina * pow(max(dot(V, R), 0), material_shininess);

    float light = 0;

	if (is_spot == 1)
	{
		float cut_off = radians(30.0f);
		float spot_light = dot(-L, light_direction);
		float spot_light_limit = cos(cut_off);
		
		if (spot_light > spot_light_limit) {	 
			float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
			float light_att_factor = pow(linear_att, 2);
			light = ambient_light + light_att_factor * (diffuse_light + specular_light);
		} else {
			light = ambient_light;
		}
	} else {
		light = ambient_light + diffuse_light + specular_light;
	}
    
	vec3 color = object_color * light; */
	vec3 color = vec3(0, 0, 0);
	for (int i = 0; i < 2; i++) {
		color += point_light_contribution(point_light_pos[i]);
	}

    out_color = vec4(color, 1);

}
