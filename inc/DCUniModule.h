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
// (Constructor-based registration does not reliably run before the
// uni-app runtime scans __DATA,__DCUniMethod, so we use a static
// initializer with the SEL type to avoid the SEL->void* cast that
// newer Xcode rejects.)
#define UNI_EXPORT_METHOD(method) \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniMethod"))) \
    static SEL UNI_CAT(UNI_EXPORT_METHOD_, __COUNTER__) = method

#define UNI_EXPORT_METHOD_SYNC(method) \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniMethodSync"))) \
    static SEL UNI_CAT(UNI_EXPORT_METHOD_SYNC_, __COUNTER__) = method

NS_ASSUME_NONNULL_END
