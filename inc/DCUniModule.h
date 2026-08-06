//
//  DCUniModule.h
//  uni-app iOS Plugin SDK Header
//
//  Original DCloud macro: static SEL = @selector() stored directly
//  in __DATA,__DCUniMethod section at compile time.
//
//  Requires Xcode 15.x (Clang 17) — Xcode 16+ rejects @selector()
//  as a static initializer.
//
//  Download official SDK: https://nativesupport.dcloud.net.cn/AppDocs/download/ios.html
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^UniModuleKeepAliveCallback)(id result, BOOL keepAlive);

@interface DCUniModule : NSObject

@end

// Token pasting helper for __COUNTER__ expansion
#define UNI_CAT_INNER(a, b) a##b
#define UNI_CAT(a, b) UNI_CAT_INNER(a, b)

// ---------------------------------------------------------------------------
// Original DCloud UNI_EXPORT_METHOD macro
//
// Stores the SEL value DIRECTLY in __DATA,__DCUniMethod at compile time.
// No runtime initialization needed — the SEL is baked into the binary.
// The uni-app runtime scans __DATA,__DCUniMethod at launch to discover
// registered method selectors.
//
// This is exactly how the official DCloud SDK works.
// ---------------------------------------------------------------------------

#define UNI_EXPORT_METHOD(method) \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniMethod"))) \
    static SEL UNI_CAT(_uni_sel_, __COUNTER__) = method

#define UNI_EXPORT_METHOD_SYNC(method) \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniMethodSync"))) \
    static SEL UNI_CAT(_uni_sel_sync_, __COUNTER__) = method

NS_ASSUME_NONNULL_END
