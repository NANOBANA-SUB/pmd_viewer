/*ボーン情報が扱える頂点シェーダ*/
#version 330 core

layout(location = 0) in vec4 aPos;          // 頂点座標
layout(location = 1) in vec4 aNormal;       // 法線
layout(location = 2) in vec2 aUV;           // UV座標
layout(location = 3) in uvec2 aBoneNo;      // ボーンインデックス (2つ)
layout(location = 4) in int aWeight;       // ボーン影響度

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 boneMatrices[210];

void main() 
{
    float w = aWeight / 100.0f;

    // 2 つのボーンのスキニング計算
    mat4 boneMat1 = boneMatrices[aBoneNo[0]];
    mat4 boneMat2 = boneMatrices[aBoneNo[1]];

    // ウェイト補間
    mat4 skinMatrix = w * boneMat1 + (1 - w) * boneMat2;

    // スキニング適用
    vec4 skinnedPos = skinMatrix * vec4(aPos.xyz, 1.0);
    mat3 normalMatrix = mat3(skinMatrix);
    vec3 skinnedNormal = normalMatrix * aNormal.xyz;

    // モデル行列適用
    FragPos = vec3(model * skinnedPos);
    Normal = mat3(transpose(inverse(model))) * skinnedNormal;
    TexCoords = aUV;

    // 投影座標
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
