/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 2c9b261629dd80b2ef987b30320829cc9dac83af */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_commit_amend, 0, 7, IS_STRING, 0)
	ZEND_ARG_INFO(0, commit_to_amend)
	ZEND_ARG_TYPE_INFO(0, update_ref, IS_STRING, 1)
	ZEND_ARG_INFO(0, author)
	ZEND_ARG_INFO(0, committer)
	ZEND_ARG_TYPE_INFO(0, message_encoding, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 1)
	ZEND_ARG_INFO(0, tree)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_commit_author, 0, 0, 1)
	ZEND_ARG_INFO(0, commit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_commit_body, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, commit)
ZEND_END_ARG_INFO()

#define arginfo_git_commit_committer arginfo_git_commit_author

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_commit_create, 0, 8, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, update_ref, IS_STRING, 1)
	ZEND_ARG_INFO(0, author)
	ZEND_ARG_INFO(0, committer)
	ZEND_ARG_TYPE_INFO(0, message_encoding, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
	ZEND_ARG_INFO(0, tree)
	ZEND_ARG_TYPE_INFO(0, parents, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_commit_create_buffer, 0, 7, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, author)
	ZEND_ARG_INFO(0, committer)
	ZEND_ARG_TYPE_INFO(0, message_encoding, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
	ZEND_ARG_INFO(0, tree)
	ZEND_ARG_TYPE_INFO(0, parents, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_commit_create_from_callback, 0, 9, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, update_ref, IS_STRING, 1)
	ZEND_ARG_INFO(0, author)
	ZEND_ARG_INFO(0, committer)
	ZEND_ARG_TYPE_INFO(0, message_encoding, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, tree_id, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_commit_create_from_ids, 0, 8, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, update_ref, IS_STRING, 1)
	ZEND_ARG_INFO(0, author)
	ZEND_ARG_INFO(0, committer)
	ZEND_ARG_TYPE_INFO(0, message_encoding, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, tree_id, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, parents, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_commit_create_with_signature, 0, 3, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, commit_content, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, signature, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, signature_field, IS_STRING, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_commit_dup, 0, 0, 1)
	ZEND_ARG_INFO(0, source)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_commit_extract_signature, 0, 3, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(1, signed_data_out, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, commit_id, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, field, IS_STRING, 0, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_commit_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, commit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_commit_header_field, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, commit)
	ZEND_ARG_TYPE_INFO(0, field, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_commit_id arginfo_git_commit_body

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_commit_lookup, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_commit_lookup_prefix arginfo_git_commit_lookup

#define arginfo_git_commit_message arginfo_git_commit_body

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_commit_message_encoding, 0, 1, IS_STRING, 1)
	ZEND_ARG_INFO(0, commit)
ZEND_END_ARG_INFO()

#define arginfo_git_commit_message_raw arginfo_git_commit_body

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_commit_nth_gen_ancestor, 0, 0, 2)
	ZEND_ARG_INFO(0, commit)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_commit_owner arginfo_git_commit_author

#define arginfo_git_commit_parent arginfo_git_commit_nth_gen_ancestor

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_commit_parent_id, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, commit)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_commit_parentcount, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, commit)
ZEND_END_ARG_INFO()

#define arginfo_git_commit_raw_header arginfo_git_commit_body

#define arginfo_git_commit_summary arginfo_git_commit_body

#define arginfo_git_commit_time arginfo_git_commit_parentcount

#define arginfo_git_commit_time_offset arginfo_git_commit_parentcount

#define arginfo_git_commit_tree arginfo_git_commit_author

#define arginfo_git_commit_tree_id arginfo_git_commit_body
