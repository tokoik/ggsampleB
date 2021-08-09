#version 410 core

//
// simple.vert
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

// 変換行列
uniform mat4 mp;                    // 投影変換行列
uniform mat4 mv;                    // モデルビュー変換行列
uniform mat4 mn;                    // 法線変換行列

// オブジェクトの回転
uniform mat4 turn;

// 頂点属性
layout (location = 0) in vec4 pv;   // ローカル座標系における頂点の位置
layout (location = 1) in vec4 nv;   // ローカル座標系における頂点の法線ベクトル

// ラスタライザに送る頂点属性
out vec3 v;                         // 視点座標系における視線ベクトル
out vec3 l;                         // 視点座標系における光線ベクトル
out vec3 h;                         // 視点座標系における中間ベクトル
out vec3 n;                         // 視点座標系における法線ベクトル

void main(void)
{
  // 視点座標系における頂点の位置
  vec4 p = mv * turn * pv;

  // 視点座標系における光源の位置
  vec4 q = mv * lpos;

  // 視点座標系における視線ベクトル
  v = -normalize(p.xyz);

  // 視点座標系における光線ベクトル
  l = normalize(vec3(q * p.w - q.w * p));

  // 視点座標系における中間ベクトル
  h = l + v;

  // 視点座標系における法線ベクトル
  n = vec3(mn * turn * nv);

  // クリッピング座標系における頂点の位置
  gl_Position = mp * p;
}
