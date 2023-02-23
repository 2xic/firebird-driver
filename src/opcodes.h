#ifndef opcodes_H
#define opcodes_H

// From https://github.com/hgourvest/node-firebird/blob/765bc1aead64e6df37c8baa330413f4210c6cd91/lib/index.js#L283
inline int
    CNCT_user = 1,   // User name
    CNCT_passwd = 2, // CNCT_ppo = 3, // Apollo person, project, organization. OBSOLETE.
    CNCT_host = 4,
    CNCT_group = 5,             // Effective Unix group id
    CNCT_user_verification = 6, // Attach/create using this connection will use user verification
    CNCT_specific_data = 7,     // Some data, needed for user verification on server
    CNCT_plugin_name = 8,       // Name of plugin, which generated that data
    CNCT_login = 9,             // Same data as isc_dpb_user_name
    CNCT_plugin_list = 10,      // List of plugins, available on client
    CNCT_client_crypt = 11,     // Client encyption level (DISABLED/ENABLED/REQUIRED)
    WIRE_CRYPT_DISABLED = 0,
    WIRE_CRYPT_ENABLED = 1,
    WIRE_CRYPT_REQUIRED = 2;

inline int
    op_void = 0,       // Packet has been voided
    op_connect = 1,    // Connect to remote server
    op_exit = 2,       // Remote end has exitted
    op_accept = 3,     // Server accepts connection
    op_reject = 4,     // Server rejects connection
    op_disconnect = 6, // Connect is going away
    op_response = 9,   // Generic response block

    // Full context server operations

    op_attach = 19,  // Attach database
    op_create = 20,  // Create database
    op_detach = 21,  // Detach database
    op_compile = 22, // Request based operations
    op_start = 23,
    op_start_and_send = 24,
    op_send = 25,
    op_receive = 26,
    op_unwind = 27, // apparently unused, see protocol.cpp's case op_unwind
    op_release = 28,

    op_transaction = 29, // Transaction operations
    op_commit = 30,
    op_rollback = 31,
    op_prepare = 32,
    op_reconnect = 33,

    op_create_blob = 34, // Blob operations
    op_open_blob = 35,
    op_get_segment = 36,
    op_put_segment = 37,
    op_cancel_blob = 38,
    op_close_blob = 39,

    op_info_database = 40, // Information services
    op_info_request = 41,
    op_info_transaction = 42,
    op_info_blob = 43,

    op_batch_segments = 44, // Put a bunch of blob segments

    op_que_events = 48,       // Que event notification request
    op_cancel_events = 49,    // Cancel event notification request
    op_commit_retaining = 50, // Commit retaining (what else)
    op_prepare2 = 51,         // Message form of prepare
    op_event = 52,            // Completed event request (asynchronous)
    op_connect_request = 53,  // Request to establish connection
    op_aux_connect = 54,      // Establish auxiliary connection
    op_ddl = 55,              // DDL call
    op_open_blob2 = 56,
    op_create_blob2 = 57,
    op_get_slice = 58,
    op_put_slice = 59,
    op_slice = 60,     // Successful response to op_get_slice
    op_seek_blob = 61, // Blob seek operation

    // DSQL operations

    op_allocate_statement = 62, // allocate a statment handle
    op_execute = 63,            // execute a prepared statement
    op_exec_immediate = 64,     // execute a statement
    op_fetch = 65,              // fetch a record
    op_fetch_response = 66,     // response for record fetch
    op_free_statement = 67,     // free a statement
    op_prepare_statement = 68,  // prepare a statement
    op_set_cursor = 69,         // set a cursor name
    op_info_sql = 70,

    op_dummy = 71,              // dummy packet to detect loss of client
    op_response_piggyback = 72, // response block for piggybacked messages
    op_start_and_receive = 73,
    op_start_send_and_receive = 74,
    op_exec_immediate2 = 75, // execute an immediate statement with msgs
    op_execute2 = 76,        // execute a statement with msgs
    op_insert = 77,
    op_sql_response = 78, // response from execute, exec immed, insert
    op_transact = 79,
    op_transact_response = 80,
    op_drop_database = 81,
    op_service_attach = 82,
    op_service_detach = 83,
    op_service_info = 84,
    op_service_start = 85,
    op_rollback_retaining = 86,
    op_partial = 89, // packet is not complete - delay processing
    op_trusted_auth = 90,
    op_cancel = 91,
    op_cont_auth = 92,
    op_ping = 93,
    op_accept_data = 94,          // Server accepts connection and returns some data to client
    op_abort_aux_connection = 95, // Async operation - stop waiting for async connection to arrive
    op_crypt = 96,
    op_crypt_key_callback = 97,
    op_cond_accept = 98; // Server accepts connection, returns some data to client
                         // and asks client to continue authentication before attach call

inline int
    CONNECT_VERSION2 = 2,
    CONNECT_VERSION3 = 3,
    ARCHITECTURE_GENERIC = 1;

inline int
    // Protocol 10 includes support for warnings and removes the requirement for
    // encoding and decoding status codes
    PROTOCOL_VERSION10 = 10,

    // Since protocol 11 we must be separated from Borland Interbase.
    // Therefore always set highmost bit in protocol version to 1.
    // For unsigned protocol version this does not break version's compare.

    FB_PROTOCOL_FLAG = 0x8000,
    FB_PROTOCOL_MASK = ~FB_PROTOCOL_FLAG & 0xFFFF,

    // Protocol 11 has support for user authentication related
    // operations (op_update_account_info, op_authenticate_user and
    // op_trusted_auth). When specific operation is not supported,
    // we say "sorry".

    PROTOCOL_VERSION11 = (FB_PROTOCOL_FLAG | 11),

    // Protocol 12 has support for asynchronous call op_cancel.
    // Currently implemented asynchronously only for TCP/IP.

    PROTOCOL_VERSION12 = (FB_PROTOCOL_FLAG | 12),

    // Protocol 13 has support for authentication plugins (op_cont_auth).

    PROTOCOL_VERSION13 = (FB_PROTOCOL_FLAG | 13);

// Protocols types (accept_type)
inline int
    ptype_rpc = 2,          // Simple remote procedure call
    ptype_batch_send = 3,   // Batch sends, no asynchrony
    ptype_out_of_band = 4,  // Batch sends w/ out of band notification
    ptype_lazy_send = 5,    // Deferred packets delivery;
    ptype_mask = 0xFF,      // Mask - up to 255 types of protocol
    pflag_compress = 0x100; // Turn on compression if possible

inline int suppoerted_protocols[4][5] = {
    {PROTOCOL_VERSION10, ARCHITECTURE_GENERIC, ptype_rpc, ptype_batch_send, 1},
    {PROTOCOL_VERSION11, ARCHITECTURE_GENERIC, ptype_lazy_send, ptype_lazy_send, 2},
	{PROTOCOL_VERSION12, ARCHITECTURE_GENERIC, ptype_lazy_send, ptype_lazy_send, 3},
	{PROTOCOL_VERSION13, ARCHITECTURE_GENERIC, ptype_lazy_send, ptype_lazy_send, 4}
};

#endif