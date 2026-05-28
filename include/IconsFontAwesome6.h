#pragma once

// ============================================================
// IconsFontAwesome6.h - 이 프로젝트에서 사용하는 FA6 아이콘 정의
// Unicode 코드포인트를 UTF-8 문자열로 변환한 매크로입니다.
// fa-solid-900.ttf 파일과 함께 사용합니다.
// ============================================================

// ImGui에 폰트 범위를 알려주기 위한 최소/최대 코드포인트
#define ICON_MIN_FA 0xF000
#define ICON_MAX_FA 0xF8FF

// --- 기계 아이콘 ---
#define ICON_FA_UTENSILS    "\xef\x8b\xa7"  // U+F2E7  Prep Machine
#define ICON_FA_FIRE        "\xef\x81\xad"  // U+F06D  Grill
#define ICON_FA_FLASK       "\xef\x83\x83"  // U+F0C3  Sauce
#define ICON_FA_BOX_OPEN    "\xef\x92\x9e"  // U+F49E  Packing

// --- 인벤토리 아이콘 ---
#define ICON_FA_BREAD_SLICE "\xef\x9f\xac"  // U+F7EC  Buns
#define ICON_FA_BURGER      "\xef\xa0\x85"  // U+F805  Patties
#define ICON_FA_LEAF        "\xef\x81\xac"  // U+F06C  Lettuce
#define ICON_FA_APPLE_WHOLE "\xef\x97\x91"  // U+F5D1  Tomato (근사값)
#define ICON_FA_CHEESE      "\xef\x9f\xaf"  // U+F7EF  Cheese
#define ICON_FA_DROPLET     "\xef\x81\x83"  // U+F043  Sauce bottle

// --- UI 상태 아이콘 ---
#define ICON_FA_CIRCLE      "\xef\x84\x91"  // U+F111  상태 표시 점
#define ICON_FA_STAR        "\xef\x80\x85"  // U+F005  별점
#define ICON_FA_STAR_HALF   "\xef\x84\x89"  // U+F089  반별 (미사용)
