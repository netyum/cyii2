
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
#include "kernel/hash.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "Zend/zend_closures.h"
#include "kernel/exception.h"
#include "kernel/concat.h"


/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */
/**
 * BaseArrayHelper provides concrete implementation for [[ArrayHelper]].
 *
 * Do not use BaseArrayHelper. Use [[ArrayHelper]] instead.
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
ZEPHIR_INIT_CLASS(yii_helpers_ArrayHelper) {

	ZEPHIR_REGISTER_CLASS(yii\\helpers, ArrayHelper, yii, helpers_arrayhelper, yii_helpers_arrayhelper_method_entry, 0);

	return SUCCESS;

}

/**
 * Converts an object or an array of objects into an array.
 * @param object|array $object the object to be converted into an array
 * @param array $properties a mapping from object class names to the properties that need to put into the resulting arrays.
 * The properties specified for each class is an array of the following format:
 *
 * ~~~
 * [
 *     'app\models\Post' => [
 *         'id',
 *         'title',
 *         // the key name in array result => property name
 *         'createTime' => 'created_at',
 *         // the key name in array result => anonymous function
 *         'length' => function ($post) {
 *             return strlen($post->content);
 *         },
 *     ],
 * ]
 * ~~~
 *
 * The result of `ArrayHelper::toArray($post, $properties)` could be like the following:
 *
 * ~~~
 * [
 *     'id' => 123,
 *     'title' => 'test',
 *     'createTime' => '2013-01-01 12:00AM',
 *     'length' => 301,
 * ]
 * ~~~
 *
 * @param boolean $recursive whether to recursively converts properties which are objects into arrays.
 * @return array the array representation of the object
 */
PHP_METHOD(yii_helpers_ArrayHelper, toArray) {

	zephir_nts_static zephir_fcall_cache_entry *_14 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_5 = NULL, *_13 = NULL;
	HashTable *_1, *_10;
	HashPosition _0, _9;
	zend_bool recursive, _3;
	zval *object, *properties = NULL, *recursive_param = NULL, *key = NULL, *value = NULL, *className, *name = NULL, *result = NULL, **_2, *_4 = NULL, *_6, *_7, *_8, **_11, *_12 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 2, &object, &properties, &recursive_param);

	ZEPHIR_SEPARATE_PARAM(object);
	if (!properties) {
		ZEPHIR_INIT_VAR(properties);
		array_init(properties);
	}
	if (!recursive_param) {
		recursive = 1;
	} else {
		recursive = zephir_get_boolval(recursive_param);
	}


	if (Z_TYPE_P(object) == IS_ARRAY) {
		if (recursive == 1) {
			zephir_is_iterable(object, &_1, &_0, 0, 0);
			for (
			  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
			  ; zephir_hash_move_forward_ex(_1, &_0)
			) {
				ZEPHIR_GET_HMKEY(key, _1, _0);
				ZEPHIR_GET_HVALUE(value, _2);
				_3 = Z_TYPE_P(value) == IS_ARRAY;
				if (!(_3)) {
					_3 = Z_TYPE_P(value) == IS_OBJECT;
				}
				if (_3) {
					ZEPHIR_CALL_SELF(&_4, "toarray", &_5, value, properties, ZEPHIR_GLOBAL(global_true));
					zephir_check_call_status();
					zephir_array_update_zval(&object, key, &_4, PH_COPY | PH_SEPARATE);
				}
			}
		}
		RETURN_CCTOR(object);
	} else {
		if (Z_TYPE_P(object) == IS_OBJECT) {
			if (zephir_fast_count_int(properties TSRMLS_CC) > 0) {
				ZEPHIR_INIT_VAR(className);
				zephir_get_class(className, object, 0 TSRMLS_CC);
				_3 = zephir_array_isset(properties, className);
				if (_3) {
					zephir_array_fetch(&_6, properties, className, PH_NOISY | PH_READONLY TSRMLS_CC);
					_3 = !ZEPHIR_IS_EMPTY(_6);
				}
				if (_3) {
					ZEPHIR_OBS_VAR(_7);
					zephir_array_fetch(&_7, properties, className, PH_NOISY TSRMLS_CC);
					if (Z_TYPE_P(_7) == IS_ARRAY) {
						ZEPHIR_INIT_VAR(result);
						array_init(result);
						zephir_array_fetch(&_8, properties, className, PH_NOISY | PH_READONLY TSRMLS_CC);
						zephir_is_iterable(_8, &_10, &_9, 0, 0);
						for (
						  ; zephir_hash_get_current_data_ex(_10, (void**) &_11, &_9) == SUCCESS
						  ; zephir_hash_move_forward_ex(_10, &_9)
						) {
							ZEPHIR_GET_HMKEY(key, _10, _9);
							ZEPHIR_GET_HVALUE(name, _11);
							if (Z_TYPE_P(key) == IS_LONG) {
								ZEPHIR_OBS_NVAR(_12);
								zephir_read_property_zval(&_12, object, name, PH_NOISY_CC);
								zephir_array_update_zval(&result, name, &_12, PH_COPY | PH_SEPARATE);
							} else {
								ZEPHIR_CALL_SELF(&_4, "getvalue", &_13, object, name);
								zephir_check_call_status();
								zephir_array_update_zval(&result, key, &_4, PH_COPY | PH_SEPARATE);
							}
						}
						if (recursive == 1) {
							ZEPHIR_RETURN_CALL_SELF("toarray", &_5, result);
							zephir_check_call_status();
							RETURN_MM();
						} else {
							RETURN_CCTOR(result);
						}
					}
				}
			}
			if (zephir_is_instance_of(object, SL("yii\\base\\Arrayable") TSRMLS_CC)) {
				ZEPHIR_CALL_METHOD(&result, object, "toarray",  NULL);
				zephir_check_call_status();
			} else {
				ZEPHIR_CALL_FUNCTION(&result, "get_object_vars", &_14, object);
				zephir_check_call_status();
			}
			if (recursive == 1) {
				ZEPHIR_RETURN_CALL_SELF("toarray", &_5, result);
				zephir_check_call_status();
				RETURN_MM();
			} else {
				RETURN_CCTOR(result);
			}
		} else {
			ZEPHIR_INIT_NVAR(result);
			array_init(result);
			zephir_array_append(&result, object, PH_SEPARATE);
			RETURN_CCTOR(result);
		}
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Merges two or more arrays into one recursively.
 * If each array has an element with the same string key value, the latter
 * will overwrite the former (different from array_merge_recursive).
 * Recursive merging will be conducted if both arrays have an element of array
 * type and are having the same key.
 * For integer-keyed elements, the elements from the latter array will
 * be appended to the former array.
 * @param array $a array to be merged to
 * @param array $b array to be merged from. You can specify additional
 * arrays via third argument, fourth argument etc.
 * @return array the merged array (the original arrays are not changed.)
 */
PHP_METHOD(yii_helpers_ArrayHelper, merge) {

     zephir_fcall_cache_entry *_9 = NULL;
     zend_bool _5, _6;
     HashTable *_3;
     HashPosition _2;
     int ZEPHIR_LAST_CALL_STATUS;
     zephir_nts_static zephir_fcall_cache_entry *_0 = NULL, *_1 = NULL;
     zval *a, *b, *args = NULL, *res = NULL, *next = NULL, *k = NULL, *v = NULL, **_4, *_7 = NULL, *_8 = NULL, *_10;

     zval ***fun_args;
     int fun_argc;

     ZEPHIR_MM_GROW();

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "+", &fun_args, &fun_argc) == FAILURE) {
         RETURN_MM_NULL();
    }
   
     if (fun_argc < 2) {
          zephir_throw_exception_string(spl_ce_BadMethodCallException, SL("Wrong number of parameters") TSRMLS_CC);
          RETURN_MM_NULL();
     }


     ZEPHIR_CALL_FUNCTION(&args, "func_get_args", &_0);
     zephir_check_call_status();
     Z_SET_ISREF_P(args);
     ZEPHIR_CALL_FUNCTION(&res, "array_shift", &_1, args);
     Z_UNSET_ISREF_P(args);
     zephir_check_call_status();
     while (1) {
          if (!(zephir_fast_count_int(args TSRMLS_CC) > 0)) {
               break;
          }
          Z_SET_ISREF_P(args);
          ZEPHIR_CALL_FUNCTION(&next, "array_shift", &_1, args);
          Z_UNSET_ISREF_P(args);
          zephir_check_call_status();
          zephir_is_iterable(next, &_3, &_2, 0, 0);
          for (
            ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
            ; zephir_hash_move_forward_ex(_3, &_2)
          ) {
               ZEPHIR_GET_HMKEY(k, _3, _2);
               ZEPHIR_GET_HVALUE(v, _4);
               if (Z_TYPE_P(k) == IS_LONG) {
                    if (zephir_array_isset(res, k)) {
                         zephir_array_append(&res, v, PH_SEPARATE);
                    } else {
                         zephir_array_update_zval(&res, k, &v, PH_COPY | PH_SEPARATE);
                    }
               } else {
                    _5 = Z_TYPE_P(v) == IS_ARRAY;
                    if (_5) {
                         _5 = zephir_array_isset(res, k);
                    }
                    _6 = _5;
                    if (_6) {
                         ZEPHIR_OBS_NVAR(_7);
                         zephir_array_fetch(&_7, res, k, PH_NOISY TSRMLS_CC);
                         _6 = Z_TYPE_P(_7) == IS_ARRAY;
                    }
                    if (_6) {
                         zephir_array_fetch(&_10, res, k, PH_NOISY | PH_READONLY TSRMLS_CC);
                         ZEPHIR_CALL_SELF(&_8, "merge", &_9, _10, v);
                         zephir_check_call_status();
                         zephir_array_update_zval(&res, k, &_8, PH_COPY | PH_SEPARATE);
                    } else {
                         zephir_array_update_zval(&res, k, &v, PH_COPY | PH_SEPARATE);
                    }
               }
          }
     }
     RETURN_CCTOR(res);

}

/**
 * Retrieves the value of an array element or object property with the given key or property name.
 * If the key does not exist in the array or object, the default value will be returned instead.
 *
 * The key may be specified in a dot format to retrieve the value of a sub-array or the property
 * of an embedded object. In particular, if the key is `x.y.z`, then the returned value would
 * be `$array['x']['y']['z']` or `$array->x->y->z` (if `$array` is an object). If `$array['x']`
 * or `$array->x` is neither an array nor an object, the default value will be returned.
 * Note that if the array already has an element `x.y.z`, then its value will be returned
 * instead of going through the sub-arrays.
 *
 * Below are some usage examples,
 *
 * ~~~
 * // working with array
 * $username = \yii\helpers\ArrayHelper::getValue($_POST, 'username');
 * // working with object
 * $username = \yii\helpers\ArrayHelper::getValue($user, 'username');
 * // working with anonymous function
 * $fullName = \yii\helpers\ArrayHelper::getValue($user, function ($user, $defaultValue) {
 *     return $user->firstName . ' ' . $user->lastName;
 * });
 * // using dot format to retrieve the property of embedded object
 * $street = \yii\helpers\ArrayHelper::getValue($users, 'address.street');
 * ~~~
 *
 * @param array|object $array array or object to extract value from
 * @param string|\Closure $key key name of the array element, or property name of the object,
 * or an anonymous function returning the value. The anonymous function signature should be:
 * `function($array, $defaultValue)`.
 * @param mixed $default the default value to be returned if the specified key does not exist
 * @return mixed the value of the element if found, default value otherwise
 * @throws InvalidParamException if $array is neither an array nor an object.
 */
PHP_METHOD(yii_helpers_ArrayHelper, getValue) {

	zephir_nts_static zephir_fcall_cache_entry *_3 = NULL, *_4 = NULL;
	zend_bool _0;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *array, *key, *default_value = NULL, *pos = NULL, *new_key = NULL, *temp_key = NULL, *new_array = NULL, *_1, _2 = zval_used_for_init, *_5;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &array, &key, &default_value);

	if (!default_value) {
		default_value = ZEPHIR_GLOBAL(global_null);
	}


	if (Z_TYPE_P(key) == IS_OBJECT) {
		if (zephir_instance_of_ev(key, zend_ce_closure TSRMLS_CC)) {
			ZEPHIR_RETURN_CALL_ZVAL_FUNCTION(key, NULL, array, default_value);
			zephir_check_call_status();
			RETURN_MM();
		}
	}
	if (Z_TYPE_P(key) == IS_STRING) {
		_0 = Z_TYPE_P(array) == IS_ARRAY;
		if (_0) {
			_0 = zephir_array_key_exists(array, key TSRMLS_CC);
		}
		if (_0) {
			zephir_array_fetch(&_1, array, key, PH_NOISY | PH_READONLY TSRMLS_CC);
			RETURN_CTOR(_1);
		}
		ZEPHIR_SINIT_VAR(_2);
		ZVAL_STRING(&_2, ".", 0);
		ZEPHIR_CALL_FUNCTION(&pos, "strrpos", &_3, key, &_2);
		zephir_check_call_status();
		if (Z_TYPE_P(pos) != IS_BOOL) {
			ZEPHIR_SINIT_NVAR(_2);
			ZVAL_LONG(&_2, 0);
			ZEPHIR_CALL_FUNCTION(&temp_key, "substr", &_4, key, &_2, pos);
			zephir_check_call_status();
			ZEPHIR_CALL_SELF(&new_array, "getvalue", NULL, array, temp_key, default_value);
			zephir_check_call_status();
			ZEPHIR_SINIT_NVAR(_2);
			ZVAL_LONG(&_2, (zephir_get_numberval(pos) + 1));
			ZEPHIR_CALL_FUNCTION(&new_key, "substr", &_4, key, &_2);
			zephir_check_call_status();
		} else {
			ZEPHIR_CPY_WRT(new_array, array);
			ZEPHIR_CPY_WRT(new_key, key);
		}
		if (Z_TYPE_P(new_array) == IS_OBJECT) {
			ZEPHIR_OBS_VAR(_5);
			zephir_read_property_zval(&_5, new_array, new_key, PH_NOISY_CC);
			RETURN_CCTOR(_5);
		} else {
			if (Z_TYPE_P(new_array) == IS_ARRAY) {
				if (zephir_array_key_exists(new_array, new_key TSRMLS_CC)) {
					zephir_array_fetch(&_1, new_array, new_key, PH_NOISY | PH_READONLY TSRMLS_CC);
					RETURN_CTOR(_1);
				} else {
					RETURN_CCTOR(default_value);
				}
			} else {
				RETURN_CCTOR(default_value);
			}
		}
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Removes an item from an array and returns the value. If the key does not exist in the array, the default value
 * will be returned instead.
 *
 * Usage examples,
 *
 * ~~~
 * // $array = ['type' => 'A', 'options' => [1, 2]];
 * // working with array
 * $type = \yii\helpers\ArrayHelper::remove($array, 'type');
 * // $array content
 * // $array = ['options' => [1, 2]];
 * ~~~
 *
 * @param array $array the array to extract value from
 * @param string $key key name of the array element
 * @param mixed $default the default value to be returned if the specified key does not exist
 * @return mixed|null the value of the element if found, default value otherwise
 */
PHP_METHOD(yii_helpers_ArrayHelper, remove) {

     zend_bool _0;
     zval *array, *key, *default_value = NULL, *value;

     ZEPHIR_MM_GROW();
     zephir_fetch_params(1, 2, 1, &array, &key, &default_value);

     if (!default_value) {
          default_value = ZEPHIR_GLOBAL(global_null);
     }


     if (Z_TYPE_P(array) == IS_ARRAY) {
          _0 = zephir_array_isset(array, key);
          if (!(_0)) {
               _0 = zephir_array_key_exists(array, key TSRMLS_CC);
          }
          if (_0) {
               Z_SET_ISREF_P(array);
               zephir_array_fetch(&value, array, key, PH_NOISY | PH_SEPARATE TSRMLS_CC);
               zephir_array_unset(&array, key, 0);
               Z_UNSET_ISREF_P(array);
               RETURN_CTOR(value);
          }
     }
     RETURN_CCTOR(default_value);

}

/**
 * Indexes an array according to a specified key.
 * The input array should be multidimensional or an array of objects.
 *
 * The key can be a key name of the sub-array, a property name of object, or an anonymous
 * function which returns the key value given an array element.
 *
 * If a key value is null, the corresponding array element will be discarded and not put in the result.
 *
 * For example,
 *
 * ~~~
 * $array = [
 *     ['id' => '123', 'data' => 'abc'],
 *     ['id' => '345', 'data' => 'def'],
 * ];
 * $result = ArrayHelper::index($array, 'id');
 * // the result is:
 * // [
 * //     '123' => ['id' => '123', 'data' => 'abc'],
 * //     '345' => ['id' => '345', 'data' => 'def'],
 * // ]
 *
 * // using anonymous function
 * $result = ArrayHelper::index($array, function ($element) {
 *     return $element['id'];
 * });
 * ~~~
 *
 * @param array $array the array that needs to be indexed
 * @param string|\Closure $key the column name or anonymous function whose result will be used to index the array
 * @return array the indexed array
 */
PHP_METHOD(yii_helpers_ArrayHelper, index) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_3 = NULL;
	HashTable *_1;
	HashPosition _0;
	zval *array, *key, *result, *value = NULL, *element = NULL, **_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &array, &key);

	ZEPHIR_INIT_VAR(result);
	array_init(result);


	zephir_is_iterable(array, &_1, &_0, 0, 0);
	for (
	  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
	  ; zephir_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HVALUE(element, _2);
		ZEPHIR_CALL_SELF(&value, "getvalue", &_3, element, key);
		zephir_check_call_status();
		zephir_array_update_zval(&result, value, &element, PH_COPY | PH_SEPARATE);
	}
	RETURN_CCTOR(result);

}

/**
 * Returns the values of a specified column in an array.
 * The input array should be multidimensional or an array of objects.
 *
 * For example,
 *
 * ~~~
 * $array = [
 *     ['id' => '123', 'data' => 'abc'],
 *     ['id' => '345', 'data' => 'def'],
 * ];
 * $result = ArrayHelper::getColumn($array, 'id');
 * // the result is: ['123', '345']
 *
 * // using anonymous function
 * $result = ArrayHelper::getColumn($array, function ($element) {
 *     return $element['id'];
 * });
 * ~~~
 *
 * @param array $array
 * @param string|\Closure $name
 * @param boolean $keepKeys whether to maintain the array keys. If false, the resulting array
 * will be re-indexed with integers.
 * @return array the list of column values
 */
PHP_METHOD(yii_helpers_ArrayHelper, getColumn) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_4 = NULL;
	HashTable *_1, *_6;
	HashPosition _0, _5;
	zend_bool keepKeys;
	zval *array_param = NULL, *name, *keepKeys_param = NULL, *result, *k = NULL, *element = NULL, **_2, *_3 = NULL, **_7;
	zval *array = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &array_param, &name, &keepKeys_param);

	zephir_get_arrval(array, array_param);
	if (!keepKeys_param) {
		keepKeys = 1;
	} else {
		keepKeys = zephir_get_boolval(keepKeys_param);
	}
	ZEPHIR_INIT_VAR(result);
	array_init(result);


	if (keepKeys == 1) {
		zephir_is_iterable(array, &_1, &_0, 0, 0);
		for (
		  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		  ; zephir_hash_move_forward_ex(_1, &_0)
		) {
			ZEPHIR_GET_HMKEY(k, _1, _0);
			ZEPHIR_GET_HVALUE(element, _2);
			ZEPHIR_CALL_SELF(&_3, "getvalue", &_4, element, name);
			zephir_check_call_status();
			zephir_array_update_zval(&result, k, &_3, PH_COPY | PH_SEPARATE);
		}
	} else {
		zephir_is_iterable(array, &_6, &_5, 0, 0);
		for (
		  ; zephir_hash_get_current_data_ex(_6, (void**) &_7, &_5) == SUCCESS
		  ; zephir_hash_move_forward_ex(_6, &_5)
		) {
			ZEPHIR_GET_HVALUE(element, _7);
			ZEPHIR_CALL_SELF(&_3, "getvalue", &_4, element, name);
			zephir_check_call_status();
			zephir_array_append(&result, _3, PH_SEPARATE);
		}
	}
	RETURN_CCTOR(result);

}

/**
 * Builds a map (key-value pairs) from a multidimensional array or an array of objects.
 * The `$from` and `$to` parameters specify the key names or property names to set up the map.
 * Optionally, one can further group the map according to a grouping field `$group`.
 *
 * For example,
 *
 * ~~~
 * $array = [
 *     ['id' => '123', 'name' => 'aaa', 'class' => 'x'],
 *     ['id' => '124', 'name' => 'bbb', 'class' => 'x'],
 *     ['id' => '345', 'name' => 'ccc', 'class' => 'y'],
 * );
 *
 * $result = ArrayHelper::map($array, 'id', 'name');
 * // the result is:
 * // [
 * //     '123' => 'aaa',
 * //     '124' => 'bbb',
 * //     '345' => 'ccc',
 * // ]
 *
 * $result = ArrayHelper::map($array, 'id', 'name', 'class');
 * // the result is:
 * // [
 * //     'x' => [
 * //         '123' => 'aaa',
 * //         '124' => 'bbb',
 * //     ],
 * //     'y' => [
 * //         '345' => 'ccc',
 * //     ],
 * // ]
 * ~~~
 *
 * @param array $array
 * @param string|\Closure $from
 * @param string|\Closure $to
 * @param string|\Closure $group
 * @return array
 */
PHP_METHOD(yii_helpers_ArrayHelper, map) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_3 = NULL;
	HashTable *_1;
	HashPosition _0;
	zval *array_param = NULL, *from, *to, *group = NULL, *result, *element = NULL, *key = NULL, *value = NULL, *group_key = NULL, **_2;
	zval *array = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 1, &array_param, &from, &to, &group);

	zephir_get_arrval(array, array_param);
	if (!group) {
		group = ZEPHIR_GLOBAL(global_null);
	}
	ZEPHIR_INIT_VAR(result);
	array_init(result);


	zephir_is_iterable(array, &_1, &_0, 0, 0);
	for (
	  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
	  ; zephir_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HVALUE(element, _2);
		ZEPHIR_CALL_SELF(&key, "getvalue", &_3, element, from);
		zephir_check_call_status();
		ZEPHIR_CALL_SELF(&value, "getvalue", &_3, element, to);
		zephir_check_call_status();
		if (Z_TYPE_P(group) != IS_NULL) {
			ZEPHIR_CALL_SELF(&group_key, "getvalue", &_3, element, group);
			zephir_check_call_status();
			zephir_array_update_multi(&result, &value TSRMLS_CC, SL("zz"), 2, group_key, key);
		} else {
			zephir_array_update_zval(&result, key, &value, PH_COPY | PH_SEPARATE);
		}
	}
	RETURN_CCTOR(result);

}

/**
 * Checks if the given array contains the specified key.
 * This method enhances the `array_key_exists()` function by supporting case-insensitive
 * key comparison.
 * @param string $key the key to check
 * @param array $array the array with keys to check
 * @param boolean $caseSensitive whether the key comparison should be case-sensitive
 * @return boolean whether the array contains the specified key
 */
PHP_METHOD(yii_helpers_ArrayHelper, keyExists) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_3 = NULL;
	HashTable *_1;
	HashPosition _0;
	zend_bool caseSensitive;
	zval *array = NULL;
	zval *key, *array_param = NULL, *caseSensitive_param = NULL, *casecmp = NULL, *keys, *k = NULL, **_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &key, &array_param, &caseSensitive_param);

	zephir_get_arrval(array, array_param);
	if (!caseSensitive_param) {
		caseSensitive = 1;
	} else {
		caseSensitive = zephir_get_boolval(caseSensitive_param);
	}


	if (caseSensitive == 1) {
		RETURN_MM_BOOL(zephir_array_key_exists(array, key TSRMLS_CC));
	} else {
		ZEPHIR_INIT_VAR(keys);
		zephir_array_keys(keys, array TSRMLS_CC);
		zephir_is_iterable(keys, &_1, &_0, 0, 0);
		for (
		  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		  ; zephir_hash_move_forward_ex(_1, &_0)
		) {
			ZEPHIR_GET_HVALUE(k, _2);
			ZEPHIR_CALL_FUNCTION(&casecmp, "strcasecmp", &_3, key, k);
			zephir_check_call_status();
			if (ZEPHIR_IS_LONG(casecmp, 0)) {
				RETURN_MM_BOOL(1);
			}
		}
		RETURN_MM_BOOL(0);
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Sorts an array of objects or arrays (with the same structure) by one or several keys.
 * @param array $array the array to be sorted. The array will be modified after calling this method.
 * @param string|\Closure|array $key the key(s) to be sorted by. This refers to a key name of the sub-array
 * elements, a property name of the objects, or an anonymous function returning the values for comparison
 * purpose. The anonymous function signature should be: `function($item)`.
 * To sort by multiple keys, provide an array of keys here.
 * @param integer|array $direction the sorting direction. It can be either `SORT_ASC` or `SORT_DESC`.
 * When sorting by multiple keys with different sorting directions, use an array of sorting directions.
 * @param integer|array $sortFlag the PHP sort flag. Valid values include
 * `SORT_REGULAR`, `SORT_NUMERIC`, `SORT_STRING`, `SORT_LOCALE_STRING`, `SORT_NATURAL` and `SORT_FLAG_CASE`.
 * Please refer to [PHP manual](http://php.net/manual/en/function.sort.php)
 * for more details. When sorting by multiple keys with different sort flags, use an array of sort flags.
 * @throws InvalidParamException if the $descending or $sortFlag parameters do not have
 * correct number of elements as that of $key.
 */
PHP_METHOD(yii_helpers_ArrayHelper, multisort) {

	zephir_fcall_cache_entry *_11 = NULL;
	HashTable *_9;
	HashPosition _8;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL, *_5 = NULL;
	zend_bool _0;
	int n = 0, ZEPHIR_LAST_CALL_STATUS;
	zval *array_param = NULL, *key, *direction = NULL, *sortFlag = NULL, *keys = NULL, *directions = NULL, *sortFlags = NULL, *args, *i = NULL, *k = NULL, *flag = NULL, *_1 = NULL, _3 = zval_used_for_init, _4 = zval_used_for_init, *_6 = NULL, *_7 = NULL, **_10, *_12, *_13;
	zval *array = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 2, &array_param, &key, &direction, &sortFlag);

	zephir_get_arrval(array, array_param);
	if (!direction) {
		ZEPHIR_CPY_WRT(direction, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(direction);
	}
	if (!sortFlag) {
		ZEPHIR_CPY_WRT(sortFlag, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(sortFlag);
	}
	ZEPHIR_INIT_VAR(args);
	array_init(args);


	if (Z_TYPE_P(direction) == IS_NULL) {
		ZEPHIR_INIT_NVAR(direction);
		ZVAL_LONG(direction, 4);
	}
	if (Z_TYPE_P(sortFlag) == IS_NULL) {
		ZEPHIR_INIT_NVAR(sortFlag);
		ZVAL_LONG(sortFlag, 0);
	}
	if (Z_TYPE_P(key) == IS_ARRAY) {
		ZEPHIR_CPY_WRT(keys, key);
	} else {
		ZEPHIR_INIT_VAR(keys);
		array_init(keys);
		zephir_array_append(&keys, key, PH_SEPARATE);
	}
	n = zephir_fast_count_int(keys TSRMLS_CC);
	_0 = n == 0;
	if (!(_0)) {
		_0 = zephir_fast_count_int(array TSRMLS_CC) == 0;
	}
	if (_0) {
		RETURN_MM_NULL();
	}
	ZEPHIR_CALL_FUNCTION(&_1, "is_scalar", &_2, direction);
	zephir_check_call_status();
	if (zephir_is_true(_1)) {
		ZEPHIR_SINIT_VAR(_3);
		ZVAL_LONG(&_3, 0);
		ZEPHIR_SINIT_VAR(_4);
		ZVAL_LONG(&_4, n);
		ZEPHIR_CALL_FUNCTION(&directions, "array_fill", &_5, &_3, &_4, direction);
		zephir_check_call_status();
	} else {
		if (zephir_fast_count_int(direction TSRMLS_CC) != n) {
			ZEPHIR_INIT_VAR(_6);
			object_init_ex(_6, yii_base_invalidparamexception_ce);
			ZEPHIR_INIT_VAR(_7);
			ZEPHIR_CONCAT_SVSVS(_7, "The length of ", direction, " parameter must be the same as that of ", keys, ".");
			ZEPHIR_CALL_METHOD(NULL, _6, "__construct", NULL, _7);
			zephir_check_call_status();
			zephir_throw_exception_debug(_6, "yii/helpers/ArrayHelper.zep", 496 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		} else {
			ZEPHIR_CPY_WRT(directions, direction);
		}
	}
	ZEPHIR_CALL_FUNCTION(&_1, "is_scalar", &_2, sortFlag);
	zephir_check_call_status();
	if (zephir_is_true(_1)) {
		ZEPHIR_SINIT_NVAR(_3);
		ZVAL_LONG(&_3, 0);
		ZEPHIR_SINIT_NVAR(_4);
		ZVAL_LONG(&_4, n);
		ZEPHIR_CALL_FUNCTION(&sortFlags, "array_fill", &_5, &_3, &_4, sortFlag);
		zephir_check_call_status();
	} else {
		if (zephir_fast_count_int(sortFlag TSRMLS_CC) != n) {
			ZEPHIR_INIT_LNVAR(_6);
			object_init_ex(_6, yii_base_invalidparamexception_ce);
			ZEPHIR_INIT_LNVAR(_7);
			ZEPHIR_CONCAT_SVSVS(_7, "The length of ", sortFlag, " parameter must be the same as that of ", keys, ".");
			ZEPHIR_CALL_METHOD(NULL, _6, "__construct", NULL, _7);
			zephir_check_call_status();
			zephir_throw_exception_debug(_6, "yii/helpers/ArrayHelper.zep", 506 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		} else {
			ZEPHIR_CPY_WRT(sortFlags, sortFlag);
		}
	}
	zephir_is_iterable(keys, &_9, &_8, 0, 0);
	for (
	  ; zephir_hash_get_current_data_ex(_9, (void**) &_10, &_8) == SUCCESS
	  ; zephir_hash_move_forward_ex(_9, &_8)
	) {
		ZEPHIR_GET_HMKEY(i, _9, _8);
		ZEPHIR_GET_HVALUE(k, _10);
		ZEPHIR_OBS_NVAR(flag);
		zephir_array_fetch(&flag, sortFlags, i, PH_NOISY TSRMLS_CC);
		ZEPHIR_CALL_SELF(&_1, "getcolumn", &_11, array, k);
		zephir_check_call_status();
		zephir_array_append(&args, _1, PH_SEPARATE);
		zephir_array_fetch(&_12, directions, i, PH_NOISY | PH_READONLY TSRMLS_CC);
		zephir_array_append(&args, _12, PH_SEPARATE);
		zephir_array_append(&args, flag, PH_SEPARATE);
	}

	Z_SET_ISREF_P(array);
	zephir_array_append(&args, array, 0);
	ZEPHIR_INIT_VAR(_13);
	ZEPHIR_SINIT_NVAR(_3);
	ZVAL_STRING(&_3, "array_multisort", 0);
	ZEPHIR_CALL_USER_FUNC_ARRAY(_13, &_3, args);
	zephir_check_call_status();
        Z_UNSET_ISREF_P(array);
	ZEPHIR_MM_RESTORE();

}

/**
 * Encodes special characters in an array of strings into HTML entities.
 * Both the array keys and values will be encoded.
 * If a value is an array, this method will also encode it recursively.
 * @param array $data data to be encoded
 * @param boolean $valuesOnly whether to encode array values only. If false,
 * both the array keys and array values will be encoded.
 * @param string $charset the charset that the data is using. If not set,
 * [[\yii\base\Application::charset]] will be used.
 * @return array the encoded data
 * @see http://www.php.net/manual/en/function.htmlspecialchars.php
 */
PHP_METHOD(yii_helpers_ArrayHelper, htmlEncode) {

	zephir_fcall_cache_entry *_9 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_7 = NULL;
	HashTable *_3;
	HashPosition _2;
	zend_bool valuesOnly, _5;
	zval *data_param = NULL, *valuesOnly_param = NULL, *charset = NULL, *d, *key = NULL, *value = NULL, *new_key = NULL, *new_value = NULL, *_0, *_1, **_4, _6 = zval_used_for_init, *_8 = NULL;
	zval *data = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 2, &data_param, &valuesOnly_param, &charset);

	zephir_get_arrval(data, data_param);
	if (!valuesOnly_param) {
		valuesOnly = 1;
	} else {
		valuesOnly = zephir_get_boolval(valuesOnly_param);
	}
	if (!charset) {
		ZEPHIR_CPY_WRT(charset, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(charset);
	}
	ZEPHIR_INIT_VAR(d);
	array_init(d);


	if (Z_TYPE_P(charset) == IS_NULL) {
		zephir_read_static_property_ce(&_0, yii_baseyii_ce, SL("app") TSRMLS_CC);
		ZEPHIR_OBS_VAR(_1);
		zephir_read_property(&_1, _0, SL("charset"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(charset, _1);
	}
	zephir_is_iterable(data, &_3, &_2, 0, 0);
	for (
	  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
	  ; zephir_hash_move_forward_ex(_3, &_2)
	) {
		ZEPHIR_GET_HMKEY(key, _3, _2);
		ZEPHIR_GET_HVALUE(value, _4);
		_5 = valuesOnly != 1;
		if (_5) {
			_5 = Z_TYPE_P(key) == IS_STRING;
		}
		if (_5) {
			ZEPHIR_SINIT_NVAR(_6);
			ZVAL_LONG(&_6, 3);
			ZEPHIR_CALL_FUNCTION(&new_key, "htmlspecialchars", &_7, key, &_6, charset);
			zephir_check_call_status();
		} else {
			ZEPHIR_CPY_WRT(new_key, key);
		}
		if (Z_TYPE_P(value) == IS_STRING) {
			ZEPHIR_SINIT_NVAR(_6);
			ZVAL_LONG(&_6, 3);
			ZEPHIR_CALL_FUNCTION(&_8, "htmlspecialchars", &_7, value, &_6, charset);
			zephir_check_call_status();
			zephir_array_update_zval(&d, key, &_8, PH_COPY | PH_SEPARATE);
		} else {
			ZEPHIR_CPY_WRT(new_value, value);
			ZEPHIR_CALL_SELF(&_8, "htmlencode", &_9, new_value, charset);
			zephir_check_call_status();
			zephir_array_update_zval(&d, key, &_8, PH_COPY | PH_SEPARATE);
		}
	}
	RETURN_CCTOR(d);

}

/**
 * Decodes HTML entities into the corresponding characters in an array of strings.
 * Both the array keys and values will be decoded.
 * If a value is an array, this method will also decode it recursively.
 * @param array $data data to be decoded
 * @param boolean $valuesOnly whether to decode array values only. If false,
 * both the array keys and array values will be decoded.
 * @return array the decoded data
 * @see http://www.php.net/manual/en/function.htmlspecialchars-decode.php
 */
PHP_METHOD(yii_helpers_ArrayHelper, htmlDecode) {

	zephir_fcall_cache_entry *_7 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_5 = NULL;
	HashTable *_1;
	HashPosition _0;
	zend_bool valuesOnly, _3;
	zval *data_param = NULL, *valuesOnly_param = NULL, *d, *key = NULL, *value = NULL, *new_key = NULL, *new_value = NULL, **_2, _4 = zval_used_for_init, *_6 = NULL;
	zval *data = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &data_param, &valuesOnly_param);

	zephir_get_arrval(data, data_param);
	if (!valuesOnly_param) {
		valuesOnly = 1;
	} else {
		valuesOnly = zephir_get_boolval(valuesOnly_param);
	}
	ZEPHIR_INIT_VAR(d);
	array_init(d);


	zephir_is_iterable(data, &_1, &_0, 0, 0);
	for (
	  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
	  ; zephir_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HMKEY(key, _1, _0);
		ZEPHIR_GET_HVALUE(value, _2);
		_3 = valuesOnly != 1;
		if (_3) {
			_3 = Z_TYPE_P(key) == IS_STRING;
		}
		if (_3) {
			ZEPHIR_SINIT_NVAR(_4);
			ZVAL_LONG(&_4, 3);
			ZEPHIR_CALL_FUNCTION(&new_key, "htmlspecialchars_decode", &_5, key, &_4);
			zephir_check_call_status();
		} else {
			ZEPHIR_CPY_WRT(new_key, key);
		}
		if (Z_TYPE_P(value) == IS_STRING) {
			ZEPHIR_SINIT_NVAR(_4);
			ZVAL_LONG(&_4, 3);
			ZEPHIR_CALL_FUNCTION(&_6, "htmlspecialchars_decode", &_5, value, &_4);
			zephir_check_call_status();
			zephir_array_update_zval(&d, key, &_6, PH_COPY | PH_SEPARATE);
		} else {
			ZEPHIR_CPY_WRT(new_value, value);
			ZEPHIR_CALL_SELF(&_6, "htmldecode", &_7, new_value);
			zephir_check_call_status();
			zephir_array_update_zval(&d, key, &_6, PH_COPY | PH_SEPARATE);
		}
	}
	RETURN_CCTOR(d);

}

/**
 * Returns a value indicating whether the given array is an associative array.
 *
 * An array is associative if all its keys are strings. If `$allStrings` is false,
 * then an array will be treated as associative if at least one of its keys is a string.
 *
 * Note that an empty array will NOT be considered associative.
 *
 * @param array $array the array being checked
 * @param boolean $allStrings whether the array keys must be all strings in order for
 * the array to be treated as associative.
 * @return boolean whether the array is associative
 */
PHP_METHOD(yii_helpers_ArrayHelper, isAssociative) {

	HashTable *_1, *_4;
	HashPosition _0, _3;
	zend_bool allStrings;
	zval *array, *allStrings_param = NULL, *key = NULL, *value = NULL, **_2, **_5;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &array, &allStrings_param);

	if (!allStrings_param) {
		allStrings = 1;
	} else {
		allStrings = zephir_get_boolval(allStrings_param);
	}


	if (Z_TYPE_P(array) != IS_ARRAY) {
		RETURN_MM_BOOL(0);
	}
	if (zephir_fast_count_int(array TSRMLS_CC) == 0) {
		RETURN_MM_BOOL(0);
	}
	if (allStrings == 1) {
		zephir_is_iterable(array, &_1, &_0, 0, 0);
		for (
		  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		  ; zephir_hash_move_forward_ex(_1, &_0)
		) {
			ZEPHIR_GET_HMKEY(key, _1, _0);
			ZEPHIR_GET_HVALUE(value, _2);
			if (Z_TYPE_P(key) != IS_STRING) {
				RETURN_MM_BOOL(0);
			}
		}
		RETURN_MM_BOOL(1);
	} else {
		zephir_is_iterable(array, &_4, &_3, 0, 0);
		for (
		  ; zephir_hash_get_current_data_ex(_4, (void**) &_5, &_3) == SUCCESS
		  ; zephir_hash_move_forward_ex(_4, &_3)
		) {
			ZEPHIR_GET_HMKEY(key, _4, _3);
			ZEPHIR_GET_HVALUE(value, _5);
			if (Z_TYPE_P(key) == IS_STRING) {
				RETURN_MM_BOOL(1);
			}
		}
		RETURN_MM_BOOL(0);
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns a value indicating whether the given array is an indexed array.
 *
 * An array is indexed if all its keys are integers. If `$consecutive` is true,
 * then the array keys must be a consecutive sequence starting from 0.
 *
 * Note that an empty array will be considered indexed.
 *
 * @param array $array the array being checked
 * @param boolean $consecutive whether the array keys must be a consecutive sequence
 * in order for the array to be treated as indexed.
 * @return boolean whether the array is associative
 */
PHP_METHOD(yii_helpers_ArrayHelper, isIndexed) {

	HashTable *_5;
	HashPosition _4;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_3 = NULL;
	zend_bool consecutive;
	zval *array, *consecutive_param = NULL, *count, *max, *_0, _1, *_2 = NULL, *key = NULL, *value = NULL, **_6;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &array, &consecutive_param);

	if (!consecutive_param) {
		consecutive = 0;
	} else {
		consecutive = zephir_get_boolval(consecutive_param);
	}


	if (Z_TYPE_P(array) != IS_ARRAY) {
		RETURN_MM_BOOL(0);
	}
	ZEPHIR_INIT_VAR(count);
	ZVAL_LONG(count, zephir_fast_count_int(array TSRMLS_CC));
	if (ZEPHIR_IS_LONG(count, 0)) {
		RETURN_MM_BOOL(1);
	}
	if (consecutive == 1) {
		ZEPHIR_INIT_VAR(max);
		ZVAL_LONG(max, (zephir_get_numberval(count) - 1));
		ZEPHIR_INIT_VAR(_0);
		zephir_array_keys(_0, array TSRMLS_CC);
		ZEPHIR_SINIT_VAR(_1);
		ZVAL_LONG(&_1, 0);
		ZEPHIR_CALL_FUNCTION(&_2, "range", &_3, &_1, max);
		zephir_check_call_status();
		RETURN_MM_BOOL(ZEPHIR_IS_EQUAL(_0, _2));
	} else {
		zephir_is_iterable(array, &_5, &_4, 0, 0);
		for (
		  ; zephir_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
		  ; zephir_hash_move_forward_ex(_5, &_4)
		) {
			ZEPHIR_GET_HMKEY(key, _5, _4);
			ZEPHIR_GET_HVALUE(value, _6);
			if (Z_TYPE_P(key) != IS_LONG) {
				RETURN_MM_BOOL(0);
			}
		}
		RETURN_MM_BOOL(1);
	}
	ZEPHIR_MM_RESTORE();

}

