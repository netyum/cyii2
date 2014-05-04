
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/array.h"


/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */
/**
 * ArrayAccessTrait provides the implementation for [[\IteratorAggregate]], [[\ArrayAccess]] and [[\Countable]].
 *
 * Note that ArrayAccessTrait requires the class using it contain a property named `data` which should be an array.
 * The data will be exposed by ArrayAccessTrait to support accessing the class object like an array.
 *
 * @property array $data
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
ZEPHIR_INIT_CLASS(yii_base_ArrayAccessTrait) {

	ZEPHIR_REGISTER_CLASS(yii\\base, ArrayAccessTrait, yii, base_arrayaccesstrait, yii_base_arrayaccesstrait_method_entry, ZEND_ACC_TRAIT);
	return SUCCESS;

}

/**
 * Returns an iterator for traversing the data.
 * This method is required by the SPL interface `IteratorAggregate`.
 * It will be implicitly called when you use `foreach` to traverse the collection.
 * @return \ArrayIterator an iterator for traversing the cookies in the collection.
 */
PHP_METHOD(yii_base_ArrayAccessTrait, getIterator) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_1;
	zend_class_entry *_0;

	ZEPHIR_MM_GROW();

	_0 = zend_fetch_class(SL("ArrayIterator"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(return_value, _0);
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("data"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, _1);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the number of data items.
 * This method is required by Countable interface.
 * @return integer number of data elements.
 */
PHP_METHOD(yii_base_ArrayAccessTrait, count) {

	zval *_0;


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("data"), PH_NOISY_CC);
	RETURN_LONG(zephir_fast_count_int(_0 TSRMLS_CC));

}

/**
 * This method is required by the interface ArrayAccess.
 * @param mixed $offset the offset to check on
 * @return boolean
 */
PHP_METHOD(yii_base_ArrayAccessTrait, offsetExists) {

	zval *offset, *_0;

	zephir_fetch_params(0, 1, 0, &offset);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("data"), PH_NOISY_CC);
	RETURN_BOOL(zephir_array_isset(_0, offset));

}

/**
 * This method is required by the interface ArrayAccess.
 * @param integer $offset the offset to retrieve element.
 * @return mixed the element at the offset, null if no element is found at the offset
 */
PHP_METHOD(yii_base_ArrayAccessTrait, offsetGet) {

	zval *offset, *data = NULL, *d, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &offset);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("data"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(data, _0);
	if (zephir_array_isset_fetch(&d, data, offset, 1 TSRMLS_CC)) {
		RETURN_CTOR(d);
	}
	RETURN_MM_NULL();

}

/**
 * This method is required by the interface ArrayAccess.
 * @param integer $offset the offset to set element
 * @param mixed $item the element value
 */
PHP_METHOD(yii_base_ArrayAccessTrait, offsetSet) {

	zval *offset, *item, *data = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &offset, &item);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("data"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(data, _0);
	zephir_array_update_zval(&data, offset, &item, PH_COPY | PH_SEPARATE);
	zephir_update_property_this(this_ptr, SL("data"), data TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * This method is required by the interface ArrayAccess.
 * @param mixed $offset the offset to unset element
 */
PHP_METHOD(yii_base_ArrayAccessTrait, offsetUnset) {

	zval *offset, *data = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &offset);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("data"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(data, _0);
	zephir_array_unset(&data, offset, PH_SEPARATE);
	zephir_update_property_this(this_ptr, SL("data"), data TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

