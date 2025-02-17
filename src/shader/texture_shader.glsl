/*テクスチャも有効になっているフラグメントシェーダ*/
#version 330 core

out vec4 FragColor; // フラグメントシェーダの出力

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform bool useTexture;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

// テクスチャの追加
uniform sampler2D texture1; // ディフューズマップとして使用するテクスチャ

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords; // 頂点シェーダから渡されるテクスチャ座標

void main() {
    // 環境光
    vec3 ambient = material.ambient * lightColor;

    // 拡散光
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = material.diffuse * diff * lightColor;

    // 鏡面光
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * lightColor;

    // テクスチャから色をサンプリング
    vec4 texColor = vec4(1.0); // デフォルトカラー（白）
    if (useTexture) {
        texColor = texture(texture1, TexCoords); // テクスチャを使用
    }
    vec3 result = (ambient + diffuse + specular) * texColor.rgb;
    FragColor = vec4(result, texColor.a); // アルファ値も反映
}
