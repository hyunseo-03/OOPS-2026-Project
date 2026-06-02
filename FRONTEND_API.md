# 프론트엔드 API 가이드

View(ImGui)에서 사용할 수 있는 함수 목록입니다.
Controller를 통해 Model을 조작하고, Model의 getter로 상태를 읽어 화면에 표시합니다.

---

## 기본 규칙

```
버튼 클릭  → controller.on...() 호출      ← 상태 변경은 반드시 Controller 경유
화면 표시  → model.get...()   호출      ← 읽기는 Model 직접 접근 가능
매 프레임  → controller.update(dt) 호출  ← 게임 루프에서 반드시 호출
```

---

## FactoryController - 버튼 이벤트 함수

### 매 프레임 호출 (게임 루프 필수)
```cpp
controller.update(float dt);
// 모든 기계를 dt초만큼 진행시킴. ImGui 렌더 루프 안에서 매 프레임 호출해야 함.
```

---

### 주문
```cpp
controller.onNewOrder(BurgerType::CLASSIC);  // 클래식 버거 주문
controller.onNewOrder(BurgerType::CHEESE);   // 치즈 버거 주문
controller.onNewOrder(BurgerType::DOUBLE);   // 더블 버거 주문
// 주문이 쌓이면 큐에 대기, 현재 작업 끝나면 자동으로 다음 주문 시작
```

---

### 기계 시작
```cpp
controller.onStartMachine(ProcessStep::PreparingIngredients);
// 현재 단계가 PreparingIngredients일 때만 동작
// 재고 확인 → 재료 차감 → PrepMachine 시작
// canProceed() 실패 시 (재고 부족, 고장, 주문 없음) 아무것도 안 함
```

---

### 포장 완료
```cpp
controller.onPackBurger();
// currentStep == Done 일 때만 동작
// 버거 완성 처리 → 돈 추가 → 다음 주문으로
```

---

### 일시정지 / 재개 (토글)
```cpp
controller.onTogglePause(ProcessStep::GrillPatty);
// 해당 단계 기계를 일시정지 또는 재개
// 현재 progress를 유지한 채로 멈춤/재개
// 어떤 ProcessStep이든 전달 가능
```

---

### 기계 수리
```cpp
controller.onRepairMachine(ProcessStep::GrillPatty);
// 해당 기계가 고장(isFailed == true) 상태일 때만 동작
// 수리비 $50 차감 (돈 부족 시 수리 안 됨)
// 수리 후 기계 리셋 → 다시 start() 가능
```

---

### 재료 보충
```cpp
controller.onRefillInventory();
// 모든 재료를 20개씩 보충
// 아무 때나 호출 가능
```

---

## BurgerFactoryModel - 상태 읽기 함수 (getter)

### 공장 전체 상태
```cpp
model.getCurrentStep();           // 현재 공정 단계 반환 (ProcessStep 열거형)
model.getMoney();                 // 현재 보유 금액 (int)
model.getTotalBurgersProduced();  // 지금까지 완성한 버거 수 (int)
model.hasOrder();                 // 처리할 주문이 있는지 (bool)
model.isQualityCheckPassed();     // 마지막 품질검사 결과 (bool)
model.isCurrentMachineFailed();   // 현재 단계 기계가 고장났는지 (bool)
model.isCurrentMachinePaused();   // 현재 단계 기계가 일시정지 중인지 (bool)
```

---

### 현재 주문 정보
```cpp
const Order& order = model.getCurrentOrder();
order.type;         // BurgerType::CLASSIC / CHEESE / DOUBLE
order.isCompleted;  // 완료됐는지 여부
```

---

### 기계 진행률 (ProgressBar 표시용)
```cpp
// 특정 단계 기계의 Machine* 포인터를 가져옴
Machine* m = model.getMachine(ProcessStep::GrillPatty);

if (m != nullptr) {
    m->getProgress();   // 0.0f ~ 1.0f  → ImGui::ProgressBar()에 바로 사용
    m->isRunning();     // 현재 작동 중?
    m->isPaused();      // 일시정지 중?
    m->isFailed();      // 고장났는지?
    m->getName();       // "Grill Machine" 등 기계 이름 문자열
}

// 사용 가능한 ProcessStep 목록:
// ProcessStep::PreparingIngredients  → PrepMachine
// ProcessStep::GrillPatty            → GrillMachine
// ProcessStep::AddSauce              → SauceMachine
// ProcessStep::AssembleBurger        → AssemblyMachine
// ProcessStep::QualityCheck          → QualityChecker
// ProcessStep::PackBurger            → PackingMachine
```

---

### GrillMachine 패티 진행 표시 (더블버거 구분용)
```cpp
// GrillMachine은 dynamic_cast로 꺼내야 상세 정보 접근 가능
#include "model/GrillMachine.h"

GrillMachine* grill = dynamic_cast<GrillMachine*>(model.getMachine(ProcessStep::GrillPatty));
if (grill) {
    grill->getPattiesGrilled();  // 지금까지 구운 패티 수
    grill->getPattyCount();      // 총 구워야 할 패티 수
    // 예시: "패티 1 / 2 완료" 표시 가능
}
```

---

### 재고 현황
```cpp
model.getIngredientAmount(IngredientType::BUN);      // 번 재고
model.getIngredientAmount(IngredientType::PATTY);    // 패티 재고
model.getIngredientAmount(IngredientType::CHEESE);   // 치즈 재고
model.getIngredientAmount(IngredientType::LETTUCE);  // 양상추 재고
model.getIngredientAmount(IngredientType::TOMATO);   // 토마토 재고
model.getIngredientAmount(IngredientType::ONION);    // 양파 재고
model.getIngredientAmount(IngredientType::SAUCE);    // 소스 재고
// 모두 int 반환
```

---

### 준비된 재료 (PrepMachine 완료 후 QualityCheck 전까지 표시용)
```cpp
const auto& prepared = model.getPreparedIngredients();
// map<IngredientType, int> 반환
// PrepMachine이 재고에서 꺼낸 재료 목록
// QualityCheck에서 레시피와 비교하는 데이터
```

---

## 화면 구성 제안

### 현재 단계 표시 예시
```cpp
ProcessStep step = model.getCurrentStep();
switch (step) {
    case ProcessStep::Idle:                  ImGui::Text("대기 중"); break;
    case ProcessStep::PreparingIngredients:  ImGui::Text("재료 준비 중"); break;
    case ProcessStep::GrillPatty:            ImGui::Text("패티 굽는 중"); break;
    case ProcessStep::AddSauce:              ImGui::Text("소스 뿌리는 중"); break;
    case ProcessStep::AssembleBurger:        ImGui::Text("조립 중"); break;
    case ProcessStep::QualityCheck:          ImGui::Text("품질 검사 중"); break;
    case ProcessStep::PackBurger:            ImGui::Text("포장 중"); break;
    case ProcessStep::Done:                  ImGui::Text("완성! 포장 버튼을 눌러주세요"); break;
}
```

### 기계별 ProgressBar 예시
```cpp
const ProcessStep steps[] = {
    ProcessStep::PreparingIngredients,
    ProcessStep::GrillPatty,
    ProcessStep::AddSauce,
    ProcessStep::AssembleBurger,
    ProcessStep::QualityCheck,
    ProcessStep::PackBurger
};

for (auto step : steps) {
    Machine* m = model.getMachine(step);
    if (!m) continue;

    ImGui::Text("%s", m->getName().c_str());

    if (m->isFailed()) {
        ImGui::SameLine();
        ImGui::TextColored({1,0,0,1}, "[고장]");
        if (ImGui::Button("수리 ($50)"))
            controller.onRepairMachine(step);
    } else {
        ImGui::ProgressBar(m->getProgress());
        if (m->isPaused()) {
            if (ImGui::Button("재개")) controller.onTogglePause(step);
        } else if (m->isRunning()) {
            if (ImGui::Button("정지")) controller.onTogglePause(step);
        }
    }
}
```

### 주문 버튼 예시
```cpp
if (ImGui::Button("클래식 주문")) controller.onNewOrder(BurgerType::CLASSIC);
if (ImGui::Button("치즈 주문"))   controller.onNewOrder(BurgerType::CHEESE);
if (ImGui::Button("더블 주문"))   controller.onNewOrder(BurgerType::DOUBLE);
```

### 시작/포장 버튼 예시
```cpp
ProcessStep cur = model.getCurrentStep();

if (cur == ProcessStep::PreparingIngredients) {
    if (model.canProceed(cur)) {
        if (ImGui::Button("생산 시작"))
            controller.onStartMachine(cur);
    } else {
        ImGui::BeginDisabled();
        ImGui::Button("재료 부족");
        ImGui::EndDisabled();
    }
}

if (cur == ProcessStep::Done) {
    if (ImGui::Button("포장 완료"))
        controller.onPackBurger();
}
```

---

## 상수
```cpp
BurgerFactoryModel::REPAIR_COST  // 수리비: 50
```
