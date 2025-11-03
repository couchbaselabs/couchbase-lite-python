from ._PyCBL import ffi, lib
from .common import *


class ReplicatorConfiguration:
    def __init__(
        self,
        database,
        url,
        push_filter,
        pull_filter,
        conflict_resolver,
        username,
        password,
        cert_path,
        max_attempt_wait_time=30,  # Default is 30 seconds
    ):
        pinned_server_cert = []
        if cert_path:
            cert_as_bytes = open(cert_path, "rb").read()
            pinned_server_cert = [asSlice(cert_as_bytes)]

        self.database = database
        self.endpoint = lib.CBLEndpoint_CreateWithURL(stringParam(url), gError)
        self.replicator_type = 0
        self.continuous = True
        self.disable_auto_purge = True
        self.max_atempts = 0
        self.max_attempt_wait_time = max_attempt_wait_time
        self.heartbeat = 0
        self.authenticator = lib.CBLAuth_CreatePassword(stringParam(username), stringParam(password))
        self.proxy = ffi.NULL
        self.headers = ffi.NULL
        self.pinned_server_cert = pinned_server_cert
        self.truested_root_cert = []
        self.channels = ffi.NULL
        self.document_ids = ffi.NULL
        self.push_filter = push_filter
        self.pull_filter = pull_filter
        self.conflict_resolver = conflict_resolver
        self.context = ffi.NULL

    def _cblConfig(self):
        return ffi.new("CBLReplicatorConfiguration*",
                       [self.database._ref,
                        self.endpoint,
                        self.replicator_type,
                        self.continuous,
                        self.disable_auto_purge,
                        self.max_atempts,
                        self.max_attempt_wait_time,
                        self.heartbeat,
                        self.authenticator,
                        self.proxy,
                        self.headers,
                        self.pinned_server_cert,
                        self.truested_root_cert,
                        self.channels,
                        self.document_ids,
                        self.push_filter,
                        self.pull_filter,
                        self.conflict_resolver,
                        self.context])


class Replicator (CBLObject):
    def __init__(self, config):
        if config != None:
            config = config._cblConfig()
        CBLObject.__init__(self,
                           lib.CBLReplicator_Create(config, gError),
                           "Couldn't create replicator", gError)
        self.config = config

    def start(self, resetCheckpoint = False):
        lib.CBLReplicator_Start(self._ref, resetCheckpoint)

    def stop(self):
        lib.CBLReplicator_Stop(self._ref)
