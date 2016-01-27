//
//  SteamSettingsViewController.h
//  Steam IM
//
//  Created by Tobias Tangemann on 21.09.14

#import <Cocoa/Cocoa.h>
#import <AdiumLibpurple/PurpleAccountViewController.h>

@interface SteamSettingsViewController : PurpleAccountViewController {
  IBOutlet NSButton *alwaysHTTPS;
  IBOutlet NSButton *changeIngameStatus;
  IBOutlet NSButton *downloadOfflineHistory;
  IBOutlet NSMatrix *identifyAs;
  IBOutlet NSTextField *steamGuardCode;
  IBOutlet NSView *optionsView;
}

@property (nonatomic, assign) IBOutlet NSView *optionsView;
@end
