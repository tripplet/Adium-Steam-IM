/*
 *  ESPurpleSteamAccount.m
 *  AdiumSteamPlugin
 *
 *  Created by Hermi on 2012.08.12.
 *  Copyright 2012 Hermi. All rights reserved.
 *
 */

#import "ESPurpleSteamAccount.h"

@implementation ESPurpleSteamAccount

- (const char*)protocolPlugin
{
	return "prpl-steam-mobile";
}

- (PurpleAccount *)getPurpleAccount {
  return account;
}

// Bridge Adium settings over to the libpurple side
- (void) configurePurpleAccount
{
  [super configurePurpleAccount];
  
  NSString* steam_guard_code;
  
  //Default encoding
  if ((steam_guard_code = [self preferenceForKey:@"steam_guard_code" group:GROUP_ACCOUNT_STATUS])) {
    purple_account_set_string(account, "steam_guard_code", [steam_guard_code UTF8String]);
  }
    
  NSString* identifyAs;
  if ((identifyAs = [self preferenceForKey:@"ui_mode" group:GROUP_ACCOUNT_STATUS])) {
   purple_account_set_string(account, "ui_mode", [identifyAs UTF8String]);
  }

  purple_account_set_bool(account, "always_use_https", [[self preferenceForKey:@"always_use_https" group:GROUP_ACCOUNT_STATUS] boolValue]);
  purple_account_set_bool(account, "change_status_to_game", [[self preferenceForKey:@"change_status_to_game" group:GROUP_ACCOUNT_STATUS] boolValue]);
  purple_account_set_bool(account, "download_offline_history", [[self preferenceForKey:@"download_offline_history" group:GROUP_ACCOUNT_STATUS] boolValue]);
}

- (NSString *)host
{
	return @"api.steampowered.com";
}
@end
