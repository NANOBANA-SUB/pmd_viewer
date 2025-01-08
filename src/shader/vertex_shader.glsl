#version 330 core

layout(location = 0) in vec3 aPos; // 頂点座標
layout(location = 1) in vec3 aNormal; // 法線
layout(location = 2) in vec2 aUV; // UV座標

out vec3 FragPos; // フラグメントシェーダーに送る座標
out vec3 Normal; // フラグメントシェーダーに送る法線
out vec2 TexCoords; // フラグメントシェーダーに送るUV座標

uniform mat4 model; // モデル行列
uniform mat4 view; // ビュー行列
uniform mat4 projection; // プロジェクション行列

void main() 
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; // 法線変換
    TexCoords = aUV;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
