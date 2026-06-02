#include "view/FactoryView.h"

FactoryView::FactoryView(BurgerFactoryModel& model, FactoryController& controller)
    : model(model), controller(controller) {}

const char* FactoryView::stepToString(ProcessStep step) const
{
    switch (step)
    {
        case ProcessStep::Idle:                  return "Idle";
        case ProcessStep::PreparingIngredients:  return "Preparing Ingredients";
        case ProcessStep::GrillPatty:            return "Grilling Patty";
        case ProcessStep::AddSauce:              return "Adding Sauce";
        case ProcessStep::AssembleBurger:        return "Assembling Burger";
        case ProcessStep::QualityCheck:          return "Quality Check";
        case ProcessStep::PackBurger:            return "Packing";
        case ProcessStep::Done:                  return "Done";
        default:                                 return "Unknown";
    }
}

void FactoryView::render()
{
    ImGuiIO& io   = ImGui::GetIO();
    float    W    = io.DisplaySize.x;
    float    H    = io.DisplaySize.y;
    float    left = W * 0.65f;   // 왼쪽 공장 창 너비
    float    right = W - left;   // 오른쪽 주문 창 너비

    // ═══════════════════════════════════════════
    // 왼쪽: 공장 메인 창
    // ═══════════════════════════════════════════
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(left, H), ImGuiCond_Always);
    ImGui::Begin("Burger Factory", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("=== BURGER FACTORY ===");
    ImGui::Separator();
    ImGui::Text("Money : $%d", model.getMoney());
    ImGui::SameLine(200);
    ImGui::Text("Produced : %d", model.getTotalBurgersProduced());
    ImGui::Separator();

    // 현재 공정 단계
    ProcessStep cur = model.getCurrentStep();
    ImGui::Text("Step : %s", stepToString(cur));

    if (model.isCurrentMachineFailed())
        ImGui::TextColored(ImVec4(1,0,0,1), "!! MACHINE FAILED !!");
    else if (model.isCurrentMachinePaused())
        ImGui::TextColored(ImVec4(1,1,0,1), "-- PAUSED --");

    ImGui::Separator();

    // 주문 버튼
    ImGui::Text("[ORDER]");
    if (ImGui::Button("Classic ($100)")) controller.onNewOrder(BurgerType::CLASSIC);
    ImGui::SameLine();
    if (ImGui::Button("Cheese ($130)"))  controller.onNewOrder(BurgerType::CHEESE);
    ImGui::SameLine();
    if (ImGui::Button("Double ($160)"))  controller.onNewOrder(BurgerType::DOUBLE);
    ImGui::Separator();

    // 기계 목록
    ImGui::Text("[MACHINES]");

    struct MachineRow { const char* name; ProcessStep step; };
    MachineRow rows[] = {
        { "1. Prep      ", ProcessStep::PreparingIngredients },
        { "2. Grill     ", ProcessStep::GrillPatty           },
        { "3. Sauce     ", ProcessStep::AddSauce             },
        { "4. Assembly  ", ProcessStep::AssembleBurger       },
        { "5. Quality   ", ProcessStep::QualityCheck         },
        { "6. Packing   ", ProcessStep::PackBurger           },
    };

    for (auto& row : rows)
    {
        Machine* m = model.getMachine(row.step);
        if (!m) continue;

        bool isActive = (cur == row.step);
        if (isActive) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 1.0f, 0.3f, 1.0f));
        ImGui::Text("%s", row.name);
        if (isActive) ImGui::PopStyleColor();

        ImGui::SameLine(130);

        if (m->isFailed())
        {
            ImGui::TextColored(ImVec4(1,0,0,1), "[FAILED]");
            ImGui::SameLine();
            char lbl[32];
            snprintf(lbl, sizeof(lbl), "Repair $%d##%d", BurgerFactoryModel::REPAIR_COST, (int)row.step);
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
                snprintf(lbl, sizeof(lbl), m->isPaused() ? "Resume##%d" : "Pause##%d", (int)row.step);
                if (ImGui::Button(lbl)) controller.onTogglePause(row.step);
            }
        }
    }
    ImGui::Separator();

    // 제어 버튼
    ImGui::Text("[CONTROLS]");
    bool canStart = model.canProceed(cur);
    if (!canStart) ImGui::BeginDisabled();
    if (ImGui::Button("Start Production")) controller.onStartMachine(cur);
    if (!canStart) ImGui::EndDisabled();

    ImGui::SameLine();
    if (ImGui::Button("Refill Inventory")) controller.onRefillInventory();
    ImGui::Separator();

    // 재고
    ImGui::Text("[INVENTORY]");
    struct IngRow { const char* name; IngredientType type; };
    IngRow ings[] = {
        { "Bun",    IngredientType::BUN     },
        { "Patty",  IngredientType::PATTY   },
        { "Cheese", IngredientType::CHEESE  },
        { "Lettuce",IngredientType::LETTUCE },
        { "Tomato", IngredientType::TOMATO  },
        { "Sauce",  IngredientType::SAUCE   },
    };

    for (int i = 0; i < 6; i++)
    {
        int amt = model.getIngredientAmount(ings[i].type);
        bool low = (amt <= 5);
        if (low) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.3f, 0.3f, 1));
        // 진행률 바 형태로 재고 표시 (max=20)
        char label[32];
        snprintf(label, sizeof(label), "%s: %d", ings[i].name, amt);
        ImGui::ProgressBar(amt / 20.0f, ImVec2(150, 14), label);
        if (low) ImGui::PopStyleColor();
        if (i % 2 == 0) ImGui::SameLine(200);
    }

    // QualityCheck 결과
    if (cur == ProcessStep::Idle)
    {
        ImGui::Separator();
        if (model.isQualityCheckPassed())
            ImGui::TextColored(ImVec4(0.3f,1,0.3f,1), "Quality Check: PASSED");
        else
            ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "Quality Check: FAILED");
    }

    ImGui::End();

    // ═══════════════════════════════════════════
    // 오른쪽: 주문 내역 창
    // ═══════════════════════════════════════════
    ImGui::SetNextWindowPos(ImVec2(left, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(right, H), ImGuiCond_Always);
    ImGui::Begin("Orders", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    auto burgerName  = [](BurgerType t) -> const char* {
        return t == BurgerType::CLASSIC ? "Classic" :
               t == BurgerType::CHEESE  ? "Cheese"  : "Double";
    };
    auto burgerPrice = [](BurgerType t) -> int {
        return t == BurgerType::CLASSIC ? 100 :
               t == BurgerType::CHEESE  ? 130 : 160;
    };

    // ── 현재 생산 중인 주문 ──────────────────
    ImGui::TextColored(ImVec4(1.0f,1.0f,0.3f,1), "[In Production]");
    ImGui::Separator();
    if (model.hasOrder())
    {
        const Order& o = model.getCurrentOrder();
        ImGui::Text("  %s Burger  $%d", burgerName(o.type), burgerPrice(o.type));
        ImGui::TextDisabled("  Step: %s", stepToString(cur));
    }
    else if (!model.hasOrder())
    {
        ImGui::TextDisabled("  None");
    }
    ImGui::Spacing();

    // ── 3. 대기 중인 주문 ──────────────────────
    const auto& queued = model.getQueuedOrders();
    ImGui::TextColored(ImVec4(0.8f,0.6f,1.0f,1), "[Waiting (%d)]", (int)queued.size());
    ImGui::Separator();
    if (queued.empty())
    {
        ImGui::TextDisabled("  No queued orders");
    }
    else
    {
        for (int i = 0; i < (int)queued.size(); i++)
            ImGui::Text("  %d. %s  $%d", i+1,
                burgerName(queued[i].type), burgerPrice(queued[i].type));
    }
    ImGui::Spacing();

    // ── 4. 판매 완료 목록 ──────────────────────
    const auto& done = model.getCompletedOrders();
    ImGui::TextColored(ImVec4(0.6f,0.8f,1.0f,1), "[Sold (%d)]", (int)done.size());
    ImGui::Separator();
    if (done.empty())
    {
        ImGui::TextDisabled("  None yet.");
    }
    else
    {
        ImGui::BeginChild("SoldScroll", ImVec2(0, 0), false);
        for (int i = (int)done.size() - 1; i >= 0; i--)
            ImGui::Text("  #%d  %-8s  +$%d", i+1,
                burgerName(done[i].type), burgerPrice(done[i].type));
        ImGui::EndChild();
    }

    ImGui::End();
}

void FactoryView::renderOrderHistory() {}  // render()에 통합됨
