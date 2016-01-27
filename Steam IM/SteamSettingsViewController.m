//
//  SteamSettingsViewController.m
//  Steam IM
//
//  Created by Tobias Tangemann on 21.09.14

#import "SteamSettingsViewController.h"
#import <AdiumLibpurple/CBPurpleAccount.h>
#import "ESPurpleSteamAccount.h"

enum identifyAsTag {
    IDENTIFY_AS_MOBILE = 0,
    IDENTIFY_AS_WEB = 1
};

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
  [downloadOfflineHistory setState:[[account preferenceForKey:@"download_offline_history" group:GROUP_ACCOUNT_STATUS] boolValue]];

  enum identifyAsTag identifyTag = IDENTIFY_AS_WEB;
  if ([[account preferenceForKey:@"ui_mode" group:GROUP_ACCOUNT_STATUS] isEqualToString:@"mobile"])
    identifyTag = IDENTIFY_AS_MOBILE;
  [identifyAs selectCellWithTag:identifyTag];
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

  [account setPreference:[NSNumber numberWithBool:[downloadOfflineHistory state]]
                 forKey:@"download_offline_history" group:GROUP_ACCOUNT_STATUS];

  NSInteger identifyTag = [identifyAs selectedTag];
  NSString* identifyStr = nil;
  switch (identifyTag) {
    case IDENTIFY_AS_MOBILE:
      identifyStr = @"mobile";
      break;
    case IDENTIFY_AS_WEB:
      identifyStr = @"web";
      break;
    default:
      identifyStr = @"web";
  }
    
  if (identifyStr) {
    [account setPreference:identifyStr forKey:@"ui_mode" group:GROUP_ACCOUNT_STATUS];
  }
}

@end
