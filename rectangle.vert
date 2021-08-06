#version 410 core
 
void main()
{
  // テクスチャ座標を求める
  vec2 coord = vec2(gl_VertexID % 2, gl_VertexID / 2);
  
  // テクスチャ座標から頂点座標を求める
  gl_Position = vec4(coord * 2.0 - 1.0, 0.0, 1.0);
}
