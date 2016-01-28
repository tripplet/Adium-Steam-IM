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
  AILog(@"Loading SteamIM plugin %s ("
        MBEDTLS_VERSION_STRING_FULL", "
        "pidgin-opensteamworks "STEAM_PLUGIN_VERSION
        ")",[[self pluginVersion] UTF8String]);
}

- (NSString *)pluginAuthor      { return @"Tobias Tangemann, Eion Robb <eion@robbmob.com>"; }
- (NSString *)pluginDescription { return @"Steam"; }
- (NSString *)pluginURL         { return @"https://github.com/tripplet/Adium-Steam-IM#readme"; }

- (NSString *)pluginVersion
{
  return [NSString stringWithFormat:@"v%@", [[[NSBundle bundleForClass:[self class]] infoDictionary] objectForKey:@"CFBundleVersion"]];
}

- (void)uninstallPlugin {}
- (void)installLibpurplePlugin {}

@end