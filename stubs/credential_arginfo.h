/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 51393e44990bad3ace17e2dda56983c301d584a7 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_credential_default_new, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_credential_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, cred)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_credential_has_username, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, cred)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_credential_ssh_key_from_agent, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, username, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_credential_ssh_key_memory_new, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, username, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, publickey, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, privatekey, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, passphrase, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_credential_ssh_key_new arginfo_git_credential_ssh_key_memory_new

#define arginfo_git_credential_username_new arginfo_git_credential_ssh_key_from_agent

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_credential_userpass_plaintext_new, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, username, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, password, IS_STRING, 0)
ZEND_END_ARG_INFO()
