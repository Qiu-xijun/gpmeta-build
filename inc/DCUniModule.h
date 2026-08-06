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

// Store the selector in the registration section.  We avoid the
// SEL->void* pointer cast that Xcode 26 rejects by casting to an
// integer of the same size (uintptr_t).  The uni-app runtime reads
// this section as pointer-sized values and casts back to SEL.
#define UNI_EXPORT_METHOD(method) \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniMethod"))) \
    static uintptr_t UNI_CAT(UNI_EXPORT_METHOD_, __COUNTER__) = (uintptr_t)(method)

#define UNI_EXPORT_METHOD_SYNC(method) \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniMethodSync"))) \
    static uintptr_t UNI_CAT(UNI_EXPORT_METHOD_SYNC_, __COUNTER__) = (uintptr_t)(method)

NS_ASSUME_NONNULL_END
