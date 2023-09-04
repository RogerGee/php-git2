/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 743a993cbd15cba0797561a05ab3421fbc14c476 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_refspec_direction, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, refspec)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_refspec_dst, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, refspec)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_refspec_dst_matches, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, refspec)
	ZEND_ARG_TYPE_INFO(0, refname, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_refspec_force, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, refspec)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_refspec_rtransform, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, refspec)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_refspec_src arginfo_git_refspec_dst

#define arginfo_git_refspec_src_matches arginfo_git_refspec_dst_matches

#define arginfo_git_refspec_string arginfo_git_refspec_dst

#define arginfo_git_refspec_transform arginfo_git_refspec_rtransform
