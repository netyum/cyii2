/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */

namespace yii\helpers;

use yii\BaseYii;
use yii\base\InvalidParamException;

/**
 * BaseArrayHelper provides concrete implementation for [[ArrayHelper]].
 *
 * Do not use BaseArrayHelper. Use [[ArrayHelper]] instead.
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
class ArrayHelper
{
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
    public static function toArray($object, properties = [], boolean recursive = true)
    {
        var key, value, className, name, result;
        if typeof $object == "array" {
            if recursive == true {
                for key, value in $object {
                    if typeof value == "array" || typeof value == "object" {
                        let $object[key] = $static::toArray(value, properties, true);
                    }
                }
            }
            return $object;
        } else {
            if typeof $object == "object" {
                if count(properties) > 0 {
                    let className = get_class($object);
                    if isset properties[className] && !empty properties[className] {
                        if typeof properties[className] == "array" {
                            let result = [];
                            for key, name in properties[className] {
                                if typeof key == "integer" {
                                    let result[name] = $object->{name};
                                }
                                else {
                                    let result[key] = $static::getValue($object, name);
                                }
                            }
                            if recursive == true {
                                return $static::toArray(result);
                            }
                            else {
                                return result;
                            }

                        }
                    }
                }

                if $object instanceof "yii\\base\\Arrayable" {
                    let result = $object->toArray();
                }
                else {
                    let result = get_object_vars($object);
                }

                if recursive == true {
                    return $static::toArray(result);
                }
                else {
                    return result;
                }
            }
            else {
                let result = [],
                    result[] = $object;
                return result;
            }
        }
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
    public static function merge(a, b)
    {
        var args, res, next;

        let args = func_get_args(),
            res = array_shift(args);

        var k, v;
        while count(args) > 0 {
            let next = array_shift(args);
            for k, v in next {
                if typeof k == "integer" {
                    if isset res[k] {
                        let res[] = v;
                    }
                    else {
                        let res[k] = v;
                    }
                }
                else {
                    if typeof v == "array" && isset res[k] && typeof res[k] == "array" {
                        let res[k] = self::merge(res[k], v);
                    }
                    else {
                        let res[k] = v;
                    }
                }
            }
        }

        return res;
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
    public static function getValue($array, key, default_value = null)
    {
        var pos, new_key, temp_key, new_array;
        if typeof key == "object" {
            if key instanceof \Closure {
                return {key}($array, default_value);
            }
        }

        if typeof key == "string" {
            if typeof $array == "array" && array_key_exists(key, $array) {
                return $array[key];
            }
            let pos = strrpos(key, ".");
            if typeof pos != "boolean" {
                let temp_key = substr(key, 0, pos),
                    new_array = $static::getValue($array, temp_key, default_value),
                    new_key = substr(key, pos + 1);
            }
            else {
                let new_array = $array,
                    new_key = key;
            }

            if typeof new_array == "object" {
                return new_array->{new_key};
            } else {
                if typeof new_array == "array" {
                    if array_key_exists(new_key, new_array) {
                        return new_array[new_key];
                    }
                    else {
                        return default_value;
                    }
                }
                else {
                    return default_value;
                }
            }
        }
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
     //remove(&$array, $key, $default = null)
    public static function remove($array, key, default_value = null)
    {
        var value;
        if typeof $array == "array" {
            if isset $array[key] || array_key_exists(key, $array) {
                let value = $array[key];
                unset $array[key];

                return value;
            }
        }

        return default_value;
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
    public static function index($array, key)
    {
        var result = [], value, element;
        for element in $array {
            let value = $static::getValue(element, key),
                result[value] = element;
        }
        return result;
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
    public static function getColumn(array $array, name, boolean keepKeys = true)
    {
        var result = [], k, element;
        if keepKeys == true {
            for k, element in  $array {
                let result[k] = $static::getValue(element, name);
            }
        } else {
            for element in $array {
                let result[] = $static::getValue(element, name);
            }
        }

        return result;
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
    public static function map(array $array, $from, $to, $group = null)
    {
        var result = [], element, key, value, group_key, result_group_key;
        for element in $array {
            let key = $static::getValue(element, from),
                value = $static::getValue(element, to);

            if typeof group != "null" {
                let group_key = $static::getValue(element, group);
                if !isset result[group_key] {
                    let result[group_key] = [];
                }
                let result_group_key = result[group_key],
                    result_group_key[key] = value,
                    result[group_key] = result_group_key;
            } else {
                let result[key] = value;
            }
        }

        return result;
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
    public static function keyExists(key, array $array, boolean caseSensitive = true)
    {
        var casecmp, keys, k;

        if caseSensitive == true {
            return array_key_exists(key, $array);
        } else {
            let keys = array_keys($array);
            for k in keys {
                let casecmp = strcasecmp(key, k);

                if casecmp == 0 {
                    return true;
                }
            }
            return false;
        }
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
     //multisort(&$array, key, direction = SORT_ASC, sortFlag = SORT_REGULAR)
    public static function multisort(array $array, key, direction = null , sortFlag = null)
    {
        var keys;
        int n = 0;
        if typeof direction == "null" {
            let direction = SORT_ASC;
        }

        if typeof sortFlag == "null" {
            let sortFlag = SORT_REGULAR;
        }


        if typeof key == "array" {
            let keys = key;
        }
        else {
            let keys = [],
                keys[] = key;
        }
    
        let n = count(keys);

        if n == 0 || count($array) == 0 {
            return;
        }

        var directions, sortFlags, args = [], i, k, flag;

        if is_scalar(direction) {
            let directions = array_fill(0, n, direction);
        } else {
            if count(direction) != n {
                throw new InvalidParamException("The length of ". direction ." parameter must be the same as that of " . keys .".");
            }
            else {
                let directions = direction;
            }
        }
        if is_scalar(sortFlag) {
            let sortFlags = array_fill(0, n, sortFlag);
        } else {
            if count(sortFlag) != n {
                throw new InvalidParamException("The length of ". sortFlag . " parameter must be the same as that of ". keys .".");
            }
            else {
                let sortFlags = sortFlag;
            }
        }

        for i, k  in keys {
             let flag = sortFlags[i],
                args[] = $static::getColumn($array, k),
                args[] = directions[i],
                args[] = flag;
        }
        //&$array;
        let args[] = $array;
        call_user_func_array("array_multisort", args);
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
    public static function htmlEncode(array data, boolean valuesOnly = true, $charset = null)
    {
        var d =[], key, value, new_key, new_value;
        if typeof charset == "null" {
            let charset = BaseYii::$app->charset;
        }

        for key, value in data {
            if valuesOnly != true && typeof key == "string" {
                let new_key = htmlspecialchars(key, ENT_QUOTES, charset);
            }
            else {
                let new_key = key;
            }

            if typeof value == "string" {
                let d[key] = htmlspecialchars(value, ENT_QUOTES, charset);
            }
            else {
                let new_value = value,
                    d[key] = $static::htmlEncode(new_value, charset);
            }
        }

        return d;
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
    public static function htmlDecode(array data, boolean valuesOnly = true)
    {
        var d = [], key, value, new_key, new_value;

        for key, value in data {
            if valuesOnly != true && typeof key == "string" {
                let new_key = htmlspecialchars_decode(key, ENT_QUOTES);
            }
            else {
                let new_key = key;
            }

            if typeof value == "string" {
                let d[key] = htmlspecialchars_decode(value, ENT_QUOTES);
            }
            else {
                let new_value = value,
                    d[key] = $static::htmlDecode(new_value);
            }
        }
        return d;
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
    public static function isAssociative($array, boolean allStrings = true)
    {
        var key, value;

        if typeof $array != "array" {
            return false;
        }

        if count($array) == 0 {
            return false;
        }

        if allStrings == true {
            for key, value in $array {
                if typeof key != "string" {
                    return false;
                }
            }
            return true;
        } else {
            for key, value in $array {
                if typeof key == "string" {
                    return true;
                }
            }
            return false;
        }
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
    public static function isIndexed($array, boolean consecutive = false)
    {
        var $count, max;
        if typeof $array != "array" {
            return false;
        }

        let $count = count($array);
        if $count == 0 {
            return true;
        }

        if consecutive == true {
            let max = $count - 1;
            return array_keys($array) == range(0, max);
        } else {
            var key, value;
            for key, value in $array {
                if typeof key != "integer" {
                    return false;
                }
            }
            return true;
        }
    }
}
