//
//  DCUniModule.h
//  uni-app iOS Plugin SDK Header
//
//  STUB VERSION - Replace with real header from uni-app iOS SDK
//  Download: https://nativesupport.dcloud.net.cn/AppDocs/download/ios.html
//

#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#import <mach-o/getsect.h>
#import <mach-o/dyld.h>
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
// Instead we store:
//   1. A zero-filled SEL slot in __DATA,__DCUniMethod  (what the uni-app
//      runtime reads — must be an array of SEL values)
//   2. A registration record in __DATA,__DCUniReg that contains:
//        - a pointer to the SEL slot above
//        - the stringified selector name
//
// In +load (which runs before the uni-app runtime scans the section) we call
// DCUniRegisterAllMethods() which iterates every loaded Mach-O image, finds
// __DCUniReg sections, calls sel_registerName() on each name, and writes the
// resulting SEL into the slot pointed to by sel_ptr.
//
// This pointer-based approach avoids any index-alignment issues that would
// arise if the uni-app SDK also contributes entries to __DCUniMethod.
// ---------------------------------------------------------------------------

typedef struct {
    SEL *sel_ptr;        // pointer to the SEL slot in __DCUniMethod
    const char *name;    // stringified selector name, e.g. "@selector(initMeta)"
} UniMethodReg;

#define UNI_EXPORT_METHOD_INNER(method, counter) \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniMethod"))) \
    static SEL UNI_CAT(UNI_SEL_, counter); \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniReg"))) \
    static UniMethodReg UNI_CAT(UNI_REG_, counter) = { \
        .sel_ptr = &UNI_CAT(UNI_SEL_, counter), \
        .name = #method \
    };

#define UNI_EXPORT_METHOD(method) \
    UNI_EXPORT_METHOD_INNER(method, __COUNTER__)

#define UNI_EXPORT_METHOD_SYNC_INNER(method, counter) \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniMethodSync"))) \
    static SEL UNI_CAT(UNI_SEL_SYNC_, counter); \
    __attribute__((used)) \
    __attribute__((section("__DATA,__DCUniRegSync"))) \
    static UniMethodReg UNI_CAT(UNI_REG_SYNC_, counter) = { \
        .sel_ptr = &UNI_CAT(UNI_SEL_SYNC_, counter), \
        .name = #method \
    };

#define UNI_EXPORT_METHOD_SYNC(method) \
    UNI_EXPORT_METHOD_SYNC_INNER(method, __COUNTER__)

// ---------------------------------------------------------------------------
// Runtime helper — call once from +load
// ---------------------------------------------------------------------------

// Strip the "@selector(" prefix and ")" suffix from a stringified selector.
// If the input doesn't have the wrapper, use it as-is.
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

static inline void DCUniFillRegistrations(const struct mach_header *mh,
                                          const char *reg_sect) {
    unsigned long reg_sz = 0;
    UniMethodReg *regs = (UniMethodReg *)getsectiondata(
        mh, "__DATA", reg_sect, &reg_sz);
    if (!regs || reg_sz == 0) return;

    size_t n = reg_sz / sizeof(UniMethodReg);
    for (size_t i = 0; i < n; i++) {
        if (!regs[i].sel_ptr) continue;           // no target slot
        if (*regs[i].sel_ptr != NULL) continue;    // already filled
        if (!regs[i].name) continue;               // no name

        *regs[i].sel_ptr = DCUniRegisterSel(regs[i].name);
    }
}

#define DCUniRegisterAllMethods() do { \
    uint32_t _img_count = _dyld_image_count(); \
    for (uint32_t _i = 0; _i < _img_count; _i++) { \
        const struct mach_header *_mh = _dyld_get_image_header(_i); \
        if (!_mh) continue; \
        DCUniFillRegistrations(_mh, "__DCUniReg"); \
        DCUniFillRegistrations(_mh, "__DCUniRegSync"); \
    } \
} while (0)

NS_ASSUME_NONNULL_END
