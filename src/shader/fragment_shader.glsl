#version 330 core

in vec3 FragPos; // 頂点シェーダーから受け取る座標
in vec3 Normal; // 頂点シェーダーから受け取る法線
in vec2 TexCoords; // 頂点シェーダーから受け取るUV座標

out vec4 FragColor; // 出力する色

uniform sampler2D texture1; // テクスチャ

void main() 
{
    // 簡単なディフューズライティング（デフォルト光源）
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    float diff = max(dot(normalize(Normal), lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0); // ライトの色（白）

    vec3 textureColor = texture(texture1, TexCoords).rgb; // テクスチャカラー
    FragColor = vec4(textureColor * diffuse, 1.0);
}
