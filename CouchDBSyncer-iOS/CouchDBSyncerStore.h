//
//  CouchDBSyncerStore.h
//  CouchDBSyncer
//
//  Created by ASW on 26/02/11.
//  Copyright 2011 2moro mobile. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "CouchDBSyncerDatabase.h"
#import "CouchDBSyncerUpdateContext.h"
#import "CouchDBSyncerDocument.h"
#import "CouchDBSyncerAttachment.h"

@interface CouchDBSyncerStore : NSObject {
    NSString *modelTypeKey, *parentKey;
    
    // core data
    NSManagedObjectModel *managedObjectModel;
    NSManagedObjectContext *managedObjectContext;
    NSPersistentStoreCoordinator *persistentStoreCoordinator;
    
    NSError *error;
    NSString *shippedPath;
}

@property (nonatomic, readonly) NSManagedObjectModel *managedObjectModel;
@property (nonatomic, readonly) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, readonly) NSPersistentStoreCoordinator *persistentStoreCoordinator;

@property (nonatomic, retain) NSString *modelTypeKey, *parentKey;
@property (nonatomic, retain) NSError *error;

- (id)initWithShippedDatabasePath:(NSString *)shippedPath;

// purge the store (removes all databases)
- (void)destroy;

// purge all data for the specified database
- (void)purge:(CouchDBSyncerDatabase *)database;

// remove the specified database completely
- (void)destroy:(CouchDBSyncerDatabase *)database;

- (NSDictionary *)statistics:(CouchDBSyncerDatabase *)database;

// get database with given name
- (CouchDBSyncerDatabase *)database:(NSString *)name;
- (NSArray *)databases;

// get database with the given name. if the database is not found locally, creates a new database with the 
// given name and url.
- (CouchDBSyncerDatabase *)database:(NSString *)name url:(NSURL *)url;

// get documents
- (NSArray *)documents:(CouchDBSyncerDatabase *)database;
- (NSArray *)documents:(CouchDBSyncerDatabase *)database ofType:(NSString *)type;
- (NSArray *)documents:(CouchDBSyncerDatabase *)database ofType:(NSString *)type tagged:(NSString *)tag;
- (NSArray *)documents:(CouchDBSyncerDatabase *)database tagged:(NSString *)tag;
- (NSArray *)documents:(CouchDBSyncerDatabase *)database parent:(CouchDBSyncerDocument *)parent;

- (CouchDBSyncerDocument *)document:(CouchDBSyncerDatabase *)database documentId:(NSString *)documentId;

// get document types (array of NSString)
- (NSArray *)documentTypes:(CouchDBSyncerDatabase *)database;

// get attachments
- (NSArray *)attachments:(CouchDBSyncerDocument *)document;
- (CouchDBSyncerAttachment *)attachment:(CouchDBSyncerDocument *)document named:(NSString *)name;

// update methods
// used by couchdbsyncer
// get an update context for the given database and current thread
- (CouchDBSyncerUpdateContext *)updateContext:(CouchDBSyncerDatabase *)database;
- (BOOL)update:(CouchDBSyncerUpdateContext *)context database:(CouchDBSyncerDatabase *)database;
- (BOOL)update:(CouchDBSyncerUpdateContext *)context document:(CouchDBSyncerDocument *)document;
- (BOOL)update:(CouchDBSyncerUpdateContext *)context attachment:(CouchDBSyncerAttachment *)attachment;

// returns a list of all stale attachments (need downloading)
- (NSArray *)staleAttachments:(CouchDBSyncerDatabase *)database;

@end
