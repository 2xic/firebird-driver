#ifndef opcodes_H
#define opcodes_H

// From https://github.com/hgourvest/node-firebird/blob/765bc1aead64e6df37c8baa330413f4210c6cd91/lib/index.js#L283

inline const int
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

inline const int
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

inline const int
    CONNECT_VERSION2 = 2,
    CONNECT_VERSION3 = 3,
    ARCHITECTURE_GENERIC = 1;

inline const int
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
inline const int
    ptype_rpc = 2,          // Simple remote procedure call
    ptype_batch_send = 3,   // Batch sends, no asynchrony
    ptype_out_of_band = 4,  // Batch sends w/ out of band notification
    ptype_lazy_send = 5,    // Deferred packets delivery;
    ptype_mask = 0xFF,      // Mask - up to 255 types of protocol
    pflag_compress = 0x100; // Turn on compression if possible

/**********************************/
/* Database parameter block stuff */
/**********************************/
inline const int
    isc_dpb_version1                = 1,
    isc_dpb_version2                = 2, // >= FB30
    isc_dpb_cdd_pathname            = 1,
    isc_dpb_allocation              = 2,
    isc_dpb_journal                 = 3,
    isc_dpb_page_size               = 4,
    isc_dpb_num_buffers             = 5,
    isc_dpb_buffer_length           = 6,
    isc_dpb_debug                   = 7,
    isc_dpb_garbage_collect         = 8,
    isc_dpb_verify                  = 9,
    isc_dpb_sweep                   = 10,
    isc_dpb_enable_journal          = 11,
    isc_dpb_disable_journal         = 12,
    isc_dpb_dbkey_scope             = 13,
    isc_dpb_number_of_users         = 14,
    isc_dpb_trace                   = 15,
    isc_dpb_no_garbage_collect      = 16,
    isc_dpb_damaged                 = 17,
    isc_dpb_license                 = 18,
    isc_dpb_sys_user_name           = 19,
    isc_dpb_encrypt_key             = 20,
    isc_dpb_activate_shadow         = 21,
    isc_dpb_sweep_interval          = 22,
    isc_dpb_delete_shadow           = 23,
    isc_dpb_force_write             = 24,
    isc_dpb_begin_log               = 25,
    isc_dpb_quit_log                = 26,
    isc_dpb_no_reserve              = 27,
    isc_dpb_user_name               = 28,
    isc_dpb_password                = 29,
    isc_dpb_password_enc            = 30,
    isc_dpb_sys_user_name_enc       = 31,
    isc_dpb_interp                  = 32,
    isc_dpb_online_dump             = 33,
    isc_dpb_old_file_size           = 34,
    isc_dpb_old_num_files           = 35,
    isc_dpb_old_file                = 36,
    isc_dpb_old_start_page          = 37,
    isc_dpb_old_start_seqno         = 38,
    isc_dpb_old_start_file          = 39,
    isc_dpb_old_dump_id             = 41,
    isc_dpb_lc_messages             = 47,
    isc_dpb_lc_ctype                = 48,
    isc_dpb_cache_manager           = 49,
    isc_dpb_shutdown                = 50,
    isc_dpb_online                  = 51,
    isc_dpb_shutdown_delay          = 52,
    isc_dpb_reserved                = 53,
    isc_dpb_overwrite               = 54,
    isc_dpb_sec_attach              = 55,
    isc_dpb_connect_timeout         = 57,
    isc_dpb_dummy_packet_interval   = 58,
    isc_dpb_gbak_attach             = 59,
    isc_dpb_sql_role_name           = 60,
    isc_dpb_set_page_buffers        = 61,
    isc_dpb_working_directory       = 62,
    isc_dpb_sql_dialect             = 63,
    isc_dpb_set_db_readonly         = 64,
    isc_dpb_set_db_sql_dialect      = 65,
    isc_dpb_gfix_attach             = 66,
    isc_dpb_gstat_attach            = 67,
    isc_dpb_set_db_charset          = 68,
    isc_dpb_gsec_attach             = 69,
    isc_dpb_address_path            = 70,
    isc_dpb_process_id              = 71,
    isc_dpb_no_db_triggers          = 72,
    isc_dpb_trusted_auth            = 73,
    isc_dpb_process_name            = 74,
    isc_dpb_trusted_role            = 75,
    isc_dpb_org_filename            = 76,
    isc_dpb_utf8_filename           = 77,
    isc_dpb_ext_call_depth          = 78,
	isc_dpb_auth_block 				= 79,
	isc_dpb_client_version 			= 80,
	isc_dpb_remote_protocol 		= 81,
	isc_dpb_host_name 				= 82,
	isc_dpb_os_user 				= 83,
	isc_dpb_specific_auth_data 		= 84,
	isc_dpb_auth_plugin_list 		= 85,
	isc_dpb_auth_plugin_name 		= 86,
	isc_dpb_config 					= 87,
	isc_dpb_nolinger 				= 88,
	isc_dpb_reset_icu 				= 89,
	isc_dpb_map_attach 				= 90,
	isc_dpb_session_time_zone 		= 91;

inline const int
    isc_arg_end                     = 0,  // end of argument list
    isc_arg_gds                     = 1,  // generic DSRI status value
    isc_arg_string                  = 2,  // string argument
    isc_arg_cstring                 = 3,  // count & string argument
    isc_arg_number                  = 4,  // numeric argument (long)
    isc_arg_interpreted             = 5,  // interpreted status code (string)
    isc_arg_unix                    = 7,  // UNIX error code
    isc_arg_next_mach               = 15, // NeXT/Mach error code
    isc_arg_win32                   = 17, // Win32 error code
    isc_arg_warning                 = 18, // warning argument
    isc_arg_sql_state               = 19; // SQLSTATE

inline const int supported_protocols[4][5] = {
    {PROTOCOL_VERSION10, ARCHITECTURE_GENERIC, ptype_rpc, ptype_batch_send, 1},
    {PROTOCOL_VERSION11, ARCHITECTURE_GENERIC, ptype_lazy_send, ptype_lazy_send, 2},
	{PROTOCOL_VERSION12, ARCHITECTURE_GENERIC, ptype_lazy_send, ptype_lazy_send, 3},
	{PROTOCOL_VERSION13, ARCHITECTURE_GENERIC, ptype_lazy_send, ptype_lazy_send, 4}
};

/*************************************/
/* Transaction parameter block stuff */
/*************************************/
inline const int
    isc_tpb_version1                =  1,
    isc_tpb_version3                =  3,
    isc_tpb_consistency             =  1,
    isc_tpb_concurrency             =  2,
    isc_tpb_shared                  =  3, // < FB21
    isc_tpb_protected               =  4, // < FB21
    isc_tpb_exclusive               =  5, // < FB21
    isc_tpb_wait                    =  6,
    isc_tpb_nowait                  =  7,
    isc_tpb_read                    =  8,
    isc_tpb_write                   =  9,
    isc_tpb_lock_read               =  10,
    isc_tpb_lock_write              =  11,
    isc_tpb_verb_time               =  12,
    isc_tpb_commit_time             =  13,
    isc_tpb_ignore_limbo            =  14,
    isc_tpb_read_committed          =  15,
    isc_tpb_autocommit              =  16,
    isc_tpb_rec_version             =  17,
    isc_tpb_no_rec_version          =  18,
    isc_tpb_restart_requests        =  19,
    isc_tpb_no_auto_undo            =  20,
    isc_tpb_lock_timeout            =  21; // >= FB20

inline const int 
    ISOLATION_READ_UNCOMMITTED[5]          = {isc_tpb_version3, isc_tpb_write, isc_tpb_wait, isc_tpb_read_committed, isc_tpb_rec_version};
inline const int 
    ISOLATION_READ_COMMITTED[5]             = {isc_tpb_version3, isc_tpb_write, isc_tpb_wait, isc_tpb_read_committed, isc_tpb_no_rec_version};
inline const int 
    ISOLATION_REPEATABLE_READ[4]           = {isc_tpb_version3, isc_tpb_write, isc_tpb_wait, isc_tpb_concurrency};
inline const int 
    ISOLATION_SERIALIZABLE[4]              = {isc_tpb_version3, isc_tpb_write, isc_tpb_wait, isc_tpb_consistency};
inline const int 
    ISOLATION_READ_COMMITTED_READ_ONLY[5]   = {isc_tpb_version3, isc_tpb_read, isc_tpb_wait, isc_tpb_read_committed, isc_tpb_no_rec_version};

/*******************/
/* Blr definitions */
/*******************/
inline const int
    blr_text            = 14,
    blr_text2           = 15,
    blr_short           = 7,
    blr_long            = 8,
    blr_quad            = 9,
    blr_float           = 10,
    blr_double          = 27,
    blr_d_float         = 11,
    blr_timestamp       = 35,
    blr_varying         = 37,
    blr_varying2        = 38,
    blr_blob            = 261,
    blr_cstring         = 40,
    blr_cstring2        = 41,
    blr_blob_id         = 45,
    blr_sql_date        = 12,
    blr_sql_time        = 13,
    blr_int64           = 16,
    blr_blob2           = 17, // >= 2.0
    blr_domain_name     = 18, // >= 2.1
    blr_domain_name2    = 19, // >= 2.1
    blr_not_nullable    = 20, // >= 2.1
    blr_column_name     = 21, // >= 2.5
    blr_column_name2    = 22, // >= 2.5
    blr_bool            = 23, // >= 3.0

    blr_version4        = 4,
    blr_version5        = 5, // dialect 3
    blr_eoc             = 76,
    blr_end             = 255,

    blr_assignment      = 1,
    blr_begin           = 2,
    blr_dcl_variable    = 3,
    blr_message         = 4;

/*************************/
/* SQL information items */
/*************************/
inline const int
    isc_info_sql_select             = 4,
    isc_info_sql_bind               = 5,
    isc_info_sql_num_variables      = 6,
    isc_info_sql_describe_vars      = 7,
    isc_info_sql_describe_end       = 8,
    isc_info_sql_sqlda_seq          = 9,
    isc_info_sql_message_seq        = 10,
    isc_info_sql_type               = 11,
    isc_info_sql_sub_type           = 12,
    isc_info_sql_scale              = 13,
    isc_info_sql_length             = 14,
    isc_info_sql_null_ind           = 15,
    isc_info_sql_field              = 16,
    isc_info_sql_relation           = 17,
    isc_info_sql_owner              = 18,
    isc_info_sql_alias              = 19,
    isc_info_sql_sqlda_start        = 20,
    isc_info_sql_stmt_type          = 21,
    isc_info_sql_get_plan           = 22,
    isc_info_sql_records            = 23,
    isc_info_sql_batch_fetch        = 24,
    isc_info_sql_relation_alias     = 25, // >= 2.0
    isc_info_sql_explain_plan       = 26; // >= 3.0


inline const int DESCRIBE[20] = {
	isc_info_sql_stmt_type,
	isc_info_sql_select,
	isc_info_sql_describe_vars,
	isc_info_sql_sqlda_seq,
	isc_info_sql_type,
	isc_info_sql_sub_type,
	isc_info_sql_scale,
	isc_info_sql_length,
	isc_info_sql_field,
	isc_info_sql_relation,
	//isc_info_sql_owner,
	isc_info_sql_alias,
	isc_info_sql_describe_end,
	isc_info_sql_bind,
	isc_info_sql_describe_vars,
	isc_info_sql_sqlda_seq,
	isc_info_sql_type,
	isc_info_sql_sub_type,
	isc_info_sql_scale,
	isc_info_sql_length,
	isc_info_sql_describe_end
};
#endif


