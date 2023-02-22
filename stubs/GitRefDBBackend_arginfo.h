/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 7926bf51aecd4d309ef17773503d6a3c019b2ab6 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitRefDBBackend_exists, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, ref_name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitRefDBBackend_lookup, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, ref_name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitRefDBBackend_iterator_new, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, glob, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitRefDBBackend_iterator_next, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(1, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitRefDBBackend_write, 0, 6, IS_VOID, 0)
	ZEND_ARG_INFO(0, ref)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, who, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, old, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, old_target, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_GitRefDBBackend_rename, 0, 0, 5)
	ZEND_ARG_TYPE_INFO(0, old_name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, new_name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, who, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitRefDBBackend_del, 0, 3, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, ref_name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, old_id, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, old_target, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitRefDBBackend_compress, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_GitRefDBBackend_has_log arginfo_class_GitRefDBBackend_exists

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitRefDBBackend_ensure_log, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, ref_name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitRefDBBackend_reflog_read, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitRefDBBackend_reflog_write, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, reflog)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitRefDBBackend_reflog_rename, 0, 2, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, old_name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, new_name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitRefDBBackend_reflog_delete, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitRefDBBackend_lock, 0, 1, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, ref_name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_GitRefDBBackend_unlock, 0, 6, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, success, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, update_reflog, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, ref)
	ZEND_ARG_TYPE_INFO(0, sig, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
ZEND_END_ARG_INFO()
