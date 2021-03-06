CouchDBSyncer
=============

CouchDBSyncer - syncs couchdb databases from the server, saving data locally in a core data database.

External requirements:

* json-framework

Installation
------------

    > git clone git://github.com/2moro/CouchDBSyncer-iOS.git
    > cd CouchDBSyncer-iOS
    > git submodule init
    > git submodule update

CouchDBSyncer is a Cocoa Touch Static Library project, and can be incorporated into other xcode projects in the usual ways.

You will need CoreData framework to be linked.

You will need to add a copy of the CouchDBSyncer database model to your project (called CouchDBSyncer.xcdatamodeld).

Usage
-----

CouchDBSyncerStore handles storage of data locally in a core data store.

Applications will typically create a single shared instance of CouchDBSyncerStore.

a CouchDBSyncer object is used to download changes from a remote CouchDB database to the local store. 
The Syncer uses the bulk document fetch API (http://wiki.apache.org/couchdb/HTTP_Bulk_Document_API) to fetch all changed documents in a single HTTP request.

If you want to limit the number of documents fetched by each HTTP request, you can set the docsPerReq property on your Syncer object (see section on Tunables below).
Attachments are fetched in separate HTTP requests.

    // create a local store object
    CouchDBSyncerStore *store = [[CouchDBSyncerStore alloc] init];

    // or, to use a shipped database as a starting point:
    CouchDBSyncerStore *store = [[CouchDBSyncerStore alloc] initWithShippedDatabase:@"shipped_db.sqlite"];

    // get the database DB_NAME.  updates the database url to the given DB_URL.
    // creates a new local database record if it has not been created yet.
    CouchDBSyncerDatabase *database = [store database:DB_NAME url:[NSURL urlWithString:DB_URL]];

    // create a syncer to fetch changes
    CouchDBSyncer *syncer = [[CouchDBSyncer alloc] initWithStore:store database:database];
    [syncer update];   // asynchronous call - creates an update thread

Sync Progress
-------------

CouchDBSyncer broadcasts progress notifications which you can listen to if required.
The notifications are:

* CouchDBSyncerProgressNotification - some progress has been made
* CouchDBSyncerCompleteNotification - sync has completed
* CouchDBSyncerErrorNotification - sync encountered an error

Database/Document/Attachment API
--------------------------------

the following attributes are saved separately as part of a document record, if available:

* type      (to change this, set the store.modelTypeKey tunable property)
* parent_id (accessible as "parentId" in the document record)

Download policies
-----------------

A variant of the update method of CouchDBSyncer accepts an object implementing the CouchDBSyncerDownloadPolicy protocol.
This can be used to download documents/attachments selectively, or with different priorities.

e.g.
    // assumes self implements the CouchDBSyncerDownloadPolicyDelegate protocol
    syncer.downloadPolicyDelegate = self;
    [syncer update];

The default is to download all documents and attachments.  By default, documents are downloaded with priority NSOperationQueuePriorityNormal, and attachments
are downloaded with priority NSOperationQueuePriorityLow.

Metadata for attachments that are not downloaded is stored in the database, and is accessible via the attachments method of CouchDBSyncerDocument.
(This allows attachments to be accessed later if required).  If an attachment has newer content on the server, its stale attribute will be true.

Accessing databases / documents / attachments
---------------------------------------------

see CouchDBSyncerStore.h for the full list of public API methods.

documents can be accessed using the following methods of CouchDBSyncerStore.

    - (NSArray *)documents:(CouchDBSyncerDatabase *)database;
    - (NSArray *)documents:(CouchDBSyncerDatabase *)database matching:(NSPredicate *)predicate;
    - (NSArray *)documents:(CouchDBSyncerDatabase *)database ofType:(NSString *)type;

The above methods return arrays of CouchDBSyncerDocument objects.  The dictionary method of CouchDBSyncerDocument can be used to access the 
document contents as an NSDictionary (converted from JSON).  Each document has an array of attachment metadata attached to it.

Attachment content is not read from the database by the above methods. To fetch attachment content, use the following methods of CouchDBSyncerStore.

    - (NSArray *)attachments:(CouchDBSyncerDocument *)document;
    - (CouchDBSyncerAttachment *)attachment:(CouchDBSyncerDocument *)document named:(NSString *)name;

Tunables
--------

Tunable properties:

    syncer.docsPerReq = 100;          // 0 (unlimited) by default, limit the number of documents per fetch request
    syncer.maxConcurrentFetches = 3;  // 3 by default, limit the number of maximum concurrent fetch requests
    
    store.modelTypeKey = @"type";     // "type" by default, the document field to store in the document.type attribute.

License
-------
Copyright 2011 2moro mobile
see also LICENSE.txt

