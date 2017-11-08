
#import <FBControlCore/FBControlCore.h>
#import <FBSimulatorControl/FBSimulatorControl.h>
#import <FBDeviceControl/FBDeviceControl.h>
#import <Foundation/Foundation.h>
#import <XCTestBootstrap/XCTestBootstrap.h>
#import "iOSReturnStatusCode.h"
#import "CodesignIdentity.h"

@interface FBProcessOutputConfiguration (iOSDeviceManagerAdditions)

+ (FBProcessOutputConfiguration *)defaultForDeviceManager;

@end

@class MobileProfile;
@class Application;

@interface Device : NSObject

@property (nonatomic, strong) NSString *uuid;
@property (nonatomic, strong) NSString *name;
@property BOOL testingComplete;

+ (instancetype)withID:(NSString *)uuid;
+ (iOSReturnStatusCode)generateXCAppDataBundleAtPath:(NSString *)path
                                           overwrite:(BOOL)overwrite;

- (iOSReturnStatusCode)installApp:(Application *)app shouldUpdate:(BOOL)shouldUpdate;
- (iOSReturnStatusCode)installApp:(Application *)app
                resourcesToInject:(NSArray<NSString *> *)resourcePaths
                     shouldUpdate:(BOOL)shouldUpdate;
- (iOSReturnStatusCode)installApp:(Application *)app
                    mobileProfile:(MobileProfile *)profile
                     shouldUpdate:(BOOL)shouldUpdate;
- (iOSReturnStatusCode)installApp:(Application *)app
                    mobileProfile:(MobileProfile *)profile
                resourcesToInject:(NSArray<NSString *> *)resourcePaths
                     shouldUpdate:(BOOL)shouldUpdate;
- (iOSReturnStatusCode)installApp:(Application *)app
                 codesignIdentity:(CodesignIdentity *)codesignID
                     shouldUpdate:(BOOL)shouldUpdate;
- (iOSReturnStatusCode)installApp:(Application *)app
                 codesignIdentity:(CodesignIdentity *)codesignID
                resourcesToInject:(NSArray<NSString *> *)resourcePaths
                     shouldUpdate:(BOOL)shouldUpdate;
- (iOSReturnStatusCode)uninstallApp:(NSString *)bundleID;
- (iOSReturnStatusCode)simulateLocationWithLat:(double)lat lng:(double)lng;
- (iOSReturnStatusCode)stopSimulatingLocation;

//TODO: this should accept Env and Args
- (iOSReturnStatusCode)launchApp:(NSString *)bundleID;

- (BOOL)launchApplicationWithConfiguration:(FBApplicationLaunchConfiguration *)configuration
                                     error:(NSError **)error;
- (iOSReturnStatusCode)killApp:(NSString *)bundleID;
- (BOOL)shouldUpdateApp:(Application *)app statusCode:(iOSReturnStatusCode *)sc;
- (iOSReturnStatusCode)isInstalled:(NSString *)bundleID;
- (BOOL)isInstalled:(NSString *)bundleID withError:(NSError **)error;
- (Application *)installedApp:(NSString *)bundleID;
- (iOSReturnStatusCode)uploadFile:(NSString *)filepath
                   forApplication:(NSString *)bundleID
                        overwrite:(BOOL)overwrite;
- (iOSReturnStatusCode)uploadXCAppDataBundle:(NSString *)filepath
                              forApplication:(NSString *)bundleIdentifier;
- (NSString *)containerPathForApplication:(NSString *)bundleID;
- (NSString *)installPathForApplication:(NSString *)bundleID;
- (NSString *)xctestBundlePathForTestRunnerAtPath:(NSString *)testRunnerPath;
- (BOOL)stageXctestConfigurationToTmpForBundleIdentifier:(NSString *)bundleIdentifier
                                                   error:(NSError **)error;

@end
