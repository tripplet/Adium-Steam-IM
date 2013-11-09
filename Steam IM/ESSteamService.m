/*
 *  ESSteamService.m
 *  AdiumSteamPlugin
 *
 *  Created by Hermi on 2012.08.12.
 *  Copyright 2012 Hermi. All rights reserved.
 *
 */

#import "ESSteamService.h"
#import "ESPurpleSteamAccount.h"

#import <Adium/AISharedAdium.h>
#import <Adium/AIStatusControllerProtocol.h>
#import <AIUtilities/AIStringUtilities.h>
#import <AIUtilities/AIImageAdditions.h>

@implementation ESSteamService
- (Class)accountClass {
	return [ESPurpleSteamAccount class];
}

//Service Description
- (NSString *)serviceCodeUniqueID {
	return @"libpurple-steam";
}
- (NSString *)serviceID{
	return @"Steam";
}
- (NSString *)serviceClass {
	return @"Steam";
}
- (NSString *)shortDescription {
	return @"Steam";
}
- (NSString *)longDescription {
	return @"Steam IM";
}

- (NSCharacterSet *)allowedCharacters {
  NSMutableCharacterSet *allowed = [NSMutableCharacterSet alphanumericCharacterSet];
  [allowed formUnionWithCharacterSet:[NSCharacterSet  punctuationCharacterSet]];
  return allowed;
}

- (NSUInteger)allowedLength {
	return 64;
}

- (BOOL)caseSensitive {
	return NO;
}
- (AIServiceImportance)serviceImportance {
	return AIServicePrimary;
}

- (NSString *)userNameLabel {
    return AILocalizedString(@"Account Name", nil); // Sign-in name
}

- (void)registerStatuses {
  #define ADDSTATUS(name, type) \
    [adium.statusController registerStatus:name \
    withDescription:[adium.statusController localizedDescriptionForCoreStatusName:name] \
    ofType:type forService:self]
  
  ADDSTATUS(STATUS_NAME_AVAILABLE, AIAvailableStatusType);
  ADDSTATUS(STATUS_NAME_BUSY, AIAvailableStatusType);
  ADDSTATUS(STATUS_NAME_AWAY, AIAwayStatusType);
  ADDSTATUS(STATUS_NAME_EXTENDED_AWAY, AIAwayStatusType);
  ADDSTATUS(STATUS_NAME_OFFLINE, AIOfflineStatusType);
}

- (NSImage *)defaultServiceIconOfType:(AIServiceIconType)iconType
{
  if ((iconType == AIServiceIconSmall) || (iconType == AIServiceIconList)) {
    return [NSImage imageNamed:@"steam_small" forClass:[self class] loadLazily:YES];
  } else {
    return [NSImage imageNamed:@"steam" forClass:[self class] loadLazily:YES];
  }
}

- (NSString *)pathForDefaultServiceIconOfType:(AIServiceIconType)iconType
{
  if ((iconType == AIServiceIconSmall) || (iconType == AIServiceIconList)) {
		return [[NSBundle bundleForClass:[self class]] pathForImageResource:@"steam_small"];
	}
	return [[NSBundle bundleForClass:[self class]] pathForImageResource:@"steam"];
}


@end