/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: b64e84e7fa50b6421659c9aa4160e193dabf67cb */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitODBBackend_read, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(1, type, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, oid, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitODBBackend_read_prefix, 0, 3, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(1, full_oid, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(1, type, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, abbrev_oid, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitODBBackend_read_header, 0, 3, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(1, size, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(1, type, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, oid, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitODBBackend_write, 0, 3, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, oid, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_GitODBBackend_writestream, 0, 2, GitODBStream, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, object_type, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_GitODBBackend_readstream, 0, 3, GitODBStream, 0)
	ZEND_ARG_TYPE_INFO(1, length, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(1, type, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, oid, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitODBBackend_exists, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, oid, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitODBBackend_exists_prefix, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(1, full_oid, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, prefix, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitODBBackend_refresh, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitODBBackend_for_each, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, payload, IS_MIXED, 0, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_GitODBBackend_writepack, 0, 2, GitODBWritepack, 0)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, payload, IS_MIXED, 0, "null")
ZEND_END_ARG_INFO()
