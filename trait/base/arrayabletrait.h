
extern zend_class_entry *yii_base_arrayabletrait_ce;

ZEPHIR_INIT_CLASS(yii_base_ArrayableTrait);

PHP_METHOD(yii_base_ArrayableTrait, fields);
PHP_METHOD(yii_base_ArrayableTrait, extraFields);
PHP_METHOD(yii_base_ArrayableTrait, toArray);
PHP_METHOD(yii_base_ArrayableTrait, resolveFields);

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_base_arrayabletrait_toarray, 0, 0, 0)
     ZEND_ARG_INFO(0, fields)
     ZEND_ARG_INFO(0, expand)
     ZEND_ARG_INFO(0, recursive)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_base_arrayabletrait_resolvefields, 0, 0, 2)
     ZEND_ARG_INFO(0, fields)
     ZEND_ARG_INFO(0, expand)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(yii_base_arrayabletrait_method_entry) {
     PHP_ME(yii_base_ArrayableTrait, fields, NULL, ZEND_ACC_PUBLIC)
     PHP_ME(yii_base_ArrayableTrait, extraFields, NULL, ZEND_ACC_PUBLIC)
     PHP_ME(yii_base_ArrayableTrait, toArray, arginfo_yii_base_arrayabletrait_toarray, ZEND_ACC_PUBLIC)
     PHP_ME(yii_base_ArrayableTrait, resolveFields, arginfo_yii_base_arrayabletrait_resolvefields, ZEND_ACC_PROTECTED)
  PHP_FE_END
};