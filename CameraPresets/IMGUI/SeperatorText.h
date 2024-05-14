#include "pch.h"

#include "imgui_internal.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_rangeslider.h"
#include "imgui_additions.h"

namespace ImGui {
    void SeparatorTextV(const char* fmt, va_list args)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        const char* label, * label_end;
        label_end = FindRenderedTextEnd(label, label_end);

        static float Style_SeparatorPaddingY = style.FramePadding.y;
        static float Style_SeparatorMinWidth = 10.0f;
        static float Style_SeparatorAlign = 0.0f;       // When set to >0.0f (most often 0.5f or 1.0f) generally MinLeadingWidth would be 0.0f
        static float Style_SeparatorSize = 1.0f;        // FIXME-STYLE: If intending a Separator (as per current name) abs thickness is good. If intending a larger fill may better be expressed as a % of 

        const ImVec2 label_size = CalcTextSize(label, label_end, false);
        const ImVec2 pos = window->DC.CursorPos;
        const float sep1_min_w = Style_SeparatorMinWidth;
        const float sep2_min_w = Style_SeparatorMinWidth;// 0.0f;

        const float min_size_padding = (sep1_min_w > 0.0f ? sep1_min_w + style.ItemSpacing.x : 0.0f) + (sep2_min_w > 0.0f ? sep2_min_w + style.ItemSpacing.x : 0.0f);
        const ImVec2 min_size(min_size_padding + label_size.x, label_size.y + Style_SeparatorPaddingY * 2.0f);
        const ImRect bb(pos, ImVec2(window->WorkRect.Max.x, pos.y + min_size.y));
        ItemSize(min_size);
        if (!ItemAdd(bb, 0))
            return;

        const float seps_y = ImFloor((bb.Min.y + bb.Max.y) * 0.5f + 0.99999f);
        const float sep1_x1 = pos.x;
        const float sep2_x2 = bb.Max.x;

        const float sep1_spacing = (sep1_min_w > 0.0f || Style_SeparatorAlign > 0.0f) ? style.ItemSpacing.x : 0.0f;
        const float sep2_spacing = (sep2_min_w > 0.0f || Style_SeparatorAlign < 1.0f) ? style.ItemSpacing.x : 0.0f;

        const float label_avail_w = ImMax(0.0f, sep2_x2 - sep1_x1 - sep1_spacing - sep1_min_w - sep2_spacing - sep2_min_w);

        ImVec2 label_pos;
        label_pos.x = pos.x + sep1_min_w + sep1_spacing + ImMax(0.0f, (label_avail_w - label_size.x) * Style_SeparatorAlign);
        label_pos.y = pos.y + Style_SeparatorPaddingY;

        const ImU32 col = GetColorU32(ImGuiCol_Separator);
        if (label_size.x > 0.0f)
        {
            const float sep1_x2 = label_pos.x - sep1_spacing;
            const float sep2_x1 = label_pos.x + label_size.x + sep2_spacing;
            if (sep1_x2 > sep1_x1)
                window->DrawList->AddLine(ImVec2(sep1_x1, seps_y), ImVec2(sep1_x2, seps_y), col, Style_SeparatorSize);
            if (sep2_x2 > sep2_x1)
                window->DrawList->AddLine(ImVec2(sep2_x1, seps_y), ImVec2(sep2_x2, seps_y), col, Style_SeparatorSize);
            RenderTextEllipsis(window->DrawList, label_pos, ImVec2(bb.Max.x, bb.Max.y + style.ItemSpacing.y), bb.Max.x, bb.Max.x, label, label_end, &label_size);
        }
        else
        {
            window->DrawList->AddLine(ImVec2(sep1_x1, seps_y), ImVec2(sep2_x2, seps_y), col, Style_SeparatorSize);
        }
    }
}