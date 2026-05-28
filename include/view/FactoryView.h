#pragma once

#include "imgui.h"
#include "model/BurgerFactoryModel.h"
#include "controller/FactoryController.h"

// ============================================================
// FactoryView - ImGui 화면 출력 담당 (View)
// MVC에서 View 역할:
//   - Model의 데이터를 읽어서 화면에 표시
//   - 버튼 클릭 시 Controller 함수를 호출
//   - 데이터를 직접 수정하지 않음
// ============================================================
class FactoryView
{
private:
    BurgerFactoryModel& model;
    FactoryController&  controller;

    // UI 내부 상태
    bool      showMachineDetail;
    int       selectedMachine;   // 선택된 기계 인덱스 (0~3)
    ImFont*   largeIconFont;     // 기계 카드 큰 아이콘용 폰트

    // --- 레이아웃 상수 ---
    static const float WINDOW_WIDTH;
    static const float WINDOW_HEIGHT;
    static const float CARD_WIDTH;
    static const float CARD_HEIGHT;
    static const float INVENTORY_WIDTH;

    // --- 색상 팔레트 ---
    ImVec4 colorBg;
    ImVec4 colorCard;
    ImVec4 colorCardActive;
    ImVec4 colorRunning;
    ImVec4 colorIdle;
    ImVec4 colorWaiting;
    ImVec4 colorMoney;
    ImVec4 colorBurger;
    ImVec4 colorOrders;
    ImVec4 colorStar;

    // --- 내부 렌더 함수들 ---
    void renderHeader();
    void renderStatBox(const char* label, const char* value, ImVec4 valueColor, float width);
    void renderInventoryPanel();
    void renderProductionLine();
    void renderMachineCard(const Machine& machine, const char* icon,
                           int index, bool isActiveStep);
    void renderArrow();
    void renderStatusBar();
    void renderMachineDetailPopup();

    // --- 헬퍼 함수들 ---
    bool        isMachineActiveStep(int index) const;
    const char* getMachineIcon(int index)        const;
    const char* getMachineDisplayName(int index) const;

public:
    FactoryView(BurgerFactoryModel& model, FactoryController& controller);

    void setLargeIconFont(ImFont* font) { largeIconFont = font; }
    void setupStyle();
    void render();
};
