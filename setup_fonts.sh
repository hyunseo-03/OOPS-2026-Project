#!/bin/bash
set -e

FONTS_DIR="libs/fonts"
mkdir -p "$FONTS_DIR"

echo "[1/2] Font Awesome TTF 다운로드 중..."
curl -L "https://use.fontawesome.com/releases/v6.5.1/webfonts/fa-solid-900.ttf" \
     -o "$FONTS_DIR/fa-solid-900.ttf" \
     --progress-bar

echo ""
echo "[2/2] 다운로드 완료!"
echo ""
echo "이제 다음 명령어로 커밋하세요:"
echo "  git add libs/fonts/fa-solid-900.ttf"
echo "  git commit -m 'Add Font Awesome TTF for icons'"
echo ""
