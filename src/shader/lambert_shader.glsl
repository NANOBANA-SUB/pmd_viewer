/*ランベルトの余弦則によるシェーディング*/
#version 330 core

in vec3 FragPos; // 頂点シェーダーから受け取る座標
in vec3 Normal; // 頂点シェーダーから受け取る法線
out vec4 FragColor; // 出力する色

uniform vec3 lightPos; // 光源の位置
uniform vec3 lightColor; // 光の色
uniform vec3 objectColor; // オブジェクトの色

void main() 
{
    // 法線を正規化
    vec3 norm = normalize(Normal);

    // 光の方向ベクトルを計算し、正規化
    vec3 lightDir = normalize(lightPos - FragPos);

    // ランバートの余弦則: 法線と光の方向の内積を計算
    float diff = max(dot(norm, lightDir), 0.0);

    // ディフューズライティングの計算
    vec3 diffuse = diff * lightColor;

    // 最終色を計算
    vec3 result = diffuse * objectColor;
    FragColor = vec4(result, 1.0);
}
