import AppKit
import CoreGraphics
import Foundation

let outputPath = CommandLine.arguments.count > 1
    ? CommandLine.arguments[1]
    : "docs/presentation_flow_explanation.pdf"

let pageWidth: CGFloat = 595.0
let pageHeight: CGFloat = 842.0
let margin: CGFloat = 46.0
let contentWidth = pageWidth - margin * 2
let bottomLimit = pageHeight - margin

let bodyFont = NSFont(name: "AppleSDGothicNeo-Regular", size: 11.5) ?? NSFont.systemFont(ofSize: 11.5)
let bodyBold = NSFont(name: "AppleSDGothicNeo-Bold", size: 11.5) ?? NSFont.boldSystemFont(ofSize: 11.5)
let titleFont = NSFont(name: "AppleSDGothicNeo-Bold", size: 23) ?? NSFont.boldSystemFont(ofSize: 23)
let h2Font = NSFont(name: "AppleSDGothicNeo-Bold", size: 16) ?? NSFont.boldSystemFont(ofSize: 16)
let h3Font = NSFont(name: "AppleSDGothicNeo-Bold", size: 13) ?? NSFont.boldSystemFont(ofSize: 13)
let codeFont = NSFont(name: "Menlo", size: 8.8) ?? NSFont.monospacedSystemFont(ofSize: 8.8, weight: .regular)

let textColor = NSColor(calibratedRed: 0.12, green: 0.16, blue: 0.22, alpha: 1.0)
let mutedColor = NSColor(calibratedRed: 0.32, green: 0.38, blue: 0.45, alpha: 1.0)
let titleColor = NSColor(calibratedRed: 0.06, green: 0.16, blue: 0.27, alpha: 1.0)
let ruleColor = NSColor(calibratedRed: 0.62, green: 0.70, blue: 0.78, alpha: 1.0)
let codeBg = NSColor(calibratedRed: 0.94, green: 0.96, blue: 0.98, alpha: 1.0)

enum Block {
    case title(String)
    case h2(String)
    case h3(String)
    case p(String)
    case bullets([String])
    case code(String)
    case flow(String)
    case note(String)
}

let blocks: [Block] = [
    .title("Burger Factory Project 발표 설명 노트"),
    .p("주제: MVC 구조, 주문 흐름, 기계 흐름, struct/enum 관계, 추가 기능, SOLID 원칙 설명"),

    .h2("1. 전체 MVC 구조"),
    .p("이 프로젝트는 MVC 구조로 나누어져 있다. Model은 실제 데이터와 생산 로직을 담당하고, View는 ImGui 화면 출력과 버튼 표시만 담당하며, Controller는 View에서 발생한 사용자 입력을 Model 명령으로 전달한다."),
    .flow("main.cpp\n→ BurgerFactoryModel 생성\n→ FactoryController 생성\n→ FactoryView 생성\n→ 매 프레임 controller.update(dt)\n→ view.render()"),
    .h3("객체 관계"),
    .bullets([
        "main.cpp는 BurgerFactoryModel, FactoryController, FactoryView를 생성하고 사용하므로 dependency 관계이다.",
        "FactoryView는 IFactoryViewData&와 FactoryController&를 참조로 받는다. 직접 소유하지 않으므로 composition이 아니라 dependency이다.",
        "FactoryController는 IFactoryActions&에 의존한다. Model 전체를 알지 않고 명령 인터페이스만 안다.",
        "BurgerFactoryModel은 IFactoryActions, IFactoryViewData를 public inheritance로 상속한다."
    ]),
    .note("발표 문장: View는 UI만 담당하고, Controller는 버튼 입력을 Model 명령으로 전달합니다. Model은 주문, 재고, 돈, 공정 상태, 기계 상태 같은 실제 로직을 관리합니다."),

    .h2("2. 주문 넣기 흐름"),
    .p("사용자가 버거 주문 버튼을 누르면 View가 직접 주문 데이터를 수정하지 않고 Controller를 호출한다. Controller는 IFactoryActions 인터페이스를 통해 Model의 addOrder()를 호출한다."),
    .flow("FactoryView 버튼\n→ FactoryController::onNewOrder(type)\n→ IFactoryActions::addOrder(type)\n→ BurgerFactoryModel::addOrder(type)\n→ OrderManager::addOrder(type)\n→ 필요하면 enterStep(PreparingIngredients)"),
    .code("""
if (ImGui::Button(\"Classic $100\", ...))
    controller.onNewOrder(BurgerType::CLASSIC);

void FactoryController::onNewOrder(BurgerType type)
{
    model.addOrder(type);
}

void BurgerFactoryModel::addOrder(BurgerType type)
{
    if (gameOver) return;
    orderManager.addOrder(type);
    statusMessage = \"\";
    if (currentStep == ProcessStep::Idle)
        enterStep(ProcessStep::PreparingIngredients);
}
"""),
    .p("BurgerFactoryModel은 OrderManager를 멤버 변수로 직접 가지고 있으므로 composition 관계이다. 실제 주문 저장은 OrderManager::addOrder()가 담당한다."),

    .h2("3. Start 버튼 이후 기계 흐름"),
    .p("주문이 들어간 뒤 Start 버튼을 누르면 현재 공정 단계가 Controller로 전달되고, Model은 현재 단계와 기계 상태를 검사한 다음 실제 기계를 시작한다."),
    .flow("FactoryView Start 버튼\n→ FactoryController::onStartMachine(cur)\n→ BurgerFactoryModel::startProcess(step)\n→ consumeIngredients()\n→ ProductionLine::startMachine(step)\n→ Machine::setState(Running)"),
    .p("consumeIngredients()는 BurgerFactoryModel의 private 함수이다. 현재 주문의 버거 타입을 OrderManager에서 가져오고, getRecipe()로 레시피를 확인한 뒤, InventoryManager로 재료가 충분한지 검사하고 실제 재고를 차감한다."),
    .code("""
bool BurgerFactoryModel::consumeIngredients()
{
    BurgerRecipe recipe = getRecipe(orderManager.getCurrentOrder().type);
    for (const auto& [ingredient, amount] : recipe.ingredients)
        if (!inventoryManager.hasEnough(ingredient, amount)) return false;

    preparedIngredients.clear();
    for (const auto& [ingredient, amount] : recipe.ingredients)
    {
        inventoryManager.use(ingredient, amount);
        preparedIngredients[ingredient] = amount;
    }
    return true;
}
"""),

    .h2("4. Machine 상속과 다형성"),
    .p("Machine은 추상 부모 클래스이고, 실제 기계들은 Machine을 상속한다."),
    .flow("PrepMachine ─▷ Machine\nGrillMachine ─▷ Machine\nSauceMachine ─▷ Machine\nAssemblyMachine ─▷ Machine\nQualityChecker ─▷ Machine\nPackingMachine ─▷ Machine"),
    .p("ProductionLine은 std::vector<std::unique_ptr<Machine>>으로 기계들을 소유한다. 따라서 ProductionLine ◆ Machine은 composition 관계이다."),
    .code("""
void ProductionLine::update(float dt)
{
    for (auto& m : machines) m->update(dt);
}
"""),
    .p("m은 Machine 타입이지만 실제 객체가 GrillMachine이면 GrillMachine::update()가 실행된다. 이것이 다형성이다."),
    .note("발표 문장: ProductionLine은 Machine 포인터 목록만 가지고 있지만, 실제로는 각 자식 기계의 update가 실행됩니다. 그래서 새 기계를 추가할 때 Machine을 상속하고 update만 구현하면 됩니다."),

    .h2("5. struct / enum 관계"),
    .p("struct와 enum은 데이터를 명확하게 표현하기 위해 사용된다. 이들은 대부분 독립 객체를 소유하는 관계가 아니라 타입을 사용하는 dependency 관계이다."),
    .bullets([
        "Order는 BurgerType을 사용한다. 어떤 버거 주문인지 저장한다.",
        "BurgerRecipe는 map<IngredientType, int>를 가진다. 필요한 재료와 수량을 저장한다.",
        "MachineConfig는 BurgerType과 pattyCount를 가진다. 기계 설정값 전달에 사용된다.",
        "ProcessStep은 현재 공정 단계를 표현한다.",
        "MachineState는 기계 상태를 표현한다."
    ]),
    .flow("ProcessStep:\nIdle → PreparingIngredients → GrillPatty → AddSauce → AssembleBurger → QualityCheck → PackBurger\n\nMachineState:\nIdle, Running, Paused, Failed, Completed"),
    .p("여러 bool 변수 대신 enum 하나로 상태를 관리하면 Running, Paused, Failed가 동시에 true가 되는 문제를 막을 수 있다."),

    .h2("6. QualityChecker와 Packing 완료 처리"),
    .p("품질 검사는 QualityChecker가 담당한다. 품질 검사 단계가 끝나면 BurgerFactoryModel::nextStep()에서 checkQuality()를 호출한다."),
    .code("""
bool BurgerFactoryModel::checkQuality() const
{
    BurgerRecipe recipe = getRecipe(orderManager.getCurrentOrder().type);
    return productionLine.inspectQuality(preparedIngredients, recipe);
}
"""),
    .p("ProductionLine은 자신이 가지고 있는 QualityChecker에게 실제 검사를 맡긴다. 준비된 재료와 레시피가 정확히 일치하면 통과한다."),
    .p("포장 기계가 완료되면 PackingMachine은 자기 상태를 Completed로 바꿀 뿐, 돈 추가나 주문 완료 처리는 하지 않는다. 이 정리는 BurgerFactoryModel::nextStep()의 PackBurger case에서 처리한다."),
    .code("""
case ProcessStep::PackBurger:
{
    BurgerRecipe r = getRecipe(orderManager.getCurrentOrder().type);
    moneyManager.add(r.price);
    orderManager.completeOrder();
    productionTimerRunning = false;
    preparedIngredients.clear();
    qualityCheckPassed = false;
    if (orderManager.hasActiveOrder())
        enterStep(ProcessStep::PreparingIngredients);
    else
        enterStep(ProcessStep::Idle);
}
"""),

    .h2("7. upgrade, refill, repair 추가 기능"),
    .h3("Upgrade"),
    .p("기계 업그레이드는 cycleTime을 줄이고 malfunctionRate를 낮춰서 더 빠르고 안정적인 생산을 가능하게 한다. View의 upgrade 버튼은 Controller를 통해 BurgerFactoryModel::upgradeMachine()을 호출한다."),
    .h3("Refill"),
    .p("재고가 부족하면 InventoryManager가 재고를 보충한다. 비용은 REFILL_COST로 관리된다. 재고 관리 책임은 Model Coordinator가 아니라 InventoryManager에 있다."),
    .h3("Repair"),
    .p("각 Machine은 update 중 checkMalfunction()을 호출한다. 고장이 발생하면 MachineState::Failed가 되고, Repair 버튼을 통해 수리할 수 있다."),
    .flow("Machine::checkMalfunction()\n→ MachineState::Failed\n→ View Repair button\n→ Controller::onRepairMachine()\n→ BurgerFactoryModel::repairMachine()\n→ ProductionLine::repairMachine()\n→ Machine::reset()"),

    .h2("8. SOLID 원칙 연결"),
    .h3("S - Single Responsibility Principle"),
    .p("재고는 InventoryManager, 돈은 MoneyManager, 주문은 OrderManager, 기계 목록과 기계 실행은 ProductionLine이 담당한다. 그래서 모든 기능이 하나의 클래스에 몰리지 않도록 분리했다."),
    .h3("O - Open/Closed Principle"),
    .p("새로운 기계를 추가하려면 Machine을 상속하고 update()를 구현하면 된다. 기존 기계 코드를 크게 수정하지 않고 확장할 수 있다."),
    .h3("L - Liskov Substitution Principle"),
    .p("ProductionLine은 Machine 포인터로 모든 기계를 다룬다. 자식 기계들은 부모 Machine처럼 사용할 수 있으므로 LSP에 맞는다."),
    .h3("I - Interface Segregation Principle"),
    .p("View는 IFactoryViewData를 통해 조회 함수만 알고, Controller는 IFactoryActions를 통해 명령 함수만 안다. 그래서 클라이언트가 사용하지 않는 함수에 의존하지 않는다."),
    .h3("D - Dependency Inversion Principle"),
    .p("View와 Controller는 concrete class인 BurgerFactoryModel에 직접 의존하지 않고, 인터페이스에 의존한다. 이 구조는 Model 내부 구현이 바뀌어도 View와 Controller의 영향을 줄여준다.")
]

var mediaBox = CGRect(x: 0, y: 0, width: pageWidth, height: pageHeight)
guard let context = CGContext(URL(fileURLWithPath: outputPath) as CFURL, mediaBox: &mediaBox, nil) else {
    fatalError("Cannot create PDF context")
}

var y = margin

func beginPage() {
    context.beginPDFPage(nil)
    context.saveGState()
    context.translateBy(x: 0, y: pageHeight)
    context.scaleBy(x: 1, y: -1)
    NSGraphicsContext.current = NSGraphicsContext(cgContext: context, flipped: true)
    y = margin
}

func endPage() {
    context.restoreGState()
    context.endPDFPage()
}

func paragraphStyle(lineSpacing: CGFloat = 2.0) -> NSMutableParagraphStyle {
    let style = NSMutableParagraphStyle()
    style.lineSpacing = lineSpacing
    style.paragraphSpacing = 2.0
    return style
}

func attr(_ font: NSFont, _ color: NSColor = textColor, spacing: CGFloat = 2.0) -> [NSAttributedString.Key: Any] {
    return [
        .font: font,
        .foregroundColor: color,
        .paragraphStyle: paragraphStyle(lineSpacing: spacing)
    ]
}

func height(_ text: String, width: CGFloat, attrs: [NSAttributedString.Key: Any]) -> CGFloat {
    let rect = NSAttributedString(string: text, attributes: attrs).boundingRect(
        with: CGSize(width: width, height: CGFloat.greatestFiniteMagnitude),
        options: [.usesLineFragmentOrigin, .usesFontLeading]
    )
    return ceil(rect.height)
}

func ensure(_ needed: CGFloat) {
    if y + needed > bottomLimit {
        endPage()
        beginPage()
    }
}

func drawText(_ text: String, font: NSFont = bodyFont, color: NSColor = textColor, width: CGFloat = contentWidth, x: CGFloat = margin, before: CGFloat = 0, after: CGFloat = 7) {
    let attrs = attr(font, color)
    let h = height(text, width: width, attrs: attrs)
    ensure(before + h + after)
    y += before
    NSAttributedString(string: text, attributes: attrs).draw(with: CGRect(x: x, y: y, width: width, height: h + 4), options: [.usesLineFragmentOrigin, .usesFontLeading])
    y += h + after
}

func drawBox(_ text: String, font: NSFont, bg: NSColor, border: NSColor, before: CGFloat = 4, after: CGFloat = 9) {
    let pad: CGFloat = 8
    let attrs = attr(font, textColor, spacing: 1.5)
    let h = height(text, width: contentWidth - pad * 2, attrs: attrs)
    ensure(before + h + pad * 2 + after)
    y += before
    let rect = CGRect(x: margin, y: y, width: contentWidth, height: h + pad * 2)
    bg.setFill()
    NSBezierPath(roundedRect: rect, xRadius: 5, yRadius: 5).fill()
    border.setStroke()
    NSBezierPath(roundedRect: rect, xRadius: 5, yRadius: 5).stroke()
    NSAttributedString(string: text, attributes: attrs).draw(with: CGRect(x: rect.minX + pad, y: rect.minY + pad, width: rect.width - pad * 2, height: h + 4), options: [.usesLineFragmentOrigin, .usesFontLeading])
    y += rect.height + after
}

func drawRule() {
    ruleColor.setStroke()
    let path = NSBezierPath()
    path.lineWidth = 1.2
    path.move(to: CGPoint(x: margin, y: y))
    path.line(to: CGPoint(x: pageWidth - margin, y: y))
    path.stroke()
    y += 8
}

beginPage()
for block in blocks {
    switch block {
    case .title(let s):
        drawText(s, font: titleFont, color: titleColor, before: 0, after: 4)
        drawRule()
    case .h2(let s):
        ensure(54)
        y += 12
        drawText(s, font: h2Font, color: titleColor, before: 0, after: 2)
        drawRule()
    case .h3(let s):
        drawText(s, font: h3Font, color: titleColor, before: 6, after: 3)
    case .p(let s):
        drawText(s)
    case .bullets(let items):
        for item in items {
            drawText("• " + item, width: contentWidth - 10, x: margin + 10, before: 0, after: 3)
        }
        y += 3
    case .code(let s):
        drawBox(s, font: codeFont, bg: codeBg, border: NSColor(calibratedRed: 0.80, green: 0.86, blue: 0.92, alpha: 1))
    case .flow(let s):
        drawBox(s, font: codeFont, bg: NSColor.white, border: NSColor(calibratedRed: 0.45, green: 0.55, blue: 0.65, alpha: 1))
    case .note(let s):
        drawBox(s, font: bodyBold, bg: NSColor(calibratedRed: 0.96, green: 0.98, blue: 1.0, alpha: 1), border: NSColor(calibratedRed: 0.75, green: 0.83, blue: 0.90, alpha: 1))
    }
}
endPage()
context.closePDF()

