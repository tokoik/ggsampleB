#version 410 core
 
// テクスチャ
uniform sampler2D image;
 
// 背景テクスチャの回転
uniform mat4 rotate;

// 背景テクスチャのスケール
uniform float scale;

// 焦点距離
uniform float focal = -2.0;

// 角度からテクスチャ座標に変換する係数 (-1/2π, 1/π)
const vec2 rad2tex = vec2(-0.15915494, 0.31830989);

// フレームバッファに出力するデータ
layout (location = 0) out vec4 fc;                  // フラグメントの色

void main(void)
{
  // 視点からこのフラグメントに向かう視線を求める
  vec3 ray = mat3(rotate) * vec3(gl_FragCoord.xy * scale * 2.0 - 1.0, focal);

  // 視線の方向から正距円筒図法のテクスチャ座標を求める
  vec2 texcoord = atan(ray.xy, vec2(ray.z, length(ray.xz))) * rad2tex + 0.5;

  // テクスチャの色を散布リグしてフラグメントの色にする
  fc = texture(image, texcoord);
}
