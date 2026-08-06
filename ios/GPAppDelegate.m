#import <Foundation/Foundation.h>
#import <FBSDKCoreKit/FBSDKCoreKit.h>


@interface GPAppDelegate : NSObject

@end


@implementation GPAppDelegate


+ (void)load {
    [[FBSDKApplicationDelegate sharedInstance]
     application:nil
     didFinishLaunchingWithOptions:nil];

    // 启动时激活 Meta 追踪
    [[FBSDKAppEvents shared] activateApp];
}

- (BOOL)application:(UIApplication *)app
            openURL:(NSURL *)url
            options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options {
    return [[FBSDKApplicationDelegate sharedInstance]
            application:app
            openURL:url
            options:options];
}

- (BOOL)application:(UIApplication *)application
continueUserActivity:(NSUserActivity *)userActivity
  restorationHandler:(void (^)(NSArray<id<UIUserActivityRestoring>> * _Nullable))restorationHandler {
    id delegate = [FBSDKApplicationDelegate sharedInstance];
    if ([delegate respondsToSelector:@selector(application:continueUserActivity:restorationHandler:)]) {
        return [delegate application:application
                  continueUserActivity:userActivity
                    restorationHandler:restorationHandler];
    }
    return NO;
}


@end
