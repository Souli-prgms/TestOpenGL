#shader vertex
#version 330 core

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat3 normal_matrix;
uniform mat4 light_space_matrix;

in vec3 vtx_position;
in vec2 vtx_texcoord;
in vec3 vtx_normal;
in vec3 vtx_tangent;
in vec3 vtx_bitangent;

out vec3 v_position;
out vec2 v_texcoord;
out mat3 TBN;
out vec4 v_pos_light_space;

void main()
{
	// vertex position and texture coordinates
	v_position = (view_matrix * model_matrix * vec4(vtx_position, 1.0)).xyz;
	gl_Position = projection_matrix * vec4(v_position, 1.0);
	v_texcoord = vtx_texcoord;

	// TBN space
	vec3 T = normalize((model_matrix * vec4(vtx_tangent, 0.0)).xyz);
	vec3 B = normalize((model_matrix * vec4(vtx_bitangent, 0.0)).xyz);
	vec3 N = normalize((model_matrix * vec4(vtx_normal, 0.0)).xyz);
	TBN = mat3(T, B, N);

	// light space position
	v_pos_light_space = light_space_matrix * model_matrix * vec4(vtx_position, 1.0);
};

#shader fragment
#version 330 core

struct DirectionalLight {
	vec4 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

in vec3 v_position;
in vec2 v_texcoord;
in mat3 TBN;
in vec4 v_pos_light_space;

uniform DirectionalLight directional_light;
uniform PointLight point_light;
uniform sampler2D ground_texture;
uniform sampler2D normal_map;
uniform sampler2D shadow_map;

out vec4 out_color;

vec3 calc_dir_light(vec3 normal, vec3 view_direction)
{
	vec3 light_dir = normalize(-directional_light.direction.xyz);
	vec3 light_color = vec3(1.0);

	// ambient
	vec3 ambient = directional_light.ambient;

	// diffuse
	vec3 diffuse = directional_light.diffuse * max(dot(normal, light_dir), 0.0);

	// specular
	float shininess = 32.0;
	vec3 r = reflect(-light_dir, normal);
	vec3 specular = directional_light.specular * pow(max(dot(view_direction, r), 0.0), shininess);

	return 0.0 * (ambient + diffuse + specular);
}

float calc_shadow(float bias, vec4 fragPosLightSpace)
{
	vec3 proj_coords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	proj_coords = proj_coords * 0.5 + 0.5;
	float closest_depth = texture(shadow_map, proj_coords.xy).r;
	float shadow = proj_coords.z - bias > closest_depth ? 1.0 : 0.0;

	return shadow;
}

void main()
{
	vec3 tex_color = texture(ground_texture, v_texcoord).rgb;
	
	vec3 normal = normalize(texture(normal_map, v_texcoord).rgb * 2.0 - 1.0);
	vec3 n = normalize(TBN * normal);
	vec3 view_dir = normalize(-v_position);

	float gamma = 2.2;

	// directional light
	vec3 result = pow(calc_dir_light(n, view_dir), vec3(1.0 / gamma));
	
	// pont light
	vec3 light_dir = normalize(point_light.position.xyz - v_position);
	vec3 halfway_dir = normalize(light_dir + view_dir);
	float distance = length(point_light.position.xyz - v_position);
	float attenuation = 1.0 / (point_light.constant + point_light.linear * distance + point_light.quadratic * (distance * distance));

	// ambient
	vec3 ambient = point_light.ambient;

	// diffuse
	vec3 diffuse = point_light.diffuse * max(dot(n, light_dir), 0.0);

	// specular
	float shininess = 32.0;
	vec3 specular = point_light.specular * pow(max(dot(n, halfway_dir), 0.0), shininess);

	// shadow
	//float bias = max(0.05 * (1.0 - dot(n, light_dir)), 0.005);
	float bias = 0.005;
	float shadow = calc_shadow(bias, v_pos_light_space);
	vec3 lighting = ambient + (1 - shadow) * (diffuse + specular);
	//vec3 lighting = ambient + diffuse + specular;
	
	// final color
	out_color = vec4(tex_color * (result + attenuation * pow(lighting, vec3(1.0 / gamma))), 1.0);
};
