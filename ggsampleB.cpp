//
// ゲームグラフィックス特論宿題アプリケーション
//
#include "GgApp.h"

// ファイルダイアログ
#include "nfd.h"

// 形状データ
static std::string model{ "bunny.obj" };

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
class Rectangle
{
  const GLuint vao;
  const GLuint shader;
  const GLint imageLoc;
  const GLint scaleLoc;

public:

  Rectangle()
    : vao{ [] { GLuint vao; glGenVertexArrays(1, &vao); return vao; } () }
    , shader{ ggLoadShader("rectangle.vert", "rectangle.frag") }
    , imageLoc{ glGetUniformLocation(shader, "image") }
    , scaleLoc{ glGetUniformLocation(shader, "scale") }
  {
  }

  void draw(const GLfloat* scale) const
  {
    glUseProgram(shader);
    glUniform1i(imageLoc, 0);
    glUniform2fv(scaleLoc, 1, scale);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
};

//
// アプリケーション本体
//
int GgApp::main(int argc, const char* const* argv)
{
  // ウィンドウを作成する (この行は変更しないでください)
  Window window{ argc > 1 ? argv[1] : "ggsampleA" };

  // 図形データを読み込む (大きさを正規化する)
  GgSimpleObj object{ model, true };

  // シェーダを作成する
  const GgSimpleShader simple{ "simple.vert", "simple.frag" };
  const GLint rotateLoc{ glGetUniformLocation(simple.get(), "rotate") };
  const GLint translateLoc{ glGetUniformLocation(simple.get(), "translate") };
  const GLint backLoc{ glGetUniformLocation(simple.get(), "back") };
  const GLint scaleLoc{ glGetUniformLocation(simple.get(), "scale") };
  const GLint offsetLoc{ glGetUniformLocation(simple.get(), "offset") };

  // 背景テクスチャを読み込む
  GLsizei backWidth, backHeight;
  const GLuint back{ ggLoadImage("campus.tga", &backWidth, &backHeight) };

  // 背景描画用のオブジェクトを作成する
  const Rectangle rect;

  // 光源データから光源のバッファオブジェクトを作成する
  GgSimpleShader::LightBuffer lightBuffer{ light };

  // ビュー変換行列を設定する
  const GgMatrix mv{ ggLookat(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f) };

#ifdef IMGUI_VERSION
  //
  // ImGui の初期設定
  //

  //ImGuiIO& io = ImGui::GetIO();
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  //ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Read 'docs/FONTS.txt' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != NULL);
#endif

  // 背景色を設定する
  glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

  // 背面ポリゴンを除去する
  glEnable(GL_CULL_FACE);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef IMGUI_VERSION
    //
    // ImGui によるユーザインタフェース
    //

    // ImGui のフレームを準備する
    ImGui::NewFrame();

    //
    // メニューバー
    //
    if (ImGui::BeginMainMenuBar())
    {
      // File メニュー
      if (ImGui::BeginMenu("File"))
      {
        // モデルファイルを開く
        if (ImGui::MenuItem("Open Model File"))
        {
          // ファイルダイアログから得るパス
          nfdchar_t* filepath(nullptr);

          // ファイルダイアログを開く
          if (NFD_OpenDialog("obj", nullptr, &filepath) == NFD_OKAY)
          {
            // 画像を読み込む
            object = GgSimpleObj(filepath, true);

            // ファイルパスの取り出しに使ったメモリを開放する
            free(filepath);
          }
        }

        // 終了
        else if (ImGui::MenuItem("Quit")) window.setClose();

        // File メニュー修了
        ImGui::EndMenu();
      }

      // メインメニューバー終了
      ImGui::EndMainMenuBar();
    }

    //
    // 光源位置
    //
    ImGui::SetNextWindowPos(ImVec2(4, 24), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(400, 54), ImGuiCond_Once);
    ImGui::Begin("Contrl Panel");
    if (ImGui::SliderFloat3("Light Position", light.position.data(), -10.0f, 10.0f, "%.2f"))
      lightBuffer.loadPosition(light.position);
    ImGui::End();

    // ImGui のフレームに描画する
    ImGui::Render();
#endif

    // テクスチャがウィンドウにぴったり収まるサイズを求める
    GLfloat scale[2];
    if (window.getWidth() * backHeight > window.getHeight() * backWidth)
    {
      scale[0] = 1.0f / window.getWidth();
      scale[1] = scale[0] * backWidth / backHeight;
    }
    else
    {
      scale[1] = 1.0f / window.getHeight();
      scale[0] = scale[1] * backHeight / backWidth;
    }

    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, back);

    // 背景を描画する
    rect.draw(scale);

    glEnable(GL_DEPTH_TEST);
    // 投影変換行列を設定する
    const GgMatrix mp{ ggPerspective(0.5f, window.getAspect(), 1.0f, 15.0f) };

    // シェーダプログラムを指定する
    simple.use(mp, mv, lightBuffer);

    // オブジェクトの回転を指定する
    glUniformMatrix4fv(rotateLoc, 1, GL_FALSE, window.getRotationMatrix(0).get());

    // オブジェクトの並進を指定する
    glUniformMatrix4fv(translateLoc, 1, GL_FALSE, window.getScrollMatrix(1).get());

    // テクスチャの拡大率を指定する
    glUniform2fv(scaleLoc, 1, scale);

    // 背景のテクスチャを指定する
    glUniform1i(backLoc, 0);

    // 図形を描画する
    object.draw();

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }

  return 0;
}
