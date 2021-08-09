//
// ゲームグラフィックス特論宿題アプリケーション
//
#include "GgApp.h"

// 光源データ
static GgSimpleShader::Light light
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
  const GLint scaleLoc;

public:

  // コンストラクタ
  Background()
    : vao{ [] { GLuint vao; glGenVertexArrays(1, &vao); return vao; } () }
    , shader{ ggLoadShader("rectangle.vert", "rectangle.frag") }
    , imageLoc{ glGetUniformLocation(shader, "image") }
    , rotateLoc{ glGetUniformLocation(shader, "rotate") }
    , scaleLoc{ glGetUniformLocation(shader, "scale") }
  {
  }

  // コピーを作らない
  Background(const Background& o) = delete;

  // デストラクタ
  ~Background()
  {
    glDeleteVertexArrays(1, &vao);
  }

  // 描画
  void draw(const Window& window, GLfloat scale) const
  {
    glUseProgram(shader);
    glUniform1i(imageLoc, 0);
    glUniformMatrix4fv(rotateLoc, 1, GL_FALSE, window.getRotationMatrix(1).get());
    glUniform1f(scaleLoc, scale);
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
  const GLint scaleLoc;

public:

  // コンストラクタ
  Object(const char* model)
    : obj{ model, true }
    , shader{ "simple.vert", "simple.frag" }
    , lightBuffer{ light }
    , mv{ ggLookat(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f) }
    , turnLoc{ glGetUniformLocation(shader.get(), "turn") }
    , imageLoc{ glGetUniformLocation(shader.get(), "image") }
    , rotateLoc{ glGetUniformLocation(shader.get(), "rotate") }
    , scaleLoc{ glGetUniformLocation(shader.get(), "scale") }
  {
  }

  // コピーを作らない
  Object(const Object& o) = delete;

  // 描画
  void draw(const Window& window, GLfloat scale) const
  {
    // 投影変換行列を設定する
    const GgMatrix mp{ ggPerspective(0.5f, window.getAspect(), 1.0f, 15.0f) };

    // シェーダプログラムを指定する
    shader.use(mp, mv, lightBuffer);

    // オブジェクトの回転を指定する
    glUniformMatrix4fv(turnLoc, 1, GL_FALSE, window.getRotationMatrix(0).get());

    // 背景のテクスチャを指定する
    glUniform1i(imageLoc, 0);

    // 背景のテクスチャの回転を指定する
    glUniformMatrix4fv(rotateLoc, 1, GL_FALSE, window.getRotationMatrix(1).get());

    // 背景のテクスチャの拡大率を指定する
    glUniform1f(scaleLoc, scale);

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
  GLsizei backWidth, backHeight;
  const GLuint back{ ggLoadImage("a101.tga", &backWidth, &backHeight) };

  // 背景色を設定する
  glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

  // 背面ポリゴンを除去する
  glEnable(GL_CULL_FACE);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // テクスチャがウィンドウにぴったり収める拡大率を求める
    const GLfloat scale
    {
      1.0f / (window.getWidth() * backHeight > window.getHeight() * backWidth
      ? window.getWidth() : window.getHeight())
    };

    // 背景テクスチャを読み込む
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, back);

    // 背景を描画する
    glDisable(GL_DEPTH_TEST);
    rect.draw(window, scale);

    // 図形を描画する
    glEnable(GL_DEPTH_TEST);
    object.draw(window, scale);

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }

  return 0;
}
