//
//  SteamSettingsViewController.m
//  Steam IM
//
//  Created by Tobias Tangemann on 21.09.14.
//  Copyright (c) 2014 Hermi. All rights reserved.
//

#import "SteamSettingsViewController.h"
#import <AdiumLibpurple/CBPurpleAccount.h>
#import "ESPurpleSteamAccount.h"

@interface SteamSettingsViewController()
@end

@implementation SteamSettingsViewController
@synthesize optionsView;

- (NSView*) optionsView {
  return optionsView;
}

- (NSString *)nibName {
  return @"SteamOptions";
}

- (void)configureForAccount:(AIAccount *) inAccount
{
  [super configureForAccount:inAccount];
  
  // We need to get this from "purple_account_get_string" as it cant be updated by opensteamworks
  PurpleAccount *pAccount = [((ESPurpleSteamAccount*)inAccount) getPurpleAccount];
  [steamGuardCode setStringValue: [NSString stringWithUTF8String: purple_account_get_string(pAccount, "steam_guard_code", "")]];
  
  [alwaysHTTPS setState:[[account preferenceForKey:@"always_use_https" group:GROUP_ACCOUNT_STATUS] boolValue]];
  [changeIngameStatus setState:[[account preferenceForKey:@"change_status_to_game" group:GROUP_ACCOUNT_STATUS] boolValue]];
}

//Save controls
- (void)saveConfiguration
{
  [super saveConfiguration];
  
  if (steamGuardCode.stringValue.length > 0) {
    [account setPreference:steamGuardCode.stringValue
                    forKey:@"steam_guard_code" group:GROUP_ACCOUNT_STATUS];
  }
  
  [account setPreference:[NSNumber numberWithBool:[alwaysHTTPS state]]
                  forKey:@"always_use_https" group:GROUP_ACCOUNT_STATUS];
  
  [account setPreference:[NSNumber numberWithBool:[changeIngameStatus state]]
                  forKey:@"change_status_to_game" group:GROUP_ACCOUNT_STATUS];
}

@end
