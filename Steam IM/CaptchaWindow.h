//
//  SteamSettingsViewController.h
//  Steam IM
//
//  Created by Tobias Tangemann on 21.09.14.
//  Copyright (c) 2014 Hermi. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "libsteam.h"

@interface CaptchaWindow : NSWindowController {
  IBOutlet NSImageView *imageCaptcha;
  IBOutlet NSTextField *txtCaptcha;
}

- (IBAction)tryCaptcha:(NSButton *)sender;

@end
