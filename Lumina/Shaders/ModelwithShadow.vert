# version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;

layout (std140) uniform Matrices {
    mat4 view;
    mat4 projection;
};

// Light Setting
const int POINT_LIGHTS_LIMITATION = 8;
const int OTHER_LIMITATION = 2;

struct LightInfo {
    // Amounts
    int num_dirlight;
    int num_pointlight;
    int num_spotlight;

    // Transform Matrices
    mat4 DirLight_Transform[OTHER_LIMITATION];
    mat4 PointLight_Transform[POINT_LIGHTS_LIMITATION];
};

uniform LightInfo lightinfo;

out VS_OUT {
    vec3 normal;
    vec3 viewspace_fragPos;
    vec2 texCoords;
    mat4 view;

    int num_dirlight;
    int num_pointlight;
    int num_spotlight;
    vec4 dirlight_fragPos[OTHER_LIMITATION];
    vec4 pointlight_fragPos[POINT_LIGHTS_LIMITATION];
} vs_out;

void main() {
    vs_out.normal = mat3(transpose(inverse(model * view))) * aNormal;
    vs_out.viewspace_fragPos = vec3(view * model * vec4(aPosition, 1.0));
    // vs_out.lightspace_fragPos = LightSpaceTransform * vec4(vec3(model * vec4(aPosition, 1.0)), 1.0);

    // DirLights
    for (int i = 0; i < lightinfo.num_dirlight; ++i)
        vs_out.dirlight_fragPos[i] = lightinfo.DirLight_Transform[i] * model * vec4(aPosition, 1.0);
    
    // PointLights
    // for (int i = 0; i < lightinfo.num_pointlight; ++i)
    //     vs_out.pointlight_fragPos[i] = lightinfo.PointLight_Transform[i] * model * vec4(aPosition, 1.0);

    vs_out.texCoords = aTexCoords;
    vs_out.view = view;

    gl_Position = projection * vec4(vs_out.viewspace_fragPos, 1.0);
}