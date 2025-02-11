#include "Toasts.hpp"

#include <deque>
#include <chrono>
#include <algorithm>
#include <cstdint>
#include "Modules/Engine.hpp"
#include "Modules/Surface.hpp"
#include "Modules/Scheme.hpp"

#define PADDING 6
#define TOAST_GAP 10
#define TOAST_BACKGROUND Color{0, 0, 0, 192}

#define SLIDE_RATE 200 // thousandths of screen / s

struct Toast
{
    std::string text;
    Color color;
    std::chrono::time_point<std::chrono::steady_clock> dismissAt;
};

static std::deque<Toast> g_toasts;

static std::chrono::time_point<std::chrono::steady_clock> g_slideOffTime;
static int g_slideOffStart;
static int g_slideOff;

Variable sar_toast_disable("sar_toast_disable", "0", "Disable all toasts from showing.\n");
Variable sar_toast_font("sar_toast_font", "6", 0, "The font index to use for toasts.\n");
Variable sar_toast_width("sar_toast_width", "250", 2 * PADDING, "The maximum width for toasts.\n");
Variable sar_toast_pos("sar_toast_pos", "0", 0, 3, "The position to display toasts in. 0 = bottom left, 1 = bottom right, 2 = top left, 3 = top right.\n");

ToastHud::ToastHud()
    : Hud(HudType_InGame | HudType_Paused | HudType_Menu, true)
{
}

bool ToastHud::ShouldDraw()
{
    return Hud::ShouldDraw() && !sar_toast_disable.GetBool();
}

bool ToastHud::GetCurrentSize(int &xSize, int &ySize)
{
    return false;
}

static std::vector<std::string> splitIntoLines(Surface::HFont font, std::string text, int maxWidth) {
    int length = text.length();
    const char *str = text.c_str();
    const char *end = str + length;

    std::vector<std::string> lines;

    int32_t lastSpace = -1;

    size_t i = 1;
    while (i < length) {
        if (str[i] == '\n') {
            lines.push_back({str, i + 1});
            str += i + 1;
            length -= i + 1;
            i = 1;
            continue;
        }

        int width = surface->GetFontLength(font, "%.*s", i, str);

        if (width > maxWidth && i > 1) {
            // We have to split onto a new line!
            if (lastSpace != -1) {
                // We've seen a space - split there
                lines.push_back({str, (unsigned)lastSpace});
                str += lastSpace + 1;
                length -= lastSpace + 1;
                i = 1;
                lastSpace = -1;
                continue;
            } else {
                // There's been no space - just split the string here
                lines.push_back({str, i});
                str += i;
                length -= i;
                i = 1;
                continue;
            }
        }

        if (str[i] == ' ') {
            lastSpace = i;
        }

        ++i;
    }

    if (length > 0) {
        lines.push_back({str});
    }

    return lines;
}

void ToastHud::AddToast(std::string text, Color color, double duration, bool doConsole)
{
    auto now = NOW_STEADY();

    g_toasts.push_back({
        text,
        color,
        now + std::chrono::microseconds((int64_t)(duration * 1000000)),
    });

    Surface::HFont font = scheme->GetDefaultFont() + sar_toast_font.GetInt();
    int lineHeight = surface->GetFontHeight(font) + PADDING;
    int maxWidth = sar_toast_width.GetInt();
    auto lines = splitIntoLines(font, text, maxWidth - 2 * PADDING);
    g_slideOffStart = g_slideOff + (lines.size() * lineHeight + PADDING + TOAST_GAP);
    g_slideOffTime = now;

    if (doConsole) {
        Color conCol = color;
        if (color.r() == 255 && color.g() == 255 && color.b() == 255 && color.a() == 255) {
            conCol.SetColor(255, 150, 50, 255);
        }
        console->ColorMsg(conCol, "%s\n", text.c_str());
    }
}

void ToastHud::Update()
{
    auto now = NOW_STEADY();

    g_toasts.erase(
        std::remove_if(
            g_toasts.begin(),
            g_toasts.end(),
            [=](Toast toast) {
                return now >= toast.dismissAt;
            }
        ),
        g_toasts.end()
    );

    int screenWidth, screenHeight;
#ifdef _WIN32
    engine->GetScreenSize(screenWidth, screenHeight);
#else
    engine->GetScreenSize(nullptr, screenWidth, screenHeight);
#endif

    g_slideOff = g_slideOffStart - SLIDE_RATE * std::chrono::duration_cast<std::chrono::milliseconds>(now - g_slideOffTime).count() * screenHeight / 1000 / 1000;
    if (g_slideOff < 0) {
        g_slideOff = 0;
    }
}

void ToastHud::Paint(int slot)
{
    if (slot != 0 && !engine->IsOrange()) {
        // Don't double-draw
        return;
    }

    Update();

    Surface::HFont font = scheme->GetDefaultFont() + sar_toast_font.GetInt();

    int screenWidth, screenHeight;
#ifdef _WIN32
    engine->GetScreenSize(screenWidth, screenHeight);
#else
    engine->GetScreenSize(nullptr, screenWidth, screenHeight);
#endif

    int maxWidth = sar_toast_width.GetInt();

    int lineHeight = surface->GetFontHeight(font) + PADDING;

    bool alignRight = sar_toast_pos.GetInt() & 1;
    bool alignTop = sar_toast_pos.GetInt() & 2;

    int yOffset = TOAST_GAP - g_slideOff;
    if (!alignTop) {
        yOffset = screenHeight - yOffset;
    }

    for (auto iter = g_toasts.rbegin(); iter != g_toasts.rend(); ++iter) {
        auto toast = *iter;

        auto lines = splitIntoLines(font, toast.text, maxWidth - 2 * PADDING);

        if (lines.size() == 0) {
            continue;
        }

        int longestLine = -1;
        for (std::string line : lines) {
            int length = surface->GetFontLength(font, "%s", line.c_str());
            if (length > longestLine) {
                longestLine = length;
            }
        }

        int width = longestLine + 2 * PADDING;
        int height = lines.size() * lineHeight + PADDING;

        int xLeft = alignRight ? screenWidth - TOAST_GAP - width : TOAST_GAP;

        if (!alignTop) {
            yOffset -= height;
        }

        surface->DrawRect(TOAST_BACKGROUND, xLeft, yOffset, xLeft + width, yOffset + height);

        yOffset += PADDING;

        for (std::string line : lines) {
            int length = surface->GetFontLength(font, "%s", line.c_str());
            surface->DrawTxt(font, xLeft + PADDING, yOffset, toast.color, "%s", line.c_str());
            yOffset += lineHeight;
        }

        if (alignTop) {
            yOffset += TOAST_GAP;
        } else {
            yOffset -= height + TOAST_GAP;
        }
    }
}

CON_COMMAND(sar_toast_create, "sar_toast_create <duration> <r> <g> <b> <text> - create a toast.\n")
{
    if (args.ArgC() != 6) {
        console->Print(sar_toast_create.ThisPtr()->m_pszHelpString);
        return;
    }

    double duration = atof(args[1]);
    int r = atoi(args[2]);
    int g = atoi(args[3]);
    int b = atoi(args[4]);
    std::string text(args[5]);

    toastHud.AddToast(text, Color{r, g, b, 255}, duration);
}

CON_COMMAND(sar_toast_dismiss_all, "sar_toast_dismiss_all - dismiss all active toasts.\n")
{
    g_toasts.clear();
}

ToastHud toastHud;
