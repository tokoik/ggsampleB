#version 410 core
 
// テクスチャ
uniform sampler2D image;
 
// 背景テクスチャの回転
uniform mat4 rotate;

// ウィンドウの中心位置
uniform vec3 center;

// 角度からテクスチャ座標に変換する係数 (-1/2π, 1/π)
const vec2 rad2tex = vec2(-0.15915494, 0.31830989);

// フレームバッファに出力するデータ
layout (location = 0) out vec4 fc;                  // フラグメントの色

void main(void)
{
  // 視点からこのフラグメントに向かう視線を求める
  vec3 ray = mat3(rotate) * vec3(gl_FragCoord.xy - center.xy, center.z);

  // 視線の方向から正距円筒図法のテクスチャ座標を求める
  vec2 texcoord = atan(ray.xy, vec2(ray.z, length(ray.xz))) * rad2tex + 0.5;

  // テクスチャの色をサンプリングしてフラグメントの色とする
  fc = texture(image, texcoord);
}
