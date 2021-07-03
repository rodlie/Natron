/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <https://natrongithub.github.io/>,
 * (C) 2018-2021 The Natron developers
 * (C) 2013-2018 INRIA and Alexandre Gauthier-Foichat
 *
 * Natron is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Natron is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Natron.  If not, see <http://www.gnu.org/licenses/gpl-2.0.html>
 * ***** END LICENSE BLOCK ***** */

#include "PowerManagementMac.h"

CLANG_DIAG_OFF(deprecated)
#include <QApplication>
#include <QWidget>
CLANG_DIAG_ON(deprecated)

#include <IOKit/pwr_mgt/IOPMLib.h>
#include <Cocoa/Cocoa.h>

@interface AppObjC : NSObject {
}

- (id) init;
@end

@implementation AppObjC
- (id) init
{
	NSAutoreleasePool *pool = [NSAutoreleasePool new];
	[super init];
	[[NSAppleEventManager sharedAppleEventManager] setEventHandler:self
		andSelector:@selector(appReopen:withReplyEvent:)
		forEventClass:kCoreEventClass
		andEventID:kAEReopenApplication];
	[pool release];
	return self;
}

- (void) dealloc
{
	NSAutoreleasePool *pool = [NSAutoreleasePool new];
	[[NSAppleEventManager sharedAppleEventManager] removeEventHandlerForEventClass:kCoreEventClass
		andEventID:kAEReopenApplication];
	[pool release];
	[super dealloc];
}
@end

NATRON_NAMESPACE_ENTER

struct PowerManagementMac::PMD
{
    AppObjC *objc;
    bool eventsLoaded;
    IOPMAssertionID aid = 0;
    CFMutableDictionaryRef prop = 0;
    CFNumberRef on = 0, off = 0;
};

PowerManagementMac::PowerManagementMac(QObject *parent):
    QObject(parent)
  , pmd(new PMD)
{
    pmd->eventsLoaded = false;
    pmd->objc = [[AppObjC alloc] init];
}

PowerManagementMac::~PowerManagementMac()
{
    if (pmd->aid) {
        IOPMAssertionRelease(pmd->aid);
    }
    if (pmd->on) {
        CFRelease(pmd->on);
    }
    if (pmd->off) {
        CFRelease(pmd->off);
    }
    [pmd->objc release];
    delete pmd;
}

void
PowerManagementMac::setScreensaverDisabled(bool disabled)
{
    if (disabled == (pmd->aid != 0)) {
        return;
    }
    if (pmd->aid) {
        IOPMAssertionRelease(pmd->aid);
        pmd->aid = 0;
    }
    if (disabled) {
        IOPMAssertionCreateWithName(kIOPMAssertionTypePreventUserIdleDisplaySleep,
                                    kIOPMAssertionLevelOn,
                                    CFSTR("fr.inria.Natron.playing"),
                                    &pmd->aid);
    }
}

void
PowerManagementMac::setSystemSleepDisabled(bool disabled)
{
    if ( disabled == (pmd->aid != 0) ) {
        return;
    }
    if (pmd->aid) {
        IOPMAssertionRelease(pmd->aid);
        pmd->aid = 0;
    }
    if (disabled) {
        IOPMAssertionCreateWithName(kIOPMAssertionTypePreventUserIdleSystemSleep,
                                    kIOPMAssertionLevelOn,
                                    CFSTR("fr.inria.Natron.rendering"),
                                    &pmd->aid);
    }
}

NATRON_NAMESPACE_EXIT
