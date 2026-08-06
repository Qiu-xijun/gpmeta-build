#import "GPMetaModule.h"
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKCoreKit/FBSDKAppLinkUtility.h>
#import <AppTrackingTransparency/AppTrackingTransparency.h>
#import <AdSupport/AdSupport.h>

@implementation GPMetaModule

UNI_EXPORT_METHOD(@selector(initMeta))
UNI_EXPORT_METHOD(@selector(testPlugin:))
UNI_EXPORT_METHOD(@selector(fetchDeferredLink:))
UNI_EXPORT_METHOD(@selector(requestTracking:))

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
