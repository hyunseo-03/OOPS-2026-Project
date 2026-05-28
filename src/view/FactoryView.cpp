#include "view/FactoryView.h"
#include "IconsFontAwesome6.h"
#include <cstdio>
#include <string>

// ============================================================
// 레이아웃 상수
// ============================================================
const float FactoryView::WINDOW_WIDTH    = 1000.0f;
const float FactoryView::WINDOW_HEIGHT   = 580.0f;
const float FactoryView::CARD_WIDTH      = 140.0f;
const float FactoryView::CARD_HEIGHT     = 165.0f;
const float FactoryView::INVENTORY_WIDTH = 185.0f;

// ============================================================
// 생성자
// ============================================================
FactoryView::FactoryView(BurgerFactoryModel& model, FactoryController& controller)
    : model(model),
      controller(controller),
      showMachineDetail(false),
      selectedMachine(0),
      largeIconFont(nullptr)
{
    colorBg          = ImVec4(0.08f, 0.08f, 0.13f, 1.00f);
    colorCard        = ImVec4(0.13f, 0.13f, 0.20f, 1.00f);
    colorCardActive  = ImVec4(0.08f, 0.18f, 0.10f, 1.00f);
    colorRunning     = ImVec4(0.25f, 0.85f, 0.35f, 1.00f);
    colorIdle        = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);
    colorWaiting     = ImVec4(1.00f, 0.70f, 0.10f, 1.00f);
    colorMoney       = ImVec4(0.25f, 0.90f, 0.30f, 1.00f);
    colorBurger      = ImVec4(1.00f, 0.75f, 0.10f, 1.00f);
    colorOrders      = ImVec4(0.30f, 0.65f, 1.00f, 1.00f);
    colorStar        = ImVec4(1.00f, 0.80f, 0.00f, 1.00f);
}

// ============================================================
// setupStyle - ImGui 전역 스타일 설정 (1회 호출)
// ============================================================
void FactoryView::setupStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding    = 8.0f;
    style.ChildRounding     = 6.0f;
    style.FrameRounding     = 5.0f;
    style.PopupRounding     = 8.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding      = 4.0f;
    style.WindowPadding     = ImVec2(12.0f, 10.0f);
    style.FramePadding      = ImVec2(8.0f,  5.0f);
    style.ItemSpacing       = ImVec2(8.0f,  6.0f);
    style.ItemInnerSpacing  = ImVec2(6.0f,  4.0f);

    ImVec4* c = style.Colors;
    c[ImGuiCol_WindowBg]          = ImVec4(0.10f, 0.10f, 0.15f, 1.00f);
    c[ImGuiCol_ChildBg]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    c[ImGuiCol_PopupBg]           = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
    c[ImGuiCol_Border]            = ImVec4(0.25f, 0.25f, 0.35f, 1.00f);
    c[ImGuiCol_FrameBg]           = ImVec4(0.15f, 0.15f, 0.22f, 1.00f);
    c[ImGuiCol_TitleBg]           = ImVec4(0.08f, 0.08f, 0.13f, 1.00f);
    c[ImGuiCol_TitleBgActive]     = ImVec4(0.10f, 0.10f, 0.16f, 1.00f);
    c[ImGuiCol_Button]            = ImVec4(0.20f, 0.45f, 0.85f, 1.00f);
    c[ImGuiCol_ButtonHovered]     = ImVec4(0.30f, 0.55f, 0.95f, 1.00f);
    c[ImGuiCol_ButtonActive]      = ImVec4(0.15f, 0.35f, 0.75f, 1.00f);
    c[ImGuiCol_PlotHistogram]     = ImVec4(0.25f, 0.80f, 0.35f, 1.00f);
    c[ImGuiCol_Text]              = ImVec4(0.90f, 0.90f, 0.92f, 1.00f);
    c[ImGuiCol_TextDisabled]      = ImVec4(0.45f, 0.45f, 0.50f, 1.00f);
    c[ImGuiCol_Separator]         = ImVec4(0.25f, 0.25f, 0.35f, 1.00f);
    c[ImGuiCol_ModalWindowDimBg]  = ImVec4(0.00f, 0.00f, 0.00f, 0.55f);
}

// ============================================================
// render - 매 프레임 호출되는 UI 진입점
// ============================================================
void FactoryView::render()
{
    ImGuiIO& io = ImGui::GetIO();
    float posX  = (io.DisplaySize.x - WINDOW_WIDTH)  * 0.5f;
    float posY  = (io.DisplaySize.y - WINDOW_HEIGHT) * 0.5f;
    ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT), ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::Begin("##BurgerFactory", nullptr, flags);

    renderHeader();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::BeginGroup();
    renderInventoryPanel();
    ImGui::EndGroup();

    ImGui::SameLine(0.0f, 10.0f);

    ImGui::BeginGroup();
    renderProductionLine();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    renderStatusBar();
    ImGui::EndGroup();

    ImGui::End();

    if (showMachineDetail)
        renderMachineDetailPopup();
}

// ============================================================
// renderHeader - 타이틀 + 통계 박스 + 제어 버튼
// ============================================================
void FactoryView::renderHeader()
{
    ImGui::SetWindowFontScale(1.25f);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::Text("  " ICON_FA_BURGER "  BURGER FACTORY");
    ImGui::PopStyleColor();
    ImGui::SetWindowFontScale(1.0f);

    ImGui::SameLine(WINDOW_WIDTH - 135.0f);

    // 플레이 버튼
    if (model.isProducing())
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.60f, 0.20f, 1.0f));
    else
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.35f, 1.0f));

    if (ImGui::Button(" > ##play", ImVec2(32, 28))) controller.startFactory();
    ImGui::PopStyleColor();

    ImGui::SameLine();
    if (ImGui::Button("||##pause", ImVec2(32, 28))) controller.stopFactory();
    ImGui::SameLine();
    ImGui::Button(" @ ##settings", ImVec2(32, 28));

    ImGui::Spacing();

    // 통계 박스 4개
    float boxWidth = (WINDOW_WIDTH - 40.0f) / 4.0f;
    char buf[64];

    snprintf(buf, sizeof(buf), "$%d", model.getMoney());
    renderStatBox("Money", buf, colorMoney, boxWidth);
    ImGui::SameLine(0.0f, 8.0f);

    snprintf(buf, sizeof(buf), "%d", model.getBurgerCount());
    renderStatBox("Completed Burgers", buf, colorBurger, boxWidth);
    ImGui::SameLine(0.0f, 8.0f);

    snprintf(buf, sizeof(buf), "%d / %d", model.getOrders(), model.getMaxOrders());
    renderStatBox("Orders", buf, colorOrders, boxWidth);
    ImGui::SameLine(0.0f, 8.0f);

    // 별점: ICON_FA_STAR / ICON_FA_CIRCLE(빈 별)
    int rep = model.getReputation();
    std::string stars;
    for (int i = 0; i < 5; i++)
        stars += (i < rep) ? ICON_FA_STAR " " : ICON_FA_CIRCLE " ";
    renderStatBox("Reputation", stars.c_str(), colorStar, boxWidth);
}

void FactoryView::renderStatBox(const char* label, const char* value,
                                 ImVec4 valueColor, float width)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.13f, 0.13f, 0.20f, 1.0f));
    ImGui::BeginChild(label, ImVec2(width, 55.0f), true);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 0.70f, 1.0f));
    float labelW = ImGui::CalcTextSize(label).x;
    ImGui::SetCursorPosX((width - labelW) * 0.5f);
    ImGui::Text("%s", label);
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Text, valueColor);
    ImGui::SetWindowFontScale(1.15f);
    float valW = ImGui::CalcTextSize(value).x;
    ImGui::SetCursorPosX((width - valW) * 0.5f);
    ImGui::Text("%s", value);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleColor();

    ImGui::EndChild();
    ImGui::PopStyleColor();
}

// ============================================================
// renderInventoryPanel - 왼쪽 재고 패널
// ============================================================
void FactoryView::renderInventoryPanel()
{
    float panelHeight = WINDOW_HEIGHT - 230.0f;

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.11f, 0.11f, 0.17f, 1.0f));
    ImGui::BeginChild("##inventory", ImVec2(INVENTORY_WIDTH, panelHeight), true);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 0.70f, 1.0f));
    ImGui::Text("INVENTORY");
    ImGui::PopStyleColor();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    const Inventory& inv = model.getInventory();

    struct Item { const char* icon; const char* name; int count; };
    Item items[] = {
        { ICON_FA_BREAD_SLICE, "Buns",    inv.getBuns()    },
        { ICON_FA_BURGER,      "Patties", inv.getPatties() },
        { ICON_FA_LEAF,        "Lettuce", inv.getLettuce() },
        { ICON_FA_APPLE_WHOLE, "Tomato",  inv.getTomato()  },
        { ICON_FA_CHEESE,      "Cheese",  inv.getCheese()  },
        { ICON_FA_DROPLET,     "Sauce",   inv.getSauces()  },
    };

    for (auto& item : items)
    {
        ImGui::Text("%s %s", item.icon, item.name);

        char countStr[16];
        snprintf(countStr, sizeof(countStr), "%d", item.count);
        float textW = ImGui::CalcTextSize(countStr).x;
        ImGui::SameLine(INVENTORY_WIDTH - textW - 18.0f);

        ImVec4 countColor = (item.count <= 5)
            ? ImVec4(1.0f, 0.3f, 0.3f, 1.0f)
            : ImVec4(0.90f, 0.90f, 0.92f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, countColor);
        ImGui::Text("%s", countStr);
        ImGui::PopStyleColor();
        ImGui::Spacing();
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();
}

// ============================================================
// renderProductionLine - 오른쪽 생산 라인
// ============================================================
void FactoryView::renderProductionLine()
{
    float lineWidth = WINDOW_WIDTH - INVENTORY_WIDTH - 30.0f;

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::BeginChild("##prodline", ImVec2(lineWidth, CARD_HEIGHT + 30.0f), false);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 0.70f, 1.0f));
    ImGui::Text("PRODUCTION LINE");
    ImGui::PopStyleColor();
    ImGui::Spacing();

    const Machine* machines[4] = {
        &model.getPrepMachine(),
        &model.getGrillMachine(),
        &model.getSauceMachine(),
        &model.getPackingMachine()
    };

    for (int i = 0; i < 4; i++)
    {
        renderMachineCard(*machines[i], getMachineIcon(i), i, isMachineActiveStep(i));
        if (i < 3)
        {
            ImGui::SameLine(0.0f, 0.0f);
            renderArrow();
            ImGui::SameLine(0.0f, 0.0f);
        }
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();
}

// ============================================================
// renderMachineCard - 기계 카드 하나 렌더
// ============================================================
void FactoryView::renderMachineCard(const Machine& machine, const char* icon,
                                     int index, bool isActiveStep)
{
    ImVec4 cardColor   = isActiveStep ? colorCardActive : colorCard;
    ImVec4 borderColor = isActiveStep
        ? ImVec4(0.20f, 0.70f, 0.25f, 1.0f)
        : ImVec4(0.22f, 0.22f, 0.32f, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_ChildBg, cardColor);
    ImGui::PushStyleColor(ImGuiCol_Border,  borderColor);

    char childId[16];
    snprintf(childId, sizeof(childId), "##card%d", index);
    ImGui::BeginChild(childId, ImVec2(CARD_WIDTH, CARD_HEIGHT), true);

    // 기계 이름 (중앙 정렬)
    const char* displayName = getMachineDisplayName(index);
    float nameW = ImGui::CalcTextSize(displayName).x;
    ImGui::SetCursorPosX((CARD_WIDTH - nameW) * 0.5f);
    ImGui::Text("%s", displayName);

    // Font Awesome 큰 아이콘 (중앙)
    if (largeIconFont) ImGui::PushFont(largeIconFont);
    float iconW = ImGui::CalcTextSize(icon).x;
    ImGui::SetCursorPosX((CARD_WIDTH - iconW) * 0.5f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);

    // 활성 기계는 아이콘도 초록, 아니면 흰색
    ImVec4 iconColor = isActiveStep
        ? ImVec4(0.40f, 1.0f, 0.50f, 1.0f)
        : ImVec4(0.65f, 0.65f, 0.70f, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, iconColor);
    ImGui::Text("%s", icon);
    ImGui::PopStyleColor();
    if (largeIconFont) ImGui::PopFont();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);

    // 상태 표시 (● Running / ● Idle / ● Waiting)
    const char* statusText;
    ImVec4      statusColor;
    if      (machine.isPaused())               { statusText = "Paused";  statusColor = colorWaiting; }
    else if (machine.isRunning())              { statusText = "Running"; statusColor = colorRunning; }
    else if (isActiveStep && !machine.isDone()){ statusText = "Waiting"; statusColor = colorWaiting; }
    else                                       { statusText = "Idle";    statusColor = colorIdle;    }

    ImGui::PushStyleColor(ImGuiCol_Text, statusColor);
    float statusW = ImGui::CalcTextSize(statusText).x + ImGui::CalcTextSize(ICON_FA_CIRCLE "  ").x;
    ImGui::SetCursorPosX((CARD_WIDTH - statusW) * 0.5f);
    ImGui::Text(ICON_FA_CIRCLE "  %s", statusText);
    ImGui::PopStyleColor();

    // 프로그레스바
    float progress = machine.getProgress();
    char overlay[16];
    snprintf(overlay, sizeof(overlay), "%d%%", (int)(progress * 100.0f));

    ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
        machine.isRunning()
            ? ImVec4(0.20f, 0.75f, 0.30f, 1.0f)
            : ImVec4(0.30f, 0.30f, 0.40f, 1.0f));
    ImGui::ProgressBar(progress, ImVec2(CARD_WIDTH - 20.0f, 14.0f), overlay);
    ImGui::PopStyleColor();

    // 카드 클릭 → 상세 팝업
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
    {
        selectedMachine  = index;
        showMachineDetail = true;
    }

    ImGui::EndChild();
    ImGui::PopStyleColor(2);
}

// ============================================================
// renderArrow - 기계 사이 화살표
// ============================================================
void FactoryView::renderArrow()
{
    ImGui::BeginChild("##arr", ImVec2(28.0f, CARD_HEIGHT), false);
    ImGui::SetCursorPosY(CARD_HEIGHT * 0.5f - 8.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.45f, 0.45f, 0.55f, 1.0f));
    ImGui::Text(" ->");
    ImGui::PopStyleColor();
    ImGui::EndChild();
}

// ============================================================
// renderStatusBar - 하단 상태 + 버튼
// ============================================================
void FactoryView::renderStatusBar()
{
    float lineWidth = WINDOW_WIDTH - INVENTORY_WIDTH - 30.0f;

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 0.70f, 1.0f));
    ImGui::Text("CURRENT STATUS");
    ImGui::PopStyleColor();
    ImGui::Spacing();
    ImGui::Text("%s", model.getStatusMessage().c_str());
    ImGui::Spacing();

    float buttonWidth  = 130.0f;
    float rightX       = lineWidth - buttonWidth * 2.0f - 10.0f;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + rightX);

    // New Order (파란색)
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.15f, 0.40f, 0.85f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.50f, 0.95f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.10f, 0.30f, 0.75f, 1.0f));
    if (ImGui::Button("New Order", ImVec2(buttonWidth, 32.0f)))
        controller.newOrder();
    ImGui::PopStyleColor(3);

    ImGui::SameLine(0.0f, 8.0f);

    // Sell Burger (버거 없으면 비활성)
    bool canSell = (model.getReadyBurgers() > 0);
    if (!canSell) ImGui::BeginDisabled();

    char sellLabel[40];
    snprintf(sellLabel, sizeof(sellLabel), "Sell Burger (%d)", model.getReadyBurgers());
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.25f, 0.25f, 0.35f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.48f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.18f, 0.18f, 0.28f, 1.0f));
    if (ImGui::Button(sellLabel, ImVec2(buttonWidth, 32.0f)))
        controller.sellBurger();
    ImGui::PopStyleColor(3);

    if (!canSell) ImGui::EndDisabled();
}

// ============================================================
// renderMachineDetailPopup - 기계 상세 팝업
// ============================================================
void FactoryView::renderMachineDetailPopup()
{
    const Machine* machines[4] = {
        &model.getPrepMachine(),  &model.getGrillMachine(),
        &model.getSauceMachine(), &model.getPackingMachine()
    };
    const Machine& m = *machines[selectedMachine];

    ImGui::SetNextWindowSize(ImVec2(540.0f, 360.0f), ImGuiCond_Always);
    ImGui::SetNextWindowPos(
        ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f,
               ImGui::GetIO().DisplaySize.y * 0.5f),
        ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    char title[64];
    snprintf(title, sizeof(title), "MACHINE DETAIL - %s",
             getMachineDisplayName(selectedMachine));

    ImGui::Begin(title, &showMachineDetail,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

    // --- 왼쪽: 큰 아이콘 + 상태 ---
    ImGui::BeginChild("##dleft", ImVec2(160.0f, 280.0f), false);

    if (largeIconFont) ImGui::PushFont(largeIconFont);
    ImGui::SetCursorPosX(30.0f);
    ImGui::SetCursorPosY(15.0f);
    ImVec4 bigIconColor = m.isRunning()
        ? ImVec4(0.40f, 1.0f, 0.50f, 1.0f)
        : ImVec4(0.55f, 0.55f, 0.60f, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, bigIconColor);
    ImGui::Text("%s", getMachineIcon(selectedMachine));
    ImGui::PopStyleColor();
    if (largeIconFont) ImGui::PopFont();

    ImGui::Spacing(); ImGui::Spacing();

    bool running = m.isRunning();
    ImGui::PushStyleColor(ImGuiCol_Text, running ? colorRunning : colorIdle);
    ImGui::Text(running ? "Running" : (m.isPaused() ? "Paused" : "Idle"));
    ImGui::PopStyleColor();

    ImGui::Spacing();
    ImGui::Text("Progress");
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
        running ? ImVec4(0.20f, 0.75f, 0.30f, 1.0f)
                : ImVec4(0.30f, 0.30f, 0.40f, 1.0f));
    char ov[16]; snprintf(ov, sizeof(ov), "%d%%", (int)(m.getProgress() * 100.0f));
    ImGui::ProgressBar(m.getProgress(), ImVec2(140.0f, 14.0f), ov);
    ImGui::PopStyleColor();

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    ImGui::Text("Cycle Time");  ImGui::SameLine(95.0f); ImGui::Text("%.1f sec", m.getCycleTime());
    ImGui::Text("Remaining");   ImGui::SameLine(95.0f); ImGui::Text("%.1f sec", m.getRemainingTime());
    ImGui::Text("Produced");    ImGui::SameLine(95.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, colorBurger);
    ImGui::Text("%d", m.getProducedCount());
    ImGui::PopStyleColor();

    ImGui::EndChild();
    ImGui::SameLine(0.0f, 10.0f);

    // --- 가운데: Input / Output / Controls ---
    ImGui::BeginChild("##dmid", ImVec2(150.0f, 280.0f), false);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 0.70f, 1.0f));
    ImGui::Text("INPUT");
    ImGui::PopStyleColor();
    ImGui::Separator(); ImGui::Spacing();

    const char* inputNames[]  = { "Buns + Veggies", "Patties", "Grilled Patties", "Assembled Burger" };
    const int   inputCounts[] = {
        model.getInventory().getBuns(),
        model.getInventory().getPatties(),
        model.getGrillMachine().getProducedCount(),
        model.getSauceMachine().getProducedCount()
    };
    ImGui::Text("%s", inputNames[selectedMachine]);
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, colorRunning);
    ImGui::Text("%d", inputCounts[selectedMachine]);
    ImGui::PopStyleColor();

    ImGui::Spacing(); ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 0.70f, 1.0f));
    ImGui::Text("OUTPUT");
    ImGui::PopStyleColor();
    ImGui::Separator(); ImGui::Spacing();

    const char* outputNames[] = { "Prepared Ingr.", "Grilled Patties", "Sauced Burger", "Packed Burger" };
    ImGui::Text("%s", outputNames[selectedMachine]);
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, colorBurger);
    ImGui::Text("%d", m.getProducedCount());
    ImGui::PopStyleColor();

    ImGui::Spacing(); ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 0.70f, 1.0f));
    ImGui::Text("CONTROLS");
    ImGui::PopStyleColor();
    ImGui::Separator(); ImGui::Spacing();

    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.75f, 0.15f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.20f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.60f, 0.10f, 0.10f, 1.0f));
    if (ImGui::Button("Stop", ImVec2(60.0f, 28.0f)))
        controller.stopFactory();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    const char* pauseLabel = m.isPaused() ? "Resume" : "Pause";
    if (ImGui::Button(pauseLabel, ImVec2(65.0f, 28.0f)))
        controller.togglePauseMachine(selectedMachine);

    ImGui::EndChild();
    ImGui::SameLine(0.0f, 10.0f);

    // --- 오른쪽: 업그레이드 ---
    ImGui::BeginChild("##dright", ImVec2(145.0f, 280.0f), false);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 0.70f, 1.0f));
    ImGui::Text("UPGRADES");
    ImGui::PopStyleColor();
    ImGui::Separator(); ImGui::Spacing();

    int  speedCost  = m.getSpeedUpgradeCost();
    int  speedLevel = m.getUpgradeLevel();
    char speedLabel[32];
    snprintf(speedLabel, sizeof(speedLabel), "Grill Speed Lv.%d", speedLevel);

    ImGui::Text("%s", speedLabel);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.55f, 0.55f, 0.60f, 1.0f));
    ImGui::Text("-20%% cook time");
    ImGui::PopStyleColor();

    bool canAfford = (model.getMoney() >= speedCost);
    if (!canAfford) ImGui::BeginDisabled();
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.15f, 0.40f, 0.85f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.50f, 0.95f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.10f, 0.30f, 0.75f, 1.0f));
    if (ImGui::Button("Upgrade##spd", ImVec2(100.0f, 26.0f)))
    {
        switch (selectedMachine)
        {
        case 0: controller.upgradePrepSpeed();  break;
        case 1: controller.upgradeGrillSpeed(); break;
        case 2: controller.upgradeSauceSpeed(); break;
        case 3: controller.upgradePackSpeed();  break;
        }
    }
    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, colorMoney);
    ImGui::Text("$%d", speedCost);
    ImGui::PopStyleColor();
    if (!canAfford) ImGui::EndDisabled();

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 0.70f, 1.0f));
    ImGui::Text("Capacity Lv.1");
    ImGui::Text("+20 per cycle");
    ImGui::PopStyleColor();
    ImGui::BeginDisabled();
    ImGui::Button("Upgrade##cap", ImVec2(100.0f, 26.0f));
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, colorMoney);
    ImGui::Text("$400");
    ImGui::PopStyleColor();

    ImGui::Spacing();

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 0.70f, 1.0f));
    ImGui::Text("Quality Lv.1");
    ImGui::Text("+5%% reputation");
    ImGui::PopStyleColor();
    ImGui::BeginDisabled();
    ImGui::Button("Upgrade##qual", ImVec2(100.0f, 26.0f));
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, colorMoney);
    ImGui::Text("$300");
    ImGui::PopStyleColor();

    ImGui::EndChild();
    ImGui::End();
}

// ============================================================
// 헬퍼 함수들
// ============================================================
bool FactoryView::isMachineActiveStep(int index) const
{
    ProcessStep step = model.getCurrentStep();
    switch (index)
    {
    case 0: return step == ProcessStep::Preparing;
    case 1: return step == ProcessStep::Grilling;
    case 2: return step == ProcessStep::Saucing;
    case 3: return step == ProcessStep::Packing;
    default: return false;
    }
}

const char* FactoryView::getMachineIcon(int index) const
{
    static const char* icons[4] = {
        ICON_FA_UTENSILS,   // Prep
        ICON_FA_FIRE,       // Grill
        ICON_FA_FLASK,      // Sauce
        ICON_FA_BOX_OPEN    // Packing
    };
    return (index >= 0 && index < 4) ? icons[index] : "?";
}

const char* FactoryView::getMachineDisplayName(int index) const
{
    static const char* names[4] = {
        "Prep Machine", "Patty Grill", "Sauce Dispenser", "Packing Machine"
    };
    return (index >= 0 && index < 4) ? names[index] : "Unknown";
}
