#import "GPMetaModule.h"
#import <SafariServices/SafariServices.h>
#import <AuthenticationServices/AuthenticationServices.h>
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKCoreKit/FBSDKCoreKit-Swift.h>
#import <FBSDKCoreKit/FBSDKAppLinkUtility.h>
#import <AppTrackingTransparency/AppTrackingTransparency.h>
#import <AdSupport/AdSupport.h>

/*
 * uni-app Method Registration — Objective-C++ (.mm) approach
 *
 * Problem: Xcode 15+ (Clang 16) rejects `static SEL = @selector()` in .m files
 *          as "initializer element is not a compile-time constant".
 *
 * Solution: Use .mm (Objective-C++) extension. C++ allows non-constant
 *           static initialization, so `static SEL = @selector()` compiles.
 *           The SEL is resolved during image initialization (before main,
 *           before +load), so values are valid when uni-app scans
 *           __DATA,__DCUniMethod.
 *           Works with ALL Xcode versions.
 */

@implementation GPMetaModule

UNI_EXPORT_METHOD(@selector(initMeta))
UNI_EXPORT_METHOD(@selector(testPlugin:))
UNI_EXPORT_METHOD(@selector(fetchDeferredLink:))
UNI_EXPORT_METHOD(@selector(requestTracking:))

- (void)requestTracking:(UniModuleKeepAliveCallback)callback
{
    void (^fetchMetaLinkAfterTracking)(NSNumber *) = ^(NSNumber *trackingStatus) {
        [[FBSDKApplicationDelegate sharedInstance] application:nil didFinishLaunchingWithOptions:nil];
        [[FBSDKAppEvents shared] activateApp];

        [FBSDKAppLinkUtility fetchDeferredAppLink:^(NSURL *url, NSError *error){
            if (!callback) {
                return;
            }
            if (error) {
                callback(@{@"trackingStatus": trackingStatus, @"status": @"error", @"message": error.localizedDescription}, NO);
                return;
            }
            if (url) {
                callback(@{@"trackingStatus": trackingStatus, @"status": @"success", @"url": url.absoluteString}, NO);
                return;
            }
            callback(@{@"trackingStatus": trackingStatus, @"status": @"empty", @"message": @"No deferred link found"}, NO);
        }];
    };

    if (@available(iOS 14, *)) {
        [ATTrackingManager requestTrackingAuthorizationWithCompletionHandler:^(ATTrackingManagerAuthorizationStatus status) {
            fetchMetaLinkAfterTracking(@(status));
        }];
    } else {
        fetchMetaLinkAfterTracking(@(3));
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
    [[FBSDKAppEvents shared] activateApp();

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
