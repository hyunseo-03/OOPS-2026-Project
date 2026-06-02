#include "view/FactoryView.h"

FactoryView::FactoryView(BurgerFactoryModel& model, FactoryController& controller)
    : model(model), controller(controller) {}

const char* FactoryView::stepToString(ProcessStep step) const
{
    switch (step)
    {
        case ProcessStep::Idle:                  return "Idle";
        case ProcessStep::PreparingIngredients:  return "Preparing...";
        case ProcessStep::GrillPatty:            return "Grilling";
        case ProcessStep::AddSauce:              return "Sauce";
        case ProcessStep::AssembleBurger:        return "Assembly";
        case ProcessStep::QualityCheck:          return "Quality Check";
        case ProcessStep::PackBurger:            return "Packing";
        case ProcessStep::Done:                  return "Done!";
        default:                                 return "Unknown";
    }
}

static const char* burgerName(BurgerType t)
{
    return t == BurgerType::CLASSIC ? "Classic" :
           t == BurgerType::CHEESE  ? "Cheese"  : "Double";
}
static int burgerPrice(BurgerType t)
{
    return t == BurgerType::CLASSIC ? 100 :
           t == BurgerType::CHEESE  ? 130 : 160;
}

void FactoryView::render()
{
    ImGuiIO& io    = ImGui::GetIO();
    float    W     = io.DisplaySize.x;
    float    H     = io.DisplaySize.y;
    float    leftW = W * 0.65f;
    float    rightW = W - leftW;

    // ══════════════════════════════════════════════════
    // 왼쪽: 공장 창
    // ══════════════════════════════════════════════════
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(leftW, H), ImGuiCond_Always);
    ImGui::Begin("Burger Factory", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // 상태 바
    ImGui::Text("Money: $%d", model.getMoney());
    ImGui::SameLine(180);
    ImGui::Text("Sold: %d", model.getTotalBurgersProduced());
    ImGui::Separator();

    ProcessStep cur = model.getCurrentStep();
    ImGui::Text("Step: %s", stepToString(cur));
    if (model.isCurrentMachineFailed())
        ImGui::TextColored(ImVec4(1,0.2f,0.2f,1), "  !! MACHINE FAILED !!");
    else if (model.isCurrentMachinePaused())
        ImGui::TextColored(ImVec4(1,1,0,1), "  -- PAUSED --");
    ImGui::Separator();

    // 주문 버튼
    ImGui::Text("[ORDER]");
    if (model.hasOrder())
    {
        const Order& o = model.getCurrentOrder();
        ImGui::TextColored(ImVec4(1,1,0.3f,1), "  Making: %s Burger", burgerName(o.type));
    }
    else ImGui::TextDisabled("  No active order");

    if (ImGui::Button("Classic ($100)")) controller.onNewOrder(BurgerType::CLASSIC);
    ImGui::SameLine();
    if (ImGui::Button("Cheese ($130)"))  controller.onNewOrder(BurgerType::CHEESE);
    ImGui::SameLine();
    if (ImGui::Button("Double ($160)"))  controller.onNewOrder(BurgerType::DOUBLE);
    ImGui::Separator();

    // 기계 목록
    ImGui::Text("[MACHINES]");
    struct MachineRow { const char* label; ProcessStep step; };
    MachineRow rows[] = {
        { "1. Prep    ", ProcessStep::PreparingIngredients },
        { "2. Grill   ", ProcessStep::GrillPatty           },
        { "3. Sauce   ", ProcessStep::AddSauce             },
        { "4. Assembly", ProcessStep::AssembleBurger       },
        { "5. Quality ", ProcessStep::QualityCheck         },
        { "6. Packing ", ProcessStep::PackBurger           },
    };

    for (auto& row : rows)
    {
        Machine* m = model.getMachine(row.step);
        if (!m) continue;

        bool isActive = (cur == row.step);
        if (isActive) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f,1.0f,0.3f,1));
        ImGui::Text("%s", row.label);
        if (isActive) ImGui::PopStyleColor();

        ImGui::SameLine(120);

        if (m->isFailed())
        {
            ImGui::TextColored(ImVec4(1,0.2f,0.2f,1), "[FAILED]");
            ImGui::SameLine();
            char lbl[32];
            snprintf(lbl, sizeof(lbl), "Repair $%d##r%d", BurgerFactoryModel::REPAIR_COST, (int)row.step);
            if (ImGui::Button(lbl)) controller.onRepairMachine(row.step);
        }
        else
        {
            char overlay[16];
            snprintf(overlay, sizeof(overlay), "%.0f%%", m->getProgress() * 100.0f);
            ImGui::ProgressBar(m->getProgress(), ImVec2(160, 14), overlay);

            if (isActive && (m->isRunning() || m->isPaused()))
            {
                ImGui::SameLine();
                char lbl[32];
                snprintf(lbl, sizeof(lbl), m->isPaused() ? "Resume##p%d" : "Pause##p%d", (int)row.step);
                if (ImGui::Button(lbl)) controller.onTogglePause(row.step);
            }
        }
    }
    ImGui::Separator();

    // 제어
    ImGui::Text("[CONTROLS]");
    // Start 버튼은 PreparingIngredients 단계에서만 의미있음
    bool atPrepStep = (cur == ProcessStep::PreparingIngredients);
    bool canStart   = atPrepStep && model.canProceed(cur);
    if (!canStart) ImGui::BeginDisabled();
    if (ImGui::Button("Start Production"))
        controller.onStartMachine(ProcessStep::PreparingIngredients);
    if (!canStart) ImGui::EndDisabled();

    ImGui::SameLine();
    if (ImGui::Button("Refill ($100)")) controller.onRefillInventory();

    // 디버그: 현재 상태 표시
    ImGui::SameLine(0, 20);
    if (!canStart && atPrepStep)
        ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "(need order + ingredients)");
    else if (!atPrepStep && cur != ProcessStep::Idle)
        ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1), "(auto-running)");
    ImGui::Separator();

    // 재고
    ImGui::Text("[INVENTORY]");
    struct IngRow { const char* name; IngredientType type; };
    IngRow ings[] = {
        { "Bun",     IngredientType::BUN     },
        { "Patty",   IngredientType::PATTY   },
        { "Cheese",  IngredientType::CHEESE  },
        { "Lettuce", IngredientType::LETTUCE },
        { "Tomato",  IngredientType::TOMATO  },
        { "Onion",   IngredientType::ONION   },
        { "Sauce",   IngredientType::SAUCE   },
    };

    for (int i = 0; i < 7; i++)
    {
        int amt = model.getIngredientAmount(ings[i].type);
        bool low = (amt <= 5);
        if (low) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,0.3f,0.3f,1));
        ImGui::Text("  %-7s %2d", ings[i].name, amt);
        if (low) ImGui::PopStyleColor();
        // 짝수 인덱스면 SameLine으로 옆에 배치
        if (i % 2 == 0) ImGui::SameLine(190);
    }

    if (cur == ProcessStep::Idle)
    {
        ImGui::Separator();
        if (model.isQualityCheckPassed())
            ImGui::TextColored(ImVec4(0.3f,1,0.3f,1), "Last QC: PASSED");
        else
            ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "Last QC: FAILED");
    }

    ImGui::End();

    // ══════════════════════════════════════════════════
    // 오른쪽: 주문 내역 창
    // ══════════════════════════════════════════════════
    ImGui::SetNextWindowPos(ImVec2(leftW, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(rightW, H), ImGuiCond_Always);
    ImGui::Begin("Orders", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // ── 현재 생산 중 ──────────────────────────────
    ImGui::TextColored(ImVec4(1,1,0.3f,1), "[In Production]");
    ImGui::Separator();
    if (model.hasOrder())
    {
        const Order& o = model.getCurrentOrder();
        ImGui::Text("  %s  $%d  (%s)",
            burgerName(o.type), burgerPrice(o.type), stepToString(cur));
    }
    else ImGui::TextDisabled("  None");
    ImGui::Spacing();

    // ── 대기 주문 ──────────────────────────────────
    const auto& queued = model.getQueuedOrders();
    ImGui::TextColored(ImVec4(0.8f,0.6f,1,1), "[Waiting (%d)]", (int)queued.size());
    ImGui::Separator();
    if (queued.empty()) ImGui::TextDisabled("  None");
    else
        for (int i = 0; i < (int)queued.size(); i++)
            ImGui::Text("  %d. %s  $%d", i+1,
                burgerName(queued[i].type), burgerPrice(queued[i].type));
    ImGui::Spacing();

    // ── 판매 완료 ──────────────────────────────────
    const auto& done = model.getCompletedOrders();
    ImGui::TextColored(ImVec4(0.5f,0.8f,1,1), "[Sold (%d)]", (int)done.size());
    ImGui::Separator();
    if (done.empty()) ImGui::TextDisabled("  None yet");
    else
    {
        ImGui::BeginChild("SoldList", ImVec2(0, 0), false);
        for (int i = (int)done.size()-1; i >= 0; i--)
            ImGui::Text("  #%d  %s  +$%d", i+1,
                burgerName(done[i].type), burgerPrice(done[i].type));
        ImGui::EndChild();
    }

    ImGui::End();
}

void FactoryView::renderOrderHistory() {}
