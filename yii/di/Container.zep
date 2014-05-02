/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */

namespace yii\di;

use ReflectionClass;
use yii\base\Component;
use yii\base\InvalidConfigException;

/**
 * Container implements a [dependency injection](http://en.wikipedia.org/wiki/Dependency_injection) container.
 *
 * A dependency injection (DI) container is an object that knows how to instantiate and configure objects and
 * all their dependent objects. For more information about DI, please refer to
 * [Martin Fowler's article](http://martinfowler.com/articles/injection.html).
 *
 * Container supports constructor injection as well as property injection.
 *
 * To use Container, you first need to set up the class dependencies by calling [[set()]].
 * You then call [[get()]] to create a new class object. Container will automatically instantiate
 * dependent objects, inject them into the object being created, configure and finally return the newly created object.
 *
 * By default, [[\Yii::$container]] refers to a Container instance which is used by [[\Yii::createObject()]]
 * to create new object instances. You may use this method to replace the `new` operator
 * when creating a new object, which gives you the benefit of automatic dependency resolution and default
 * property configuration.
 *
 * Below is an example of using Container:
 *
 * ```php
 * namespace app\models;
 *
 * use yii\base\Object;
 * use yii\db\Connection;
 * use yii\di\Container;
 *
 * interface UserFinderInterface
 * {
 *     function findUser();
 * }
 *
 * class UserFinder extends Object implements UserFinderInterface
 * {
 *     public $db;
 *
 *     public function __construct(Connection $db, $config = [])
 *     {
 *         $this->db = $db;
 *         parent::__construct($config);
 *     }
 *
 *     public function findUser()
 *     {
 *     }
 * }
 *
 * class UserLister extends Object
 * {
 *     public $finder;
 *
 *     public function __construct(UserFinderInterface $finder, $config = [])
 *     {
 *         $this->finder = $finder;
 *         parent::__construct($config);
 *     }
 * }
 *
 * $container = new Container;
 * $container->set('yii\db\Connection', [
 *     'dsn' => '...',
 * ]);
 * $container->set('app\models\UserFinderInterface', [
 *     'class' => 'app\models\UserFinder',
 * ]);
 * $container->set('userLister', 'app\models\UserLister');
 *
 * $lister = $container->get('userLister');
 *
 * // which is equivalent to:
 *
 * $db = new \yii\db\Connection(['dsn' => '...']);
 * $finder = new UserFinder($db);
 * $lister = new UserLister($finder);
 * ```
 *
 * @property array $definitions The list of the object definitions or the loaded shared objects (type or ID =>
 * definition or instance). This property is read-only.
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
class Container 
//extends Component
{
    /**
     * @var array singleton objects indexed by their types
     */
    private _singletons;
    /**
     * @var array object definitions indexed by their types
     */
    private _definitions;
    /**
     * @var array constructor parameters indexed by object types
     */
    private _params;
    /**
     * @var array cached ReflectionClass objects indexed by class/interface names
     */
    private _reflections;
    /**
     * @var array cached dependencies indexed by class/interface names. Each class name
     * is associated with a list of constructor parameter types or default values.
     */
    private _dependencies;


    /**
     * Returns an instance of the requested class.
     *
     * You may provide constructor parameters (`$params`) and object configurations (`$config`)
     * that will be used during the creation of the instance.
     *
     * Note that if the class is declared to be singleton by calling [[setSingleton()]],
     * the same instance of the class will be returned each time this method is called.
     * In this case, the constructor parameters and object configurations will be used
     * only if the class is instantiated the first time.
     *
     * @param string $class the class name or an alias name (e.g. `foo`) that was previously registered via [[set()]]
     * or [[setSingleton()]].
     * @param array $params a list of constructor parameter values. The parameters should be provided in the order
     * they appear in the constructor declaration. If you want to skip some parameters, you should index the remaining
     * ones with the integers that represent their positions in the constructor parameter list.
     * @param array $config a list of name-value pairs that will be used to initialize the object properties.
     * @return object an instance of the requested class.
     * @throws InvalidConfigException if the class cannot be recognized or correspond to an invalid definition
     */
    public function get(string $class, params = [], config = [])
    {
        var singletons, singleton, definitions;
        let singletons = this->_singletons;
        if typeof singletons != "array" {
            let this->_singletons = [];
        }

        let definitions = this->_definitions;
        if typeof definitions != "array" {
            let this->_definitions = [];
        }

        if fetch singleton, this->_singletons[$class] {
            // singleton
            return singleton;
        }
        else {
            if !isset this->_definitions[$class] {
                return this->build($class, params, config);
            }
        }

        var definition, $object;
        let definition = this->_definitions[$class];

        if is_callable(definition, true) {
            let params = this->mergeParams($class, params);
            let params = this->resolveDependencies(params);
            let $object = call_user_func(definition, this, params, config);
        }
        else {
            if typeof definition == "array" {
                var concrete;
                let concrete = definition["class"];
                unset definition["class"];

                let config = array_merge(definition, config);
                let params = this->mergeParams($class, params);

                if concrete === $class {
                    let $object = this->build($class, params, config);
                } else {
                    let $object = this->get(concrete, params, config);
                }
            }
            else {

                if typeof definition == "object" {
                    let this->_singletons[$class] = definition;
                    return definition;
                } else {
                    throw new InvalidConfigException("Unexpected object definition type: " . gettype(definition));
                }
            }
        }

        if array_key_exists($class, this->_singletons) {
            // singleton
            let this->_singletons[$class] = $object;
        }

        return $object;
    }

    /**
     * Registers a class definition with this container.
     *
     * For example,
     *
     * ```php
     * // register a class name as is. This can be skipped.
     * $container->set('yii\db\Connection');
     *
     * // register an interface
     * // When a class depends on the interface, the corresponding class
     * // will be instantiated as the dependent object
     * $container->set('yii\mail\MailInterface', 'yii\swiftmailer\Mailer');
     *
     * // register an alias name. You can use $container->get('foo')
     * // to create an instance of Connection
     * $container->set('foo', 'yii\db\Connection');
     *
     * // register a class with configuration. The configuration
     * // will be applied when the class is instantiated by get()
     * $container->set('yii\db\Connection', [
     *     'dsn' => 'mysql:host=127.0.0.1;dbname=demo',
     *     'username' => 'root',
     *     'password' => '',
     *     'charset' => 'utf8',
     * ]);
     *
     * // register an alias name with class configuration
     * // In this case, a "class" element is required to specify the class
     * $container->set('db', [
     *     'class' => 'yii\db\Connection',
     *     'dsn' => 'mysql:host=127.0.0.1;dbname=demo',
     *     'username' => 'root',
     *     'password' => '',
     *     'charset' => 'utf8',
     * ]);
     *
     * // register a PHP callable
     * // The callable will be executed when $container->get('db') is called
     * $container->set('db', function ($container, $params, $config) {
     *     return new \yii\db\Connection($config);
     * });
     * ```
     *
     * If a class definition with the same name already exists, it will be overwritten with the new one.
     * You may use [[has()]] to check if a class definition already exists.
     *
     * @param string $class class name, interface name or alias name
     * @param mixed $definition the definition associated with `$class`. It can be one of the followings:
     *
     * - a PHP callable: The callable will be executed when [[get()]] is invoked. The signature of the callable
     *   should be `function ($container, $params, $config)`, where `$params` stands for the list of constructor
     *   parameters, `$config` the object configuration, and `$container` the container object. The return value
     *   of the callable will be returned by [[get()]] as the object instance requested.
     * - a configuration array: the array contains name-value pairs that will be used to initialize the property
     *   values of the newly created object when [[get()]] is called. The `class` element stands for the
     *   the class of the object to be created. If `class` is not specified, `$class` will be used as the class name.
     * - a string: a class name, an interface name or an alias name.
     * @param array $params the list of constructor parameters. The parameters will be passed to the class
     * constructor when [[get()]] is called.
     * @return static the container itself
     */
    public function set(string $class, var definition = [], array params = [])
    {
        let this->_definitions[$class] = this->normalizeDefinition($class, definition);
        let this->_params[$class] = params;
        unset this->_singletons[$class];
        return this;
    }

    /**
     * Registers a class definition with this container and marks the class as a singleton class.
     *
     * This method is similar to [[set()]] except that classes registered via this method will only have one
     * instance. Each time [[get()]] is called, the same instance of the specified class will be returned.
     *
     * @param string $class class name, interface name or alias name
     * @param mixed $definition the definition associated with `$class`. See [[set()]] for more details.
     * @param array $params the list of constructor parameters. The parameters will be passed to the class
     * constructor when [[get()]] is called.
     * @return static the container itself
     * @see set()
     */
    public function setSingleton(string $class, var definition = [], array params = [])
    {
        let this->_definitions[$class] = this->normalizeDefinition($class, definition);
        let this->_params[$class] = params;
        let this->_singletons[$class] = null;
        return this;
    }

    /**
     * Returns a value indicating whether the container has the definition of the specified name.
     * @param string $class class name, interface name or alias name
     * @return boolean whether the container has the definition of the specified name..
     * @see set()
     */
    public function has(string $class) -> boolean
    {
        return isset this->_definitions[$class];
    }

    /**
     * Returns a value indicating whether the given name corresponds to a registered singleton.
     * @param string $class class name, interface name or alias name
     * @param boolean $checkInstance whether to check if the singleton has been instantiated.
     * @return boolean whether the given name corresponds to a registered singleton. If `$checkInstance` is true,
     * the method should return a value indicating whether the singleton has been instantiated.
     */
    public function hasSingleton(string $class, bool checkInstance = false) -> boolean
    {
        return checkInstance ? isset this->_singletons[$class] : array_key_exists($class, this->_singletons);
    }

    /**
     * Removes the definition for the specified name.
     * @param string $class class name, interface name or alias name
     */
    public function clear(string $class) -> void
    {
        unset this->_definitions[$class];
        unset this->_singletons[$class];
    }

    /**
     * Normalizes the class definition.
     * @param string $class class name
     * @param string|array|callable $definition the class definition
     * @return array the normalized class definition
     * @throws InvalidConfigException if the definition is invalid.
     */
    protected function normalizeDefinition(string $class, definition)
    {
        if empty definition {
            return ["class" : $class];
        }
        else {
            if typeof definition == "string" {
                return ["class" : definition];
            }
            else {
                if is_callable(definition, true) || typeof definition == "object" {
                    return definition;
                }
                else {
                    if typeof definition == "array" {
                        if !isset definition["class"] {
                            if strpos($class , "\\") !== false {
                                let definition["class"] = $class;
                            } else {
                                string text = "class";
                                throw new InvalidConfigException("A class definition requires a \"". text ."\" member.");
                            }
                        }
                        return definition;
                    } else {
                        throw new InvalidConfigException("Unsupported definition type for \"" . $class ."\": " . gettype(definition));
                    }

                }
            }
        }
    }

    /**
     * Returns the list of the object definitions or the loaded shared objects.
     * @return array the list of the object definitions or the loaded shared objects (type or ID => definition or instance).
     */
    public function getDefinitions()
    {
        return this->_definitions;
    }

    /**
     * Creates an instance of the specified class.
     * This method will resolve dependencies of the specified class, instantiate them, and inject
     * them into the new instance of the specified class.
     * @param string $class the class name
     * @param array $params constructor parameters
     * @param array $config configurations to be applied to the new instance
     * @return object the newly created instance of the specified class
     */
    protected function build(string $class, params, config)
    {
        /** @var ReflectionClass $reflection */
        var elements = [], reflection, dependencies;
        let elements = this->getDependencies($class);

        let reflection   = elements[0],
            dependencies = elements[1];

        var index, param;
        for index, param in params {
            let dependencies[index] = param;
        }

        if !empty dependencies && is_a($class, "yii\\base\\Object", true) {
            // set $config as the last parameter (existing one will be overwritten) 
            if count(dependencies)  {
                var i;
                let i = count(dependencies),
                    i = i - 1;
                let dependencies[i] = config;
            }
            let dependencies = this->resolveDependencies(dependencies, reflection);
            return reflection->newInstanceArgs(dependencies);
        } else {
            var $object;
            let dependencies = this->resolveDependencies(dependencies, reflection);
            let $object = reflection->newInstanceArgs(dependencies);
            var name, value;
            for name , value in config {
                let $object->{name} = value;
            }
            return $object;
        }
    }

    /**
     * Merges the user-specified constructor parameters with the ones registered via [[set()]].
     * @param string $class class name, interface name or alias name
     * @param array $params the constructor parameters
     * @return array the merged parameters
     */
    protected function mergeParams(string $class, params)
    {
        if empty this->_params[$class] {
            return params;
        }

        if empty params {
            return this->_params[$class];
        } else {
            var ps, index, value;
            let ps = this->_params[$class];
            for index, value in params {
                let ps[index] = value;
            }
            return ps;
        }
    }

    /**
     * Returns the dependencies of the specified class.
     * @param string $class class name, interface name or alias name
     * @return array the dependencies of the specified class.
     */
    protected function getDependencies(string $class)
    {
        var r;
        if fetch r, this->_reflections[$class] {
            return [r,  this->_dependencies[$class]];
        }

        var dependencies = [], reflection, constructor;
        let reflection = new \ReflectionClass($class);

        let constructor = reflection->getConstructor();
        if typeof constructor !== "null" {
            var param;
            for param in constructor->getParameters() {
                if param->isDefaultValueAvailable() {
                    let dependencies[] = param->getDefaultValue();
                } else {
                    var c;
                    let c = param->getClass();
                    if typeof c == "null" {
                        let dependencies[] = Instance::of(c);
                    }
                    else {
                        let dependencies[] = Instance::of(c->getName());
                    }
                }
            }
        }

        let this->_reflections[$class] = reflection;
        let this->_dependencies[$class] = dependencies;

        return [reflection, dependencies];
    }

    /**
     * Resolves dependencies by replacing them with the actual object instances.
     * @param array $dependencies the dependencies
     * @param ReflectionClass $reflection the class reflection associated with the dependencies
     * @return array the resolved dependencies
     * @throws InvalidConfigException if a dependency cannot be resolved or if a dependency cannot be fulfilled.
     */
    protected function resolveDependencies(dependencies, var reflection = null)
    {
        var index, dependency;
        for index, dependency in dependencies {
            if typeof dependency == "object" && (dependency instanceof Instance) {
                if dependency->id !== null {
                    let dependencies[index] = this->get(dependency->id);
                }
                else {
                    if typeof reflection !== "null" {
                        var name, $class;
                        let name = reflection->getConstructor()->getParameters()[index]->getName();
                        let $class = reflection->getName();
                        throw new InvalidConfigException("Missing required parameter \"" . name ."\" when instantiating \"". $class ."\".");
                    }
                }
            }
        }
        return dependencies;
    }
}
