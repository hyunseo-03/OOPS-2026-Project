#include "view/FactoryView.h"

#include <cstdio>

#include "imgui.h"

FactoryView::FactoryView(BurgerFactoryModel& model, FactoryController& controller)
    : model(model), controller(controller) {}

const char* FactoryView::stepToString(ProcessStep step) const
{
    switch (step)
    {
        case ProcessStep::Idle:                return "Idle";
        case ProcessStep::PreparingIngredients: return "Preparing Ingredients";
        case ProcessStep::GrillPatty:           return "Grilling Patty";
        case ProcessStep::AddSauce:            return "Adding Sauce";
        case ProcessStep::AssembleBurger:       return "Assembling Burger";
        case ProcessStep::QualityCheck:         return "Quality Check";
        case ProcessStep::PackBurger:           return "Packing";
        case ProcessStep::Done:                return "Done";
        default:                               return "Unknown";
    }
}

void FactoryView::render()
{
    ImGuiIO& io = ImGui::GetIO();
    const float W = io.DisplaySize.x;
    const float H = io.DisplaySize.y;

    const ImVec4 bg          = ImVec4(0.045f, 0.060f, 0.075f, 1.0f);
    const ImVec4 panel       = ImVec4(0.075f, 0.100f, 0.125f, 1.0f);
    const ImVec4 panelSoft   = ImVec4(0.095f, 0.125f, 0.155f, 1.0f);
    const ImVec4 border      = ImVec4(0.140f, 0.180f, 0.220f, 1.0f);
    const ImVec4 text        = ImVec4(0.920f, 0.940f, 0.960f, 1.0f);
    const ImVec4 muted       = ImVec4(0.540f, 0.590f, 0.640f, 1.0f);
    const ImVec4 green       = ImVec4(0.230f, 0.820f, 0.360f, 1.0f);
    const ImVec4 blue        = ImVec4(0.320f, 0.620f, 1.000f, 1.0f);
    const ImVec4 yellow      = ImVec4(1.000f, 0.780f, 0.220f, 1.0f);
    const ImVec4 red         = ImVec4(1.000f, 0.300f, 0.260f, 1.0f);

    auto burgerName = [](BurgerType t) -> const char* {
        return t == BurgerType::CLASSIC ? "Classic" :
               t == BurgerType::CHEESE  ? "Cheese"  : "Double";
    };
    auto burgerPrice = [](BurgerType t) -> int {
        return t == BurgerType::CLASSIC ? 100 :
               t == BurgerType::CHEESE  ? 130 : 160;
    };

    auto statusColor = [&](Machine* m, bool active) -> ImVec4 {
        if (m && m->isFailed()) return red;
        if (m && m->isPaused()) return yellow;
        if (active && m && m->isRunning()) return green;
        if (active) return blue;
        return muted;
    };

    auto statusLabel = [](Machine* m, bool active) -> const char* {
        if (m && m->isFailed()) return "Repair needed";
        if (m && m->isPaused()) return "Paused";
        if (active && m && m->isRunning()) return "Running";
        if (active) return "Waiting";
        return "Idle";
    };

    auto drawStatusDot = [](const ImVec4& color) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        drawList->AddCircleFilled(ImVec2(p.x + 5.0f, p.y + 8.0f), 4.0f, ImGui::GetColorU32(color));
        ImGui::Dummy(ImVec2(14.0f, 16.0f));
        ImGui::SameLine(0.0f, 0.0f);
    };

    auto drawStatCard = [&](const char* id, float width, const char* label, const char* value, const ImVec4& valueColor) {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, panelSoft);
        ImGui::PushStyleColor(ImGuiCol_Border, border);
        ImGui::BeginChild(id, ImVec2(width, 70.0f), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::SetCursorPosY(12.0f);
        ImGui::TextColored(muted, "%s", label);
        ImGui::SetCursorPosY(36.0f);
        ImGui::TextColored(valueColor, "%s", value);
        ImGui::EndChild();
        ImGui::PopStyleColor(2);
    };

    auto drawInventoryRow = [&](const char* label, const char* code, IngredientType type) {
        const int amount = model.getIngredientAmount(type);
        const bool low = amount <= 5;
        float ratio = amount / 60.0f;
        if (ratio > 1.0f) ratio = 1.0f;

        ImGui::TextColored(low ? red : text, "%-4s", code);
        ImGui::SameLine(58.0f);
        ImGui::TextColored(low ? red : text, "%s", label);
        ImGui::SameLine(ImGui::GetWindowWidth() - 52.0f);
        ImGui::TextColored(low ? red : text, "%d", amount);

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, low ? red : green);
        ImGui::ProgressBar(ratio, ImVec2(-1.0f, 5.0f), "");
        ImGui::PopStyleColor();
        ImGui::Spacing();
    };

    auto currentStatusText = [&]() -> const char* {
        if (model.isCurrentMachineFailed()) return "Machine failure detected. Repair the current station.";
        if (model.isCurrentMachinePaused()) return "Production is paused.";
        switch (model.getCurrentStep())
        {
            case ProcessStep::Idle:
                return model.hasOrder() ? "Order received. Ready to start production." : "Waiting for a new order.";
            case ProcessStep::PreparingIngredients: return "Preparing ingredients...";
            case ProcessStep::GrillPatty:           return "Grilling patty...";
            case ProcessStep::AddSauce:            return "Adding sauce...";
            case ProcessStep::AssembleBurger:       return "Assembling burger...";
            case ProcessStep::QualityCheck:         return "Checking quality...";
            case ProcessStep::PackBurger:           return "Packing order...";
            case ProcessStep::Done:                return "Burger completed.";
            default:                               return "Factory is ready.";
        }
    };

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(W, H), ImGuiCond_Always);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 12.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12.0f, 8.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, bg);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, panel);
    ImGui::PushStyleColor(ImGuiCol_Border, border);
    ImGui::PushStyleColor(ImGuiCol_Text, text);
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, muted);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.120f, 0.250f, 0.520f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.170f, 0.350f, 0.720f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.090f, 0.200f, 0.430f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.055f, 0.075f, 0.095f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.075f, 0.105f, 0.135f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, green);
    ImGui::PushStyleColor(ImGuiCol_Separator, border);

    ImGui::Begin("Burger Factory Dashboard", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse);

    ProcessStep cur = model.getCurrentStep();

    ImGui::TextColored(text, "BURGER FACTORY");
    const float topControlsX = W > 320.0f ? W - 230.0f : 92.0f;
    ImGui::SameLine(topControlsX);

    // ✅ 개선: 더 안전하고 가시적인 비활성화 방식 적용
    bool canStart = model.canProceed(cur);
    ImGui::BeginDisabled(!canStart);
    if (ImGui::Button("Start", ImVec2(64.0f, 34.0f))) controller.onStartMachine(cur);
    ImGui::EndDisabled();

    ImGui::SameLine();
    Machine* currentMachine = model.getMachine(cur);
    bool canPause = currentMachine && (currentMachine->isRunning() || currentMachine->isPaused()) && !currentMachine->isFailed();
    ImGui::BeginDisabled(!canPause);
    if (ImGui::Button(currentMachine && currentMachine->isPaused() ? "Resume" : "Pause", ImVec2(72.0f, 34.0f)))
        controller.onTogglePause(cur);
    ImGui::EndDisabled();

    ImGui::SameLine();
    if (ImGui::Button("Refill", ImVec2(70.0f, 34.0f))) controller.onRefillInventory();

    ImGui::Separator();

    char moneyText[32];
    char completedText[32];
    char orderText[32];
    std::snprintf(moneyText, sizeof(moneyText), "$%d", model.getMoney());
    std::snprintf(completedText, sizeof(completedText), "%d", model.getTotalBurgersProduced());
    std::snprintf(orderText, sizeof(orderText), "%d / 10",
        (int)model.getQueuedOrders().size() + (model.hasOrder() ? 1 : 0));

    const float metricGap = 8.0f;
    const float metricW = (ImGui::GetContentRegionAvail().x - metricGap * 3.0f) / 4.0f;
    drawStatCard("MoneyCard", metricW, "Money", moneyText, green);
    ImGui::SameLine(0.0f, metricGap);

    drawStatCard("CompletedCard", metricW, "Completed Burgers", completedText, yellow);
    ImGui::SameLine(0.0f, metricGap);

    drawStatCard("OrdersCard", metricW, "Orders", orderText, blue);
    ImGui::SameLine(0.0f, metricGap);

    ImGui::PushStyleColor(ImGuiCol_ChildBg, panelSoft);
    ImGui::PushStyleColor(ImGuiCol_Border, border);
    ImGui::BeginChild("ReputationCard", ImVec2(metricW, 70.0f), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::SetCursorPosY(12.0f);
    ImGui::TextColored(muted, "Reputation");
    ImGui::SetCursorPosY(36.0f);
    const int reputation = 4;
    for (int i = 0; i < 5; ++i)
    {
        ImGui::TextColored(i < reputation ? yellow : muted, "*");
        if (i < 4) ImGui::SameLine();
    }
    ImGui::EndChild();
    ImGui::PopStyleColor(2);

    const float belowMetricsY = ImGui::GetCursorPosY();
    const float bottomMargin = 12.0f;
    const float contentH = H - belowMetricsY - bottomMargin;
    const float inventoryW = W * 0.205f;
    const float ordersW = W * 0.245f;
    const float mainW = W - inventoryW - ordersW - 44.0f;

    ImGui::PushStyleColor(ImGuiCol_ChildBg, panel);
    ImGui::PushStyleColor(ImGuiCol_Border, border);
    ImGui::BeginChild("InventoryPanel", ImVec2(inventoryW, contentH), true);
    ImGui::TextColored(text, "INVENTORY");
    ImGui::Separator();
    drawInventoryRow("Buns", "BUN", IngredientType::BUN);
    drawInventoryRow("Patties", "PAT", IngredientType::PATTY);
    drawInventoryRow("Lettuce", "LET", IngredientType::LETTUCE);
    drawInventoryRow("Tomato", "TOM", IngredientType::TOMATO);
    drawInventoryRow("Cheese", "CHS", IngredientType::CHEESE);
    drawInventoryRow("Sauce", "SAU", IngredientType::SAUCE);
    ImGui::EndChild();
    ImGui::PopStyleColor(2);

    ImGui::SameLine();

    ImGui::BeginGroup();
    ImGui::PushStyleColor(ImGuiCol_ChildBg, panel);
    ImGui::PushStyleColor(ImGuiCol_Border, border);
    ImGui::BeginChild("ProductionPanel", ImVec2(mainW, contentH * 0.67f), true);
    ImGui::TextColored(text, "PRODUCTION LINE");
    ImGui::Separator();

    struct MachineCard
    {
        const char* name;
        const char* shortName;
        ProcessStep step;
    };

    MachineCard cards[] = {
        { "Prep\nStation",     "PREP", ProcessStep::PreparingIngredients },
        { "Patty\nGrill",      "GRIL", ProcessStep::GrillPatty },
        { "Sauce\nDispenser",  "SAUC", ProcessStep::AddSauce },
        { "Assembly\nMachine", "ASMB", ProcessStep::AssembleBurger },
        { "Quality\nCheck",    "QC",   ProcessStep::QualityCheck },
        { "Packing\nMachine",  "PACK", ProcessStep::PackBurger },
    };

    const float arrowW = 20.0f;
    const float cardGap = 8.0f;
    float cardW = (ImGui::GetContentRegionAvail().x - arrowW * 5.0f - cardGap * 10.0f) / 6.0f;
    if (cardW < 86.0f) cardW = 86.0f;
    const float cardH = ImGui::GetContentRegionAvail().y - 6.0f;
    if (cardH > 170.0f) ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (cardH - 170.0f) * 0.35f);

    const float machineRowY = ImGui::GetCursorPosY();
    for (int i = 0; i < 6; ++i)
    {
        Machine* m = model.getMachine(cards[i].step);
        const bool active = cur == cards[i].step;
        ImVec4 stateColor = statusColor(m, active);

        ImGui::PushStyleColor(ImGuiCol_ChildBg, active ? ImVec4(0.095f, 0.155f, 0.145f, 1.0f) : panelSoft);
        ImGui::PushStyleColor(ImGuiCol_Border, m && m->isFailed() ? red : (active ? green : border));

        char childId[32];
        std::snprintf(childId, sizeof(childId), "MachineCard%d", i);
        ImGui::BeginChild(childId, ImVec2(cardW, 160.0f), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::TextWrapped("%s", cards[i].name);
        ImGui::Spacing();
        ImGui::TextColored(stateColor, "%s", cards[i].shortName);
        ImGui::Spacing();
        drawStatusDot(stateColor);
        ImGui::TextDisabled("%s", statusLabel(m, active));
        ImGui::Spacing();

        if (m && m->isFailed())
        {
            char repairLabel[48];
            std::snprintf(repairLabel, sizeof(repairLabel), "Repair $%d##repair%d", BurgerFactoryModel::REPAIR_COST, i);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.130f, 0.130f, 1.0f));
            if (ImGui::Button(repairLabel, ImVec2(-1.0f, 28.0f))) controller.onRepairMachine(cards[i].step);
            ImGui::PopStyleColor();
        }
        else if (m)
        {
            char progressText[16];
            std::snprintf(progressText, sizeof(progressText), "%.0f%%", m->getProgress() * 100.0f);
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, active ? green : ImVec4(0.180f, 0.260f, 0.350f, 1.0f));
            ImGui::ProgressBar(m->getProgress(), ImVec2(-1.0f, 8.0f), "");
            ImGui::PopStyleColor();
            ImGui::TextDisabled("%s", progressText);
        }

        ImGui::EndChild();
        ImGui::PopStyleColor(2);

        // ✅ 핵심 버그 수정: SetCursorPos를 사용하여 X축 리셋 없이 정확한 가로 정렬 보장
        if (i < 5)
        {
            ImGui::SameLine(0.0f, cardGap);
            float currentX = ImGui::GetCursorPosX();
            
            // X축은 유지한 채 Y축만 중앙으로 내려서 화살표 기호 그리기
            ImGui::SetCursorPosY(machineRowY + 68.0f);
            ImGui::TextColored(muted, ">");
            
            // 화살표 크기만큼 X축을 더 전진시킨 후, Y축을 원래 기계 시작 높이로 원상 복구
            ImGui::SameLine(0.0f, cardGap);
            ImGui::SetCursorPosY(machineRowY);
        }
    }
    ImGui::SetCursorPosY(machineRowY + 168.0f);
    ImGui::EndChild();
    ImGui::PopStyleColor(2);

    ImGui::PushStyleColor(ImGuiCol_ChildBg, panel);
    ImGui::PushStyleColor(ImGuiCol_Border, border);
    ImGui::BeginChild("StatusActionPanel", ImVec2(mainW, 0.0f), true);
    ImGui::TextColored(text, "CURRENT STATUS");
    ImGui::TextColored(model.isCurrentMachineFailed() ? red : text, "%s", currentStatusText());
    ImGui::Separator();

    const float buttonW = (ImGui::GetContentRegionAvail().x - 16.0f) / 3.0f;
    if (ImGui::Button("Classic $100", ImVec2(buttonW, 38.0f))) controller.onNewOrder(BurgerType::CLASSIC);
    ImGui::SameLine();
    if (ImGui::Button("Cheese $130", ImVec2(buttonW, 38.0f))) controller.onNewOrder(BurgerType::CHEESE);
    ImGui::SameLine();
    if (ImGui::Button("Double $160", ImVec2(buttonW, 38.0f))) controller.onNewOrder(BurgerType::DOUBLE);

    ImGui::Spacing();
    ImGui::BeginDisabled(!canStart);
    if (ImGui::Button("Start Production Line", ImVec2(buttonW * 1.5f, 38.0f))) controller.onStartMachine(cur);
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::BeginDisabled(true);
    ImGui::Button("Sell Burger", ImVec2(buttonW * 1.5f, 38.0f));
    ImGui::EndDisabled();

    ImGui::EndChild();
    ImGui::PopStyleColor(2);
    ImGui::EndGroup();

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_ChildBg, panel);
    ImGui::PushStyleColor(ImGuiCol_Border, border);
    ImGui::BeginChild("OrdersPanel", ImVec2(ordersW, contentH), true);
    ImGui::TextColored(text, "ORDERS");
    ImGui::Separator();

    ImGui::TextColored(yellow, "In Production");
    if (model.hasOrder())
    {
        const Order& order = model.getCurrentOrder();
        ImGui::Text("%s Burger", burgerName(order.type));
        ImGui::TextDisabled("$%d | %s", burgerPrice(order.type), stepToString(cur));
    }
    else
    {
        ImGui::TextDisabled("None");
    }

    ImGui::Spacing();
    const auto& queued = model.getQueuedOrders();
    ImGui::TextColored(blue, "Waiting (%d)", (int)queued.size());
    ImGui::Separator();
    if (queued.empty())
    {
        ImGui::TextDisabled("No queued orders");
    }
    else
    {
        const int visibleQueued = (int)queued.size() > 6 ? 6 : (int)queued.size();
        for (int i = 0; i < visibleQueued; ++i)
        {
            ImGui::Text("%d. %-7s $%d", i + 1, burgerName(queued[i].type), burgerPrice(queued[i].type));
        }
        if ((int)queued.size() > visibleQueued)
            ImGui::TextDisabled("+%d more", (int)queued.size() - visibleQueued);
    }

    ImGui::Spacing();
    const auto& done = model.getCompletedOrders();
    ImGui::TextColored(green, "Sold (%d)", (int)done.size());
    ImGui::Separator();
    if (done.empty())
    {
        ImGui::TextDisabled("None yet");
    }
    else
    {
        ImGui::BeginChild("SoldScroll", ImVec2(0.0f, 0.0f), false);
        for (int i = (int)done.size() - 1; i >= 0; --i)
        {
            ImGui::Text("#%d %-7s +$%d", i + 1, burgerName(done[i].type), burgerPrice(done[i].type));
        }
        ImGui::EndChild();
    }

    ImGui::EndChild();
    ImGui::PopStyleColor(2);

    ImGui::End();

    ImGui::PopStyleColor(12);
    ImGui::PopStyleVar(6);
}

void FactoryView::renderOrderHistory() {}