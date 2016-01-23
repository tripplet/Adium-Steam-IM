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

- (void)uninstallPlugin
{
}

- (void)installLibpurplePlugin
{
}

- (void)loadLibpurplePlugin
{
  AILog(@"Loading SteamIM plugin %s ("
        MBEDTLS_VERSION_STRING_FULL", "
        "pidgin-opensteamworks "STEAM_PLUGIN_VERSION
        ")",[[self pluginVersion] UTF8String]);
}

- (NSString *)pluginAuthor
{
	return @"Tobias Tangemann, Hermi <Hermi.Hg@gmail.com>, Eion Robb <eion@robbmob.com>";
}

-(NSString *)pluginVersion
{
	return @"v1.20";
}

-(NSString *)pluginDescription
{
	return @"Steam";
}

-(NSString *)pluginURL
{
  return @"https://github.com/tripplet/Adium-Steam-IM#readme";
}

@end