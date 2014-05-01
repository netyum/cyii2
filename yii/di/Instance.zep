/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */

namespace yii\di;

use yii\BaseYii;
use yii\base\InvalidConfigException;

/**
 * Instance represents a reference to a named object in a dependency injection (DI) container or a service locator.
 *
 * You may use [[get()]] to obtain the actual object referenced by [[id]].
 *
 * Instance is mainly used in two places:
 *
 * - When configuring a dependency injection container, you use Instance to reference a class name, interface name
 *   or alias name. The reference can later be resolved into the actual object by the container.
 * - In classes which use service locator to obtain dependent objects.
 *
 * The following example shows how to configure a DI container with Instance:
 *
 * ```php
 * $container = new \yii\di\Container;
 * $container->set('cache', 'yii\caching\DbCache', Instance::of('db'));
 * $container->set('db', [
 *     'class' => 'yii\db\Connection',
 *     'dsn' => 'sqlite:path/to/file.db',
 * ]);
 * ```
 *
 * And the following example shows how a class retrieves a component from a service locator:
 *
 * ```php
 * class DbCache extends Cache
 * {
 *     public $db = 'db';
 *
 *     public function init()
 *     {
 *         parent::init();
 *         $this->db = Instance::ensure($this->db, 'yii\db\Connection');
 *     }
 * }
 * ```
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
class Instance
{
    /**
     * @var string the component ID, class name, interface name or alias name
     */
    public id;

    /**
     * Constructor.
     * @param string $id the component ID
     */
    protected function __construct(id)
    {
        let this->id = id;
    }

    /**
     * Creates a new Instance object.
     * @param string $id the component ID
     * @return Instance the new Instance object.
     */
    public static function of(id)
    {
        return new $static(id);
    }

    /**
     * Resolves the specified reference into the actual object and makes sure it is of the specified type.
     *
     * The reference may be specified as a string or an Instance object. If the former,
     * it will be treated as a component ID, a class/interface name or an alias, depending on the container type.
     *
     * If you do not specify a container, the method will first try `Yii::$app` followed by `Yii::$container`.
     *
     * For example,
     *
     * ```php
     * use yii\db\Connection;
     *
     * // returns Yii::$app->db
     * $db = Instance::ensure('db', Connection::className());
     * // or
     * $instance = Instance::of('db');
     * $db = Instance::ensure($instance, Connection::className());
     * ```
     *
     * @param object|string|static $reference an object or a reference to the desired object.
     * You may specify a reference in terms of a component ID or an Instance object.
     * @param string $type the class/interface name to be checked. If null, type check will not be performed.
     * @param ServiceLocator|Container $container the container. This will be passed to [[get()]].
     * @return object the object referenced by the Instance, or `$reference` itself if it is an object.
     * @throws InvalidConfigException if the reference is invalid
     */
    public static function ensure(reference, type = null, container = null)
    {
        var temp_reference;
        if typeof reference == "object" {
            if reference instanceof type {
                return reference;
            }
        }
        else {
            if empty reference {
                throw new InvalidConfigException("The required component is not specified.");
            }
        }

        if typeof reference == "string" {
            let temp_reference = new self(reference);
        }

        if typeof temp_reference == "object" {
            let reference = temp_reference;
        }

        if reference instanceof Instance {
            var component;
            let component = reference->get(container);

            if typeof type == "null" {
                return component;
            }
            if component instanceof type {
                return component;
            } else {
                var class_name, message, id;
                let class_name = get_class(component);
                let id = reference->id;
                let message = "\"" . id . "\" refers to a ";
                let message .= class_name . " component. ". type ." is expected.";
                throw new InvalidConfigException(message);
            }
        }
        var valueType;
        
        let valueType = typeof reference == "object" ? get_class(reference) : gettype(reference);
        throw new InvalidConfigException("Invalid data type: " . valueType . type . " is expected.");
    }

    /**
     * Returns the actual object referenced by this Instance object.
     * @param ServiceLocator|Container $container the container used to locate the referenced object.
     * If null, the method will first try `Yii::$app` then `Yii::$container`.
     * @return object the actual object referenced by this Instance object.
     */
    public function get(container = null)
    {
        if typeof container == "object" {
            return container->get(this->id);
        }
        if typeof BaseYii::$app == "object" && BaseYii::$app->has(this->id) {
            return BaseYii::$app->get(this->id);
        } else {
            return BaseYii::$container->get(this->id);
        }
    }
}
