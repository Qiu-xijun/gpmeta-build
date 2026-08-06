//
//  DCUniModule.h
//  uni-app iOS Plugin SDK Header
//
//  STUB VERSION - Replace with real header from uni-app iOS SDK
//  Download: https://nativesupport.dcloud.net.cn/AppDocs/download/ios.html
//

#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#import <string.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^UniModuleKeepAliveCallback)(id result, BOOL keepAlive);

@interface DCUniModule : NSObject

@end

// Token pasting helper for __COUNTER__ expansion
#define UNI_CAT_INNER(a, b) a##b
#define UNI_CAT(a, b) UNI_CAT_INNER(a, b)

// ---------------------------------------------------------------------------
// Method Registration Strategy
//
// Xcode 15.3+ / 16 (Clang 18) rejects:  static SEL x = @selector(foo);
// so we cannot store SEL values directly in __DATA,__DCUniMethod at compile
// time.
//
// Instead we store a zero-filled SEL slot in __DATA,__DCUniMethod and use an
// __attribute__((constructor)) function (runs before main()) to fill the slot
// with sel_registerName().
//
// Each exported method gets its own constructor in the SAME object file as the
// SEL slot, so as long as the object file is linked the constructor will run.
//
// IMPORTANT: For this to work from a static library, the plugin class must be
// linked into the final executable. When using source files (placed in the
// plugin ios/ directory) HBuilderX compiles them as part of the app, so the
// class is always present and the constructors always run.
// ---------------------------------------------------------------------------

static inline SEL DCUniRegisterSel(const char *raw) {
    if (!raw) return NULL;

    const char *prefix = "@selector(";
    size_t plen = strlen(prefix);

    if (strncmp(raw, prefix, plen) == 0) {
        const char *inner = raw + plen;
        size_t len = strlen(inner);
        if (len > 0 && inner[len - 1] == ')') {
            char buf[256];
            len--;
            if (len >= sizeof(buf)) len = sizeof(buf) - 1;
            memcpy(buf, inner, len);
            buf[len] = '\0';
            return sel_registerName(buf);
        }
    }
    return sel_registerName(raw);
}

#define UNI_EXPORT_METHOD_INNER(method, counter) \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniMethod"))) \
    static SEL UNI_CAT(_uni_sel_, counter); \
    __attribute__((used)) \
    __attribute__((constructor)) \
    static void UNI_CAT(_uni_reg_, counter)(void) { \
        UNI_CAT(_uni_sel_, counter) = DCUniRegisterSel(#method); \
    }

#define UNI_EXPORT_METHOD(method) \
    UNI_EXPORT_METHOD_INNER(method, __COUNTER__)

#define UNI_EXPORT_METHOD_SYNC_INNER(method, counter) \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniMethodSync"))) \
    static SEL UNI_CAT(_uni_sel_sync_, counter); \
    __attribute__((used)) \
    __attribute__((constructor)) \
    static void UNI_CAT(_uni_reg_sync_, counter)(void) { \
        UNI_CAT(_uni_sel_sync_, counter) = DCUniRegisterSel(#method); \
    }

#define UNI_EXPORT_METHOD_SYNC(method) \
    UNI_EXPORT_METHOD_SYNC_INNER(method, __COUNTER__)

NS_ASSUME_NONNULL_END
