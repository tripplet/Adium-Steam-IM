//
//  SteamSettingsViewController.h
//  Steam IM
//
//  Created by Tobias Tangemann on 21.09.14.
//  Copyright (c) 2014 Hermi. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <AdiumLibpurple/PurpleAccountViewController.h>

@interface SteamSettingsViewController : PurpleAccountViewController {
  NSButton *alwaysHTTPS;
  NSButton *changeIngameStatus;
  
  NSView *optionsView;
}
@property (nonatomic, assign) IBOutlet NSView *optionsView;

@end
