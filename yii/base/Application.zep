/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */

namespace yii\base;

use yii\BaseYii;

/**
 * Application is the base class for all application classes.
 *
 * @property \yii\web\AssetManager $assetManager The asset manager component. This property is read-only.
 * @property \yii\rbac\ManagerInterface $authManager The auth manager for this application. Null is returned
 * if auth manager is not configured. This property is read-only.
 * @property string $basePath The root directory of the application.
 * @property \yii\caching\Cache $cache The cache application component. Null if the component is not enabled.
 * This property is read-only.
 * @property \yii\db\Connection $db The database connection. This property is read-only.
 * @property \yii\web\ErrorHandler|\yii\console\ErrorHandler $errorHandler The error handler application
 * component. This property is read-only.
 * @property \yii\base\Formatter $formatter The formatter application component. This property is read-only.
 * @property \yii\i18n\I18N $i18n The internationalization component. This property is read-only.
 * @property \yii\log\Dispatcher $log The log dispatcher component. This property is read-only.
 * @property \yii\mail\MailerInterface $mail The mailer interface. This property is read-only.
 * @property \yii\web\Request|\yii\console\Request $request The request component. This property is read-only.
 * @property \yii\web\Response|\yii\console\Response $response The response component. This property is
 * read-only.
 * @property string $runtimePath The directory that stores runtime files. Defaults to the "runtime"
 * subdirectory under [[basePath]].
 * @property string $timeZone The time zone used by this application.
 * @property string $uniqueId The unique ID of the module. This property is read-only.
 * @property \yii\web\UrlManager $urlManager The URL manager for this application. This property is read-only.
 * @property string $vendorPath The directory that stores vendor files. Defaults to "vendor" directory under
 * [[basePath]].
 * @property View|\yii\web\View $view The view object that is used to render various view files. This property
 * is read-only.
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
abstract class Application extends Module
{
    /**
     * @event Event an event raised before the application starts to handle a request.
     */
    const EVENT_BEFORE_REQUEST = "beforeRequest";
    /**
     * @event Event an event raised after the application successfully handles a request (before the response is sent out).
     */
    const EVENT_AFTER_REQUEST = "afterRequest";
    /**
     * Application state used by [[state]]: application just started.
     */
    const STATE_BEGIN = 0;
    /**
     * Application state used by [[state]]: application is initializing.
     */
    const STATE_INIT = 1;
    /**
     * Application state used by [[state]]: application is triggering [[EVENT_BEFORE_REQUEST]].
     */
    const STATE_BEFORE_REQUEST = 2;
    /**
     * Application state used by [[state]]: application is handling the request.
     */
    const STATE_HANDLING_REQUEST = 3;
    /**
     * Application state used by [[state]]: application is triggering [[EVENT_AFTER_REQUEST]]..
     */
    const STATE_AFTER_REQUEST = 4;
    /**
     * Application state used by [[state]]: application is about to send response.
     */
    const STATE_SENDING_RESPONSE = 5;
    /**
     * Application state used by [[state]]: application has ended.
     */
    const STATE_END = 6;

    /**
     * @var string the namespace that controller classes are in. If not set,
     * it will use the "app\controllers" namespace.
     */
    public controllerNamespace = "app\\controllers";
    /**
     * @var string the application name.
     */
    public name = "My Application";
    /**
     * @var string the version of this application.
     */
    public version = "1.0";
    /**
     * @var string the charset currently used for the application.
     */
    public charset = "UTF-8";
    /**
     * @var string the language that is meant to be used for end users.
     * @see sourceLanguage
     */
    public language = "en";
    /**
     * @var string the language that the application is written in. This mainly refers to
     * the language that the messages and view files are written in.
     * @see language
     */
    public sourceLanguage = "en";
    /**
     * @var Controller the currently active controller instance
     */
    public controller;
    /**
     * @var string|boolean the layout that should be applied for views in this application. Defaults to 'main'.
     * If this is false, layout will be disabled.
     */
    public layout = "main";
    /**
     * @var string the requested route
     */
    public requestedRoute;
    /**
     * @var Action the requested Action. If null, it means the request cannot be resolved into an action.
     */
    public requestedAction;
    /**
     * @var array the parameters supplied to the requested action.
     */
    public requestedParams;
    /**
     * @var array list of installed Yii extensions. Each array element represents a single extension
     * with the following structure:
     *
     * ~~~
     * [
     *     'name' => 'extension name',
     *     'version' => 'version number',
     *     'bootstrap' => 'BootstrapClassName',  // optional, may also be a configuration array
     *     'alias' => [
     *         '@alias1' => 'to/path1',
     *         '@alias2' => 'to/path2',
     *     ],
     * ]
     * ~~~
     *
     * The "bootstrap" class listed above will be instantiated during the application
     * [[bootstrap()|bootstrapping process]]. If the class implements [[BootstrapInterface]],
     * its [[BootstrapInterface::bootstrap()|bootstrap()]] method will be also be called.
     */
    public extensions;
    /**
     * @var array list of components that should be run during the application [[bootstrap()|bootstrapping process]].
     *
     * Each component may be specified in one of the following formats:
     *
     * - an application component ID as specified via [[components]].
     * - a module ID as specified via [[modules]].
     * - a class name.
     * - a configuration array.
     *
     * During the bootstrapping process, each component will be instantiated. If the component class
     * implements [[BootstrapInterface]], its [[BootstrapInterface::bootstrap()|bootstrap()]] method
     * will be also be called.
     */
    public bootstrap;
    /**
     * @var integer the current application state during a request handling life cycle.
     * This property is managed by the application. Do not modify this property.
     */
    public state;

    protected _runtimePath;

    protected _vendorPath;

    /**
     * Constructor.
     * @param array $config name-value pairs that will be used to initialize the object properties.
     * Note that the configuration must contain both [[id]] and [[basePath]].
     * @throws InvalidConfigException if either [[id]] or [[basePath]] configuration is missing.
     */
    public function __construct(config = [])
    {
        let BaseYii::$app = this,
            this->state = self::STATE_BEGIN;
        this->registerErrorHandler(config);
        this->preInit(config);

        BaseYii::configure(this, config);
        this->init();
    }

    /**
     * Pre-initializes the application.
     * This method is called at the beginning of the application constructor.
     * It initializes several important application properties.
     * If you override this method, please make sure you call the parent implementation.
     * @param array $config the application configuration
     * @throws InvalidConfigException if either [[id]] or [[basePath]] configuration is missing.
     */
     //    public function preInit(&config)
    public function preInit(config)
    {
        if !isset config["id"] {
            throw new InvalidConfigException("The " . "\"" . "id" ."\"" ." configuration for the Application is required.");
        }
        if isset config["basePath"] {
            this->setBasePath(config["basePath"]);
            unset config["basePath"];
        } else {
            throw new InvalidConfigException("The " . "\"" . "basePath" ."\"" ." configuration for the Application is required.");
        }

        if isset config["vendorPath"] {
            this->setVendorPath(config["vendorPath"]);
            unset config["vendorPath"];
        } else {
            // set "@vendor"
            this->getVendorPath();
        }
        if isset config["runtimePath"] {
            this->setRuntimePath(config["runtimePath"]);
            unset config["runtimePath"];
        } else {
            // set "@runtime"
            this->getRuntimePath();
        }

        if isset config["timeZone"] {
            this->setTimeZone(config["timeZone"]);
            unset config["timeZone"];
        } else {
            if !ini_get("date.timezone") {
                this->setTimeZone("UTC");
            }
        }

        var coreComponents, id, component;
        let coreComponents = this->coreComponents();

        // merge core components with custom components
        for id, component in coreComponents {
            if !isset config["components"][id]  {
                let config["components"][id] = component;
            } else {
                if typeof config["components"][id] == "array" && !isset config["components"][id]["class"] {
                    let config["components"][id]["class"] = component["class"];
                }
            }
        }

    }

    /**
     * @inheritdoc
     */
    public function init()
    {
        let this->state = self::STATE_INIT;
        this->bootstrap();
    }

    /**
     * Initializes extensions and executes bootstrap components.
     * This method is called by [[init()]] after the application has been fully configured.
     * If you override this method, make sure you also call the parent implementation.
     */
    protected function bootstrap()
    {
        var extensions, extension, alias, name, path, component, bootstrap, $class, bootstrap_component, pos;
        let extensions = this->extensions;
        if typeof extensions != "array" {
            let extensions = [];
        }
        for extension in extensions {
            if isset extension["alias"] && !empty extension["alias"] {
                let alias = extension["alias"];
                if typeof alias == "array" {
                    for name, path in alias {
                        BaseYii::setAlias(name, path);
                    }
                }
            }
            if isset extension["bootstrap"] {
                let component = BaseYii::createObject(extension["bootstrap"]);
                if typeof component == "object" {
                    if component instanceof BootstrapInterface {
                        BaseYii::trace("Bootstrap with " . get_class(component) . "::bootstrap()", __METHOD__);
                        component->bootstrap(this);
                    }
                } else {
                    BaseYii::trace("Bootstrap with " . get_class(component), __METHOD__);
                }
            }
        }

        let bootstrap = this->bootstrap;
        if typeof bootstrap != "array" {
            let bootstrap = [];
        }
        for $class in bootstrap{
            let bootstrap_component = null;
            if typeof $class == "string" {
                if this->has($class) {
                    let bootstrap_component = this->get($class);
                } else {
                    if this->hasModule($class) {
                        let bootstrap_component = this->getModule($class);
                    }
                    else {
                        let pos = strpos($class, "\\");
                        if typeof pos == "boolean" {
                            throw new InvalidConfigException("Unknown bootstrap component ID: " . $class);
                        }
                    }
                }
            }

            if typeof bootstrap_component == "null" {
                let bootstrap_component = BaseYii::createObject($class);
            }

            if typeof bootstrap_component == "object" {
                if bootstrap_component instanceof BootstrapInterface {
                    BaseYii::trace("Bootstrap with " . get_class(bootstrap_component) . "::bootstrap()", __METHOD__);
                    bootstrap_component->bootstrap(this);
                } else {
                    BaseYii::trace("Bootstrap with " . get_class(bootstrap_component), __METHOD__);
                }
            }
        }
    }

    /**
     * Registers the errorHandler component as a PHP error handler.
     */
     //    protected function registerErrorHandler(&$config)
    protected function registerErrorHandler($config)
    {
        if YII_ENABLE_ERROR_HANDLER {
            if !isset config["components"]["errorHandler"]["class"] {
                echo "Error: no errorHandler component is configured.\n";
                exit(1);
            }
            this->set("errorHandler", config["components"]["errorHandler"]);
            var components;
            let components = config["components"];
            unset components["errorHandler"];
            let config["components"] = components;
            this->getErrorHandler()->register();
        }
    }

    /**
     * Returns an ID that uniquely identifies this module among all modules within the current application.
     * Since this is an application instance, it will always return an empty string.
     * @return string the unique ID of the module.
     */
    public function getUniqueId()
    {
        return "";
    }

    /**
     * Sets the root directory of the application and the @app alias.
     * This method can only be invoked at the beginning of the constructor.
     * @param string $path the root directory of the application.
     * @property string the root directory of the application.
     * @throws InvalidParamException if the directory does not exist.
     */
    public function setBasePath(path)
    {
        parent::setBasePath(path);
        BaseYii::setAlias("@app", this->getBasePath());
    }

    /**
     * Runs the application.
     * This is the main entrance of an application.
     * @return integer the exit status (0 means normal, non-zero values mean abnormal)
     */
    public function run()
    {
        var response;
        //try {

            let this->state = self::STATE_BEFORE_REQUEST;
            this->trigger(self::EVENT_BEFORE_REQUEST);

            let this->state = self::STATE_HANDLING_REQUEST,
                response = this->handleRequest(this->getRequest()),
                this->state = self::STATE_AFTER_REQUEST;

            this->trigger(self::EVENT_AFTER_REQUEST);

            let this->state = self::STATE_SENDING_RESPONSE;
            response->send();

            let this->state = self::STATE_END;

            return response->exitStatus;

        //} catch (ExitException e) {

        //    this->end(e->statusCode, isset response ? response : null);
        //    return e->statusCode;

        //}
    }

    /**
     * Handles the specified request.
     *
     * This method should return an instance of [[Response]] or its child class
     * which represents the handling result of the request.
     *
     * @param Request $request the request to be handled
     * @return Response the resulting response
     */
    abstract public function handleRequest($request);

    /**
     * Returns the directory that stores runtime files.
     * @return string the directory that stores runtime files.
     * Defaults to the "runtime" subdirectory under [[basePath]].
     */
    public function getRuntimePath()
    {
        if typeof this->_runtimePath == "null" {
            this->setRuntimePath(this->getBasePath() . DIRECTORY_SEPARATOR . "runtime");
        }

        return this->_runtimePath;
    }

    /**
     * Sets the directory that stores runtime files.
     * @param string $path the directory that stores runtime files.
     */
    public function setRuntimePath(path)
    {
        let this->_runtimePath = BaseYii::getAlias(path);
        BaseYii::setAlias("@runtime", this->_runtimePath);
    }

    /**
     * Returns the directory that stores vendor files.
     * @return string the directory that stores vendor files.
     * Defaults to "vendor" directory under [[basePath]].
     */
    public function getVendorPath()
    {
        if ($this->_vendorPath === null) {
            $this->setVendorPath($this->getBasePath() . DIRECTORY_SEPARATOR . "vendor");
        }

        return $this->_vendorPath;
    }

    /**
     * Sets the directory that stores vendor files.
     * @param string $path the directory that stores vendor files.
     */
    public function setVendorPath($path)
    {
        let this->_vendorPath = BaseYii::getAlias(path);
        BaseYii::setAlias("@vendor", this->_vendorPath);
    }

    /**
     * Returns the time zone used by this application.
     * This is a simple wrapper of PHP function date_default_timezone_get().
     * If time zone is not configured in php.ini or application config,
     * it will be set to UTC by default.
     * @return string the time zone used by this application.
     * @see http://php.net/manual/en/function.date-default-timezone-get.php
     */
    public function getTimeZone()
    {
        return date_default_timezone_get();
    }

    /**
     * Sets the time zone used by this application.
     * This is a simple wrapper of PHP function date_default_timezone_set().
     * Refer to the [php manual](http://www.php.net/manual/en/timezones.php) for available timezones.
     * @param string $value the time zone used by this application.
     * @see http://php.net/manual/en/function.date-default-timezone-set.php
     */
    public function setTimeZone(value)
    {
        date_default_timezone_set(value);
    }

    /**
     * Returns the database connection component.
     * @return \yii\db\Connection the database connection
     */
    public function getDb()
    {
        return this->get("db");
    }

    /**
     * Returns the log dispatcher component.
     * @return \yii\log\Dispatcher the log dispatcher component
     */
    public function getLog()
    {
        return this->get("log");
    }

    /**
     * Returns the error handler component.
     * @return \yii\web\ErrorHandler|\yii\console\ErrorHandler the error handler application component.
     */
    public function getErrorHandler()
    {
        return this->get("errorHandler");
    }

    /**
     * Returns the cache component.
     * @return \yii\caching\Cache the cache application component. Null if the component is not enabled.
     */
    public function getCache()
    {
        return this->get("cache", false);
    }

    /**
     * Returns the formatter component.
     * @return \yii\base\Formatter the formatter application component.
     */
    public function getFormatter()
    {
        return this->get("formatter");
    }

    /**
     * Returns the request component.
     * @return \yii\web\Request|\yii\console\Request the request component
     */
    public function getRequest()
    {
        return this->get("request");
    }

    /**
     * Returns the response component.
     * @return \yii\web\Response|\yii\console\Response the response component
     */
    public function getResponse()
    {
        return this->get("response");
    }

    /**
     * Returns the view object.
     * @return View|\yii\web\View the view object that is used to render various view files.
     */
    public function getView()
    {
        return this->get("view");
    }

    /**
     * Returns the URL manager for this application.
     * @return \yii\web\UrlManager the URL manager for this application.
     */
    public function getUrlManager()
    {
        return this->get("urlManager");
    }

    /**
     * Returns the internationalization (i18n) component
     * @return \yii\i18n\I18N the internationalization component
     */
    public function getI18n()
    {
        return this->get("i18n");
    }

    /**
     * Returns the mailer component.
     * @return \yii\mail\MailerInterface the mailer interface
     */
    public function getMail()
    {
        return this->get("mail");
    }

    /**
     * Returns the auth manager for this application.
     * @return \yii\rbac\ManagerInterface the auth manager for this application.
     * Null is returned if auth manager is not configured.
     */
    public function getAuthManager()
    {
        return this->get("authManager", false);
    }

    /**
     * Returns the asset manager.
     * @return \yii\web\AssetManager the asset manager component
     */
    public function getAssetManager()
    {
        return this->get("assetManager");
    }

    /**
     * Returns the core application components.
     * @see set
     */
    public function coreComponents()
    {
        var elements = [];

        let elements["log"]          = ["class" : "yii\\log\\Dispatcher"],
            elements["view"]         = ["class" : "yii\\web\\View"],
            elements["formatter"]    = ["class" : "yii\\base\\Formatter"],
            elements["i18n"]         = ["class" : "yii\\i18n\\I18N"],
            elements["mail"]         = ["class" : "yii\\swiftmailer\\Mailer"],
            elements["urlManager"]   = ["class" : "yii\\web\\UrlManager"],
            elements["assetManager"] = ["class" : "yii\\web\\AssetManager"];

        return elements;
    }

    /**
     * Terminates the application.
     * This method replaces the `exit()` function by ensuring the application life cycle is completed
     * before terminating the application.
     * @param integer $status the exit status (value 0 means normal exit while other values mean abnormal exit).
     * @param Response $response the response to be sent. If not set, the default application [[response]] component will be used.
     * @throws ExitException if the application is in testing mode
     */
    public function end(status = 0, response = null)
    {
        if this->state == self::STATE_BEFORE_REQUEST || this->state == self::STATE_HANDLING_REQUEST {
            let this->state = self::STATE_AFTER_REQUEST;
            this->trigger(self::EVENT_AFTER_REQUEST);
        }

        if this->state != self::STATE_SENDING_RESPONSE && this->state != self::STATE_END {
            let this->state = self::STATE_END;
            if typeof response == "null" {
                let response = this->getResponse();
            }
            response->send();
        }

        if YII_ENV_TEST {
            throw new ExitException(status);
        } else {
            exit(status);
        }
    }
}
