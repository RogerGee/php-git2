/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: d05cdd45bb876fe87528274943bc748ad6e46dbb */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_blob_create_frombuffer, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, buffer, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_blob_create_fromdisk, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_git_blob_create_fromstream, 0, 2, GitWritestream, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, hintpath, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_blob_create_fromstream_commit, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, stream, GitWritestream, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_blob_create_fromworkdir, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, relative_path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_blob_dup, 0, 0, 1)
	ZEND_ARG_INFO(0, blob)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_blob_filtered_content, 0, 3, IS_STRING, 0)
	ZEND_ARG_INFO(0, blob)
	ZEND_ARG_TYPE_INFO(0, as_path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, check_for_binary_data, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_blob_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, blob)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_blob_id, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, blob)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_blob_is_binary, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, blob)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_blob_lookup, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_blob_lookup_prefix arginfo_git_blob_lookup

#define arginfo_git_blob_owner arginfo_git_blob_dup

#define arginfo_git_blob_rawcontent arginfo_git_blob_id

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_blob_rawsize, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, blob)
ZEND_END_ARG_INFO()
