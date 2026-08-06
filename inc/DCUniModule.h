//
//  DCUniModule.h
//  uni-app iOS Plugin SDK Header
//
//  STUB VERSION - Replace with real header from uni-app iOS SDK
//  Download: https://nativesupport.dcloud.net.cn/AppDocs/download/ios.html
//  After downloading, copy ALL files from SDK/inc/ to this inc/ directory.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^UniModuleKeepAliveCallback)(id result, BOOL keepAlive);

@interface DCUniModule : NSObject

@end

// Token pasting helper for __COUNTER__ expansion
#define UNI_CAT_INNER(a, b) a##b
#define UNI_CAT(a, b) UNI_CAT_INNER(a, b)

// Store the selector directly in the registration section.
// This is the original DCloud macro definition.
// It requires Xcode 15.x or earlier (Clang 17) which treats
// @selector() as a valid compile-time constant for static initializers.
// Xcode 16+ (Clang 18) rejects this, so the CI workflow must use Xcode 15.x.
#define UNI_EXPORT_METHOD(method) \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniMethod"))) \
    static SEL UNI_CAT(UNI_EXPORT_METHOD_, __COUNTER__) = method

#define UNI_EXPORT_METHOD_SYNC(method) \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniMethodSync"))) \
    static SEL UNI_CAT(UNI_EXPORT_METHOD_SYNC_, __COUNTER__) = method

NS_ASSUME_NONNULL_END
