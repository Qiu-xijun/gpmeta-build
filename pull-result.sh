#!/bin/bash
#
# 拉取编译好的 libGPMetaModule.a
#
# 使用方法: bash pull-result.sh
#

set -e

export PATH="$PATH:/c/Program Files/GitHub CLI"

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
MAIN_PROJECT_IOS="D:/Project/goldpay.sg_mobile_new-sg/nativeplugins/GPMetaModule/ios"

echo "=========================================="
echo "  Pull libGPMetaModule.a from GitHub"
echo "=========================================="
echo ""

# 检查登录
if ! gh auth status &> /dev/null; then
    echo "[ERROR] Not logged into GitHub. Run: gh auth login"
    exit 1
fi

cd "$SCRIPT_DIR"

# 拉取最新代码（.a 会被自动 commit 回仓库）
echo "Pulling latest build..."
git pull origin main 2>/dev/null || echo "[INFO] Pull done or nothing to pull"

# 检查 .a 是否存在
if [ -f "ios/libGPMetaModule.a" ]; then
    echo ""
    echo "[OK] Found libGPMetaModule.a"
    lipo -info ios/libGPMetaModule.a 2>/dev/null || echo "(lipo not available on Windows, checking file size instead)"
    ls -lh ios/libGPMetaModule.a

    # 复制到主项目
    echo ""
    echo "Copying to main project..."
    cp ios/libGPMetaModule.a "$MAIN_PROJECT_IOS/libGPMetaModule.a"

    echo ""
    echo "=========================================="
    echo "  Done!"
    echo "=========================================="
    echo ""
    echo "libGPMetaModule.a has been copied to:"
    echo "  $MAIN_PROJECT_IOS/libGPMetaModule.a"
    echo ""
    echo "Now rebuild in HBuilderX (cloud packaging)."
    echo ""
else
    echo ""
    echo "[WARNING] libGPMetaModule.a not found."
    echo "The build may still be running. Check:"
    echo "  https://github.com/$(gh api user --jq .login)/gpmeta-build/actions"
    echo ""
    echo "Or download manually from Actions -> Artifacts"
    echo ""
fi
