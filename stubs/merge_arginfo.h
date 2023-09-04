/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 47194062a0baa7eef5d8d32bdc50bc91632ba84c */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_merge, 0, 4, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, their_heads, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, merge_options, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, checkout_options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_merge_analysis, 0, 3, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(1, preference, IS_LONG, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, their_heads, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_merge_base, 0, 3, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, one, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, two, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_merge_base_many, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, input_array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_merge_base_octopus arginfo_git_merge_base_many

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_merge_bases, 0, 3, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, one, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, two, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_merge_bases_many, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, input_array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_merge_commits, 0, 0, 4)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, our_commit)
	ZEND_ARG_INFO(0, their_commit)
	ZEND_ARG_TYPE_INFO(0, merge_options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_merge_file, 0, 4, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, ancestor, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, ours, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, theirs, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, merge_file_options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_merge_file_from_index, 0, 5, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, ancestor, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, ours, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, theirs, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, merge_file_options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_merge_trees, 0, 0, 5)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, ancestor_tree)
	ZEND_ARG_INFO(0, our_tree)
	ZEND_ARG_INFO(0, their_tree)
	ZEND_ARG_TYPE_INFO(0, merge_options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()
