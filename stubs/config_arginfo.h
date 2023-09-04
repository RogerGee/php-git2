/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 5ae9be82585c84582c8fb413f6a52e666aec2962 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_add_backend, 0, 5, IS_VOID, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_OBJ_INFO(0, backend, GitConfigBackend, 0)
	ZEND_ARG_TYPE_INFO(0, level, IS_LONG, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_add_file_ondisk, 0, 5, IS_VOID, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, level, IS_LONG, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_backend_foreach_match, 0, 4, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, backend, GitConfigBackend, 0)
	ZEND_ARG_TYPE_INFO(0, regexp, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_delete_entry, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_delete_multivar, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, regexp, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_find_global, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_config_find_programdata arginfo_git_config_find_global

#define arginfo_git_config_find_system arginfo_git_config_find_global

#define arginfo_git_config_find_xdg arginfo_git_config_find_global

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_foreach, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_foreach_match, 0, 4, IS_VOID, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, regexp, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, cfg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_get_bool, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_get_entry, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_get_int32, 0, 2, IS_LONG, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_config_get_int64 arginfo_git_config_get_int32

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_get_mapped, 0, 3, IS_LONG, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, maps, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_get_multivar_foreach, 0, 5, IS_VOID, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, regexp, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_get_path, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_config_get_string arginfo_git_config_get_path

#define arginfo_git_config_get_string_buf arginfo_git_config_get_path

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_iterator_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, iter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_config_iterator_glob_new, 0, 0, 2)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, regexp, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_config_iterator_new, 0, 0, 1)
	ZEND_ARG_INFO(0, cfg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_lookup_map_value, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, maps, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_config_multivar_iterator_new, 0, 0, 3)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, regexp, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_config_new, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_git_config_next, 0, 1, MAY_BE_ARRAY|MAY_BE_BOOL)
	ZEND_ARG_INFO(0, iter)
ZEND_END_ARG_INFO()

#define arginfo_git_config_open_default arginfo_git_config_new

#define arginfo_git_config_open_global arginfo_git_config_iterator_new

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_config_open_level, 0, 0, 2)
	ZEND_ARG_INFO(0, parent)
	ZEND_ARG_TYPE_INFO(0, level, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_config_open_ondisk, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_parse_bool, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_parse_int32, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_config_parse_int64 arginfo_git_config_parse_int32

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_parse_path, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_set_bool, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_set_int32, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_config_set_int64 arginfo_git_config_set_int32

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_set_multivar, 0, 4, IS_VOID, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, regexp, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_config_set_string, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_config_snapshot arginfo_git_config_iterator_new
