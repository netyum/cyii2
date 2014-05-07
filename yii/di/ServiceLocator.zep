/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */

namespace yii\di;

use yii\BaseYii;
use Closure;
use yii\base\Component;
use yii\base\InvalidConfigException;

/**
 * ServiceLocator implements a [service locator](http://en.wikipedia.org/wiki/Service_locator_pattern).
 *
 * To use ServiceLocator, you first need to register component IDs with the corresponding component
 * definitions with the locator by calling [[set()]] or [[setComponents()]].
 * You can then call [[get()]] to retrieve a component with the specified ID. The locator will automatically
 * instantiate and configure the component according to the definition.
 *
 * For example,
 *
 * ```php
 * $locator = new \yii\di\ServiceLocator;
 * $locator->setComponents([
 *     'db' => [
 *         'class' => 'yii\db\Connection',
 *         'dsn' => 'sqlite:path/to/file.db',
 *     ],
 *     'cache' => [
 *         'class' => 'yii\caching\DbCache',
 *         'db' => 'db',
 *     ],
 * ]);
 *
 * $db = $locator->get('db');  // or $locator->db
 * $cache = $locator->get('cache');  // or $locator->cache
 * ```
 *
 * Because [[\yii\base\Module]] extends from ServiceLocator, modules and the application are all service locators.
 *
 * @property array $components The list of the component definitions or the loaded component instances (ID =>
 * definition or instance). This property is read-only.
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
class ServiceLocator extends Component
{
    /**
     * @var array shared component instances indexed by their IDs
     */
    public _components;
    /**
     * @var array component definitions indexed by their IDs
     */
    public _definitions;

    /**
     * Getter magic method.
     * This method is overridden to support accessing components like reading properties.
     * @param string $name component or property name
     * @return mixed the named property value
     */
    public function __get(string name)
    {
        if this->has(name) {
            return this->get(name);
        } else {
            return parent::__get(name);
        }
    }

    /**
     * Checks if a property value is null.
     * This method overrides the parent implementation by checking if the named component is loaded.
     * @param string $name the property name or the event name
     * @return boolean whether the property value is null
     */
    public function __isset(string name)
    {
        if this->has(name, true) {
            return true;
        } else {
            return parent::__isset(name);
        }
    }

    /**
     * Returns a value indicating whether the locator has the specified component definition or has instantiated the component.
     * This method may return different results depending on the value of `$checkInstance`.
     *
     * - If `$checkInstance` is false (default), the method will return a value indicating whether the locator has the specified
     *   component definition.
     * - If `$checkInstance` is true, the method will return a value indicating whether the locator has
     *   instantiated the specified component.
     *
     * @param string $id component ID (e.g. `db`).
     * @param boolean $checkInstance whether the method should check if the component is shared and instantiated.
     * @return boolean whether the locator has the specified component definition or has instantiated the component.
     * @see set()
     */
    public function has(string id, checkInstance = false)
    {
       var components;
        let components = this->_components;

        if typeof components != "array" {
            let components = [],
                this->_components = components;
        }

        if checkInstance {
            return isset this->_components[id];
        }
        else {
            return isset this->_definitions[id];
        }
        //return checkInstance ? isset(this->_components[id]) : isset(this->_definitions[id]);
    }

    /**
     * Returns the component instance with the specified ID.
     *
     * @param string $id component ID (e.g. `db`).
     * @param boolean $throwException whether to throw an exception if `$id` is not registered with the locator before.
     * @return object|null the component of the specified ID. If `$throwException` is false and `$id`
     * is not registered before, null will be returned.
     * @throws InvalidConfigException if `$id` refers to a nonexistent component ID
     * @see has()
     * @see set()
     */
    public function get(string id, throwException = true)
    {
        var components, component;
        let components = this->_components;

        if typeof components != "array" {
            let components = [],
                this->_components = components;
        }

        if fetch component, components[id] {
            return component;
        }

        var definition, copy_definition, copy_object, $object;
        if fetch definition, this->_definitions[id] {
            let copy_definition = definition;
            if typeof definition == "object" {
                if !(definition instanceof \Closure) {
                    let components[id] = copy_definition,
                        this->_components = components;
                    return definition;
                }
            } else {
                let $object = BaseYii::createObject(copy_definition),
                    copy_object= $object,
                    components[id] = copy_object,
                    this->_components = components;
                return $object;
            }
        }
        else {
            if throwException {
                throw new InvalidConfigException("Unknown component ID: " . id);
            } else {
                return null;
            }
        }
    }

    /**
     * Registers a component definition with this locator.
     *
     * For example,
     *
     * ```php
     * // a class name
     * $locator->set('cache', 'yii\caching\FileCache');
     *
     * // a configuration array
     * $locator->set('db', [
     *     'class' => 'yii\db\Connection',
     *     'dsn' => 'mysql:host=127.0.0.1;dbname=demo',
     *     'username' => 'root',
     *     'password' => '',
     *     'charset' => 'utf8',
     * ]);
     *
     * // an anonymous function
     * $locator->set('cache', function ($params) {
     *     return new \yii\caching\FileCache;
     * });
     *
     * // an instance
     * $locator->set('cache', new \yii\caching\FileCache);
     * ```
     *
     * If a component definition with the same ID already exists, it will be overwritten.
     *
     * @param string $id component ID (e.g. `db`).
     * @param mixed $definition the component definition to be registered with this locator.
     * It can be one of the followings:
     *
     * - a class name
     * - a configuration array: the array contains name-value pairs that will be used to
     *   initialize the property values of the newly created object when [[get()]] is called.
     *   The `class` element is required and stands for the the class of the object to be created.
     * - a PHP callable: either an anonymous function or an array representing a class method (e.g. `['Foo', 'bar']`).
     *   The callable will be called by [[get()]] to return an object associated with the specified component ID.
     * - an object: When [[get()]] is called, this object will be returned.
     *
     * @throws InvalidConfigException if the definition is an invalid configuration array
     */
    public function set(string id, definition)
    {
        var components, definitions;
        let components = this->_components;
        let definitions = this->_definitions;

        if typeof components != "array" {
            let components = [];
        }

        if typeof definitions != "array" {
            let definitions = [];
        }

        if typeof definition == "null" {
            unset components[id];
            unset definitions[id];
            let this->_components = components,
                this->_definitions = definitions;
            return;
        }

        if typeof definition == "object" || is_callable(definition, true) {
            // an object, a class name, or a PHP callable
            let definitions[id] = definition,
                this->_definitions = definitions;
        }
        else {
            if typeof definition == "array" {
                // a configuration array
                if isset definition["class"] {
                    let definitions[id] = definition,
                        this->_definitions = definitions;
                } else {
                    string text = "class";
                    throw new InvalidConfigException("The configuration for the \"" . id . "\" component must contain a \"" . text ."\" element.");
                }
            } else {
                throw new InvalidConfigException("Unexpected configuration type for the \"" . id ."\" component: " . gettype(definition));
            }
        }
    }

    /**
     * Removes the component from the locator.
     * @param string $id the component ID
     */
    public function clear(string id)
    {
        unset this->_definitions[id];
        unset this->_components[id];
    }

    /**
     * Returns the list of the component definitions or the loaded component instances.
     * @param boolean $returnDefinitions whether to return component definitions instead of the loaded component instances.
     * @return array the list of the component definitions or the loaded component instances (ID => definition or instance).
     */
    public function getComponents(bool returnDefinitions = true)
    {
        var components, definitions;
        let components = this->_components;
        let definitions = this->_definitions;

        if typeof components != "array" {
            let this->_components = [];
        }

        if typeof definitions != "array" {
            let this->_definitions = [];
        }

        return returnDefinitions ? definitions : components;
    }

    /**
     * Registers a set of component definitions in this locator.
     *
     * This is the bulk version of [[set()]]. The parameter should be an array
     * whose keys are component IDs and values the corresponding component definitions.
     *
     * For more details on how to specify component IDs and definitions, please refer to [[set()]].
     *
     * If a component definition with the same ID already exists, it will be overwritten.
     *
     * The following is an example for registering two component definitions:
     *
     * ```php
     * [
     *     'db' => [
     *         'class' => 'yii\db\Connection',
     *         'dsn' => 'sqlite:path/to/file.db',
     *     ],
     *     'cache' => [
     *         'class' => 'yii\caching\DbCache',
     *         'db' => 'db',
     *     ],
     * ]
     * ```
     *
     * @param array $components component definitions or instances
     */
    public function setComponents(components)
    {
        var id, component;
        for id, component in components {
            this->set(id, component);
        }
    }
}
