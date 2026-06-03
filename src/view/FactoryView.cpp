#include "view/FactoryView.h"
#include <cstdio> // snprintf 사용을 위해 추가

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
    
    // 스타일 가미: 메인 윈도우 패딩 및 라운딩 설정
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 15.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);   // 카드 모서리 둥글게
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);   // 버튼/바 모서리 둥글게

    ImGui::Begin("Burger Factory", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // 타이틀
    ImGui::TextColored(ImVec4(0.9f, 0.7f, 0.2f, 1.0f), "🍔 BURGER FACTORY MANAGEMENT");
    ImGui::Separator();
    ImGui::Spacing();

    // 최상단 대시보드 - 대형 스탯 카드화
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.16f, 1.0f)); // 짙은 네이비 그레이 배경
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.4f, 1.0f));    // 부드러운 테두리 색상
    
    if (ImGui::BeginChild("DashboardCard", ImVec2(-1, 65), true, ImGuiWindowFlags_None))
    {
        ImGui::SetCursorPosY(14.0f); // 카드 중앙 정렬을 위한 Y축 조정
        ImGui::Text("💰 CURRENT MONEY :");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.3f, 0.85f, 0.3f, 1.0f), "$%d", model.getMoney());

        ImGui::SameLine(left * 0.55f); 
        ImGui::SetCursorPosY(14.0f);
        ImGui::Text("📦 TOTAL PRODUCED :");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.3f, 0.7f, 1.0f, 1.0f), "%d EA", model.getTotalBurgersProduced());
    }
    ImGui::EndChild();
    ImGui::PopStyleColor(2); 

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // 현재 공정 단계 요약 요약창
    ProcessStep cur = model.getCurrentStep();
    ImGui::Text("Current Global Step : ");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 1.0f, 1.0f), "%s", stepToString(cur));

    if (model.isCurrentMachineFailed())
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "⚠️ !! MACHINE FAILED IN CURRENT STEP !!");
    else if (model.isCurrentMachinePaused())
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.3f, 1.0f), "⏸️ -- CURRENT PROCESS PAUSED --");

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ═══════════════════════════════════════════
    // 주문 버튼 영역 ([ORDER SELECTION])
    // ═══════════════════════════════════════════
    ImGui::Text("🛒 [ORDER SELECTION]");
    ImGui::Spacing();

    // 1. Classic 버거 버튼 (연녹색)
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));       
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.9f, 0.4f, 1.0f)); 
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);                  
    if (ImGui::Button("Classic ($100)", ImVec2(130, 30))) controller.onNewOrder(BurgerType::CLASSIC);
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(2);

    ImGui::SameLine();

    // 2. Cheese 버거 버튼 (체다 치즈색)
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.2f, 1.0f));       
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.8f, 0.2f, 1.0f)); 
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    if (ImGui::Button("Cheese ($130)", ImVec2(130, 30)))  controller.onNewOrder(BurgerType::CHEESE);
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(2);

    ImGui::SameLine();

    // 3. Double 버거 버튼 (패티 브라운)
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.5f, 0.2f, 1.0f));       
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.9f, 0.5f, 0.2f, 1.0f)); 
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    if (ImGui::Button("Double ($160)", ImVec2(130, 30)))  controller.onNewOrder(BurgerType::DOUBLE);
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(2);


    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ═══════════════════════════════════════════
    // 기계 목록 영역 - 흐름도형 2열 그리드 배치
    // ═══════════════════════════════════════════
    // ═══════════════════════════════════════════
    // 기계 목록 영역 - 흐름도형 2열 그리드 배치 (완전 교정본)
    // ═══════════════════════════════════════════
    ImGui::Text("⚙️ [FACTORY PRODUCTION LINES FLOW]");
    ImGui::Spacing();

    // 원하시는 직관적인 공정 순서 배열 구조 정의
    struct MachineRow { 
        const char* name; 
        const char* icon; 
        ProcessStep step; 
    };
    
    // 왼쪽 라인 (전반부 공정)
    MachineRow leftColumnRows[] = {
        { "1. Prep Machine",    "🥣", ProcessStep::PreparingIngredients },
        { "2. Grill Machine",   "🔥", ProcessStep::GrillPatty           },
        { "3. Sauce Machine",   "🧴", ProcessStep::AddSauce             }
    };

    // 오른쪽 라인 (후반부 공정)
    MachineRow rightColumnRows[] = {
        { "4. Assembly Line",   "🍔", ProcessStep::AssembleBurger       },
        { "5. Quality Checker", "✅", ProcessStep::QualityCheck         },
        { "6. Packing Machine", "🎁", ProcessStep::PackBurger           }
    };

    // ImGui Table 시작 (2열 배치)
    if (ImGui::BeginTable("MachineGridUMLFlowTable", 2, ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_BordersOuter))
    {
        // 테이블 헤더 설정으로 가독성 상향
        ImGui::TableSetupColumn("◀ FIRST HALF PROCESS (1-3)", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("▶ SECOND HALF PROCESS (4-6)", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();

        // 총 3개의 행(Row)을 돌며 왼쪽과 오른쪽을 한 쌍씩 배치
        for (int rowIdx = 0; rowIdx < 3; rowIdx++)
        {
            // ----------------------------------------------------------------
            // [1] 좌측 열 배치 (1, 2, 3 공정 세로 나열)
            // ----------------------------------------------------------------
            ImGui::TableNextColumn();
            {
                MachineRow r = leftColumnRows[rowIdx];
                Machine* m = model.getMachine(r.step);
                if (m) 
                {
                    bool isActive = (cur == r.step);
                    
                    // 스타일 주입
                    if (m->isFailed()) {
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.4f, 0.1f, 0.1f, 0.3f)); // 투명한 빨강
                        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));   // 고장 붉은 테두리
                    } else if (isActive) {
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.05f, 0.18f, 0.15f, 0.8f)); // 연한 녹색빛 감도는 배경
                        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2f, 0.9f, 0.5f, 1.0f));   // 활성화: 녹색 네온 조명
                    } else {
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.11f, 0.11f, 0.13f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.25f, 0.25f, 0.28f, 1.0f));
                    }

                    char cardID[32];
                    snprintf(cardID, sizeof(cardID), "LeftMacCard##%d", rowIdx);
                    if (ImGui::BeginChild(cardID, ImVec2(-1, 100.0f), true, ImGuiWindowFlags_None))
                    {
                        // 아이콘 및 텍스트 출력
                        ImGui::SetCursorPos(ImVec2(10.0f, 15.0f));
                        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", r.icon);

                        ImGui::SameLine();
                        ImGui::SetCursorPosY(15.0f);
                        if (m->isFailed()) ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "%s [FAILED]", r.name);
                        else if (isActive) ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.5f, 1.0f), "%s [RUNNING]", r.name);
                        else ImGui::TextDisabled("%s", r.name);

                        // 아래로 흐르는 화살표 시각화
                        ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x - 50, 15.0f));
                        ImGui::TextDisabled("▼ Flow");

                        ImGui::Spacing();
                        ImGui::Separator();

                        // 컨텐츠(수리 버튼 또는 프로그레스 바)
                        if (m->isFailed()) {
                            char repairLbl[32];
                            snprintf(repairLbl, sizeof(repairLbl), "FIX NOW ($%d)##L%d", BurgerFactoryModel::REPAIR_COST, (int)r.step);
                            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.15f, 0.15f, 1.0f));
                            if (ImGui::Button(repairLbl, ImVec2(-1, 26.0f))) controller.onRepairMachine(r.step);
                            ImGui::PopStyleColor();
                        } else {
                            char overlay[16];
                            snprintf(overlay, sizeof(overlay), "%.0f%% Done", m->getProgress() * 100.0f);
                            float barWidth = ImGui::GetContentRegionAvail().x - (isActive ? 75.0f : 0.0f);
                            ImGui::ProgressBar(m->getProgress(), ImVec2(barWidth, 22.0f), overlay);

                            if (isActive && (m->isRunning() || m->isPaused())) {
                                ImGui::SameLine();
                                char pauseLbl[32];
                                snprintf(pauseLbl, sizeof(pauseLbl), m->isPaused() ? "Resume##L%d" : "Pause##L%d", (int)r.step);
                                if (ImGui::Button(pauseLbl, ImVec2(65, 22.0f))) controller.onTogglePause(r.step);
                            }
                        }
                    }
                    ImGui::EndChild();
                    ImGui::PopStyleColor(2);
                }
            }

            // ----------------------------------------------------------------
            // [2] 우측 열 배치 (4, 5, 6 공정 세로 나열)
            // ----------------------------------------------------------------
            ImGui::TableNextColumn();
            {
                MachineRow r = rightColumnRows[rowIdx];
                Machine* m = model.getMachine(r.step);
                if (m) 
                {
                    bool isActive = (cur == r.step);
                    
                    // 스타일 주입
                    if (m->isFailed()) {
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.4f, 0.1f, 0.1f, 0.3f)); // 투명한 빨강
                        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));   // 고장 붉은 테두리
                    } else if (isActive) {
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.15f, 0.22f, 0.8f)); // 연한 하늘색 배경
                        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.8f, 1.0f, 1.0f));   // 활성화: 하늘색 네온 조명
                    } else {
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.11f, 0.11f, 0.13f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.25f, 0.25f, 0.28f, 1.0f));
                    }

                    char cardID[32];
                    snprintf(cardID, sizeof(cardID), "RightMacCard##%d", rowIdx);
                    if (ImGui::BeginChild(cardID, ImVec2(-1, 100.0f), true, ImGuiWindowFlags_None))
                    {
                        // 아이콘 및 텍스트 출력
                        ImGui::SetCursorPos(ImVec2(10.0f, 15.0f));
                        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", r.icon);

                        ImGui::SameLine();
                        ImGui::SetCursorPosY(15.0f);
                        if (m->isFailed()) ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "%s [FAILED]", r.name);
                        else if (isActive) ImGui::TextColored(ImVec4(0.3f, 0.8f, 1.0f, 1.0f), "%s [RUNNING]", r.name);
                        else ImGui::TextDisabled("%s", r.name);

                        // 아래로 똑같이 흐르거나 진행되는 흐름 시각화
                        ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x - 50, 15.0f));
                        ImGui::TextDisabled("▼ Flow");

                        ImGui::Spacing();
                        ImGui::Separator();

                        // 컨텐츠 (수리 버튼 또는 프로그레스 바)
                        if (m->isFailed()) {
                            char repairLbl[32];
                            snprintf(repairLbl, sizeof(repairLbl), "FIX NOW ($%d)##R%d", BurgerFactoryModel::REPAIR_COST, (int)r.step);
                            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.15f, 0.15f, 1.0f));
                            if (ImGui::Button(repairLbl, ImVec2(-1, 26.0f))) controller.onRepairMachine(r.step);
                            ImGui::PopStyleColor();
                        } else {
                            char overlay[16];
                            snprintf(overlay, sizeof(overlay), "%.0f%% Done", m->getProgress() * 100.0f);
                            float barWidth = ImGui::GetContentRegionAvail().x - (isActive ? 75.0f : 0.0f);
                            ImGui::ProgressBar(m->getProgress(), ImVec2(barWidth, 22.0f), overlay);

                            if (isActive && (m->isRunning() || m->isPaused())) {
                                ImGui::SameLine();
                                char pauseLbl[32];
                                snprintf(pauseLbl, sizeof(pauseLbl), m->isPaused() ? "Resume##R%d" : "Pause##R%d", (int)r.step);
                                if (ImGui::Button(pauseLbl, ImVec2(65, 22.0f))) controller.onTogglePause(r.step);
                            }
                        }
                    }
                    ImGui::EndChild();
                    ImGui::PopStyleColor(2);
                }
            }
        }
        ImGui::EndTable();
    }
    // 통합 마스터 공장 제어 영역 (중복 완벽 청소됨)
    // ═══════════════════════════════════════════
    ImGui::Text("🎮 FACTORY CONTROL PANEL");
    ImGui::Spacing();

    bool canStart = model.canProceed(cur); 
    if (!canStart) ImGui::BeginDisabled();

    // 대형 네온 블루 스타일 가동 버튼
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.35f, 0.65f, 1.0f));        
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.22f, 0.55f, 0.95f, 1.0f)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.08f, 0.25f, 0.50f, 1.0f));  
    if (ImGui::Button("🚀 START PRODUCTION LINE", ImVec2(-1, 38.0f))) controller.onStartMachine(cur);
    ImGui::PopStyleColor(3);

    if (!canStart) ImGui::EndDisabled();

    ImGui::Spacing();

    // 재고 리필 마스터 버튼
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.72f, 0.42f, 0.12f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.92f, 0.52f, 0.22f, 1.0f));
    if (ImGui::Button("📦 Refill Factory Inventory Storage", ImVec2(-1, 30.0f))) controller.onRefillInventory();
    ImGui::PopStyleColor(2);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ═══════════════════════════════════════════
    // 재고 상태 현황 영역 ([INVENTORY])
    // ═══════════════════════════════════════════
    ImGui::Text("🏪 [RAW MATERIAL INVENTORY STATUS]");
    ImGui::Spacing();

    struct IngRow { const char* name; IngredientType type; };
    IngRow ings[] = {
        { "Bun",     IngredientType::BUN     },
        { "Patty",   IngredientType::PATTY   },
        { "Cheese",  IngredientType::CHEESE  },
        { "Lettuce", IngredientType::LETTUCE },
        { "Tomato",  IngredientType::TOMATO  },
        { "Sauce",   IngredientType::SAUCE   },
    };

    for (int i = 0; i < 6; i++)
    {
        int amt = model.getIngredientAmount(ings[i].type);
        bool low = (amt <= 5);
        if (low) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
        
        char label[32];
        snprintf(label, sizeof(label), "%s: %d", ings[i].name, amt);
        ImGui::ProgressBar(amt / 20.0f, ImVec2(150, 14), label);
        if (low) ImGui::PopStyleColor();
        if (i % 2 == 0) ImGui::SameLine(200);
    }

    // QualityCheck 결과 출력 (Idle 상태일 때 노출)
    if (cur == ProcessStep::Idle)
    {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        if (model.isQualityCheckPassed())
            ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "✅ Quality Check Summary: LAST ORDER PASSED");
        else
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "❌ Quality Check Summary: LAST ORDER FAILED");
    }
    
    ImGui::PopStyleVar(3); // 맨 위에서 추가한 3개의 스타일 변수들 안전 해제
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

    // 현재 생산 중인 주문
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.3f, 1.0f), "[In Production]");
    ImGui::Separator();
    if (model.hasOrder())
    {
        const Order& o = model.getCurrentOrder();
        ImGui::Text("  %s Burger  $%d", burgerName(o.type), burgerPrice(o.type));
        ImGui::TextDisabled("  Step: %s", stepToString(cur));
    }
    else
    {
        ImGui::TextDisabled("  None");
    }
    ImGui::Spacing();

    // 대기 중인 주문
    const auto& queued = model.getQueuedOrders();
    ImGui::TextColored(ImVec4(0.8f, 0.6f, 1.0f, 1.0f), "[Waiting (%d)]", (int)queued.size());
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

    // 판매 완료 목록
    const auto& done = model.getCompletedOrders();
    ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f), "[Sold (%d)]", (int)done.size());
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

void FactoryView::renderOrderHistory() {}