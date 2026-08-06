#import "GPMetaModule.h"
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKCoreKit/FBSDKAppLinkUtility.h>
#import <AppTrackingTransparency/AppTrackingTransparency.h>
#import <AdSupport/AdSupport.h>

/*
 * uni-app Method Registration via +load
 *
 * Problem: Xcode 15+ (Clang 16) rejects `static SEL x = @selector(...)`
 *          as "initializer element is not a compile-time constant".
 *
 * Solution: Initialize SEL variables to NULL in __DATA,__DCUniMethod,
 *           then fill them in +load (runs before uni-app scans the section).
 *           Works with ALL Xcode versions.
 */
__attribute__((used))
__attribute__((section("__DATA,__DCUniMethod")))
static SEL _gp_sel_initMeta = NULL;

__attribute__((used))
__attribute__((section("__DATA,__DCUniMethod")))
static SEL _gp_sel_testPlugin = NULL;

__attribute__((used))
__attribute__((section("__DATA,__DCUniMethod")))
static SEL _gp_sel_fetchDeferredLink = NULL;

__attribute__((used))
__attribute__((section("__DATA,__DCUniMethod")))
static SEL _gp_sel_requestTracking = NULL;

@implementation GPMetaModule

+ (void)load
{
    _gp_sel_initMeta         = @selector(initMeta);
    _gp_sel_testPlugin       = @selector(testPlugin:);
    _gp_sel_fetchDeferredLink = @selector(fetchDeferredLink:);
    _gp_sel_requestTracking  = @selector(requestTracking:);
}

- (void)requestTracking:(UniModuleKeepAliveCallback)callback
{
    if (@available(iOS 14, *)) {
        [ATTrackingManager requestTrackingAuthorizationWithCompletionHandler:^(ATTrackingManagerAuthorizationStatus status) {
            if (callback) {
                callback(@{@"status": @(status)}, NO);
            }
        }];
    } else {
        if (callback) {
            callback(@{@"status": @(3)}, NO);
        }
    }
}

- (void)initMeta
{
    [[FBSDKApplicationDelegate sharedInstance]
                  application:nil
didFinishLaunchingWithOptions:nil
    ];
    [[FBSDKAppEvents shared] activateApp];
}

- (void)testPlugin:(UniModuleKeepAliveCallback)callback
{
    callback(@{@"status":@"success", @"message":@"GPMetaModule is Active"}, NO);
}

- (void)fetchDeferredLink:(UniModuleKeepAliveCallback)callback
{
    [[FBSDKApplicationDelegate sharedInstance] application:nil didFinishLaunchingWithOptions:nil];
    [[FBSDKAppEvents shared] activateApp];

    [FBSDKAppLinkUtility fetchDeferredAppLink:^(NSURL *url, NSError *error){
        if(error) {
            callback(@{@"status":@"error", @"message":error.localizedDescription}, NO);
            return;
        }
        if(url) {
            callback(@{@"status":@"success", @"url":url.absoluteString}, NO);
            return;
        }
        callback(@{@"status":@"empty", @"message":@"No deferred link found"}, NO);
    }];
}

@end
