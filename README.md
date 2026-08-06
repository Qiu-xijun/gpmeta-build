# GPMetaModule iOS 静态库构建仓库

这个独立仓库用于在 GitHub Actions (免费 macOS runner) 上编译 `libGPMetaModule.a` 静态库。

主项目在 Gitee 上，Gitee 没有免费的 macOS CI，所以需要用 GitHub 来编译。

## 目录结构

```
ios-build/
├── .github/workflows/
│   └── build-ios-plugin.yml   ← GitHub Actions 自动编译脚本
├── .gitignore
├── inc/
│   └── DCUniModule.h          ← uni-app SDK 头文件（当前是 stub，需替换）
├── ios/
│   ├── GPMetaModule.h         ← 插件头文件
│   ├── GPMetaModule.m         ← 插件实现
│   └── GPAppDelegate.m        ← AppDelegate 钩子
├── project.yml                ← xcodegen 工程配置
├── Podfile                    ← CocoaPods 依赖 (FBSDKCoreKit)
└── README.md
```

## 使用步骤

### 1. 替换 DCUniModule.h（重要！）

当前 `inc/DCUniModule.h` 是 stub，能编译但运行时方法注册可能不正确。

1. 打开 https://nativesupport.dcloud.net.cn/AppDocs/download/ios.html
2. 下载 uni-app iOS 离线 SDK（百度网盘）
3. 解压后把 `SDK/inc/` 里的**所有头文件**拷贝到 `inc/` 目录，覆盖 stub

### 2. 创建 GitHub 仓库

1. 登录 https://github.com → New repository
2. 仓库名随便取，比如 `gpmeta-build`
3. **必须选 Public**（免费 macOS runner 只对公开仓库免费）
4. 不要勾选 "Add a README"

### 3. 推送代码

打开 Git Bash，执行：

```bash
cd "D:/Project/goldpay.sg_mobile_new-sg/nativeplugins/GPMetaModule/ios-build"

git init
git add .
git commit -m "Initial commit: GPMetaModule iOS build"

# 替换成你的 GitHub 仓库地址
git remote add origin https://github.com/你的用户名/gpmeta-build.git
git branch -M main
git push -u origin main
```

Push 之后 GitHub Actions 会自动开始编译（约 3-5 分钟）。

### 4. 获取编译产物

编译完成后有两种方式获取 `.a`：

**方式 A：直接 git pull（推荐）**

编译成功后 .a 会自动 commit 回仓库：

```bash
cd "D:/Project/goldpay.sg_mobile_new-sg/nativeplugins/GPMetaModule/ios-build"
git pull
```

**方式 B：从 Actions 页面下载**

1. 打开 GitHub 仓库页面 → Actions 标签
2. 点击最新的运行记录
3. 在页面底部 "Artifacts" 下载 `libGPMetaModule-a`

### 5. 放入主项目

把 `.a` 文件拷贝到主项目的插件目录：

```bash
cp "D:/Project/goldpay.sg_mobile_new-sg/nativeplugins/GPMetaModule/ios-build/ios/libGPMetaModule.a" \
   "D:/Project/goldpay.sg_mobile_new-sg/nativeplugins/GPMetaModule/ios/libGPMetaModule.a"
```

然后在 HBuilderX 中重新云打包即可。

## 修改源码后重新编译

如果修改了 `GPMetaModule.m` 或 `GPAppDelegate.m`：

1. 把修改后的文件复制到 `ios-build/ios/`
2. 在 `ios-build` 目录里 `git add . && git commit -m "update" && git push`
3. 等待 GitHub Actions 自动编译完成
4. `git pull` 获取新的 `.a`
5. 复制到主项目

## 检查编译结果

编译完成后，`.a` 文件应该包含以下架构：

```
Architectures in the fat file: libGPMetaModule.a are: arm64 x86_64
```

导出的符号应包含：

```
GPMetaModule (initMeta, testPlugin:, fetchDeferredLink:, requestTracking:)
GPAppDelegate (load, application:openURL:options:, application:continueUserActivity:restorationHandler:)
```
