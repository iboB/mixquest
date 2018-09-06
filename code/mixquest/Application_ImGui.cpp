// text-rendering-demo
// Copyright(c) 2018 Chobolabs Inc.
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Application.h"

#include "rendering/GPUProgram.h"
#include "rendering/Shader.h"
#include "rendering/Texture.h"

#include <imgui.h>

#include <GLFW/glfw3.h>

using namespace std;

namespace
{
const char* GetClipboardText(void* userData)
{
    auto wnd = reinterpret_cast<GLFWwindow*>(userData);
    return glfwGetClipboardString(wnd);
}

void SetClipboardText(void* userData, const char* text)
{
    auto wnd = reinterpret_cast<GLFWwindow*>(userData);
    return glfwSetClipboardString(wnd, text);
}

const char* vertexShaderSource =
    "\
    #version 100\n\
    uniform mat4 proj; \
    attribute vec2 v_pos; \
    attribute vec2 v_texCoord; \
    attribute vec4 v_color; \
    varying vec2 texCoord; \
    varying vec4 color; \
    void main(void) \
    { \
        texCoord = v_texCoord; \
        color = v_color; \
        gl_Position = proj * vec4(v_pos.xy, 0.0, 1.0); \
    } \
    ";

const char* fragmentShaderSource =
    "\
    #version 100\n\
    precision mediump float; \
    uniform sampler2D tex; \
    varying vec2 texCoord; \
    varying vec4 color; \
    void main(void) \
    { \
        gl_FragColor = color * texture2D(tex, texCoord).r; \
    } \
    ";

}

enum VertexAttrib
{
    Attrib_Position, Attrib_TexCoord, Attrib_Color
};

#define OFFSET_OF(type, member) (&static_cast<type*>(nullptr)->member)

class ImGuiRenderData
{
public:
    ImGuiRenderData()
        : fontsTexture("imgui fonts")
        , m_program("imgui program")
    {
        auto vs = make_shared<Shader>(ShaderType::Vertex, "imgui vertex shader");
        vs->load(vertexShaderSource);

        auto fs = make_shared<Shader>(ShaderType::Fragment, "imgui fragment shader");
        fs->load(fragmentShaderSource);

        m_program.attachShader(vs);
        m_program.attachShader(fs);

        m_program.bindAttributeAt("v_pos", Attrib_Position);
        m_program.bindAttributeAt("v_texCoord", Attrib_TexCoord);
        m_program.bindAttributeAt("v_color", Attrib_Color);

        m_program.link();

        m_projParam = m_program.getParameterByName("proj");
        m_textureParam = m_program.getParameterByName("tex");

        glGenBuffers(1, &m_vertexBuffer);
        glGenBuffers(1, &m_indexBuffer);
    }

    ~ImGuiRenderData()
    {
        glDeleteBuffers(1, &m_vertexBuffer);
        glDeleteBuffers(1, &m_indexBuffer);
    }

    void render()
    {
        ImGui::EndFrame();
        ImGui::Render();

        auto data = ImGui::GetDrawData();
        assert(data);

        if (data->CmdListsCount == 0)
            return;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_SCISSOR_TEST);

        // ortho 2d matrix
        const float w = ImGui::GetIO().DisplaySize.x;
        const float h = ImGui::GetIO().DisplaySize.y;
        glViewport(0, 0, GLsizei(w), GLsizei(h));
        auto projection = yama::matrix::ortho_rh(0, w, h, 0, 0, 1); // note the inverted height. ImGui uses 0,0 as top left

        m_program.use();
        m_program.setParameter(m_projParam, projection);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

        glEnableVertexAttribArray(Attrib_Position);
        glEnableVertexAttribArray(Attrib_TexCoord);
        glEnableVertexAttribArray(Attrib_Color);

        glVertexAttribPointer(Attrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), OFFSET_OF(ImDrawVert, pos));
        glVertexAttribPointer(Attrib_TexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), OFFSET_OF(ImDrawVert, uv));
        glVertexAttribPointer(Attrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), OFFSET_OF(ImDrawVert, col));

        for (int i = 0; i < data->CmdListsCount; ++i)
        {
            const auto list = data->CmdLists[i];

            glBufferData(GL_ARRAY_BUFFER, sizeof(ImDrawVert) * list->VtxBuffer.size(), list->VtxBuffer.Data, GL_DYNAMIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ImDrawIdx) * list->IdxBuffer.size(), list->IdxBuffer.Data, GL_DYNAMIC_DRAW);

            ImDrawIdx* offsetPtr = nullptr;
            for (const auto& cmd : list->CmdBuffer)
            {
                if (cmd.UserCallback)
                {
                    cmd.UserCallback(list, &cmd);
                }
                else
                {
                    // draw
                    const Texture* t = reinterpret_cast<Texture*>(cmd.TextureId);
                    assert(t);
                    m_program.setParameter(m_textureParam, *t);
                    glScissor(int(cmd.ClipRect.x), (int)(h - cmd.ClipRect.w), int(cmd.ClipRect.z - cmd.ClipRect.x), int(cmd.ClipRect.w - cmd.ClipRect.y));
                    glDrawElements(GL_TRIANGLES, cmd.ElemCount, GL_UNSIGNED_SHORT, offsetPtr);
                    m_program.resetUniforms();
                }
                offsetPtr += cmd.ElemCount;
            }
        }

        glDisableVertexAttribArray(Attrib_Position);
        glDisableVertexAttribArray(Attrib_TexCoord);
        glDisableVertexAttribArray(Attrib_Color);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDisable(GL_SCISSOR_TEST);
    }

    Texture fontsTexture;

private:
    GPUProgram m_program;

    GLuint m_vertexBuffer = 0;
    GLuint m_indexBuffer = 0;

    // uniforms
    int m_textureParam = -1;
    int m_projParam = -1;

};

bool Application::initImGui()
{
    auto ctx = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);

    auto& io = ImGui::GetIO();

    io.IniFilename = nullptr;
    
    io.ClipboardUserData = m_window;
    io.GetClipboardTextFn = GetClipboardText;
    io.SetClipboardTextFn = SetClipboardText;

    io.UserData = this;

    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

    ///////////////////////////////////////////////////////////
    // graphics
    m_imguiRenderData = new ImGuiRenderData;

    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);

    m_imguiRenderData->fontsTexture.loadFromData(width, height, GL_LUMINANCE, pixels);
    io.Fonts->TexID = &m_imguiRenderData->fontsTexture;

    io.Fonts->ClearInputData();
    io.Fonts->ClearTexData();

    return true;
}

void Application::deinitImGui()
{
    delete m_imguiRenderData;
    m_imguiRenderData = nullptr;
    ImGui::DestroyContext();
}

bool Application::imguiHandleKeyboardEvent(int key, int action, int /*mods*/)
{
    auto& io = ImGui::GetIO();
    bool down = (action == GLFW_PRESS);
    io.KeysDown[key] = down;

    switch (key)
    {
    case GLFW_KEY_LEFT_ALT:
    case GLFW_KEY_RIGHT_ALT:
        io.KeyAlt = down;
        break;
    case GLFW_KEY_LEFT_CONTROL:
    case GLFW_KEY_RIGHT_CONTROL:
        io.KeyCtrl = down;
        break;
    case GLFW_KEY_LEFT_SHIFT:
    case GLFW_KEY_RIGHT_SHIFT:
        io.KeyShift = down;
        break;
    case GLFW_KEY_LEFT_SUPER:
    case GLFW_KEY_RIGHT_SUPER:
        io.KeySuper = down;
        break;
    }

    return io.WantCaptureKeyboard;
}

bool Application::imguiHandleCharacterEvent(unsigned int codepoint)
{
    ImGui::GetIO().AddInputCharacter(ImWchar(codepoint));
    return false;
}

bool Application::imguiHandleMouseButtonEvent(int button, int action, int /*mods*/)
{
    if (button > 4) return false;
    auto& io = ImGui::GetIO();
    io.MouseDown[button] = (action == GLFW_PRESS);
    return io.WantCaptureMouse;
}

bool Application::imguiHandleScrollEvent(double x, double y)
{
    auto& io = ImGui::GetIO();
    io.MouseWheel += float(y);
    io.MouseWheelH += float(x);
    return io.WantCaptureMouse;
}

void Application::updateImGui()
{
    auto& io = ImGui::GetIO();

    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);

    io.DisplaySize = { float(width), float(height) };
    io.DeltaTime = float(m_timeSinceLastFrame) / 1000;

    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
    io.MousePos = { float(xpos), float(ypos) };

    //bool hadMouseFocus = io.WantCaptureMouse;

    ImGui::NewFrame();

    io.MouseWheel = 0;
    io.MouseWheelH = 0;
}

void Application::renderImGui()
{
    m_imguiRenderData->render();
}
