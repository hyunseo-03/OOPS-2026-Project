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
    ImGui::Text("⚙️ [FACTORY PRODUCTION LINES FLOW]");
    ImGui::Spacing();

    // 왼쪽 열: 1, 2, 3공정 / 오른쪽 열: 4, 5, 6공정 매핑 구조
    // ══════════════════════════════════════════════════════════════════
    // [배열 순서 핵심] 테이블이 좌->우 순서로 채워지므로, 
    // 코드를 지그재그로 배치해야 화면에서는 좌측열(1,2,3) / 우측열(4,5,6)로 나뉩니다.
    // ══════════════════════════════════════════════════════════════════
    struct MachineRow { 
        const char* name; 
        const char* icon; // 그림을 대신할 큰 텍스트 아이콘
        ProcessStep step; 
    };
    MachineRow rows[] = {
        { "1. Prep Machine",    "🥣", ProcessStep::PreparingIngredients }, // 1열 1행
        { "4. Assembly Line",   "🍔", ProcessStep::AssembleBurger       }, // 2열 1행
        { "2. Grill Machine",   "🔥", ProcessStep::GrillPatty           }, // 1열 2행
        { "5. Quality Checker", "✅", ProcessStep::QualityCheck         }, // 2열 2행
        { "3. Sauce Machine",   "🧴", ProcessStep::AddSauce             }, // 1열 3행
        { "6. Packing Machine", "🎁", ProcessStep::PackBurger           }, // 2열 3행
    };

    // ImGui Table을 이용한 좌우 2열 배치 시작
    if (ImGui::BeginTable("MachineGridUMLFlowTable", 2, ImGuiTableFlags_SizingFixedSame))
    {
        for (int i = 0; i < 6; i++)
        {
            ImGui::TableNextColumn(); // 자동으로 다음 칸으로 이동

            Machine* m = model.getMachine(rows[i].step);
            if (!m) continue;

            bool isActive = (cur == rows[i].step);

            // 상태별 커스텀 스타일링 (배경 및 테두리 조명 효과 강화)
            if (m->isFailed()) 
            {
                // 고장: 카드 전체 배경을 투명한 빨간색으로 덮어 시각적 경고 극대화
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.4f, 0.1f, 0.1f, 0.3f));
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.2f, 0.2f, 1.0f)); 
            } 
            else if (isActive) 
            {
                // 활성화: 테두리를 밝은 하늘색(Cyan)으로 네온 조명 효과
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.15f, 0.22f, 0.8f)); 
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.8f, 1.0f, 1.0f)); 
            } 
            else 
            {
                // 기본 어두운 카드 스타일
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.11f, 0.11f, 0.13f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.25f, 0.25f, 0.28f, 1.0f));
            }

            // 개별 기계 영역(Child)을 `-1, 100.0f`로 대폭 확장하여 시각화 요소 배치
            char cardID[32];
            snprintf(cardID, sizeof(cardID), "MacUMLCard##%d", i);
            if (ImGui::BeginChild(cardID, ImVec2(-1, 100.0f), true, ImGuiWindowFlags_None))
            {
                // [시각화 ①] 크게 표현된 기계 그림(아이콘)과 이름
                // ImGui는 그래픽 파일이 없으면 이모지를 크게 출력하여 가독성을 높입니다.
                ImGui::SetCursorPos(ImVec2(10.0f, 15.0f)); // 아이콘 위치 조정
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", rows[i].icon); // 크기 강화 (폰트 크기 조절이 필요할 수 있습니다.)

                ImGui::SameLine();
                ImGui::SetCursorPosY(15.0f);
                if (m->isFailed()) {
                    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "%s [FAILED]", rows[i].name);
                } else if (isActive) {
                    ImGui::TextColored(ImVec4(0.3f, 0.8f, 1.0f, 1.0f), "%s [RUNNING]", rows[i].name);
                } else {
                    ImGui::TextDisabled("%s", rows[i].name);
                }

                // [시각화 ②] UML 흐름 화살표 (텍스트 및 그래픽 조합)
                // 전반부(1열) 기계 밑에는 내려가는 화살표, 후반부(2열) 밑에는 올라가는 화살표
                ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x - 40, 15.0f)); // 우측 상단 화살표 위치
                if (i % 2 == 0) // 1열 (1,2,3단계)
                    ImGui::TextDisabled("▼ Flow"); // 아래로 흐르는 화살표
                else // 2열 (4,5,6단계)
                    ImGui::TextDisabled("▲ Flow"); // 위로 흐르는 화살표 (UML 흐름도를 위한 시각적 표현)

                ImGui::Spacing();
                ImGui::Separator(); // 타이틀과 메인 컨텐츠 분리선

                // 메인 컨텐츠 영역
                if (m->isFailed())
                {
                    // 긴급 수리 버튼 레이아웃
                    char repairLbl[32];
                    snprintf(repairLbl, sizeof(repairLbl), "FIX NOW ($%d)##%d", BurgerFactoryModel::REPAIR_COST, (int)rows[i].step);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.15f, 0.15f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.25f, 0.25f, 1.0f));
                    if (ImGui::Button(repairLbl, ImVec2(-1, 26.0f))) controller.onRepairMachine(rows[i].step);
                    ImGui::PopStyleColor(2);
                }
                else
                {
                    // [시각화 ③] 진행 상황 시각화 크기 대폭 강화 (높이 22px로 확장)
                    char overlay[16];
                    snprintf(overlay, sizeof(overlay), "%.0f%% Done", m->getProgress() * 100.0f);
                    
                    // 우측에 일시정지 버튼 공간(75px)을 제외한 나머지 폭을 가득 채움
                    float barWidth = ImGui::GetContentRegionAvail().x - (isActive ? 75.0f : 0.0f);
                    ImGui::ProgressBar(m->getProgress(), ImVec2(barWidth, 22.0f), overlay); 

                    // 활성화 상태일 때만 일시정지/재개 제어 패널 노출
                    if (isActive && (m->isRunning() || m->isPaused()))
                    {
                        ImGui::SameLine();
                        char pauseLbl[32];
                        snprintf(pauseLbl, sizeof(pauseLbl), m->isPaused() ? "Resume##%d" : "Pause##%d", (int)rows[i].step);
                        if (ImGui::Button(pauseLbl, ImVec2(65, 22.0f))) controller.onTogglePause(rows[i].step);
                    }
                }
            }
            ImGui::EndChild();
            ImGui::PopStyleColor(2); 
        }
        ImGui::EndTable();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();    
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