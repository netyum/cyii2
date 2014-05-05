
extern zend_class_entry *yii_helpers_arrayhelper_ce;

ZEPHIR_INIT_CLASS(yii_helpers_ArrayHelper);

PHP_METHOD(yii_helpers_ArrayHelper, toArray);
PHP_METHOD(yii_helpers_ArrayHelper, merge);
PHP_METHOD(yii_helpers_ArrayHelper, getValue);
PHP_METHOD(yii_helpers_ArrayHelper, remove);
PHP_METHOD(yii_helpers_ArrayHelper, index);
PHP_METHOD(yii_helpers_ArrayHelper, getColumn);
PHP_METHOD(yii_helpers_ArrayHelper, map);
PHP_METHOD(yii_helpers_ArrayHelper, keyExists);
PHP_METHOD(yii_helpers_ArrayHelper, multisort);
PHP_METHOD(yii_helpers_ArrayHelper, htmlEncode);
PHP_METHOD(yii_helpers_ArrayHelper, htmlDecode);
PHP_METHOD(yii_helpers_ArrayHelper, isAssociative);
PHP_METHOD(yii_helpers_ArrayHelper, isIndexed);

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_helpers_arrayhelper_toarray, 0, 0, 1)
	ZEND_ARG_INFO(0, object)
	ZEND_ARG_INFO(0, properties)
	ZEND_ARG_INFO(0, recursive)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_helpers_arrayhelper_merge, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
	ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_helpers_arrayhelper_getvalue, 0, 0, 2)
	ZEND_ARG_INFO(0, array)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, default_value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_helpers_arrayhelper_remove, 0, 0, 2)
	ZEND_ARG_INFO(0, array)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, default_value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_helpers_arrayhelper_index, 0, 0, 2)
	ZEND_ARG_INFO(0, array)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_helpers_arrayhelper_getcolumn, 0, 0, 2)
	ZEND_ARG_INFO(0, array)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, keepKeys)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_helpers_arrayhelper_map, 0, 0, 3)
	ZEND_ARG_INFO(0, array)
	ZEND_ARG_INFO(0, from)
	ZEND_ARG_INFO(0, to)
	ZEND_ARG_INFO(0, group)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_helpers_arrayhelper_keyexists, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, array)
	ZEND_ARG_INFO(0, caseSensitive)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_helpers_arrayhelper_multisort, 0, 0, 2)
	ZEND_ARG_INFO(0, array)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, direction)
	ZEND_ARG_INFO(0, sortFlag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_helpers_arrayhelper_htmlencode, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, valuesOnly)
	ZEND_ARG_INFO(0, charset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_helpers_arrayhelper_htmldecode, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, valuesOnly)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_helpers_arrayhelper_isassociative, 0, 0, 1)
	ZEND_ARG_INFO(0, array)
	ZEND_ARG_INFO(0, allStrings)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_helpers_arrayhelper_isindexed, 0, 0, 1)
	ZEND_ARG_INFO(0, array)
	ZEND_ARG_INFO(0, consecutive)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(yii_helpers_arrayhelper_method_entry) {
	PHP_ME(yii_helpers_ArrayHelper, toArray, arginfo_yii_helpers_arrayhelper_toarray, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yii_helpers_ArrayHelper, merge, arginfo_yii_helpers_arrayhelper_merge, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yii_helpers_ArrayHelper, getValue, arginfo_yii_helpers_arrayhelper_getvalue, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yii_helpers_ArrayHelper, remove, arginfo_yii_helpers_arrayhelper_remove, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yii_helpers_ArrayHelper, index, arginfo_yii_helpers_arrayhelper_index, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yii_helpers_ArrayHelper, getColumn, arginfo_yii_helpers_arrayhelper_getcolumn, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yii_helpers_ArrayHelper, map, arginfo_yii_helpers_arrayhelper_map, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yii_helpers_ArrayHelper, keyExists, arginfo_yii_helpers_arrayhelper_keyexists, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yii_helpers_ArrayHelper, multisort, arginfo_yii_helpers_arrayhelper_multisort, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yii_helpers_ArrayHelper, htmlEncode, arginfo_yii_helpers_arrayhelper_htmlencode, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yii_helpers_ArrayHelper, htmlDecode, arginfo_yii_helpers_arrayhelper_htmldecode, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yii_helpers_ArrayHelper, isAssociative, arginfo_yii_helpers_arrayhelper_isassociative, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yii_helpers_ArrayHelper, isIndexed, arginfo_yii_helpers_arrayhelper_isindexed, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
  PHP_FE_END
};
