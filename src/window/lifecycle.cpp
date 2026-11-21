#include "marx/graphics-internal/window.hpp"

#include <glad/gl.h>

#include <clocale>
#include <exception>
#include <iostream>
#include <vector>

#include "internal.hpp"

using namespace marx::graphics::internal;

namespace
{
XVisualInfo *Visual_   = nullptr;
Colormap     Colormap_ = 0;

std::vector<float> Data_;
} // namespace

namespace marx::graphics
{
Display *get_display()
{
    return Display_;
}

window::window(iVector2 Size, string Name, window_config Config)
{
    std::setlocale(LC_CTYPE, "");

    Display_ = XOpenDisplay(nullptr);

    if (!Display_)
    {
        std::cerr << "Cannot open display.\n";
        std::terminate();
    }

    int Supported = false;
    XkbSetDetectableAutoRepeat(Display_, true, &Supported);

    Scr_  = DefaultScreen(Display_);
    Root_ = RootWindow(Display_, Scr_);

    Visual_ = choose_visual(Config.has(config::Transparent),
                            Config.has(config::Msaa));

    if (!Visual_ && Config.has(config::Transparent) && Config.has(config::Msaa))
    {
        Visual_ = choose_visual(true, false);

        if (Visual_)
        {
            std::cerr << "Multisampling is not available with transparency, "
                         "continuing without it.\n";
            Config.remove(config::Msaa);
        }
    }

    if (!Visual_ && Config.has(config::Transparent))
    {
        std::cerr
            << "A transparent visual is not available, continuing without "
               "transparency.\n";
        Config.remove(config::Transparent);
        Visual_ = choose_visual(false, Config.has(config::Msaa));
    }

    if (!Visual_ && Config.has(config::Msaa))
    {
        std::cerr << "Multisampling is not available, continuing without it.\n";
        Config.remove(config::Msaa);
        Visual_ = choose_visual(false, false);
    }

    if (!Visual_)
    {
        std::cerr << "Cannot find a GLX-compatible visual.\n";
        XCloseDisplay(Display_);
        Display_ = nullptr;
        std::terminate();
    }

    Transparent_ = Config.has(config::Transparent) && Visual_->depth == 32;

    if (Config.has(config::Transparent) && !Transparent_)
        std::cerr
            << "The visual has no alpha channel, transparency is disabled.\n";

    Colormap_ = XCreateColormap(Display_, Root_, Visual_->visual, AllocNone);

    XSetWindowAttributes xwa{};

    xwa.background_pixel = Transparent_ ? 0 : WhitePixel(Display_, Scr_);
    xwa.border_pixel     = BlackPixel(Display_, Scr_);
    xwa.colormap         = Colormap_;
    xwa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
                     ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
                     StructureNotifyMask | FocusChangeMask | EnterWindowMask |
                     LeaveWindowMask;

    iVector2 Start = { 500, 500 };

    if (Config.has(config::Centered))
        Start = { (DisplayWidth(Display_, Scr_) - Size.x) / 2,
                  (DisplayHeight(Display_, Scr_) - Size.y) / 2 };

    Window_ = XCreateWindow(
        Display_, Root_, Start.x, Start.y, Size.x, Size.y, 0, Visual_->depth,
        InputOutput, Visual_->visual,
        CWBackPixel | CWBorderPixel | CWColormap | CWEventMask, &xwa);

    Context_ = glXCreateContext(Display_, Visual_, nullptr, True);

    XFree(Visual_);
    Visual_ = nullptr;

    if (!Context_)
    {
        std::cerr << "Failed to create GLX context.\n";
        XDestroyWindow(Display_, Window_);
        XFreeColormap(Display_, Colormap_);
        XCloseDisplay(Display_);
        Display_  = nullptr;
        Window_   = 0;
        Colormap_ = 0;
        std::terminate();
    }

    if (!glXMakeCurrent(Display_, Window_, Context_))
    {
        std::cerr << "Failed to make GLX context current.\n";
        glXDestroyContext(Display_, Context_);
        Context_ = nullptr;
        XDestroyWindow(Display_, Window_);
        XFreeColormap(Display_, Colormap_);
        XCloseDisplay(Display_);
        Display_  = nullptr;
        Window_   = 0;
        Colormap_ = 0;
        std::terminate();
    }

    if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(glXGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD.\n";
        glXMakeCurrent(Display_, None, nullptr);
        glXDestroyContext(Display_, Context_);
        Context_ = nullptr;
        XDestroyWindow(Display_, Window_);
        XFreeColormap(Display_, Colormap_);
        XCloseDisplay(Display_);
        Display_  = nullptr;
        Window_   = 0;
        Colormap_ = 0;
        std::terminate();
    }

    SwapInterval_ =
        reinterpret_cast<PFNGLXSWAPINTERVALEXTPROC>(glXGetProcAddress(
            reinterpret_cast<const GLubyte *>("glXSwapIntervalEXT")));

    enable_vsync(Config.has(config::Vsync));

    WM_DELETE_ = XInternAtom(Display_, "WM_DELETE_WINDOW", False);

    XSetWMProtocols(Display_, Window_, &WM_DELETE_, 1);

    set_size(Size);
    set_name(Name);

    ReportedSize_ = Size;

    apply_config(Config, Size, Start);

    if (XSupportsLocale())
    {
        XSetLocaleModifiers("");
        InputMethod_ = XOpenIM(Display_, nullptr, nullptr, nullptr);
    }

    if (InputMethod_)
        InputContext_ = XCreateIC(
            InputMethod_, XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
            XNClientWindow, Window_, XNFocusWindow, Window_, nullptr);

    if (InputContext_)
    {
        unsigned long FilterEvents = 0;
        XGetICValues(InputContext_, XNFilterEvents, &FilterEvents, nullptr);
        XSelectInput(Display_, Window_, xwa.event_mask | FilterEvents);
        XSetICFocus(InputContext_);
    }

    if (!Config.has(config::Hidden))
        XMapWindow(Display_, Window_);

    XFlush(Display_);

    glViewport(0, 0, Size.x, Size.y);

    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE,
                        GL_ONE_MINUS_SRC_ALPHA);

    if (Config.has(config::Msaa))
        glEnable(GL_MULTISAMPLE);

    Shader_       = make_shader(VertShader, FragShader);
    TextShader_   = make_shader(TextVertShader, TextFragShader);
    SpriteShader_ = make_shader(SpriteVertShader, SpriteFragShader);
    glUseProgram(Shader_);

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);

    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    constexpr GLsizei Stride = 6 * sizeof(float);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, Stride, nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Stride,
                          reinterpret_cast<void *>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &TextVAO_);
    glGenBuffers(1, &TextVBO_);

    glBindVertexArray(TextVAO_);
    glBindBuffer(GL_ARRAY_BUFFER, TextVBO_);

    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    constexpr GLsizei TextStride = text::FloatsPerVertex * sizeof(float);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, TextStride, nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, TextStride,
                          reinterpret_cast<void *>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, TextStride,
                          reinterpret_cast<void *>(4 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &SpriteVAO_);
    glGenBuffers(1, &SpriteVBO_);

    glBindVertexArray(SpriteVAO_);
    glBindBuffer(GL_ARRAY_BUFFER, SpriteVBO_);

    glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), nullptr,
                 GL_DYNAMIC_DRAW);

    constexpr GLsizei SpriteStride = 4 * sizeof(float);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, SpriteStride, nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, SpriteStride,
                          reinterpret_cast<void *>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Data_.clear();

    Closed_ = false;
}

window::~window()
{
    if (!Closed_)
        close();
}

void window::close()
{
    if (Closed_)
        return;

    if (Context_)
    {
        if (glXGetCurrentContext() != Context_)
            glXMakeCurrent(Display_, Window_, Context_);

        glDeleteBuffers(1, &VBO_);
        glDeleteVertexArrays(1, &VAO_);
        glDeleteProgram(Shader_);

        glDeleteBuffers(1, &TextVBO_);
        glDeleteVertexArrays(1, &TextVAO_);
        glDeleteProgram(TextShader_);

        VBO_        = 0;
        VAO_        = 0;
        Shader_     = 0;
        TextVBO_    = 0;
        TextVAO_    = 0;
        TextShader_ = 0;

        glXMakeCurrent(Display_, None, nullptr);
        glXDestroyContext(Display_, Context_);
        Context_ = nullptr;
    }

    if (InputContext_)
    {
        XDestroyIC(InputContext_);
        InputContext_ = nullptr;
    }

    if (InputMethod_)
    {
        XCloseIM(InputMethod_);
        InputMethod_ = nullptr;
    }

    if (Display_)
    {
        for (Cursor &Created : Cursors_)
        {
            if (Created)
                XFreeCursor(Display_, Created);

            Created = 0;
        }

        if (BlankCursor_)
            XFreeCursor(Display_, BlankCursor_);

        BlankCursor_ = 0;
    }

    CursorApplied_ = false;

    Pending_.clear();
    HeldKeys_.fill(false);
    Focused_ = false;

    if (Display_ && Window_)
    {
        XUnmapWindow(Display_, Window_);
        XDestroyWindow(Display_, Window_);
    }

    if (Display_ && Colormap_)
        XFreeColormap(Display_, Colormap_);

    if (Display_)
        XCloseDisplay(Display_);

    Display_  = nullptr;
    Window_   = 0;
    Colormap_ = 0;
    Closed_   = true;
}

bool window::is_open() const
{
    return !Closed_;
}
} // namespace marx::graphics
