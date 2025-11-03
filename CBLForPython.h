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

/*
    This file is used by the CFFI library to generate Python bindings for Couchbase Lite
    when `BuildPyCBL.py` is run. The output is a shared library in the `CouchbaseLite`
    subdirectory called something like `_PyCBL.cpython-39-darwin.so` (the exact name is platform
    specific.)

    This file contains a modified version of the Couchbase Lite C headers. The CFFI parser is not
    a full C parser; in particular, it doesn't have a preprocessor. It also has a few syntactic
    extensions of its own. This means some changes need to be made to the headers:

    * Merge all headers into this file.
    * Remove preprocessor directives like `#include`, `#define`, `#ifdef`, `#if`, `#pragma`.
    * Remove preprocessor macros like `_cbl_nonnull`, `, `CBL_ENUM`, `CBL_PUBLIC`.
    * Declare opaque struct types (like CBLBlob) as `typedef ... * TYPENAME`.
    * Declare structs whose contents Python doesn't need to know as `struct { ...; }`.
    * Declare glue routines for C callbacks, as `extern "Python"` functions.

    This file is up-to-date as of Couchbase Lite For C 3.0 beta, commit 0c7e1adf.

    THIS FILE MUST BE UPDATED WHENEVER THE CBL C API CHANGES!
*/


//////// FLSlice.h

typedef struct {const void *buf; size_t size;} FLSlice;
typedef struct {const void *buf; size_t size;} FLSliceResult;
typedef FLSlice FLHeapSlice;
typedef FLSlice FLString;
typedef FLSliceResult FLStringResult;

bool FLSlice_Equal(FLSlice a, FLSlice b);
int FLSlice_Compare(FLSlice, FLSlice);
FLSliceResult FLSliceResult_Retain(FLSliceResult);
void FLSliceResult_Release(FLSliceResult);
uint32_t FLSlice_Hash(FLSlice);
bool FLSlice_ToCString(FLSlice, char*, size_t);
FLSliceResult FLSliceResult_New(size_t);
FLSliceResult FLSlice_Copy(FLSlice);
void FL_WipeMemory(void *, size_t);



//////// Fleece.h

typedef ... * FLValue;         
typedef ... * FLArray;         
typedef ... * FLDict;          
typedef ... * FLMutableArray;  
typedef ... * FLMutableDict;   
typedef ... * FLSlot;
typedef ... * FLEncoder;
typedef ... * FLDoc;
typedef ... * FLSharedKeys;
typedef unsigned FLError;

typedef enum {
    kFLUntrusted,
    kFLTrusted
} FLTrust;


typedef int64_t FLTimestamp;

FLTimestamp FLTimestamp_Now(void);
FLStringResult FLTimestamp_ToString(FLTimestamp timestamp, bool asUTC);
FLTimestamp FLTimestamp_FromString(FLString str);

typedef enum {
    kFLUndefined = -1, 
    kFLNull = 0,        
    kFLBoolean,         
    kFLNumber,          
    kFLString,          
    kFLData,            
    kFLArray,           
    kFLDict             
} FLValueType;

FLValueType FLValue_GetType(FLValue);
bool FLValue_IsInteger(FLValue);
bool FLValue_IsUnsigned(FLValue);
bool FLValue_IsDouble(FLValue);
bool FLValue_AsBool(FLValue);
int64_t FLValue_AsInt(FLValue);
uint64_t FLValue_AsUnsigned(FLValue);
float FLValue_AsFloat(FLValue);
double FLValue_AsDouble(FLValue);
FLString FLValue_AsString(FLValue);
FLTimestamp FLValue_AsTimestamp(FLValue);
FLSlice FLValue_AsData(FLValue);
FLArray FLValue_AsArray(FLValue);
FLDict FLValue_AsDict(FLValue);
FLStringResult FLValue_ToString(FLValue);
bool FLValue_IsEqual(FLValue v1, FLValue v2);
bool FLValue_IsMutable(FLValue);
FLValue FLValue_Retain(FLValue);
void FLValue_Release(FLValue);

typedef struct { ...; } FLArrayIterator;
uint32_t FLArray_Count(FLArray);
FLValue FLArray_Get(FLArray, uint32_t index);
void FLArrayIterator_Begin(FLArray, FLArrayIterator*);
FLValue FLArrayIterator_GetValue(const FLArrayIterator*);
FLValue FLArrayIterator_GetValueAt(const FLArrayIterator*, uint32_t offset);
uint32_t FLArrayIterator_GetCount(const FLArrayIterator*);
bool FLArrayIterator_Next(FLArrayIterator*);

bool FLArray_IsEmpty(FLArray);
FLMutableArray FLArray_AsMutable(FLArray);

typedef struct { ...; } FLDictIterator;
uint32_t FLDict_Count(FLDict);
FLValue FLDict_Get(FLDict, FLSlice keyString);
void FLDictIterator_Begin(FLDict, FLDictIterator*);
FLString FLDictIterator_GetKeyString(const FLDictIterator*);
FLValue FLDictIterator_GetValue(const FLDictIterator*);
uint32_t FLDictIterator_GetCount(const FLDictIterator* );
bool FLDictIterator_Next(FLDictIterator*);
void FLDictIterator_End(FLDictIterator*);
bool FLDict_IsEmpty(FLDict);
FLMutableDict FLDict_AsMutable(FLDict);
FLValue FLDictIterator_GetKey(const FLDictIterator*);

typedef struct { ...; } FLDictKey;
FLDictKey FLDictKey_Init(FLSlice string);
FLString FLDictKey_GetString(const FLDictKey*);
FLValue FLDict_GetWithKey(FLDict, FLDictKey*);

typedef ... * FLDeepIterator;
FLDeepIterator FLDeepIterator_New(FLValue);
void FLDeepIterator_Free(FLDeepIterator);
FLValue FLDeepIterator_GetValue(FLDeepIterator);
FLValue FLDeepIterator_GetParent(FLDeepIterator);
FLSlice FLDeepIterator_GetKey(FLDeepIterator);
uint32_t FLDeepIterator_GetIndex(FLDeepIterator);
size_t FLDeepIterator_GetDepth(FLDeepIterator);
void FLDeepIterator_SkipChildren(FLDeepIterator);
bool FLDeepIterator_Next(FLDeepIterator);

typedef struct {
    FLSlice key;
    uint32_t index;
} FLPathComponent;

void FLDeepIterator_GetPath(FLDeepIterator, FLPathComponent** outPath, size_t*);
FLSliceResult FLDeepIterator_GetPathString(FLDeepIterator);
FLSliceResult FLDeepIterator_GetJSONPointer(FLDeepIterator);

FLDoc FLDoc_FromResultData(FLSliceResult data, FLTrust, FLSharedKeys, FLSlice externData);

void FLDoc_Release(FLDoc);
FLDoc FLDoc_Retain(FLDoc);
FLSlice FLDoc_GetData(FLDoc);
FLSliceResult FLDoc_GetAllocedData(FLDoc);
FLValue FLDoc_GetRoot(FLDoc);
FLSharedKeys FLDoc_GetSharedKeys(FLDoc);
FLDoc FLValue_FindDoc(FLValue);
bool FLDoc_SetAssociated(FLDoc doc, void *pointer, const char *type);
void* FLDoc_GetAssociated(FLDoc doc, const char *type);

typedef enum {
    kFLEncodeFleece,
    kFLEncodeJSON,
    kFLEncodeJSON5
} FLEncoderFormat;

FLEncoder FLEncoder_New(void);

FLEncoder FLEncoder_NewWithOptions(FLEncoderFormat format, size_t reserveSize, bool uniqueStrings);

FLEncoder FLEncoder_NewWritingToFile(FILE*, bool uniqueStrings);
void FLEncoder_Free(FLEncoder);
void FLEncoder_SetSharedKeys(FLEncoder, FLSharedKeys);
void FLEncoder_SetExtraInfo(FLEncoder, void* info);
void* FLEncoder_GetExtraInfo(FLEncoder);
void FLEncoder_Reset(FLEncoder);
size_t FLEncoder_BytesWritten(FLEncoder);
bool FLEncoder_WriteNull(FLEncoder);
bool FLEncoder_WriteUndefined(FLEncoder);
bool FLEncoder_WriteBool(FLEncoder, bool);
bool FLEncoder_WriteInt(FLEncoder, int64_t);
bool FLEncoder_WriteUInt(FLEncoder, uint64_t);
bool FLEncoder_WriteFloat(FLEncoder, float);
bool FLEncoder_WriteDouble(FLEncoder, double);
bool FLEncoder_WriteString(FLEncoder, FLString);
bool FLEncoder_WriteDateString(FLEncoder encoder, FLTimestamp ts, bool asUTC);
bool FLEncoder_WriteData(FLEncoder, FLSlice);
bool FLEncoder_WriteValue(FLEncoder, FLValue);
bool FLEncoder_BeginArray(FLEncoder, size_t reserveCount);
bool FLEncoder_EndArray(FLEncoder);
bool FLEncoder_BeginDict(FLEncoder, size_t reserveCount);
bool FLEncoder_WriteKey(FLEncoder, FLString);
bool FLEncoder_WriteKeyValue(FLEncoder, FLValue);
bool FLEncoder_EndDict(FLEncoder);
bool FLEncoder_WriteRaw(FLEncoder, FLSlice);
FLDoc FLEncoder_FinishDoc(FLEncoder, FLError* outError);
FLSliceResult FLEncoder_Finish(FLEncoder, FLError* outError);
FLError FLEncoder_GetError(FLEncoder);
const char* FLEncoder_GetErrorMessage(FLEncoder);

FLStringResult FLValue_ToJSON(FLValue);
FLStringResult FLValue_ToJSON5(FLValue);
FLStringResult FLValue_ToJSONX(FLValue v, bool json5, bool canonicalForm);
FLDoc FLDoc_FromJSON(FLSlice json, FLError* outError);
FLMutableArray FLMutableArray_NewFromJSON(FLString json, FLError* outError);
FLMutableDict FLMutableDict_NewFromJSON(FLString json, FLError* outError);
bool FLEncoder_ConvertJSON(FLEncoder, FLSlice json);

typedef ... * FLKeyPath;
FLKeyPath FLKeyPath_New(FLSlice specifier, FLError* outError);
void FLKeyPath_Free(FLKeyPath);
FLValue FLKeyPath_Eval(FLKeyPath, FLValue root);

FLValue FLKeyPath_EvalOnce(FLSlice specifier, FLValue root, FLError* outError);
FLStringResult FLKeyPath_ToString(FLKeyPath path);
bool FLKeyPath_Equals(FLKeyPath path1, FLKeyPath path2);
bool FLKeyPath_GetElement(FLKeyPath, size_t i, FLSlice *outDictKey, int32_t *outArrayIndex);

typedef enum {
    kFLDefaultCopy        = 0,
    kFLDeepCopy           = 1,
    kFLCopyImmutables     = 2,
    kFLDeepCopyImmutables = (kFLDeepCopy | kFLCopyImmutables),
} FLCopyFlags;

FLMutableArray FLArray_MutableCopy(FLArray, FLCopyFlags);

FLMutableArray FLMutableArray_New(void);

FLArray FLMutableArray_GetSource(FLMutableArray);

bool FLMutableArray_IsChanged(FLMutableArray);
void FLMutableArray_SetChanged(FLMutableArray, bool changed);

void FLMutableArray_Insert(FLMutableArray array, uint32_t firstIndex, uint32_t count);

void FLMutableArray_Remove(FLMutableArray array, uint32_t firstIndex, uint32_t count);

void FLMutableArray_Resize(FLMutableArray array, uint32_t size);

FLMutableArray FLMutableArray_GetMutableArray(FLMutableArray, uint32_t index);

FLMutableDict FLMutableArray_GetMutableDict(FLMutableArray, uint32_t index);
FLMutableDict FLDict_MutableCopy(FLDict source, FLCopyFlags);
FLMutableDict FLMutableDict_New(void);
FLDict FLMutableDict_GetSource(FLMutableDict);
bool FLMutableDict_IsChanged(FLMutableDict);
void FLMutableDict_SetChanged(FLMutableDict, bool);
void FLMutableDict_Remove(FLMutableDict, FLString key);
void FLMutableDict_RemoveAll(FLMutableDict);

FLMutableArray FLMutableDict_GetMutableArray(FLMutableDict, FLString key);

FLMutableDict FLMutableDict_GetMutableDict(FLMutableDict, FLString key);

FLValue FLValue_NewString(FLString);

FLValue FLValue_NewData(FLSlice);

FLSlot FLMutableArray_Set(FLMutableArray, uint32_t index);

FLSlot FLMutableArray_Append(FLMutableArray);

FLSlot FLMutableDict_Set(FLMutableDict, FLString key);

void FLSlot_SetNull(FLSlot);
void FLSlot_SetBool(FLSlot, bool);
void FLSlot_SetInt(FLSlot, int64_t);
void FLSlot_SetUInt(FLSlot, uint64_t);
void FLSlot_SetFloat(FLSlot, float);
void FLSlot_SetDouble(FLSlot, double);
void FLSlot_SetString(FLSlot, FLString);
void FLSlot_SetData(FLSlot, FLSlice);
void FLSlot_SetValue(FLSlot, FLValue);


//////// CBLBase.h

typedef uint8_t CBLErrorDomain;
typedef int32_t CBLErrorCode;
typedef int32_t CBLNetworkErrorCode;

typedef struct {
    CBLErrorDomain domain;
    int code;
    unsigned internal_info;
} CBLError;
FLSliceResult CBLError_Message(const CBLError*);

typedef int64_t CBLTimestamp;
CBLTimestamp CBL_Now(void);

typedef ... CBLRefCounted;
CBLRefCounted* CBL_Retain(void*);
void CBL_Release(void*);
unsigned CBL_InstanceCount(void);
void CBL_DumpInstances(void);

typedef ... CBLBlob;
typedef ... CBLDatabase;
typedef ... CBLDocument;
typedef ... CBLListenerToken;
typedef ... CBLQuery;
typedef ... CBLResultSet;
typedef ... CBLReplicator;
typedef ... CBLScope;
typedef ... CBLCollection;

void CBLListener_Remove(CBLListenerToken*);



//////// CBLBlob.h

bool FLDict_IsBlob(FLDict);
const CBLBlob* FLDict_GetBlob(FLDict blobDict);

uint64_t CBLBlob_Length(const CBLBlob*);
FLString CBLBlob_Digest(const CBLBlob*);
FLString CBLBlob_ContentType(const CBLBlob*);
FLDict CBLBlob_Properties(const CBLBlob*);
FLStringResult CBLBlob_CreateJSON(const CBLBlob*);
FLSliceResult CBLBlob_Content(const CBLBlob*, CBLError *outError);

typedef ... CBLBlobReadStream;
CBLBlobReadStream* CBLBlob_OpenContentStream(const CBLBlob*, CBLError *outError);
int CBLBlobReader_Read(CBLBlobReadStream* stream, void *dst, size_t maxLength, CBLError *outError);
void CBLBlobReader_Close(CBLBlobReadStream*);
bool CBLBlob_Equals(CBLBlob* blob, CBLBlob* anotherBlob);

typedef uint8_t CBLSeekBase;
int64_t CBLBlobReader_Seek(CBLBlobReadStream* stream, int64_t offset, CBLSeekBase base, CBLError* outError);
uint64_t CBLBlobReader_Position(CBLBlobReadStream* stream);

CBLBlob* CBLBlob_CreateWithData(FLString contentType, FLSlice contents);

typedef ... CBLBlobWriteStream;
CBLBlobWriteStream* CBLBlobWriter_Create(CBLDatabase *db, CBLError *outError);
void CBLBlobWriter_Close(CBLBlobWriteStream*);
bool CBLBlobWriter_Write(CBLBlobWriteStream* writer, const void *data, size_t length, CBLError *outError);
CBLBlob* CBLBlob_CreateWithStream(FLString contentType, CBLBlobWriteStream* writer);

const CBLBlob* CBLDatabase_GetBlob(CBLDatabase* db, FLDict properties,
                                   CBLError* outError);
bool CBLDatabase_SaveBlob(CBLDatabase* db, CBLBlob* blob,
                          CBLError* outError);


//////// CBLDatabase.h

typedef struct {
    FLString directory;
    ...;
} CBLDatabaseConfiguration;

typedef enum {
    kCBLMaintenanceTypeCompact = 0,
    kCBLMaintenanceTypeReindex,
    kCBLMaintenanceTypeIntegrityCheck,
    kCBLMaintenanceTypeOptimize,
    kCBLMaintenanceTypeFullOptimize
} CBLMaintenanceType;

CBLDatabaseConfiguration CBLDatabaseConfiguration_Default(void);

bool CBL_DatabaseExists(FLString name, FLString inDirectory);
bool CBL_CopyDatabase(FLString fromPath,
                FLString toName,
                const CBLDatabaseConfiguration* config,
                CBLError*);
bool CBL_DeleteDatabase(FLString name,
                  FLString inDirectory,
                  CBLError*);
CBLDatabase* CBLDatabase_Open(FLString name,
                         const CBLDatabaseConfiguration* config,
                         CBLError* error);
bool CBLDatabase_Close(CBLDatabase*, CBLError*);
bool CBLDatabase_Delete(CBLDatabase*, CBLError*);
bool CBLDatabase_PerformMaintenance(CBLDatabase*, CBLMaintenanceType type, CBLError*);
bool CBLDatabase_BeginTransaction(CBLDatabase*, CBLError*);
bool CBLDatabase_EndTransaction(CBLDatabase*, bool commit, CBLError*);
FLString CBLDatabase_Name(const CBLDatabase*);
FLStringResult CBLDatabase_Path(const CBLDatabase*);
uint64_t CBLDatabase_Count(const CBLDatabase*);
CBLDatabaseConfiguration CBLDatabase_Config(const CBLDatabase*);

typedef void (*CBLDatabaseChangeListener)(void *context,
                                     const CBLDatabase* db,
                                     unsigned numDocs,
                                     FLString *docIDs);
extern "Python" void databaseListenerCallback(void *context, const CBLDatabase* db,
                                              unsigned numDocs, FLString *docIDs);
CBLListenerToken* CBLDatabase_AddChangeListener(const CBLDatabase* db,
                                     CBLDatabaseChangeListener listener,
                                     void *context);
typedef void (*CBLNotificationsReadyCallback)(void* context, CBLDatabase* db);
void CBLDatabase_BufferNotifications(CBLDatabase *db, CBLNotificationsReadyCallback callback, void* context);
void CBLDatabase_SendNotifications(CBLDatabase *db);



//////// CBLDocument.h

typedef uint8_t CBLConcurrencyControl;

typedef bool (*CBLConflictHandler)(void* context,
                                   CBLDocument* documentBeingSaved,
                                   const CBLDocument* conflictingDocument);

const CBLDocument* CBLDatabase_GetDocument(const CBLDatabase* database,
                                      FLString docID,
                                      CBLError* error);
bool CBLDatabase_SaveDocument(CBLDatabase* db,
                                       CBLDocument* doc,
                                       CBLError* error);
bool CBLDatabase_SaveDocumentWithConcurrencyControl(CBLDatabase* db,
                                       CBLDocument* doc,
                                       CBLConcurrencyControl concurrency,
                                       CBLError* error);
bool CBLDatabase_SaveDocumentWithConflictHandler(CBLDatabase* db,
                                                 CBLDocument* doc,
                                                 CBLConflictHandler conflictHandler,
                                                 void* context,
                                                 CBLError* outError);
bool CBLDatabase_DeleteDocument(CBLDatabase *db,
                    const CBLDocument* document,
                    CBLError* error);
bool CBLDatabase_DeleteDocumentWithConcurrencyControl(CBLDatabase *db,
                    const CBLDocument* document,
                    CBLConcurrencyControl concurrency,
                    CBLError* error);
bool CBLDatabase_PurgeDocument(CBLDatabase *db,
                   const CBLDocument* document,
                   CBLError* error);
bool CBLDatabase_PurgeDocumentByID(CBLDatabase* database,
                          FLString docID,
                          CBLError* error);
CBLDocument* CBLDatabase_GetMutableDocument(CBLDatabase* database,
                                       FLString docID,
                                       CBLError* error);
CBLDocument* CBLDocument_Create(void);
CBLDocument* CBLDocument_CreateWithID(FLString docID);
CBLDocument* CBLDocument_MutableCopy(const CBLDocument* original);
FLString CBLDocument_ID(const CBLDocument*);
FLString CBLDocument_RevisionID(const CBLDocument*);
uint64_t CBLDocument_Sequence(const CBLDocument*);
CBLCollection* CBLDocument_Collection(const CBLDocument*);
FLDict CBLDocument_Properties(const CBLDocument*);
FLMutableDict CBLDocument_MutableProperties(CBLDocument*);
void CBLDocument_SetProperties(CBLDocument*, FLMutableDict properties);
FLSliceResult CBLDocument_CreateJSON(const CBLDocument*);
bool CBLDocument_SetJSON(CBLDocument*, FLSlice json, CBLError*);

CBLTimestamp CBLDatabase_GetDocumentExpiration(CBLDatabase* db,
                                         FLString docID,
                                         CBLError* error);
bool CBLDatabase_SetDocumentExpiration(CBLDatabase* db,
                                       FLString docID,
                                       CBLTimestamp expiration,
                                       CBLError* error);

typedef void (*CBLDocumentChangeListener)(void *context,
                                          const CBLDatabase* db,
                                          FLString docID);
CBLListenerToken* CBLDatabase_AddDocumentChangeListener(const CBLDatabase* db,
                                                        FLString docID,
                                                        CBLDocumentChangeListener listener,
                                                        void *context);
extern "Python" void documentListenerCallback(void *context, const CBLDatabase*, FLString docID);



//////// CBLLog.h

typedef enum {
    kCBLLogDomainDatabase,
    kCBLLogDomainQuery,
    kCBLLogDomainReplicator,
    kCBLLogDomainNetwork
} CBLLogDomain;

typedef enum {
    kCBLLogDebug,
    kCBLLogVerbose,
    kCBLLogInfo,
    kCBLLogWarning,
    kCBLLogError,
    kCBLLogNone
} CBLLogLevel;

void CBL_Log(CBLLogDomain domain, CBLLogLevel level, const char *format, ...);
void CBL_LogMessage(CBLLogDomain domain,
                    CBLLogLevel level,
                    FLSlice message);
CBLLogLevel CBLLog_ConsoleLevel(void);
void CBLLog_SetConsoleLevel(CBLLogLevel);

typedef void (*CBLLogCallback)(CBLLogDomain domain,
                               CBLLogLevel level,
                               FLString message);

extern "Python" void logCallback(CBLLogDomain domain, CBLLogLevel level, FLString message);

CBLLogLevel CBLLog_CallbackLevel(void);
void CBLLog_SetCallbackLevel(CBLLogLevel);
CBLLogCallback CBLLog_Callback(void);
void CBLLog_SetCallback(CBLLogCallback callback);

typedef struct {
    CBLLogLevel level;       ///< The minimum level of message to write (Required).
    FLString directory;      ///< The directory where log files will be created (Required).
    uint32_t maxRotateCount;
    size_t maxSize;
    bool usePlaintext;
} CBLLogFileConfiguration;

const CBLLogFileConfiguration* CBLLog_FileConfig(void);
bool CBLLog_SetFileConfig(CBLLogFileConfiguration, CBLError*);


//////// CBLQuery.h

typedef enum {
    kCBLJSONLanguage,
    kCBLN1QLLanguage
} CBLQueryLanguage;

CBLQuery* CBLDatabase_CreateQuery(const CBLDatabase* db,
                       CBLQueryLanguage language,
                       FLString queryString,
                       int *outErrorPos,
                       CBLError* error);
FLDict CBLQuery_Parameters(CBLQuery* query);
void CBLQuery_SetParameters(CBLQuery* query,
                             FLDict parameters);
CBLResultSet* CBLQuery_Execute(CBLQuery*, CBLError*);
FLSliceResult CBLQuery_Explain(CBLQuery*);
unsigned CBLQuery_ColumnCount(CBLQuery*);
FLSlice CBLQuery_ColumnName(CBLQuery*,
                             unsigned columnIndex);

bool CBLResultSet_Next(CBLResultSet*);
FLValue CBLResultSet_ValueAtIndex(CBLResultSet*, unsigned index);
FLValue CBLResultSet_ValueForKey(CBLResultSet*, FLString key);
FLArray CBLResultSet_ResultArray(const CBLResultSet*);
FLDict CBLResultSet_ResultDict(const CBLResultSet*);
CBLQuery* CBLResultSet_GetQuery(const CBLResultSet *rs);

typedef void (*CBLQueryChangeListener)(void *context,
                                       CBLQuery* query,
                                       CBLListenerToken *token);
extern "Python" void queryListenerCallback(void *context, const CBLQuery *query);
CBLListenerToken* CBLQuery_AddChangeListener(CBLQuery* query,
                                        CBLQueryChangeListener listener,
                                        void *context);
CBLResultSet* CBLQuery_CopyCurrentResults(const CBLQuery* query,
                                        CBLListenerToken *listener,
                                        CBLError* outError);

typedef struct {
    CBLQueryLanguage expressionLanguage;
    FLString expressions;
    FLString where;
} CBLValueIndexConfiguration;

bool CBLDatabase_CreateValueIndex(CBLDatabase *db,
                                  FLString name,
                                  CBLValueIndexConfiguration config,
                                  CBLError* outError);
typedef struct {
    CBLQueryLanguage expressionLanguage;
    FLString expressions;
    bool ignoreAccents;
    FLString language;
} CBLFullTextIndexConfiguration;

bool CBLDatabase_CreateFullTextIndex(CBLDatabase *db,
                                     FLString name,
                                     CBLFullTextIndexConfiguration config,
                                     CBLError* outError);

bool CBLDatabase_DeleteIndex(CBLDatabase *db,
                             FLString name,
                             CBLError* outError);

FLArray CBLDatabase_GetIndexNames(CBLDatabase *db);



//////// CBLCollection.h

FLMutableArray CBLDatabase_ScopeNames(const CBLDatabase* db, CBLError* outError);

FLMutableArray CBLDatabase_CollectionNames(const CBLDatabase* db, FLString scopeName, CBLError* outError);

CBLScope* CBLDatabase_Scope(const CBLDatabase* db, FLString scopeName, CBLError* outError);

CBLCollection* CBLDatabase_Collection(const CBLDatabase* db, FLString collectionName, FLString scopeName, CBLError* outError);

CBLCollection* CBLDatabase_CreateCollection(CBLDatabase* db, FLString collectionName, FLString scopeName, CBLError* outError);

bool CBLDatabase_DeleteCollection(CBLDatabase* db, FLString collectionName, FLString scopeName, CBLError* outError);

CBLScope* CBLDatabase_DefaultScope(const CBLDatabase* db, CBLError* outError);

CBLCollection* CBLDatabase_DefaultCollection(const CBLDatabase* db, CBLError* outError);

CBLScope* CBLCollection_Scope(const CBLCollection* collection);

FLString CBLCollection_Name(const CBLCollection* collection);

uint64_t CBLCollection_Count(const CBLCollection* collection);

const CBLDocument* CBLCollection_GetDocument(const CBLCollection* collection, FLString docID, CBLError* outError);

bool CBLCollection_SaveDocument(CBLCollection* collection, CBLDocument* doc, CBLError* outError);

bool CBLCollection_SaveDocumentWithConcurrencyControl(CBLCollection* collection, CBLDocument* doc, CBLConcurrencyControl concurrency, CBLError* outError);

bool CBLCollection_SaveDocumentWithConflictHandler(CBLCollection* collection, CBLDocument* doc, CBLConflictHandler conflictHandler, void* context, CBLError* outError);

bool CBLCollection_DeleteDocument(CBLCollection *collection, const CBLDocument* document, CBLError* outError);

bool CBLCollection_DeleteDocumentWithConcurrencyControl(CBLCollection *collection, const CBLDocument* document, CBLConcurrencyControl concurrency, CBLError*outError);

bool CBLCollection_PurgeDocument(CBLCollection* collection, const CBLDocument* document, CBLError* outError);

bool CBLCollection_PurgeDocumentByID(CBLCollection* collection, FLString docID, CBLError* outError);

CBLTimestamp CBLCollection_GetDocumentExpiration(CBLCollection* collection, FLSlice docID, CBLError* outError);

bool CBLCollection_SetDocumentExpiration(CBLCollection* collection, FLSlice docID, CBLTimestamp expiration, CBLError* outError);

CBLDocument* CBLCollection_GetMutableDocument(CBLCollection* collection, FLString docID, CBLError*outError);

bool CBLCollection_CreateValueIndex(CBLCollection *collection, FLString name, CBLValueIndexConfiguration config, CBLError* outError);

bool CBLCollection_CreateFullTextIndex(CBLCollection *collection, FLString name, CBLFullTextIndexConfiguration config, CBLError* outError);

bool CBLCollection_DeleteIndex(CBLCollection *collection, FLString name, CBLError* outError);

FLMutableArray CBLCollection_GetIndexNames(CBLCollection *collection, CBLError* outError);

typedef ... CBLCollectionChange;

typedef void (*CBLCollectionChangeListener)(void* context, const CBLCollectionChange* change);

CBLListenerToken* CBLCollection_AddChangeListener(const CBLCollection* collection, CBLCollectionChangeListener listener, void* context);

typedef... CBLDocumentChange;

typedef void (*CBLCollectionDocumentChangeListener)(void *context, const CBLDocumentChange* change);

CBLListenerToken* CBLCollection_AddDocumentChangeListener(const CBLCollection* collection, FLString docID, CBLCollectionDocumentChangeListener listener, void* context);




//////// CBLReplicator.h


/** An opaque object representing the location of a database to replicate with. */
typedef ... CBLEndpoint;

/** Creates a new endpoint representing a server-based database at the given URL.
    The URL's scheme must be `ws` or `wss`, it must of course have a valid hostname,
    and its path must be the name of the database on that server.
    
    The port can be omitted; it defaults to 80 for `ws` and 443 for `wss`.
    For example: `wss://example.org/dbname`.
 
    If an invalid endpoint URL is specified, an error will be returned.
    */
CBLEndpoint* CBLEndpoint_CreateWithURL(FLString url,
                                                     CBLError* outError);

/** Frees a CBLEndpoint object. */
void CBLEndpoint_Free(CBLEndpoint*);


/** An opaque object representing authentication credentials for a remote server. */
typedef struct CBLAuthenticator CBLAuthenticator;

/** Creates an authenticator for HTTP Basic (username/password) auth. */
CBLAuthenticator* CBLAuth_CreatePassword(FLString username, FLString password);

/** Creates an authenticator using a Couchbase Sync Gateway login session identifier,
    and optionally a cookie name (pass NULL for the default.) */
CBLAuthenticator* CBLAuth_CreateSession(FLString sessionID, FLString cookieName);

/** Frees a CBLAuthenticator object. */
void CBLAuth_Free(CBLAuthenticator*);


/** Direction of replication: push, pull, or both. */
typedef uint8_t CBLReplicatorType;


/** Flags describing a replicated document. */
typedef unsigned CBLDocumentFlags;


/** A callback that can decide whether a particular document should be pushed or pulled.
    @warning  This callback will be called on a background thread managed by the replicator.
                It must pay attention to thread-safety. It should not take a long time to return,
                or it will slow down the replicator.
    @param context  The `context` field of the \ref CBLReplicatorConfiguration.
    @param document  The document in question.
    @param flags  Indicates whether the document was deleted or removed.
    @return  True if the document should be replicated, false to skip it. */
typedef bool (*CBLReplicationFilter)(void* context,
                                     CBLDocument* document,
                                     CBLDocumentFlags flags);

/** Conflict-resolution callback for use in replications. This callback will be invoked
    when the replicator finds a newer server-side revision of a document that also has local
    changes. The local and remote changes must be resolved before the document can be pushed
    to the server.
    @note  Any new CBLBlob objects set to the resolved document returned by the callback must
            not be released. They need to be retained for installation while the resolved document
            is being saved into the database, and the replicator will be responsible for
            releasing them after they are installed.
    @warning  This callback will be called on a background thread managed by the replicator.
                It must pay attention to thread-safety. However, unlike a filter callback,
                it does not need to return quickly. If it needs to prompt for user input,
                that's OK.
    @param context  The `context` field of the \ref CBLReplicatorConfiguration.
    @param documentID  The ID of the conflicted document.
    @param localDocument  The current revision of the document in the local database,
                or NULL if the local document has been deleted.
    @param remoteDocument  The revision of the document found on the server,
                or NULL if the document has been deleted on the server.
    @return  The resolved document to save locally (and push, if the replicator is pushing.)
        This can be the same as \p localDocument or \p remoteDocument, or you can create
        a mutable copy of either one and modify it appropriately.
        Or return NULL if the resolution is to delete the document. */
typedef const CBLDocument* (*CBLConflictResolver)(void* context,
                                                  FLString documentID,
                                                  const CBLDocument* localDocument,
                                                  const CBLDocument* remoteDocument);

/** Default conflict resolver. This always returns `localDocument`. */
extern const CBLConflictResolver CBLDefaultConflictResolver;


/** Types of proxy servers, for CBLProxySettings. */
typedef uint8_t CBLProxyType;


/** Proxy settings for the replicator. */
typedef struct {
    CBLProxyType type;                  ///< Type of proxy
    FLString hostname;               ///< Proxy server hostname or IP address
    uint16_t port;                      ///< Proxy server port
    FLString username;               ///< Username for proxy auth (optional)
    FLString password;               ///< Password for proxy auth
} CBLProxySettings;

typedef struct {
    CBLCollection* collection;
    CBLConflictResolver conflictResolver;
    CBLReplicationFilter pushFilter;
    CBLReplicationFilter pullFilter;
    FLArray channels;
    FLArray  documentIDs;
} CBLReplicationCollection;

/** The configuration of a replicator. */
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
    
    CBLReplicationCollection* collections;
    size_t collectionCount;
    //-- Advanced HTTP settings:
    bool acceptParentDomainCookies;
} CBLReplicatorConfiguration;


extern "Python" bool pushFilterCallback(void *context, CBLDocument* document,
                                              CBLDocumentFlags flags);
extern "Python" bool pullFilterCallback(void *context, CBLDocument* document,
                                              CBLDocumentFlags flags);
extern "Python" const CBLDocument* conflictResolverCallback(void* context,
                                              FLString documentID,
                                              const CBLDocument* localDocument,
                                              const CBLDocument* remoteDocument);

/** @} */


/** \name  Lifecycle
    @{ */

/** Creates a replicator with the given configuration. */
CBLReplicator* CBLReplicator_Create(const CBLReplicatorConfiguration*,
                                                  CBLError* outError);

/** Returns the configuration of an existing replicator. */
const CBLReplicatorConfiguration* CBLReplicator_Config(CBLReplicator*);

/** Starts a replicator, asynchronously. Does nothing if it's already started.
    @param replicator  The replicator instance.
    @param resetCheckpoint  If true, the persistent saved state ("checkpoint") for this replication
                        will be discarded, causing it to re-scan all documents. This significantly
                        increases time and bandwidth (redundant docs are not transferred, but their
                        IDs are) but can resolve unexpected problems with missing documents if one
                        side or the other has gotten out of sync. */
void CBLReplicator_Start(CBLReplicator *replicator,
                         bool resetCheckpoint);

/** Stops a running replicator, asynchronously. Does nothing if it's not already started.
    The replicator will call your \ref CBLReplicatorChangeListener with an activity level of
    \ref kCBLReplicatorStopped after it stops. Until then, consider it still active. */
void CBLReplicator_Stop(CBLReplicator*);

/** Informs the replicator whether it's considered possible to reach the remote host with
    the current network configuration. The default value is true. This only affects the
    replicator's behavior while it's in the Offline state:
    * Setting it to false will cancel any pending retry and prevent future automatic retries.
    * Setting it back to true will initiate an immediate retry.*/
void CBLReplicator_SetHostReachable(CBLReplicator*,
                                    bool reachable);

/** Puts the replicator in or out of "suspended" state. The default is false.
    * Setting suspended=true causes the replicator to disconnect and enter Offline state;
      it will not attempt to reconnect while it's suspended.
    * Setting suspended=false causes the replicator to attempt to reconnect, _if_ it was
      connected when suspended, and is still in Offline state. */
void CBLReplicator_SetSuspended(CBLReplicator* repl, bool suspended);

/** @} */



/** \name  Status and Progress
    @{
 */

/** The possible states a replicator can be in during its lifecycle. */
typedef uint8_t CBLReplicatorActivityLevel;

/** A fractional progress value, ranging from 0.0 to 1.0 as replication progresses.
    The value is very approximate and may bounce around during replication; making it more
    accurate would require slowing down the replicator and incurring more load on the server.
    It's fine to use in a progress bar, though. */
typedef struct {
    float complete;             /// Very-approximate fractional completion, from 0.0 to 1.0
    uint64_t documentCount;     ///< Number of documents transferred so far
} CBLReplicatorProgress;

/** A replicator's current status. */
typedef struct {
    CBLReplicatorActivityLevel activity;    ///< Current state
    CBLReplicatorProgress progress;         ///< Approximate fraction complete
    CBLError error;                         ///< Error, if any
} CBLReplicatorStatus;

/** Returns the replicator's current status. */
CBLReplicatorStatus CBLReplicator_Status(CBLReplicator*);

/** Indicates which documents have local changes that have not yet been pushed to the server
    by this replicator. This is of course a snapshot, that will go out of date as the replicator
    makes progress and/or documents are saved locally.

    The result is, effectively, a set of document IDs: a dictionary whose keys are the IDs and
    values are `true`.
    If there are no pending documents, the dictionary is empty.
    On error, NULL is returned.

    \note  This function can be called on a stopped or un-started replicator.
    \note  Documents that would never be pushed by this replicator, due to its configuration's
           `pushFilter` or `docIDs`, are ignored.
    \warning  You are responsible for releasing the returned array via \ref FLValue_Release. */
FLDict CBLReplicator_PendingDocumentIDs(CBLReplicator*,
                                                      CBLError* outError);

/** Indicates whether the document with the given ID has local changes that have not yet been
    pushed to the server by this replicator.

    This is equivalent to, but faster than, calling \ref CBLReplicator_PendingDocumentIDs and
    checking whether the result contains \p docID. See that function's documentation for details.

    \note  A `false` result means the document is not pending, _or_ there was an error.
           To tell the difference, compare the error code to zero. */
bool CBLReplicator_IsDocumentPending(CBLReplicator *repl,
                                     FLString docID,
                                     CBLError* outError);

FLDict CBLReplicator_PendingDocumentIDs2(CBLReplicator*, const CBLCollection* collection, CBLError* outError);

bool CBLReplicator_IsDocumentPending2(CBLReplicator *repl, FLString docID, const CBLCollection* collection, CBLError* outError);

/** A callback that notifies you when the replicator's status changes.
    @warning  This callback will be called on a background thread managed by the replicator.
                It must pay attention to thread-safety. It should not take a long time to return,
                or it will slow down the replicator.
    @param context  The value given when the listener was added.
    @param replicator  The replicator.
    @param status  The replicator's status. */
typedef void (*CBLReplicatorChangeListener)(void* context,
                                            CBLReplicator *replicator,
                                            const CBLReplicatorStatus *status);

/** Adds a listener that will be called when the replicator's status changes. */
CBLListenerToken* CBLReplicator_AddChangeListener(CBLReplicator*,
                                                  CBLReplicatorChangeListener,
                                                  void* context);


/** Information about a document that's been pushed or pulled. */
typedef struct {
    FLString ID;                ///< The document ID
    CBLDocumentFlags flags;     ///< Indicates whether the document was deleted or removed
    CBLError error;             ///< If the code is nonzero, the document failed to replicate.
    FLString scope;             ///<The scope name of the collection
    FLString collection;        ///<The collection name.
} CBLReplicatedDocument;

/** A callback that notifies you when documents are replicated.
    @warning  This callback will be called on a background thread managed by the replicator.
                It must pay attention to thread-safety. It should not take a long time to return,
                or it will slow down the replicator.
    @param context  The value given when the listener was added.
    @param replicator  The replicator.
    @param isPush  True if the document(s) were pushed, false if pulled.
    @param numDocuments  The number of documents reported by this callback.
    @param documents  An array with information about each document. */
typedef void (*CBLDocumentReplicationListener)(void *context,
                                               CBLReplicator *replicator,
                                               bool isPush,
                                               unsigned numDocuments,
                                               const CBLReplicatedDocument* documents);

/** Adds a listener that will be called when documents are replicated. */
CBLListenerToken*
CBLReplicator_AddDocumentReplicationListener(CBLReplicator*,
                                             CBLDocumentReplicationListener,
                                             void* context);

//////// CBLScope.h

FLString CBLScope_Name(const CBLScope* scope);
FLMutableArray CBLScope_CollectionNames(const CBLScope* scope, CBLError*);
CBLCollection* CBLScope_Collection(const CBLScope* scope, FLString collectionName, CBLError*);
