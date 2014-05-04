/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */

namespace yii\base;

use yii\BaseYii;
use yii\di\ServiceLocator;

/**
 * Module is the base class for module and application classes.
 *
 * A module represents a sub-application which contains MVC elements by itself, such as
 * models, views, controllers, etc.
 *
 * A module may consist of [[modules|sub-modules]].
 *
 * [[components|Components]] may be registered with the module so that they are globally
 * accessible within the module.
 *
 * @property array $aliases List of path aliases to be defined. The array keys are alias names (must start
 * with '@') and the array values are the corresponding paths or aliases. See [[setAliases()]] for an example.
 * This property is write-only.
 * @property string $basePath The root directory of the module.
 * @property string $controllerPath The directory that contains the controller classes. This property is
 * read-only.
 * @property string $layoutPath The root directory of layout files. Defaults to "[[viewPath]]/layouts".
 * @property array $modules The modules (indexed by their IDs).
 * @property string $uniqueId The unique ID of the module. This property is read-only.
 * @property string $viewPath The root directory of view files. Defaults to "[[basePath]]/view".
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
class Module extends ServiceLocator
{
    /**
     * @event ActionEvent an event raised before executing a controller action.
     * You may set [[ActionEvent::isValid]] to be false to cancel the action execution.
     */
    const EVENT_BEFORE_ACTION = "beforeAction";
    /**
     * @event ActionEvent an event raised after executing a controller action.
     */
    const EVENT_AFTER_ACTION = "afterAction";

    /**
     * @var array custom module parameters (name => value).
     */
    public params;
    /**
     * @var string an ID that uniquely identifies this module among other modules which have the same [[module|parent]].
     */
    public id;
    /**
     * @var Module the parent module of this module. Null if this module does not have a parent.
     */
    public module;
    /**
     * @var string|boolean the layout that should be applied for views within this module. This refers to a view name
     * relative to [[layoutPath]]. If this is not set, it means the layout value of the [[module|parent module]]
     * will be taken. If this is false, layout will be disabled within this module.
     */
    public layout;
    /**
     * @var array mapping from controller ID to controller configurations.
     * Each name-value pair specifies the configuration of a single controller.
     * A controller configuration can be either a string or an array.
     * If the former, the string should be the fully qualified class name of the controller.
     * If the latter, the array must contain a 'class' element which specifies
     * the controller's fully qualified class name, and the rest of the name-value pairs
     * in the array are used to initialize the corresponding controller properties. For example,
     *
     * ~~~
     * [
     *   'account' => 'app\controllers\UserController',
     *   'article' => [
     *      'class' => 'app\controllers\PostController',
     *      'pageTitle' => 'something new',
     *   ],
     * ]
     * ~~~
     */
    public controllerMap;
    /**
     * @var string the namespace that controller classes are in. If not set,
     * it will use the "controllers" sub-namespace under the namespace of this module.
     * For example, if the namespace of this module is "foo\bar", then the default
     * controller namespace would be "foo\bar\controllers".
     */
    public controllerNamespace;
    /**
     * @var string the default route of this module. Defaults to 'default'.
     * The route may consist of child module ID, controller ID, and/or action ID.
     * For example, `help`, `post/create`, `admin/post/create`.
     * If action ID is not given, it will take the default value as specified in
     * [[Controller::defaultAction]].
     */
    public defaultRoute = "default";
    /**
     * @var string the root directory of the module.
     */
    protected _basePath;
    /**
     * @var string the root directory that contains view files for this module
     */
    protected _viewPath;
    /**
     * @var string the root directory that contains layout view files for this module.
     */
    protected _layoutPath;
    /**
     * @var array child modules of this module
     */
    protected _modules;


    /**
     * Constructor.
     * @param string $id the ID of this module
     * @param Module $parent the parent module (if any)
     * @param array $config name-value pairs that will be used to initialize the object properties
     */
    public function __construct(string id, parent = null, config = [])
    {
        let this->id = id,
            this->module = parent;
        parent::__construct(config);
    }

    /**
     * Initializes the module.
     *
     * This method is called after the module is created and initialized with property values
     * given in configuration. The default implementation will initialize [[controllerNamespace]]
     * if it is not set.
     *
     * If you override this method, please make sure you call the parent implementation.
     */
    public function init()
    {
        var $class, pos;
        if typeof this->controllerNamespace == "null" {
            let $class = get_class(this),
                pos = strrpos($class, "\\");

            if typeof pos != "boolean" {
                let this->controllerNamespace = substr($class, 0, pos) . "\\controllers";
            }
        }
    }

    /**
     * Returns an ID that uniquely identifies this module among all modules within the current application.
     * Note that if the module is an application, an empty string will be returned.
     * @return string the unique ID of the module.
     */
    public function getUniqueId()
    {
        var id, module, this_id, temp_id, unique_id, slash;
        let module = this->module,
            this_id = this->id,
            slash = "/";

        if typeof module != "null" {
            let unique_id = module->getUniqueId(),
                temp_id = unique_id . slash . this_id,
                id = ltrim(temp_id, slash);
            return id;
        }
        else {
            return this_id;
        }
    }

    /**
     * Returns the root directory of the module.
     * It defaults to the directory containing the module class file.
     * @return string the root directory of the module.
     */
    public function getBasePath()
    {
        var $class, name;
        if typeof this->_basePath == "null" {
            let $class = new \ReflectionClass(this),
                name = $class->getFileName(),
                name = dirname(name);

            let this->_basePath = name;
        }

        return $this->_basePath;
    }

    /**
     * Sets the root directory of the module.
     * This method can only be invoked at the beginning of the constructor.
     * @param string $path the root directory of the module. This can be either a directory name or a path alias.
     * @throws InvalidParamException if the directory does not exist.
     */
    public function setBasePath(string path)
    {
        var p;
        let path = BaseYii::getAlias(path),
            p = realpath(path);

        if typeof p != "boolean" && is_dir(p) {
            let this->_basePath = p;
        } else {
            throw new InvalidParamException("The directory does not exist: " . $path);
        }
    }

    /**
     * Returns the directory that contains the controller classes according to [[controllerNamespace]].
     * Note that in order for this method to return a value, you must define
     * an alias for the root namespace of [[controllerNamespace]].
     * @return string the directory that contains the controller classes.
     * @throws InvalidParamException if there is no alias defined for the root namespace of [[controllerNamespace]].
     */
    public function getControllerPath()
    {
        var path;
        let path = $this->controllerNamespace,
            path = str_replace("\\", "/", path),
            path = "@" . path;

        return BaseYii::getAlias(path);
    }

    /**
     * Returns the directory that contains the view files for this module.
     * @return string the root directory of view files. Defaults to "[[basePath]]/view".
     */
    public function getViewPath()
    {
        var view_path;
        if typeof this->_viewPath != "null" {
            return this->_viewPath;
        } else {
            let view_path = this->getBasePath(),
                view_path .= DIRECTORY_SEPARATOR . "views",
                this->_viewPath = view_path;
            return view_path;
        }
    }

    /**
     * Sets the directory that contains the view files.
     * @param string $path the root directory of view files.
     * @throws InvalidParamException if the directory is invalid
     */
    public function setViewPath(string path)
    {
        let this->_viewPath = BaseYii::getAlias(path);
    }

    /**
     * Returns the directory that contains layout view files for this module.
     * @return string the root directory of layout files. Defaults to "[[viewPath]]/layouts".
     */
    public function getLayoutPath()
    {
        var layout_path;
        if typeof this->_layoutPath != "null" {
            return this->_layoutPath;
        } else {
            let layout_path = this->getViewPath(),
                layout_path .= DIRECTORY_SEPARATOR . "layouts",
                this->_layoutPath = layout_path;
            return layout_path;
       }
    }

    /**
     * Sets the directory that contains the layout files.
     * @param string $path the root directory of layout files.
     * @throws InvalidParamException if the directory is invalid
     */
    public function setLayoutPath(string path)
    {
        let this->_layoutPath = BaseYii::getAlias(path);
    }

    /**
     * Defines path aliases.
     * This method calls [[Yii::setAlias()]] to register the path aliases.
     * This method is provided so that you can define path aliases when configuring a module.
     * @property array list of path aliases to be defined. The array keys are alias names
     * (must start with '@') and the array values are the corresponding paths or aliases.
     * See [[setAliases()]] for an example.
     * @param array $aliases list of path aliases to be defined. The array keys are alias names
     * (must start with '@') and the array values are the corresponding paths or aliases.
     * For example,
     *
     * ~~~
     * [
     *     '@models' => '@app/models', // an existing alias
     *     '@backend' => __DIR__ . '/../backend',  // a directory
     * ]
     * ~~~
     */
    public function setAliases(aliases)
    {
        var temp_aliases, alias, name;
        let temp_aliases = aliases;

        for name, alias in temp_aliases {
            BaseYii::setAlias(name, alias);
        }
    }

    /**
     * Checks whether the child module of the specified ID exists.
     * This method supports checking the existence of both child and grand child modules.
     * @param string $id module ID. For grand child modules, use ID path relative to this module (e.g. `admin/content`).
     * @return boolean whether the named module exists. Both loaded and unloaded modules
     * are considered.
     */
    public function hasModule(string id)
    {
        var pos, sub_id, sub_id2, module, retval;
        let pos = strpos(id, "/");
        if typeof pos != "boolean" {
            // sub-module
            let sub_id = substr(id, 0, pos);
            let module = this->getModule(sub_id);
            if typeof module == "null" {
                return false;
            }
            else {
                let sub_id2 = substr(id, pos + 1);
                let retval = module->hasModule(sub_id2);
                return retval;
            }
        } else {
            var temp_module;
            let temp_module = this->_modules;

            if isset temp_module[id] {
                return true;
            }
            else {
                return false;
            }
        }
    }

    /**
     * Retrieves the child module of the specified ID.
     * This method supports retrieving both child modules and grand child modules.
     * @param string $id module ID (case-sensitive). To retrieve grand child modules,
     * use ID path relative to this module (e.g. `admin/content`).
     * @param boolean $load whether to load the module if it is not yet loaded.
     * @return Module|null the module instance, null if the module does not exist.
     * @see hasModule()
     */
    public function getModule(string id, load = true)
    {
        var pos, sub_id, sub_id2, module, retval, modules, mo;
        let pos = strpos(id, "/");
        if typeof pos != "boolean" {
            // sub-module
            let sub_id = substr(id, 0, pos);
            let module = $this->getModule(id);

            if typeof module == "null" {
                return null;
            }
            else {
                let sub_id2 = substr(id, pos + 1);
                let retval = module->getModule(sub_id2, load);
                return retval;
            }
        }
        let modules = this->_modules;

        if fetch mo, modules[id] {
            if typeof mo == "object" {
                if mo instanceof Module {
                    return mo;
                }
            }
            else {
                if load == true {
                    BaseYii::trace("Loading module: ". id, __METHOD__);
                    if typeof mo == "array" && !isset mo["class"] {
                        let mo["class"] = "yii\\base\\Module";
                    }
                    var elements = [];
                    let elements[] = id,
                        elements[] = this;

                    let retval = BaseYii::createObject(mo, elements);
                    let modules[id] = retval;
                    let this->_modules = modules;
                    return retval;
                }
            }
        }

        return null;
    }

    /**
     * Adds a sub-module to this module.
     * @param string $id module ID
     * @param Module|array|null $module the sub-module to be added to this module. This can
     * be one of the followings:
     *
     * - a [[Module]] object
     * - a configuration array: when [[getModule()]] is called initially, the array
     *   will be used to instantiate the sub-module
     * - null: the named sub-module will be removed from this module
     */
    public function setModule(string id, module)
    {
        var modules, temp_module;
        let modules = this->_modules;

        if typeof module == "null" {
            if isset modules[id] {
                unset modules[id];
            }
        } else {
            let temp_module = module,
                modules[id] = temp_module;
        }

        let this->_modules = modules;
    }

    /**
     * Returns the sub-modules in this module.
     * @param boolean $loadedOnly whether to return the loaded sub-modules only. If this is set false,
     * then all sub-modules registered in this module will be returned, whether they are loaded or not.
     * Loaded modules will be returned as objects, while unloaded modules as configuration arrays.
     * @return array the modules (indexed by their IDs)
     */
    public function getModules(boolean loadedOnly = false)
    {
        var new_modules, modules, module;
        let modules = this->_modules;

        if loadedOnly {
            let new_modules = [];
            for module in modules {
                if module instanceof Module {
                    let new_modules[] = module;
                }
            }

            return new_modules;
        } else {
            return modules;
        }
    }

    /**
     * Registers sub-modules in the current module.
     *
     * Each sub-module should be specified as a name-value pair, where
     * name refers to the ID of the module and value the module or a configuration
     * array that can be used to create the module. In the latter case, [[Yii::createObject()]]
     * will be used to create the module.
     *
     * If a new sub-module has the same ID as an existing one, the existing one will be overwritten silently.
     *
     * The following is an example for registering two sub-modules:
     *
     * ~~~
     * [
     *     'comment' => [
     *         'class' => 'app\modules\comment\CommentModule',
     *         'db' => 'db',
     *     ],
     *     'booking' => ['class' => 'app\modules\booking\BookingModule'],
     * ]
     * ~~~
     *
     * @param array $modules modules (id => module configuration or instances)
     */
    public function setModules(modules)
    {
        var id, temp_modules, module, this_modules;
        let this_modules = this->_modules;
        if typeof this_modules != "array" {
            let this_modules = [];
        }
        let temp_modules = modules;

        for id, module in temp_modules {
            let this_modules[id] = module;
        }

        let this->_modules = this_modules;
    }

    /**
     * Runs a controller action specified by a route.
     * This method parses the specified route and creates the corresponding child module(s), controller and action
     * instances. It then calls [[Controller::runAction()]] to run the action with the given parameters.
     * If the route is empty, the method will use [[defaultRoute]].
     * @param string $route the route that specifies the action.
     * @param array $params the parameters to be passed to the action
     * @return mixed the result of the action.
     * @throws InvalidRouteException if the requested route cannot be resolved into an action successfully
     */
    public function runAction(string route, params = [])
    {
        var parts, id, controller, actionID, oldController, result;
        let parts = this->createController(route);

        if typeof parts == "array" {
            /** @var Controller $controller */
            let controller = parts[0],
                actionID = parts[1];

            var app;

            let app = BaseYii::$app,
                oldController = app->controller,
                app->controller = controller,
                result = controller->runAction(actionID, params);
            //let app->controller = oldController;
            return result;
        } else {
            let id = this->getUniqueId();
            var message;
            if id == "" {
                let message = route;
            }
            else {
                let message = id . "/" . route;
            }
            throw new InvalidRouteException("Unable to resolve the request \"" . message . "\".");
        }
    }

    /**
     * Creates a controller instance based on the given route.
     *
     * The route should be relative to this module. The method implements the following algorithm
     * to resolve the given route:
     *
     * 1. If the route is empty, use [[defaultRoute]];
     * 2. If the first segment of the route is a valid module ID as declared in [[modules]],
     *    call the module's `createController()` with the rest part of the route;
     * 3. If the first segment of the route is found in [[controllerMap]], create a controller
     *    based on the corresponding configuration found in [[controllerMap]];
     * 4. The given route is in the format of `abc/def/xyz`. Try either `abc\DefController`
     *    or `abc\def\XyzController` class within the [[controllerNamespace|controller namespace]].
     *
     * If any of the above steps resolves into a controller, it is returned together with the rest
     * part of the route which will be treated as the action ID. Otherwise, false will be returned.
     *
     * @param string $route the route consisting of module, controller and action IDs.
     * @return array|boolean If the controller is created successfully, it will be returned together
     * with the requested action ID. Otherwise false will be returned.
     * @throws InvalidConfigException if the controller class and its file do not match.
     */
    public function createController(route)
    {
        if route == "" {
            let route = this->defaultRoute;
        }
        var slash, pos;
        let slash = "/";
        // double slashes or leading/ending slashes may cause substr problem
        let route = trim(route, slash);

        let pos = strpos(route, "//");

        if typeof pos != "boolean" {
            return false;
        }
        var id;

        let pos = strpos(route, slash);
        if typeof pos != "boolean" {
            var explodes;
            let explodes = explode(slash, route, 2);
            let id = explodes[0],
                route = explodes[1];
        } else {
            let id = route;
            let route = "";
        }

        // module and controller map take precedence
        var module;
        let module = $this->getModule(id);
        if typeof module != "null" {
            return module->createController(route);
        }
        var controller_map, controller_id, controller, return_elements = [];
        let controller_map = this->controllerMap; 
        if fetch controller_id,  controller_map[id] {
            var elements = [];
            let elements[] = id,
                elements[] = this;

            let controller = BaseYii::createObject(controller_id, elements);
            let return_elements[] = controller,
                return_elements[] = route;
            return return_elements;
        }

        let pos = strrpos(route, slash);
        if typeof pos != "boolean" {
            let id .= slash . substr(route, 0, pos);
            let route = substr(route, pos + 1);
        }

        let controller = this->createControllerByID(id);

        if typeof controller == "null" && route != "" {
            let controller = this->createControllerByID(id . slash . route);
            let route = "";
        }


        if typeof controller == "null" {
            return false;
        }
        else {
            var temp_controller;
            let temp_controller = controller;

            let return_elements[0] = temp_controller,
                return_elements[1] = route;
            return return_elements;
        }
    }

    /**
     * Creates a controller based on the given controller ID.
     *
     * The controller ID is relative to this module. The controller class
     * should be namespaced under [[controllerNamespace]].
     *
     * Note that this method does not check [[modules]] or [[controllerMap]].
     *
     * @param string $id the controller ID
     * @return Controller the newly created controller instance, or null if the controller ID is invalid.
     * @throws InvalidConfigException if the controller class and its file name do not match.
     * This exception is only thrown when in debug mode.
     */
    public function createControllerByID(string id)
    {
        if !preg_match("%^[a-z0-9\\-_/]+$%", id) {
            return null;
        }

        var pos, prefix, className;

        let pos = strrpos(id, "/");
        if typeof pos == "boolean" {
            let prefix = "";
            let className = id;
        } else {
            let prefix = substr(id, 0, pos + 1);
            let className = substr(id, pos + 1);
        }

        let className = str_replace("-", " ", className),
            className = ucwords(className),
            className = str_replace(" ", "", className),
            className = className . "Controller",
            className = str_replace("/", "\\", prefix) . className,
            className = "\\" . className,
            className = this->controllerNamespace . className,
            className = ltrim(className, "\\");

        let pos = strpos(className, "-");
        if typeof pos != "boolean" || !class_exists(className) {
            return null;
        }

        if is_subclass_of(className, "yii\\base\\Controller") {
            var elements = [];

            let elements[] = id,
                elements[] = this;
            return BaseYii::createObject(className, elements);

        } else {
            if (YII_DEBUG) {
                throw new InvalidConfigException("Controller class must extend from \\yii\\base\\Controller.");
            } else {
                return null;
            }
        }
    }

    /**
     * This method is invoked right before an action within this module is executed.
     *
     * The method will trigger the [[EVENT_BEFORE_ACTION]] event. The return value of the method
     * will determine whether the action should continue to run.
     *
     * If you override this method, your code should look like the following:
     *
     * ```php
     * public function beforeAction($action)
     * {
     *     if (parent::beforeAction($action)) {
     *         // your custom code here
     *         return true;  // or false if needed
     *     } else {
     *         return false;
     *     }
     * }
     * ```
     *
     * @param Action $action the action to be executed.
     * @return boolean whether the action should continue to be executed.
     */
    public function beforeAction(action)
    {
        var temp_action, event;
        let temp_action = action;

        let event = new ActionEvent(temp_action);
        $this->trigger(self::EVENT_BEFORE_ACTION, event);
        return event->isValid;
    }

    /**
     * This method is invoked right after an action within this module is executed.
     *
     * The method will trigger the [[EVENT_AFTER_ACTION]] event. The return value of the method
     * will be used as the action return value.
     *
     * If you override this method, your code should look like the following:
     *
     * ```php
     * public function afterAction($action, $result)
     * {
     *     $result = parent::afterAction($action, $result);
     *     // your custom code here
     *     return $result;
     * }
     * ```
     *
     * @param Action $action the action just executed.
     * @param mixed $result the action return result.
     * @return mixed the processed action result.
     */
    public function afterAction($action, $result)
    {
        var temp_action, temp_result, event;
        let temp_action = action,
            temp_result = result;

        let event = new ActionEvent(temp_action);
        let event->result = temp_result;
        this->trigger(self::EVENT_AFTER_ACTION, event);
        return event->result;
    }
}
