#version 330 core

in vec3 Normal; // 頂点シェーダーから受け取る法線
out vec4 FragColor; // 出力する色

void main() 
{
    // 法線ベクトルの範囲 [-1, 1] を [0, 1] に変換
    vec3 normalizedNormal = normalize(Normal); // 正規化
    vec3 color = (normalizedNormal * 0.5) + 0.5; // 範囲をシフト
    FragColor = vec4(color, 1.0); // 法線の色をフラグメントに出力
}
