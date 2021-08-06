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
uniform sampler2D back;

// 背景テクスチャのスケール
uniform vec2 scale;

// フレームバッファに出力するデータ
layout (location = 0) out vec4 fc;  // フラグメントの色

void main(void)
{
  // 補間値の正規化
  vec3 vv = normalize(v);
  vec3 ll = normalize(l);
  vec3 hh = normalize(h);
  vec3 nn = normalize(n);

  // テクスチャ座標
  vec3 r = refract(v, n, 1.5);
  vec2 coord = gl_FragCoord.xy * scale + r.xy / r.z;

  // テクスチャの色
  vec4 color = texture(back, coord);

  // 陰影計算
  vec4 idiff = mix(max(dot(nn, ll), 0.0) * kdiff * ldiff + kamb * lamb, color, 0.6);
  vec4 ispec = pow(max(dot(nn, hh), 0.0), kshi) * kspec * lspec;

  // 画素の陰影を求める
  fc = idiff + ispec;
}
