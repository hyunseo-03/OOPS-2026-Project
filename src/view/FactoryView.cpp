#include "view/FactoryView.h"
#include "imgui.h"
#include <string>

FactoryView::FactoryView(BurgerFactoryModel& model, FactoryController& controller)
    : model(model), controller(controller) {}

const char* FactoryView::stepToString(ProcessStep step) const
{
    switch (step)
    {
        case ProcessStep::Idle:                 return "Idle";
        case ProcessStep::PreparingIngredients: return "Preparing Ingredients";
        case ProcessStep::GrillPatty:           return "Grilling Patty";
        case ProcessStep::AddSauce:             return "Adding Sauce";
        case ProcessStep::AssembleBurger:       return "Assembling Burger";
        case ProcessStep::QualityCheck:         return "Quality Check";
        case ProcessStep::PackBurger:           return "Packing";
        case ProcessStep::Done:                 return "Done";
        default:                                return "Unknown";
    }
}

void FactoryView::render()
{
    ImGuiIO& io = ImGui::GetIO();
    const float W = io.DisplaySize.x;
    const float H = io.DisplaySize.y;

    const ImVec4 bg        = ImVec4(0.045f, 0.060f, 0.075f, 1.0f);
    const ImVec4 panel     = ImVec4(0.075f, 0.100f, 0.125f, 1.0f);
    const ImVec4 panel2    = ImVec4(0.095f, 0.125f, 0.155f, 1.0f);
    const ImVec4 border    = ImVec4(0.140f, 0.180f, 0.220f, 1.0f);
    const ImVec4 text      = ImVec4(0.920f, 0.940f, 0.960f, 1.0f);
    const ImVec4 muted     = ImVec4(0.540f, 0.590f, 0.640f, 1.0f);
    const ImVec4 green     = ImVec4(0.230f, 0.820f, 0.360f, 1.0f);
    const ImVec4 blue      = ImVec4(0.320f, 0.620f, 1.000f, 1.0f);
    const ImVec4 yellow    = ImVec4(1.000f, 0.780f, 0.220f, 1.0f);
    const ImVec4 red       = ImVec4(1.000f, 0.300f, 0.260f, 1.0f);

    auto burgerName = [](BurgerType type) -> const char* {
        return type == BurgerType::CLASSIC ? "Classic" :
               type == BurgerType::CHEESE  ? "Cheese"  : "Double";
    };

    auto burgerPrice = [](BurgerType type) -> int {
        return type == BurgerType::CLASSIC ? 100 :
               type == BurgerType::CHEESE  ? 130 : 160;
    };

    auto progress01 = [](float value) -> float {
        if (value < 0.0f) return 0.0f;
        if (value > 1.0f) return 1.0f;
        return value;
    };

    auto ingredientProgress = [](int amount) -> float {
        float value = amount / 60.0f;
        if (value < 0.0f) return 0.0f;
        if (value > 1.0f) return 1.0f;
        return value;
    };

    auto machineColor = [&](Machine* machine, bool active) -> ImVec4 {
        if (machine && machine->isFailed()) return red;
        if (machine && machine->isPaused()) return yellow;
        if (active && machine && machine->isRunning()) return green;
        if (active) return blue;
        return muted;
    };

    auto machineState = [](Machine* machine, bool active) -> const char* {
        if (machine && machine->isFailed()) return "Repair";
        if (machine && machine->isPaused()) return "Paused";
        if (active && machine && machine->isRunning()) return "Running";
        if (active) return "Waiting";
        return "Idle";
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
            case ProcessStep::AddSauce:             return "Adding sauce...";
            case ProcessStep::AssembleBurger:       return "Assembling burger...";
            case ProcessStep::QualityCheck:         return "Checking quality...";
            case ProcessStep::PackBurger:           return "Packing order...";
            case ProcessStep::Done:                 return "Burger completed.";
            default:                                return "Factory is ready.";
        }
    };

    auto drawStatCard = [&](const char* id, const char* title, const ImVec4& valueColor, int kind) {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, panel2);
        ImGui::PushStyleColor(ImGuiCol_Border, border);
        ImGui::BeginChild(id, ImVec2(0.0f, 74.0f), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::TextColored(muted, "%s", title);
        ImGui::Spacing();

        if (kind == 0)
        {
            ImGui::TextColored(valueColor, "$%d", model.getMoney());
        }
        else if (kind == 1)
        {
            ImGui::TextColored(valueColor, "%d", model.getTotalBurgersProduced());
        }
        else if (kind == 2)
        {
            const int activeOrders = (int)model.getQueuedOrders().size() + (model.hasOrder() ? 1 : 0);
            ImGui::TextColored(valueColor, "%d / 10", activeOrders);
        }
        else // Reputation (kind == 3)
        {
            // 백엔드에서 평균 평판 가져오기 (예: 3.8 등)
            // OrderManager에서 getAverageReputation()을 가져오도록 모델에 함수가 있다고 가정
            // (없다면 model.getAverageReputation()으로 연결해 주세요)
            float repScore = model.getAverageReputation(); 
            int repInt = (int)(repScore + 0.5f); // 반올림해서 별 개수 결정

            for (int i = 0; i < 5; ++i)
            {
                ImGui::TextColored(i < repInt ? yellow : muted, "*");
                if (i < 4) ImGui::SameLine();
            }
            ImGui::SameLine();
            ImGui::TextDisabled("(%.1f)", repScore); // 숫자도 옆에 작게 표시
        }

        ImGui::EndChild();
        ImGui::PopStyleColor(2);
    };

    auto drawInventoryRow = [&](const char* name, const char* code, IngredientType type) {
        const int amount = model.getIngredientAmount(type);
        const bool low = amount <= 5;

        ImGui::PushID(name);
        ImGui::TextColored(low ? red : text, "%s", code);
        ImGui::SameLine(56.0f);
        ImGui::TextColored(low ? red : text, "%s", name);
        ImGui::SameLine();
        ImGui::TextColored(low ? red : muted, "%d", amount);

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, low ? red : green);
        ImGui::ProgressBar(ingredientProgress(amount), ImVec2(-1.0f, 7.0f), "");
        ImGui::PopStyleColor();
        ImGui::Spacing();
        ImGui::PopID();
    };

    // 추가: 팝업을 띄우기 위해 저장할 변수
    static ProcessStep machineToUpgrade = ProcessStep::Idle;
    static bool showUpgradePopup = false;

    auto drawMachineCard = [&](const char* title, const char* shortName, ProcessStep step) {
        Machine* machine = model.getMachine(step);
        const bool active = model.getCurrentStep() == step;
        const ImVec4 stateColor = machineColor(machine, active);

        ImGui::PushID(shortName);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, active ? ImVec4(0.095f, 0.155f, 0.145f, 1.0f) : panel2);
        ImGui::PushStyleColor(ImGuiCol_Border, machine && machine->isFailed() ? red : (active ? green : border));
        
        // 카드를 클릭 가능하게 만들기 위한 꼼수 (투명한 버튼을 배경에 덮음)
        ImGui::BeginChild("MachineCard", ImVec2(0.0f, 180.0f), true, ImGuiWindowFlags_NoScrollbar); // 높이 조금 증가

        // 카드 상단: 이름과 레벨 표시
        ImGui::TextWrapped("%s", title);
        if (machine) {
            ImGui::SameLine();
            ImGui::TextColored(blue, "[Lv.%d]", machine->getLevel());
        }
        ImGui::Spacing();
        
        ImGui::TextColored(stateColor, "%s", shortName);
        ImGui::Spacing();

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 dotPos = ImGui::GetCursorScreenPos();
        drawList->AddCircleFilled(ImVec2(dotPos.x + 5.0f, dotPos.y + 8.0f), 4.0f, ImGui::GetColorU32(stateColor));
        ImGui::Dummy(ImVec2(14.0f, 16.0f));
        ImGui::SameLine();
        ImGui::TextDisabled("%s", machineState(machine, active));
        ImGui::Spacing();

        if (machine && machine->isFailed())
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.130f, 0.130f, 1.0f));
            if (ImGui::Button("Repair", ImVec2(-1.0f, 30.0f)))
                controller.onRepairMachine(step);
            ImGui::PopStyleColor();
            ImGui::TextDisabled("$%d", BurgerFactoryModel::REPAIR_COST);
        }
        else if (machine)
        {
            const float progress = progress01(machine->getProgress());
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, active ? green : ImVec4(0.180f, 0.260f, 0.350f, 1.0f));
            ImGui::ProgressBar(progress, ImVec2(-1.0f, 8.0f), "");
            ImGui::PopStyleColor();
            ImGui::TextDisabled("%.0f%%", progress * 100.0f);
            
            // 업그레이드 버튼 추가
            ImGui::Spacing();
            if (machine->getLevel() < machine->getMaxLevel()) {
                std::string upgText = "UPG ($" + std::to_string(machine->getUpgradeCost()) + ")";
                if (ImGui::Button(upgText.c_str(), ImVec2(-1.0f, 25.0f))) {
                    machineToUpgrade = step;
                    showUpgradePopup = true;
                }
            } else {
                ImGui::BeginDisabled();
                ImGui::Button("MAX LV", ImVec2(-1.0f, 25.0f));
                ImGui::EndDisabled();
            }
        }
        else
        {
            ImGui::TextDisabled("Unavailable");
        }

        ImGui::EndChild();
        ImGui::PopStyleColor(2);
        ImGui::PopID();
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

    // -----------------------------------------------------
    // 업그레이드 팝업창 렌더링 로직 (추가된 부분)
    // -----------------------------------------------------
    if (showUpgradePopup) {
        ImGui::OpenPopup("Upgrade Machine");
    }

    if (ImGui::BeginPopupModal("Upgrade Machine", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        Machine* m = model.getMachine(machineToUpgrade);
        if (m) {
            ImGui::Text("Upgrade %s?", m->getName().c_str());
            ImGui::Separator();
            ImGui::Text("Current Level: %d", m->getLevel());
            ImGui::Text("Next Level: %d", m->getLevel() + 1);
            ImGui::TextColored(yellow, "Cost: $%d", m->getUpgradeCost());
            ImGui::Spacing();
            
            if (ImGui::Button("Confirm Upgrade", ImVec2(120, 0))) {
                // 모델에 업그레이드 요청
                if (!model.upgradeMachine(machineToUpgrade)) {
                    // (선택사항) 돈 부족 에러 메시지를 띄우는 로직
                }
                showUpgradePopup = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                showUpgradePopup = false;
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }
    // -----------------------------------------------------

    ProcessStep cur = model.getCurrentStep();
    Machine* currentMachine = model.getMachine(cur);
    const bool canStart = model.canProceed(cur);
    const bool canPause = currentMachine &&
        (currentMachine->isRunning() || currentMachine->isPaused()) &&
        !currentMachine->isFailed();

    if (ImGui::BeginTable("HeaderTable", 2, ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableSetupColumn("Title", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Controls", ImGuiTableColumnFlags_WidthFixed, 240.0f);
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::TextColored(text, "BURGER FACTORY");

        ImGui::TableSetColumnIndex(1);
        ImGui::BeginDisabled(!canStart);
        if (ImGui::Button("Start", ImVec2(66.0f, 34.0f)))
            controller.onStartMachine(cur);
        ImGui::EndDisabled();

        ImGui::SameLine();
        ImGui::BeginDisabled(!canPause);
        if (ImGui::Button(currentMachine && currentMachine->isPaused() ? "Resume" : "Pause", ImVec2(74.0f, 34.0f)))
            controller.onTogglePause(cur);
        ImGui::EndDisabled();

        ImGui::SameLine();
        if (ImGui::Button("Refill", ImVec2(74.0f, 34.0f)))
            controller.onRefillInventory();

        ImGui::EndTable();
    }

    ImGui::Separator();

    if (ImGui::BeginTable("MetricCards", 4, ImGuiTableFlags_SizingStretchSame))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        drawStatCard("MoneyCard", "Money", green, 0);
        ImGui::TableSetColumnIndex(1);
        drawStatCard("CompletedCard", "Completed Burgers", yellow, 1);
        ImGui::TableSetColumnIndex(2);
        drawStatCard("OrdersCard", "Orders", blue, 2);
        ImGui::TableSetColumnIndex(3);
        drawStatCard("ReputationCard", "Reputation", yellow, 3);
        ImGui::EndTable();
    }

    const float contentH = ImGui::GetContentRegionAvail().y;
    if (ImGui::BeginTable("MainLayout", 3, ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableSetupColumn("Inventory", ImGuiTableColumnFlags_WidthStretch, 0.22f);
        ImGui::TableSetupColumn("Factory", ImGuiTableColumnFlags_WidthStretch, 0.53f);
        ImGui::TableSetupColumn("Orders", ImGuiTableColumnFlags_WidthStretch, 0.25f);
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, panel);
        ImGui::PushStyleColor(ImGuiCol_Border, border);
        ImGui::BeginChild("InventoryPanel", ImVec2(0.0f, contentH), true);
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

        ImGui::TableSetColumnIndex(1);
        ImGui::BeginGroup();
        ImGui::PushStyleColor(ImGuiCol_ChildBg, panel);
        ImGui::PushStyleColor(ImGuiCol_Border, border);
        ImGui::BeginChild("ProductionPanel", ImVec2(0.0f, contentH * 0.62f), true);
        ImGui::TextColored(text, "PRODUCTION LINE");
        ImGui::Separator();

        if (ImGui::BeginTable("ProductionLine", 11, ImGuiTableFlags_SizingStretchProp))
        {
            for (int i = 0; i < 11; ++i)
            {
                if (i % 2 == 0)
                    ImGui::TableSetupColumn("Machine", ImGuiTableColumnFlags_WidthStretch, 1.0f);
                else
                    ImGui::TableSetupColumn("Arrow", ImGuiTableColumnFlags_WidthFixed, 22.0f);
            }

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            drawMachineCard("Prep\nStation", "PREP", ProcessStep::PreparingIngredients);
            ImGui::TableSetColumnIndex(1);
            ImGui::Dummy(ImVec2(1.0f, 54.0f));
            ImGui::TextColored(muted, ">");
            ImGui::TableSetColumnIndex(2);
            drawMachineCard("Patty\nGrill", "GRIL", ProcessStep::GrillPatty);
            ImGui::TableSetColumnIndex(3);
            ImGui::Dummy(ImVec2(1.0f, 54.0f));
            ImGui::TextColored(muted, ">");
            ImGui::TableSetColumnIndex(4);
            drawMachineCard("Sauce\nDispenser", "SAUC", ProcessStep::AddSauce);
            ImGui::TableSetColumnIndex(5);
            ImGui::Dummy(ImVec2(1.0f, 54.0f));
            ImGui::TextColored(muted, ">");
            ImGui::TableSetColumnIndex(6);
            drawMachineCard("Assembly\nMachine", "ASMB", ProcessStep::AssembleBurger);
            ImGui::TableSetColumnIndex(7);
            ImGui::Dummy(ImVec2(1.0f, 54.0f));
            ImGui::TextColored(muted, ">");
            ImGui::TableSetColumnIndex(8);
            drawMachineCard("Quality\nCheck", "QC", ProcessStep::QualityCheck);
            ImGui::TableSetColumnIndex(9);
            ImGui::Dummy(ImVec2(1.0f, 54.0f));
            ImGui::TextColored(muted, ">");
            ImGui::TableSetColumnIndex(10);
            drawMachineCard("Packing\nMachine", "PACK", ProcessStep::PackBurger);

            ImGui::EndTable();
        }

        ImGui::EndChild();
        ImGui::PopStyleColor(2);

        ImGui::PushStyleColor(ImGuiCol_ChildBg, panel);
        ImGui::PushStyleColor(ImGuiCol_Border, border);
        ImGui::BeginChild("StatusPanel", ImVec2(0.0f, 0.0f), true);
        ImGui::TextColored(text, "CURRENT STATUS");
        ImGui::TextColored(model.isCurrentMachineFailed() ? red : text, "%s", currentStatusText());
        ImGui::Separator();

        if (ImGui::BeginTable("ActionButtons", 3, ImGuiTableFlags_SizingStretchSame))
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            if (ImGui::Button("Classic $100", ImVec2(-1.0f, 38.0f)))
                controller.onNewOrder(BurgerType::CLASSIC);
            ImGui::TableSetColumnIndex(1);
            if (ImGui::Button("Cheese $130", ImVec2(-1.0f, 38.0f)))
                controller.onNewOrder(BurgerType::CHEESE);
            ImGui::TableSetColumnIndex(2);
            if (ImGui::Button("Double $160", ImVec2(-1.0f, 38.0f)))
                controller.onNewOrder(BurgerType::DOUBLE);
            ImGui::EndTable();
        }

        if (ImGui::BeginTable("ControlButtons", 2, ImGuiTableFlags_SizingStretchSame))
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::BeginDisabled(!canStart);
            if (ImGui::Button("Start Production Line", ImVec2(-1.0f, 38.0f)))
                controller.onStartMachine(cur);
            ImGui::EndDisabled();
            ImGui::TableSetColumnIndex(1);
            ImGui::BeginDisabled(true);
            ImGui::Button("Sell Burger", ImVec2(-1.0f, 38.0f));
            ImGui::EndDisabled();
            ImGui::EndTable();
        }

        ImGui::EndChild();
        ImGui::PopStyleColor(2);
        ImGui::EndGroup();

        ImGui::TableSetColumnIndex(2);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, panel);
        ImGui::PushStyleColor(ImGuiCol_Border, border);
        ImGui::BeginChild("OrdersPanel", ImVec2(0.0f, contentH), true);
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
            for (int i = 0; i < (int)queued.size(); ++i)
                ImGui::Text("%d. %s $%d", i + 1, burgerName(queued[i].type), burgerPrice(queued[i].type));
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
                ImGui::Text("#%d %s +$%d", i + 1, burgerName(done[i].type), burgerPrice(done[i].type));
            ImGui::EndChild();
        }

        ImGui::EndChild();
        ImGui::PopStyleColor(2);

        ImGui::EndTable();
    }

    ImGui::End();

    ImGui::PopStyleColor(12);
    ImGui::PopStyleVar(6);
}

void FactoryView::renderOrderHistory() {}