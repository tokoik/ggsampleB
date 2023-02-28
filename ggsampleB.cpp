//
// ゲームグラフィックス特論宿題アプリケーション
//
#include "GgApp.h"

// 画角
constexpr GLfloat fovy{ 1.0f };

// 光源データ
constexpr GgSimpleShader::Light light
{
  { 0.2f, 0.2f, 0.2f, 1.0f }, // 環境光成分
  { 1.0f, 1.0f, 1.0f, 0.0f }, // 拡散反射光成分
  { 1.0f, 1.0f, 1.0f, 0.0f }, // 鏡面反射光成分
  { 3.0f, 4.0f, 5.0f, 1.0f }  // 光源位置
};

//
// 背景描画
// 
class Background
{
  const GLuint vao;
  const GLuint shader;
  const GLint imageLoc;
  const GLint rotateLoc;
  const GLint centerLoc;

public:

  // コンストラクタ
  Background()
    : vao{ [] { GLuint vao; glGenVertexArrays(1, &vao); return vao; } () }
    , shader{ ggLoadShader("rectangle.vert", "rectangle.frag") }
    , imageLoc{ glGetUniformLocation(shader, "image") }
    , rotateLoc{ glGetUniformLocation(shader, "rotate") }
    , centerLoc{ glGetUniformLocation(shader, "center") }
  {
  }

  // コピーを作らない
  Background(const Background& o) = delete;

  // デストラクタ
  virtual ~Background()
  {
    glDeleteVertexArrays(1, &vao);
  }

  // 描画
  void draw(const GgApp::Window& window, const GLfloat* center) const
  {
    // シェーダプログラムを指定する
    glUseProgram(shader);

    // 背景のテクスチャを指定する
    glUniform1i(imageLoc, 0);

    // 背景のテクスチャの回転を指定する
    glUniformMatrix4fv(rotateLoc, 1, GL_FALSE, window.getRotationMatrix(1).get());

    // ウィンドウの中心位置を指定する
    glUniform3fv(centerLoc, 1, center);

    // 図形を描画する
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
};

//
// 図形描画
//
class Object
{
  const GgSimpleObj obj;
  const GgSimpleShader shader;
  const GgSimpleShader::LightBuffer lightBuffer;
  const GgMatrix mv;
  const GLint turnLoc;
  const GLint imageLoc;
  const GLint rotateLoc;
  const GLint centerLoc;

public:

  // コンストラクタ
  Object(const char* model)
    : obj{ model, true }
    , shader{ "simple.vert", "simple.frag" }
    , lightBuffer{ light }
    , mv{ ggLookat(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f) }
    , turnLoc{ glGetUniformLocation(shader.get(), "turn") }
    , imageLoc{ glGetUniformLocation(shader.get(), "image") }
    , rotateLoc{ glGetUniformLocation(shader.get(), "rotate") }
    , centerLoc{ glGetUniformLocation(shader.get(), "center") }
  {
  }

  // デストラクタ
  virtual ~Object()
  {
  }

  // コピーを作らない
  Object(const Object& o) = delete;

  // 描画
  void draw(const GgApp::Window& window, const GLfloat* center) const
  {
    // 投影変換行列を設定する
    const GgMatrix mp{ ggPerspective(fovy, window.getAspect(), 1.0f, 15.0f) };

    // シェーダプログラムを指定する
    shader.use(mp, mv, lightBuffer);

    // オブジェクトの回転を指定する
    glUniformMatrix4fv(turnLoc, 1, GL_FALSE, window.getRotationMatrix(0).get());

    // 背景のテクスチャを指定する
    glUniform1i(imageLoc, 0);

    // 背景のテクスチャの回転を指定する
    glUniformMatrix4fv(rotateLoc, 1, GL_FALSE, window.getRotationMatrix(1).get());

    // ウィンドウの中心位置を指定する
    glUniform3fv(centerLoc, 1, center);

    // 図形を描画する
    obj.draw();
  }
};

//
// アプリケーション本体
//
int GgApp::main(int argc, const char* const* argv)
{
  // ウィンドウを作成する (この行は変更しないでください)
  Window window{ argc > 1 ? argv[1] : "ggsampleB" };

  // 背景描画用のオブジェクトを作成する
  const Background rect;

  // 図形データを読み込む
  const Object object{ "logo.obj" };

  // 背景テクスチャを読み込む
  const GLuint back{ ggLoadImage("a101.tga") };

  // 背面ポリゴンを除去する
  glEnable(GL_CULL_FACE);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // スクリーンまでの距離を求める
    const GLfloat focal{ window.getHeight() / tanf(fovy * 0.5f) };

    // ウィンドウの中心位置を求める
    const GLfloat center[]{ window.getWidth() * 0.5f, window.getHeight() * 0.5f, -focal };

    // デプスバッファを消去する
    glClear(GL_DEPTH_BUFFER_BIT);

    // 背景テクスチャを指定する
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, back);

    // 背景を描画する
    glDisable(GL_DEPTH_TEST);
    rect.draw(window, center);

    // 図形を描画する
    glEnable(GL_DEPTH_TEST);
    object.draw(window, center);

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }

  return 0;
}
