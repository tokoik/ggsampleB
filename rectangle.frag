#version 410 core
 
// テクスチャ
uniform sampler2D image;
 
// 背景テクスチャのスケール
uniform vec2 scale;

// フレームバッファに出力するデータ
layout (location = 0) out vec4 fc;                  // フラグメントの色
 
void main(void)
{
  fc = texture(image, gl_FragCoord.xy * scale);
}
