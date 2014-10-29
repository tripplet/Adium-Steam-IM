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
}

- (NSString *)pluginAuthor
{
	return @"Tobias Tangemann, Hermi <Hermi.Hg@gmail.com>, Eion Robb <eion@robbmob.com>";
}

-(NSString *)pluginVersion
{
	return @"1.8";
}

-(NSString *)pluginDescription
{
	return @"Steam";
}

@end