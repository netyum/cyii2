
extern zend_class_entry *yii_base_arrayaccesstrait_ce;

ZEPHIR_INIT_CLASS(yii_base_ArrayAccessTrait);

PHP_METHOD(yii_base_ArrayAccessTrait, getIterator);
PHP_METHOD(yii_base_ArrayAccessTrait, count);
PHP_METHOD(yii_base_ArrayAccessTrait, offsetExists);
PHP_METHOD(yii_base_ArrayAccessTrait, offsetGet);
PHP_METHOD(yii_base_ArrayAccessTrait, offsetSet);
PHP_METHOD(yii_base_ArrayAccessTrait, offsetUnset);

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_base_arrayaccesstrait_offsetexists, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_base_arrayaccesstrait_offsetget, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_base_arrayaccesstrait_offsetset, 0, 0, 2)
	ZEND_ARG_INFO(0, offset)
	ZEND_ARG_INFO(0, item)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_base_arrayaccesstrait_offsetunset, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(yii_base_arrayaccesstrait_method_entry) {
	PHP_ME(yii_base_ArrayAccessTrait, getIterator, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_ArrayAccessTrait, count, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_ArrayAccessTrait, offsetExists, arginfo_yii_base_arrayaccesstrait_offsetexists, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_ArrayAccessTrait, offsetGet, arginfo_yii_base_arrayaccesstrait_offsetget, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_ArrayAccessTrait, offsetSet, arginfo_yii_base_arrayaccesstrait_offsetset, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_ArrayAccessTrait, offsetUnset, arginfo_yii_base_arrayaccesstrait_offsetunset, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
