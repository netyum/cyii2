
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
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/hash.h"
#include "kernel/object.h"
#include "kernel/operators.h"


/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */
/**
 * ArrayableTrait provides a common implementation of the [[Arrayable]] interface.
 *
 * ArrayableTrait implements [[toArray()]] by respecting the field definitions as declared
 * in [[fields()]] and [[extraFields()]].
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
ZEPHIR_INIT_CLASS(yii_base_ArrayableTrait) {

	ZEPHIR_REGISTER_CLASS(yii\\base, ArrayableTrait, yii, base_arrayabletrait, yii_base_arrayabletrait_method_entry, ZEND_ACC_TRAIT);

	return SUCCESS;

}

/**
 * Returns the list of fields that should be returned by default by [[toArray()]] when no specific fields are specified.
 *
 * A field is a named element in the returned array by [[toArray()]].
 *
 * This method should return an array of field names or field definitions.
 * If the former, the field name will be treated as an object property name whose value will be used
 * as the field value. If the latter, the array key should be the field name while the array value should be
 * the corresponding field definition which can be either an object property name or a PHP callable
 * returning the corresponding field value. The signature of the callable should be:
 *
 * ```php
 * function ($field, $model) {
 *     // return field value
 * }
 * ```
 *
 * For example, the following code declares four fields:
 *
 * - `email`: the field name is the same as the property name `email`;
 * - `firstName` and `lastName`: the field names are `firstName` and `lastName`, and their
 *   values are obtained from the `first_name` and `last_name` properties;
 * - `fullName`: the field name is `fullName`. Its value is obtained by concatenating `first_name`
 *   and `last_name`.
 *
 * ```php
 * return [
 *     'email',
 *     'firstName' => 'first_name',
 *     'lastName' => 'last_name',
 *     'fullName' => function () {
 *         return $this->first_name . ' ' . $this->last_name;
 *     },
 * ];
 * ```
 *
 * In this method, you may also want to return different lists of fields based on some context
 * information. For example, depending on the privilege of the current application user,
 * you may return different sets of visible fields or filter out some fields.
 *
 * The default implementation of this method returns the public object member variables.
 *
 * @return array the list of field names or field definitions.
 * @see toArray()
 */
PHP_METHOD(yii_base_ArrayableTrait, fields) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL, *_2 = NULL;
	zval *fields, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(fields);
	ZEPHIR_CALL_CE_STATIC(&_0, yii_baseyii_ce, "getobjectvars", &_1, this_ptr);
	zephir_check_call_status();
	zephir_array_keys(fields, _0 TSRMLS_CC);
	ZEPHIR_RETURN_CALL_FUNCTION("array_combine", &_2, fields, fields);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the list of fields that can be expanded further and returned by [[toArray()]].
 *
 * This method is similar to [[fields()]] except that the list of fields returned
 * by this method are not returned by default by [[toArray()]]. Only when field names
 * to be expanded are explicitly specified when calling [[toArray()]], will their values
 * be exported.
 *
 * The default implementation returns an empty array.
 *
 * You may override this method to return a list of expandable fields based on some context information
 * (e.g. the current application user).
 *
 * @return array the list of expandable field names or field definitions. Please refer
 * to [[fields()]] on the format of the return value.
 * @see toArray()
 * @see fields()
 */
PHP_METHOD(yii_base_ArrayableTrait, extraFields) {


	array_init(return_value);
	return;

}

/**
 * Converts the model into an array.
 *
 * This method will first identify which fields to be included in the resulting array by calling [[resolveFields()]].
 * It will then turn the model into an array with these fields. If `$recursive` is true,
 * any embedded objects will also be converted into arrays.
 *
 * If the model implements the [[Linkable]] interface, the resulting array will also have a `_link` element
 * which refers to a list of links as specified by the interface.
 *
 * @param array $fields the fields being requested. If empty, all fields as specified by [[fields()]] will be returned.
 * @param array $expand the additional fields being requested for exporting. Only fields declared in [[extraFields()]]
 * will be considered.
 * @param boolean $recursive whether to recursively return array representation of embedded objects.
 * @return array the array representation of the object
 */
PHP_METHOD(yii_base_ArrayableTrait, toArray) {

	zephir_nts_static zephir_fcall_cache_entry *_5 = NULL, *_6 = NULL, *_9 = NULL;
	HashTable *_1;
	HashPosition _0;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *fields = NULL, *expand = NULL, *recursive = NULL, *data, *resolve_fields = NULL, *field = NULL, *definition = NULL, **_2, *_3 = NULL, *_4 = NULL, *_7 = NULL, *_8 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 3, &fields, &expand, &recursive);

	if (!fields) {
		fields = ZEPHIR_GLOBAL(global_null);
	}
	if (!expand) {
		expand = ZEPHIR_GLOBAL(global_null);
	}
	if (!recursive) {
		recursive = ZEPHIR_GLOBAL(global_null);
	}
	ZEPHIR_INIT_VAR(data);
	array_init(data);


	ZEPHIR_CALL_METHOD(&resolve_fields, this_ptr, "resolvefields", NULL, fields, expand);
	zephir_check_call_status();
	zephir_is_iterable(resolve_fields, &_1, &_0, 0, 0);
	for (
	  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
	  ; zephir_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HMKEY(field, _1, _0);
		ZEPHIR_GET_HVALUE(definition, _2);
		if (Z_TYPE_P(definition) == IS_STRING) {
			ZEPHIR_OBS_NVAR(_3);
			zephir_read_property_zval(&_3, this_ptr, definition, PH_NOISY_CC);
			zephir_array_update_zval(&data, field, &_3, PH_COPY | PH_SEPARATE);
		} else {
			ZEPHIR_CALL_FUNCTION(&_4, "call_user_func", &_5, definition, field, this_ptr);
			zephir_check_call_status();
			zephir_array_update_zval(&data, field, &_4, PH_COPY | PH_SEPARATE);
		}
	}
	if (zephir_instance_of_ev(this_ptr, yii_web_linkable_ce TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(&_7, this_ptr, "getlinks",  NULL);
		zephir_check_call_status();
		ZEPHIR_CALL_CE_STATIC(&_4, yii_web_link_ce, "serialize", &_6, _7);
		zephir_check_call_status();
		zephir_array_update_string(&data, SL("_links"), &_4, PH_COPY | PH_SEPARATE);
	}
	ZEPHIR_INIT_VAR(_8);
	if (zephir_is_true(recursive)) {
		ZEPHIR_CALL_CE_STATIC(&_8, yii_helpers_arrayhelper_ce, "toarray", &_9, data);
		zephir_check_call_status();
	} else {
		ZEPHIR_CPY_WRT(_8, data);
	}
	RETURN_CCTOR(_8);

}

/**
 * Determines which fields can be returned by [[toArray()]].
 * This method will check the requested fields against those declared in [[fields()]] and [[extraFields()]]
 * to determine which fields can be returned.
 * @param array $fields the fields being requested for exporting
 * @param array $expand the additional fields being requested for exporting
 * @return array the list of fields to be exported. The array keys are the field names, and the array values
 * are the corresponding object property names or PHP callables returning the field values.
 */
PHP_METHOD(yii_base_ArrayableTrait, resolveFields) {

	zephir_nts_static zephir_fcall_cache_entry *_5 = NULL;
	zend_bool _3;
	HashTable *_1, *_7;
	HashPosition _0, _6;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *fields_param = NULL, *expand_param = NULL, *result, *this_fields = NULL, *field = NULL, *definition = NULL, *extra_fields = NULL, *extra_field = NULL, *extra_definition = NULL, **_2, *_4 = NULL, **_8;
	zval *fields = NULL, *expand = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &fields_param, &expand_param);

	zephir_get_arrval(fields, fields_param);
	zephir_get_arrval(expand, expand_param);
	ZEPHIR_INIT_VAR(result);
	array_init(result);


	ZEPHIR_CALL_METHOD(&this_fields, this_ptr, "fields",  NULL);
	zephir_check_call_status();
	zephir_is_iterable(this_fields, &_1, &_0, 0, 0);
	for (
	  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
	  ; zephir_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HMKEY(field, _1, _0);
		ZEPHIR_GET_HVALUE(definition, _2);
		if (Z_TYPE_P(field) == IS_LONG) {
			ZEPHIR_CPY_WRT(field, definition);
		}
		_3 = zephir_fast_count_int(fields TSRMLS_CC) == 0;
		if (!(_3)) {
			ZEPHIR_CALL_FUNCTION(&_4, "in_array", &_5, field, fields, ZEPHIR_GLOBAL(global_true));
			zephir_check_call_status();
			_3 = zephir_is_true(_4);
		}
		if (_3) {
			zephir_array_update_zval(&result, field, &definition, PH_COPY | PH_SEPARATE);
		}
	}
	if (zephir_fast_count_int(expand TSRMLS_CC) == 0) {
		RETURN_CCTOR(result);
	}
	ZEPHIR_CALL_METHOD(&extra_fields, this_ptr, "extrafields",  NULL);
	zephir_check_call_status();
	zephir_is_iterable(extra_fields, &_7, &_6, 0, 0);
	for (
	  ; zephir_hash_get_current_data_ex(_7, (void**) &_8, &_6) == SUCCESS
	  ; zephir_hash_move_forward_ex(_7, &_6)
	) {
		ZEPHIR_GET_HMKEY(extra_field, _7, _6);
		ZEPHIR_GET_HVALUE(extra_definition, _8);
		if (Z_TYPE_P(extra_field) == IS_LONG) {
			ZEPHIR_CPY_WRT(extra_field, extra_definition);
		}
		ZEPHIR_CALL_FUNCTION(&_4, "in_array", &_5, extra_field, expand, ZEPHIR_GLOBAL(global_true));
		zephir_check_call_status();
		if (zephir_is_true(_4)) {
			zephir_array_update_zval(&result, extra_field, &extra_definition, PH_COPY | PH_SEPARATE);
		}
	}
	RETURN_CCTOR(result);

}

