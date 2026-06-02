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
        case ProcessStep::Done:                  return "Done - Press Pack!";
        default:                                 return "Unknown";
    }
}

void FactoryView::render()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    ImGui::Begin("Burger Factory", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

    // ─────────────────────────────────────────────
    // 상단: 기본 정보
    // ─────────────────────────────────────────────
    ImGui::Text("=== BURGER FACTORY ===");
    ImGui::Separator();
    ImGui::Text("Money : $%d",  model.getMoney());
    ImGui::SameLine(200);
    ImGui::Text("Produced : %d", model.getTotalBurgersProduced());
    ImGui::Separator();

    // ─────────────────────────────────────────────
    // 현재 공정 단계
    // ─────────────────────────────────────────────
    ImGui::Text("Current Step : %s", stepToString(model.getCurrentStep()));

    if (model.isCurrentMachineFailed())
        ImGui::TextColored(ImVec4(1,0,0,1), "!! MACHINE FAILED !!");
    else if (model.isCurrentMachinePaused())
        ImGui::TextColored(ImVec4(1,1,0,1), "-- PAUSED --");

    ImGui::Separator();

    // ─────────────────────────────────────────────
    // 현재 주문 정보
    // ─────────────────────────────────────────────
    ImGui::Text("[ORDER]");
    if (model.hasOrder())
    {
        const Order& order = model.getCurrentOrder();
        const char* burgerName = (order.type == BurgerType::CLASSIC) ? "Classic" :
                                 (order.type == BurgerType::CHEESE)  ? "Cheese"  : "Double";
        ImGui::Text("  Current : %s Burger", burgerName);
    }
    else
    {
        ImGui::TextDisabled("  No active order");
    }

    // 주문 버튼
    if (ImGui::Button("Order Classic ($100)")) controller.onNewOrder(BurgerType::CLASSIC);
    ImGui::SameLine();
    if (ImGui::Button("Order Cheese ($130)"))  controller.onNewOrder(BurgerType::CHEESE);
    ImGui::SameLine();
    if (ImGui::Button("Order Double ($160)"))  controller.onNewOrder(BurgerType::DOUBLE);
    ImGui::Separator();

    // ─────────────────────────────────────────────
    // 기계 목록 + 진행률
    // ─────────────────────────────────────────────
    ImGui::Text("[MACHINES]");

    struct MachineRow { const char* name; ProcessStep step; };
    MachineRow rows[] = {
        { "1. Prep Machine  ", ProcessStep::PreparingIngredients },
        { "2. Grill Machine ", ProcessStep::GrillPatty           },
        { "3. Sauce Machine ", ProcessStep::AddSauce             },
        { "4. Assembly      ", ProcessStep::AssembleBurger       },
        { "5. Quality Check ", ProcessStep::QualityCheck         },
        { "6. Packing       ", ProcessStep::PackBurger           },
    };

    ProcessStep cur = model.getCurrentStep();

    for (auto& row : rows)
    {
        Machine* m = model.getMachine(row.step);
        if (!m) continue;

        // 현재 단계 강조
        bool isActive = (cur == row.step);
        if (isActive) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 1.0f, 0.3f, 1.0f));

        ImGui::Text("%s", row.name);
        if (isActive) ImGui::PopStyleColor();

        ImGui::SameLine(170);

        // 고장 / 일시정지 / 작동 / 완료 상태
        if (m->isFailed())
        {
            ImGui::TextColored(ImVec4(1,0,0,1), "[FAILED]");
            ImGui::SameLine();
            char repairLabel[32];
            snprintf(repairLabel, sizeof(repairLabel), "Repair $%d##%d", BurgerFactoryModel::REPAIR_COST, (int)row.step);
            if (ImGui::Button(repairLabel))
                controller.onRepairMachine(row.step);
        }
        else
        {
            // 진행률 바
            char overlay[16];
            snprintf(overlay, sizeof(overlay), "%.0f%%", m->getProgress() * 100.0f);
            ImGui::ProgressBar(m->getProgress(), ImVec2(180, 14), overlay);

            // 일시정지 / 재개 버튼 (작동 중이거나 일시정지 중일 때만)
            if (isActive && (m->isRunning() || m->isPaused()))
            {
                ImGui::SameLine();
                char pauseLabel[32];
                snprintf(pauseLabel, sizeof(pauseLabel),
                    m->isPaused() ? "Resume##%d" : "Pause##%d", (int)row.step);
                if (ImGui::Button(pauseLabel))
                    controller.onTogglePause(row.step);
            }
        }
    }
    ImGui::Separator();

    // ─────────────────────────────────────────────
    // 제어 버튼
    // ─────────────────────────────────────────────
    ImGui::Text("[CONTROLS]");

    // 생산 완료 → 판매 버튼 강조 표시
    if (cur == ProcessStep::Done)
    {
        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.1f, 0.7f, 0.1f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.9f, 0.2f, 1.0f));
        if (ImGui::Button(">>> SELL BURGER <<<"))
            controller.onPackBurger();
        ImGui::PopStyleColor(2);
    }
    else
    {
        // 생산 시작 버튼
        bool canStart = model.canProceed(cur);
        if (!canStart) ImGui::BeginDisabled();
        if (ImGui::Button("Start Production"))
            controller.onStartMachine(cur);
        if (!canStart) ImGui::EndDisabled();
    }

    ImGui::SameLine();

    if (ImGui::Button("Refill Inventory"))
        controller.onRefillInventory();

    ImGui::Separator();

    // ─────────────────────────────────────────────
    // 재고 현황
    // ─────────────────────────────────────────────
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
        if (amt <= 5) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,0.4f,0.4f,1));
        ImGui::Text("  %-8s : %d", ings[i].name, amt);
        if (amt <= 5) ImGui::PopStyleColor();
        if (i < 6) ImGui::SameLine(160);
        else ImGui::SameLine(0);
        i++;
        if (i < 7)
        {
            int amt2 = model.getIngredientAmount(ings[i].type);
            if (amt2 <= 5) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,0.4f,0.4f,1));
            ImGui::Text("  %-8s : %d", ings[i].name, amt2);
            if (amt2 <= 5) ImGui::PopStyleColor();
        }
    }

    // QualityCheck 결과 표시
    if (cur == ProcessStep::Done || cur == ProcessStep::Idle)
    {
        ImGui::Separator();
        if (model.isQualityCheckPassed())
            ImGui::TextColored(ImVec4(0.3f,1,0.3f,1), "Quality Check: PASSED");
        else
            ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "Quality Check: FAILED (burger discarded)");
    }

    ImGui::End();
}
