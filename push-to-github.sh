#!/bin/bash
#
# GPMetaModule iOS 静态库 - GitHub Actions 一键构建脚本
#
# 使用方法:
#   1. 先运行: gh auth login  (只需一次)
#   2. 然后运行: bash push-to-github.sh
#

set -e

# GitHub CLI 路径
export PATH="$PATH:/c/Program Files/GitHub CLI"

REPO_NAME="gpmeta-build"
REPO_DESC="GPMetaModule iOS static library build (auto-generated)"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "=========================================="
echo "  GPMetaModule iOS Build - Push to GitHub"
echo "=========================================="
echo ""

# 检查 gh 是否已安装
if ! command -v gh &> /dev/null; then
    echo "[ERROR] GitHub CLI not found. Please install it first:"
    echo "  winget install --id GitHub.cli"
    exit 1
fi

# 检查是否已登录
if ! gh auth status &> /dev/null; then
    echo "[ERROR] You are not logged into GitHub."
    echo ""
    echo "Please run this command first to login:"
    echo ""
    echo "  gh auth login"
    echo ""
    echo "Choose: GitHub.com -> HTTPS -> Login with browser"
    echo ""
    exit 1
fi

echo "[OK] GitHub authenticated as: $(gh api user --jq .login)"
echo ""

# 创建仓库
echo "Creating GitHub repository: $REPO_NAME ..."
gh repo create "$REPO_NAME" --public --description "$REPO_DESC" 2>/dev/null || {
    echo "[INFO] Repository may already exist, continuing..."
}

# 获取仓库 URL
GH_USER=$(gh api user --jq .login)
REMOTE_URL="https://github.com/$GH_USER/$REPO_NAME.git"

echo "[OK] Repository: $REMOTE_URL"
echo ""

# 设置 remote 并推送
cd "$SCRIPT_DIR"

# 重置 git remote
git remote remove origin 2>/dev/null || true
git remote add origin "$REMOTE_URL"

echo "Pushing code to GitHub..."
git push -u origin main --force

echo ""
echo "=========================================="
echo "  Push complete!"
echo "=========================================="
echo ""
echo "GitHub Actions is now building your .a file."
echo "This takes about 3-5 minutes."
echo ""
echo "Watch the build at:"
echo "  https://github.com/$GH_USER/$REPO_NAME/actions"
echo ""
echo "After the build completes, run:"
echo "  bash pull-result.sh"
echo ""
echo "Or manually download from Actions -> Artifacts"
echo ""
