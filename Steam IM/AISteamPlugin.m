/*
 *  AISteamPlugin.m
 *  AdiumSteamPlugin
 *
 *  Created by Hermi on 2012.08.12.
 *  Copyright 2012 Hermi. All rights reserved.
 *
 */

#import "AISteamPlugin.h"
#import "ESSteamService.h"
#import "libsteam.h"
#import "mbedtls/version.h"
#import "NSString+CompareToVersion.h"

#import <AppKit/NSWorkspace.h>

#import <Adium/ESDebugAILog.h>

extern void purple_init_steam_plugin();

@implementation AISteamPlugin

- (void)installPlugin
{
  purple_init_steam_plugin();
  [ESSteamService registerService];
}

- (void)loadLibpurplePlugin
{
  [NSUserNotificationCenter defaultUserNotificationCenter].delegate = self;

  AILog(@"Loading SteamIM plugin %s ("MBEDTLS_VERSION_STRING_FULL", pidgin-opensteamworks "STEAM_PLUGIN_VERSION")",
        [[self pluginVersion] UTF8String]);

  [self updateCheck];
}

- (void) updateCheck
{
    NSString *url_updateCheck = @"https://api.github.com/repos/tripplet/Adium-Steam-IM/releases/latest";

    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:[NSURL URLWithString:url_updateCheck]];
    __block NSDictionary *json;

    [NSURLConnection sendAsynchronousRequest:request
                                       queue:[NSOperationQueue mainQueue]
                           completionHandler:^(NSURLResponse *response, NSData *data, NSError *connectionError)
     {
         json = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];

         NSString *latestVersion = [json objectForKey:@"name"];
         BOOL isPrerelease = [[json objectForKey:@"prerelease"] boolValue];

         if (!isPrerelease && [[self pluginVersion] isOlderThanVersion:latestVersion]) {
             [self showUpdateNotification];
         }
     }];
}

- (void) showUpdateNotification
{
    NSUserNotification *notify = [[NSUserNotification alloc] init];
    
    notify.title = @"Adium - Steam IM Plugin";
    notify.subtitle = @"Update available";
    notify.informativeText = @"Click to visit download page";
    notify.identifier = @"ADIUMSTEAMIM-UPDATE-GITHUB";
    notify.hasActionButton = NO;
    
    [[NSUserNotificationCenter defaultUserNotificationCenter] deliverNotification: notify];
}

- (BOOL) userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification
{
    // always show notifications, even with app in foreground
    return YES;
}

- (void)userNotificationCenter:(NSUserNotificationCenter *)center didActivateNotification:(NSUserNotification *)notification
{
    if ([notification.identifier isEqualToString:@"ADIUMSTEAMIM-UPDATE-GITHUB"]) {
        [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"https://github.com/tripplet/Adium-Steam-IM/releases"]];
    }
}

- (NSString *)pluginAuthor      { return @"Tobias Tangemann, Eion Robb <eion@robbmob.com>"; }
- (NSString *)pluginDescription { return @"Steam"; }
- (NSString *)pluginURL         { return @"https://github.com/tripplet/Adium-Steam-IM#readme"; }
- (NSString *)pluginVersion     { return [[[NSBundle bundleForClass:[self class]] infoDictionary] objectForKey:@"CFBundleVersion"]; }

- (void)uninstallPlugin {}
- (void)installLibpurplePlugin {}

@end