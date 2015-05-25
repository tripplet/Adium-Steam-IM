//
//  SteamSettingsViewController.m
//  Steam IM
//
//  Created by Tobias Tangemann on 21.09.14.
//  Copyright (c) 2014 Hermi. All rights reserved.
//

#import "CaptchaWindow.h"

@interface CaptchaWindow()
@end

@implementation CaptchaWindow

- (id)init
{
  self.window = [NSBundle loadNibNamed:nil owner:self];
  return self;
}

- (IBAction)tryCaptcha:(NSButton *)sender
{
  NSLog(@"Test");
}

@end
