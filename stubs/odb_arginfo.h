/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 9d32bfc62303b6601087c39632139be38ed14304 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_add_alternate, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_OBJ_INFO(0, backend, GitODBBackend, 0)
	ZEND_ARG_TYPE_INFO(0, priority, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_odb_add_backend arginfo_git_odb_add_alternate

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_add_disk_alternate, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_git_odb_backend_loose, 0, 5, GitODBBackend, 0)
	ZEND_ARG_TYPE_INFO(0, objects_dir, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, compression_level, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, do_fsync, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, dir_mode, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, file_mode, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_git_odb_backend_one_pack, 0, 1, GitODBBackend, 0)
	ZEND_ARG_TYPE_INFO(0, index_file, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_git_odb_backend_pack, 0, 1, GitODBBackend, 0)
	ZEND_ARG_TYPE_INFO(0, objects_dir, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_exists, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_exists_prefix, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_TYPE_INFO(0, prefix, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_expand_ids, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_TYPE_INFO(1, ids, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_foreach, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, odb)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_git_odb_get_backend, 0, 2, GitODBBackend, 0)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_TYPE_INFO(0, pos, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_hash, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_hashfile, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_odb_new, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_num_backends, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, odb)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_object_data, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, odb_object)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_odb_object_dup, 0, 0, 1)
	ZEND_ARG_INFO(0, odb_object)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_object_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, odb_object)
ZEND_END_ARG_INFO()

#define arginfo_git_odb_object_id arginfo_git_odb_object_data

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_object_size, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, odb_object)
ZEND_END_ARG_INFO()

#define arginfo_git_odb_object_type arginfo_git_odb_object_size

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_odb_open, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, objects_dir, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_git_odb_open_rstream, 0, 4, GitODBStream, 0)
	ZEND_ARG_TYPE_INFO(1, length, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(1, type, IS_LONG, 0)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_TYPE_INFO(0, oid, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_git_odb_open_wstream, 0, 3, GitODBStream, 0)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_odb_read, 0, 0, 2)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_read_header, 0, 3, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(1, type, IS_LONG, 0)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_odb_read_prefix, 0, 0, 2)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_TYPE_INFO(0, prefix, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_odb_refresh arginfo_git_odb_free

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_stream_finalize_write, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, stream, GitODBStream, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_stream_read, 0, 2, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, stream, GitODBStream, 0)
	ZEND_ARG_TYPE_INFO(0, buffer_length, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_stream_write, 0, 2, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, stream, GitODBStream, 0)
	ZEND_ARG_TYPE_INFO(0, buffer, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_odb_write, 0, 3, IS_STRING, 0)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_git_odb_write_pack, 0, 3, GitODBWritepack, 0)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_TYPE_INFO(0, progress_callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()
