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

// Use constructor to initialize section variables at runtime
// (newer Xcode doesn't allow @selector() as static initializer)
#define UNI_EXPORT_METHOD_INNER(method, counter) \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniMethod"))) \
    static SEL UNI_CAT(UNI_EXPORT_METHOD_, counter); \
    __attribute__((constructor)) \
    static void UNI_CAT(UNI_INIT_, counter)(void) { \
        UNI_CAT(UNI_EXPORT_METHOD_, counter) = method; \
    }

#define UNI_EXPORT_METHOD(method) \
    UNI_EXPORT_METHOD_INNER(method, __COUNTER__)

#define UNI_EXPORT_METHOD_SYNC_INNER(method, counter) \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniMethodSync"))) \
    static SEL UNI_CAT(UNI_EXPORT_METHOD_SYNC_, counter); \
    __attribute__((constructor)) \
    static void UNI_CAT(UNI_INIT_SYNC_, counter)(void) { \
        UNI_CAT(UNI_EXPORT_METHOD_SYNC_, counter) = method; \
    }

#define UNI_EXPORT_METHOD_SYNC(method) \
    UNI_EXPORT_METHOD_SYNC_INNER(method, __COUNTER__)

NS_ASSUME_NONNULL_END
