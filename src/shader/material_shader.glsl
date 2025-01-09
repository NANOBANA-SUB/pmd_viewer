/*マテリアル情報を用いてシェーディングする*/
#version 330 core

out vec4 FragColor; // フラグメントシェーダの出力として宣言

uniform vec3 lightPos;
uniform vec3 lightColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

in vec3 FragPos;
in vec3 Normal;

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

    // 結果色を計算
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0); // 出力に代入
}
