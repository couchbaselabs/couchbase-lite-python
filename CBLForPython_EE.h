//
// CBLForPython.h
//
// Copyright (c) 2019-2021 Couchbase, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// This file contains extra APIs only available in the Enterprise Edition
// of Couchbase Lite For C.
// For more information, see `CBLForPython.h`.


//////// CBLDatabase.h

typedef uint32_t CBLEncryptionAlgorithm;
typedef uint64_t CBLEncryptionKeySize;

typedef struct {
    CBLEncryptionAlgorithm algorithm;       ///< Encryption algorithm
    uint8_t bytes[32];                      ///< Raw key data
} CBLEncryptionKey;

typedef struct {
    FLString directory;                 ///< The parent directory of the database
    CBLEncryptionKey encryptionKey;     ///< The database's encryption key (if any)
} CBLDatabaseConfiguration_EE;

bool CBLDatabase_ChangeEncryptionKey(CBLDatabase*,
                                     const CBLEncryptionKey* newKey,
                                     CBLError* outError);



//////// CBLEncryptable.h

typedef ... CBLEncryptable;

extern const FLSlice kCBLEncryptableType;                ///< `"encryptable"`
extern const FLSlice kCBLEncryptableValueProperty;       ///< `"value"`

CBLEncryptable* CBLEncryptable_CreateWithNull(void);
CBLEncryptable* CBLEncryptable_CreateWithBool(bool value);
CBLEncryptable* CBLEncryptable_CreateWithInt(int64_t value);
CBLEncryptable* CBLEncryptable_CreateWithUInt(uint64_t value);
CBLEncryptable* CBLEncryptable_CreateWithFloat(float value);
CBLEncryptable* CBLEncryptable_CreateWithDouble(double value);
CBLEncryptable* CBLEncryptable_CreateWithString(FLString value);
CBLEncryptable* CBLEncryptable_CreateWithValue(FLValue value);
CBLEncryptable* CBLEncryptable_CreateWithArray(FLArray value);
CBLEncryptable* CBLEncryptable_CreateWithDict(FLDict value);

FLValue CBLEncryptable_Value(const CBLEncryptable* encryptable);
FLDict CBLEncryptable_Properties(const CBLEncryptable* encryptable);

bool FLDict_IsEncryptableValue(FLDict);
const CBLEncryptable* FLDict_GetEncryptableValue(FLDict encryptableDict);
void FLSlot_SetEncryptableValue(FLSlot slot, const CBLEncryptable* encryptable);



//////// CBLReplicator.h

CBLEndpoint* CBLEndpoint_CreateWithLocalDB(CBLDatabase*);

typedef FLSliceResult (*CBLPropertyEncryptor) (
    void* context,              ///< Replicator’s context
    FLString documentID,        ///< Document ID
    FLDict properties,          ///< Document properties
    FLString keyPath,           ///< Key path of the property to be encrypted
    FLSlice input,              ///< Property data to be encrypted
    FLStringResult* algorithm,  ///< On return: algorithm name (Optional: Default Value is 'CB_MOBILE_CUSTOM')
    FLStringResult* kid,        ///< On return: encryption key identifier (Optional)
    CBLError* error             ///< On return: error (Optional)
);

typedef FLSliceResult (*CBLPropertyDecryptor) (
    void* context,              ///< Replicator’s context
    FLString documentID,        ///< Document ID
    FLDict properties,          ///< Document properties
    FLString keyPath,           ///< Key path of the property to be decrypted
    FLSlice input,              ///< Property data to be decrypted
    FLString algorithm,         ///< Algorithm name
    FLString kid,               ///< Encryption key identifier specified when encryting the value
    CBLError* error             ///< On return: error (Optional)
);


typedef struct {
    CBLDatabase* database;              ///< The database to replicate
    CBLEndpoint* endpoint;              ///< The address of the other database to replicate with
    CBLReplicatorType replicatorType;   ///< Push, pull or both
    bool continuous;                    ///< Continuous replication?
    //-- Auto Purge:
    bool disableAutoPurge;              ///< Disable/Enable auto-purging documents when the user's access to the documents has been revoked.
    //-- Retry Logic:
    unsigned maxAttempts;               ///< Max retry attempts where the initial connect to replicate counts toward the given value.
                                        ///< Specify 0 to use the default value, 10 times for a non-continuous replicator and max-int time for a continuous replicator. Specify 1 means there will be no retry after the first attempt.
    unsigned maxAttemptWaitTime;        ///< Max wait time between retry attempts in seconds. Specify 0 to use the default value of 300 seconds.
    //-- WebSocket:
    unsigned heartbeat;                 ///< The heartbeat interval in seconds. Specify 0 to use the default value of 300 seconds.
    //-- HTTP settings:
    CBLAuthenticator* authenticator;    ///< Authentication credentials, if needed
    const CBLProxySettings* proxy;      ///< HTTP client proxy settings
    FLDict headers;                     ///< Extra HTTP headers to add to the WebSocket request
    //-- TLS settings:
    FLSlice pinnedServerCertificate;    ///< An X.509 cert to "pin" TLS connections to (PEM or DER)
    FLSlice trustedRootCertificates;    ///< Set of anchor certs (PEM format)
    //-- Filtering:
    FLArray channels;                   ///< Optional set of channels to pull from
    FLArray documentIDs;                ///< Optional set of document IDs to replicate
    CBLReplicationFilter pushFilter;    ///< Optional callback to filter which docs are pushed
    CBLReplicationFilter pullFilter;    ///< Optional callback to validate incoming docs
    CBLConflictResolver conflictResolver;///< Optional conflict-resolver callback
    void* context;                      ///< Arbitrary value that will be passed to callbacks
    
    //-- Property Encryption
    CBLPropertyEncryptor propertyEncryptor;           ///< Optional callback to encrypt \ref CBLEncryptable values.
    CBLPropertyDecryptor propertyDecryptor;           ///< Optional callback to decrypt encrypted \ref CBLEncryptable values.
} CBLReplicatorConfiguration_EE;
