#version 410 core

//
// simple.frag
//
//   単純な陰影付けを行ってオブジェクトを描画するシェーダ
//

// 光源
layout (std140) uniform Light
{
  vec4 lamb;                        // 環境光成分
  vec4 ldiff;                       // 拡散反射光成分
  vec4 lspec;                       // 鏡面反射光成分
  vec4 lpos;                        // 位置
};

// 材質
layout (std140) uniform Material
{
  vec4 kamb;                        // 環境光の反射係数
  vec4 kdiff;                       // 拡散反射係数
  vec4 kspec;                       // 鏡面反射係数
  float kshi;                       // 輝き係数
};

// ラスタライザから受け取る頂点属性の補間値
in vec3 v;                          // 視点座標系における視線ベクトル
in vec3 l;                          // 視点座標系における光線ベクトル
in vec3 h;                          // 視点座標系における中間ベクトル
in vec3 n;                          // 視点座標系における法線ベクトル

// 背景テクスチャのサンプラ
uniform sampler2D image;

// 背景テクスチャの回転
uniform mat4 rotate;

// ウィンドウの中心位置
uniform vec3 center;

// 陰影とテクスチャの混合比
uniform float alpha = 0.3;

// 角度からテクスチャ座標に変換する係数 (-1/2π, 1/π)
const vec2 rad2tex = vec2(-0.15915494, 0.31830989);

// フレームバッファに出力するデータ
layout (location = 0) out vec4 fc;  // フラグメントの色

void main(void)
{
  // 補間値の正規化
  vec3 vv = normalize(v);
  vec3 ll = normalize(l);
  vec3 hh = normalize(h);
  vec3 nn = normalize(n);

  // 視点からこのフラグメントに向かう視線を求める
  vec3 ray = mat3(rotate) * vec3(gl_FragCoord.xy - center.xy, center.z);

  // 視線の方向から正距円筒図法のテクスチャ座標を求める
  vec2 texcoord = atan(ray.xy, vec2(ray.z, length(ray.xz))) * rad2tex + 0.5;

  // テクスチャの色をサンプリングする
  vec4 color = texture(image, texcoord);

  // 陰影計算
  vec4 idiff = max(dot(nn, ll), 0.0) * kdiff * ldiff + kamb * lamb;
  vec4 ispec = pow(max(dot(nn, hh), 0.0), kshi) * kspec * lspec;

  // 拡散反射光による陰影にテクスチャの色を合成してフラグメントの色とする
  fc = mix(idiff, color, alpha) + ispec;
}
