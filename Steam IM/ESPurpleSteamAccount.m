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

/*
- (NSSet *)supportedPropertyKeys
{
	static NSMutableSet *supportedPropertyKeys = nil;
	
	if (!supportedPropertyKeys) {
		supportedPropertyKeys = [[NSMutableSet alloc] initWithObjects:
								 @"AvailableMessage",
								 @"textProfile",
								 nil];
		[supportedPropertyKeys unionSet:[super supportedPropertyKeys]];
	}
	
	return supportedPropertyKeys;
}
*/

 
- (NSString *)host
{
	return @"api.steampowered.com";
}
@end
