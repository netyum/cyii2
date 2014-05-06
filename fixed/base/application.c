
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
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/exception.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/hash.h"
#include "kernel/string.h"


/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */
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
ZEPHIR_INIT_CLASS(yii_base_Application) {

	ZEPHIR_REGISTER_CLASS_EX(yii\\base, Application, yii, base_application, yii_base_module_ce, yii_base_application_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	/**
	 * @var string the namespace that controller classes are in. If not set,
	 * it will use the "app\controllers" namespace.
	 */
	zend_declare_property_string(yii_base_application_ce, SL("controllerNamespace"), "app\\controllers", ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * @var string the application name.
	 */
	zend_declare_property_string(yii_base_application_ce, SL("name"), "My Application", ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * @var string the version of this application.
	 */
	zend_declare_property_string(yii_base_application_ce, SL("version"), "1.0", ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * @var string the charset currently used for the application.
	 */
	zend_declare_property_string(yii_base_application_ce, SL("charset"), "UTF-8", ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * @var string the language that is meant to be used for end users.
	 * @see sourceLanguage
	 */
	zend_declare_property_string(yii_base_application_ce, SL("language"), "en", ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * @var string the language that the application is written in. This mainly refers to
	 * the language that the messages and view files are written in.
	 * @see language
	 */
	zend_declare_property_string(yii_base_application_ce, SL("sourceLanguage"), "en", ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * @var Controller the currently active controller instance
	 */
	zend_declare_property_null(yii_base_application_ce, SL("controller"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * @var string|boolean the layout that should be applied for views in this application. Defaults to 'main'.
	 * If this is false, layout will be disabled.
	 */
	zend_declare_property_string(yii_base_application_ce, SL("layout"), "main", ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * @var string the requested route
	 */
	zend_declare_property_null(yii_base_application_ce, SL("requestedRoute"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * @var Action the requested Action. If null, it means the request cannot be resolved into an action.
	 */
	zend_declare_property_null(yii_base_application_ce, SL("requestedAction"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * @var array the parameters supplied to the requested action.
	 */
	zend_declare_property_null(yii_base_application_ce, SL("requestedParams"), ZEND_ACC_PUBLIC TSRMLS_CC);

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
	zend_declare_property_null(yii_base_application_ce, SL("extensions"), ZEND_ACC_PUBLIC TSRMLS_CC);

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
	zend_declare_property_null(yii_base_application_ce, SL("bootstrap"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * @var integer the current application state during a request handling life cycle.
	 * This property is managed by the application. Do not modify this property.
	 */
	zend_declare_property_null(yii_base_application_ce, SL("state"), ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_property_null(yii_base_application_ce, SL("_runtimePath"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(yii_base_application_ce, SL("_vendorPath"), ZEND_ACC_PROTECTED TSRMLS_CC);

	/**
	 * @event Event an event raised before the application starts to handle a request.
	 */
	zend_declare_class_constant_string(yii_base_application_ce, SL("EVENT_BEFORE_REQUEST"), "beforeRequest" TSRMLS_CC);

	/**
	 * @event Event an event raised after the application successfully handles a request (before the response is sent out).
	 */
	zend_declare_class_constant_string(yii_base_application_ce, SL("EVENT_AFTER_REQUEST"), "afterRequest" TSRMLS_CC);

	/**
	 * Application state used by [[state]]: application just started.
	 */
	zend_declare_class_constant_long(yii_base_application_ce, SL("STATE_BEGIN"), 0 TSRMLS_CC);

	/**
	 * Application state used by [[state]]: application is initializing.
	 */
	zend_declare_class_constant_long(yii_base_application_ce, SL("STATE_INIT"), 1 TSRMLS_CC);

	/**
	 * Application state used by [[state]]: application is triggering [[EVENT_BEFORE_REQUEST]].
	 */
	zend_declare_class_constant_long(yii_base_application_ce, SL("STATE_BEFORE_REQUEST"), 2 TSRMLS_CC);

	/**
	 * Application state used by [[state]]: application is handling the request.
	 */
	zend_declare_class_constant_long(yii_base_application_ce, SL("STATE_HANDLING_REQUEST"), 3 TSRMLS_CC);

	/**
	 * Application state used by [[state]]: application is triggering [[EVENT_AFTER_REQUEST]]..
	 */
	zend_declare_class_constant_long(yii_base_application_ce, SL("STATE_AFTER_REQUEST"), 4 TSRMLS_CC);

	/**
	 * Application state used by [[state]]: application is about to send response.
	 */
	zend_declare_class_constant_long(yii_base_application_ce, SL("STATE_SENDING_RESPONSE"), 5 TSRMLS_CC);

	/**
	 * Application state used by [[state]]: application has ended.
	 */
	zend_declare_class_constant_long(yii_base_application_ce, SL("STATE_END"), 6 TSRMLS_CC);

	return SUCCESS;

}

/**
 * Constructor.
 * @param array $config name-value pairs that will be used to initialize the object properties.
 * Note that the configuration must contain both [[id]] and [[basePath]].
 * @throws InvalidConfigException if either [[id]] or [[basePath]] configuration is missing.
 */
PHP_METHOD(yii_base_Application, __construct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *config = NULL, *_0, *obj;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &config);

	if (!config) {
		ZEPHIR_INIT_VAR(config);
		array_init(config);
	}


	zephir_update_static_property_ce(yii_baseyii_ce, SL("app"), this_ptr TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 0);
	zephir_update_property_this(this_ptr, SL("state"), _0 TSRMLS_CC);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "registererrorhandler", NULL, config);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "preinit", NULL, config);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(obj);
	object_init_ex(obj, yii_base_component_ce);
	ZEPHIR_CALL_METHOD(NULL, obj, "__construct", NULL, config);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

/**
 * Pre-initializes the application.
 * This method is called at the beginning of the application constructor.
 * It initializes several important application properties.
 * If you override this method, please make sure you call the parent implementation.
 * @param array $config the application configuration
 * @throws InvalidConfigException if either [[id]] or [[basePath]] configuration is missing.
 */
PHP_METHOD(yii_base_Application, preInit) {

	zend_bool _12;
	HashTable *_8;
	HashPosition _7;
	zephir_nts_static zephir_fcall_cache_entry *_5 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *_1 = NULL;
	zval *config, *_0 = NULL, *_2, _3, *_4 = NULL, *_6, *coreComponents = NULL, *id = NULL, *component = NULL, **_9, *_10, *_11 = NULL, *_13, *_14, *_15;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &config);

	Z_SET_ISREF_P(config);

	if (!(zephir_array_isset_string(config, SS("id")))) {
		ZEPHIR_INIT_VAR(_0);
		object_init_ex(_0, yii_base_invalidconfigexception_ce);
		ZEPHIR_INIT_VAR(_1);
		ZEPHIR_CONCAT_SSSSS(_1, "The ", "\"", "id", "\"", " configuration for the Application is required.");
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, _1);
		zephir_check_call_status();
		zephir_throw_exception_debug(_0, "yii/base/Application.zep", 207 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (zephir_array_isset_string(config, SS("basePath"))) {
		zephir_array_fetch_string(&_2, config, SL("basePath"), PH_NOISY | PH_READONLY TSRMLS_CC);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "setbasepath", NULL, _2);
		zephir_check_call_status();
		zephir_array_unset_string(&config, SS("basePath"), PH_SEPARATE);
	} else {
		ZEPHIR_INIT_LNVAR(_0);
		object_init_ex(_0, yii_base_invalidconfigexception_ce);
		ZEPHIR_INIT_LNVAR(_1);
		ZEPHIR_CONCAT_SSSSS(_1, "The ", "\"", "basePath", "\"", " configuration for the Application is required.");
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, _1);
		zephir_check_call_status();
		zephir_throw_exception_debug(_0, "yii/base/Application.zep", 213 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (zephir_array_isset_string(config, SS("vendorPath"))) {
		zephir_array_fetch_string(&_2, config, SL("vendorPath"), PH_NOISY | PH_READONLY TSRMLS_CC);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "setvendorpath", NULL, _2);
		zephir_check_call_status();
		zephir_array_unset_string(&config, SS("vendorPath"), PH_SEPARATE);
	} else {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "getvendorpath", NULL);
		zephir_check_call_status();
	}
	if (zephir_array_isset_string(config, SS("runtimePath"))) {
		zephir_array_fetch_string(&_2, config, SL("runtimePath"), PH_NOISY | PH_READONLY TSRMLS_CC);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "setruntimepath", NULL, _2);
		zephir_check_call_status();
		zephir_array_unset_string(&config, SS("runtimePath"), PH_SEPARATE);
	} else {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "getruntimepath", NULL);
		zephir_check_call_status();
	}
	if (zephir_array_isset_string(config, SS("timeZone"))) {
		zephir_array_fetch_string(&_2, config, SL("timeZone"), PH_NOISY | PH_READONLY TSRMLS_CC);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "settimezone", NULL, _2);
		zephir_check_call_status();
		zephir_array_unset_string(&config, SS("timeZone"), PH_SEPARATE);
	} else {
		ZEPHIR_SINIT_VAR(_3);
		ZVAL_STRING(&_3, "date.timezone", 0);
		ZEPHIR_CALL_FUNCTION(&_4, "ini_get", &_5, &_3);
		zephir_check_call_status();
		if (!(zephir_is_true(_4))) {
			ZEPHIR_INIT_VAR(_6);
			ZVAL_STRING(_6, "UTC", 0);
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "settimezone", NULL, _6);
			zephir_check_temp_parameter(_6);
			zephir_check_call_status();
		}
	}
	ZEPHIR_CALL_METHOD(&coreComponents, this_ptr, "corecomponents",  NULL);
	zephir_check_call_status();
	zephir_is_iterable(coreComponents, &_8, &_7, 0, 0);
	for (
	  ; zephir_hash_get_current_data_ex(_8, (void**) &_9, &_7) == SUCCESS
	  ; zephir_hash_move_forward_ex(_8, &_7)
	) {
		ZEPHIR_GET_HMKEY(id, _8, _7);
		ZEPHIR_GET_HVALUE(component, _9);
		zephir_array_fetch_string(&_2, config, SL("components"), PH_NOISY | PH_READONLY TSRMLS_CC);
		if (!(zephir_array_isset(_2, id))) {
			zephir_array_update_multi(&config, &component TSRMLS_CC, SL("sz"), 3, SL("components"), id);
		} else {
			zephir_array_fetch_string(&_10, config, SL("components"), PH_NOISY | PH_READONLY TSRMLS_CC);
			ZEPHIR_OBS_NVAR(_11);
			zephir_array_fetch(&_11, _10, id, PH_NOISY TSRMLS_CC);
			_12 = Z_TYPE_P(_11) == IS_ARRAY;
			if (_12) {
				zephir_array_fetch_string(&_13, config, SL("components"), PH_NOISY | PH_READONLY TSRMLS_CC);
				zephir_array_fetch(&_14, _13, id, PH_NOISY | PH_READONLY TSRMLS_CC);
				_12 = !zephir_array_isset_string(_14, SS("class"));
			}
			if (_12) {
				zephir_array_fetch_string(&_15, component, SL("class"), PH_NOISY | PH_READONLY TSRMLS_CC);
				zephir_array_update_multi(&config, &_15 TSRMLS_CC, SL("szs"), 5, SL("components"), id, SL("class"));
			}
		}
	}
	Z_UNSET_ISREF_P(config);
	ZEPHIR_MM_RESTORE();

}

/**
 * @inheritdoc
 */
PHP_METHOD(yii_base_Application, init) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 1);
	zephir_update_property_this(this_ptr, SL("state"), _0 TSRMLS_CC);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "bootstrap", NULL);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

/**
 * Initializes extensions and executes bootstrap components.
 * This method is called by [[init()]] after the application has been fully configured.
 * If you override this method, make sure you also call the parent implementation.
 */
PHP_METHOD(yii_base_Application, bootstrap) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_9 = NULL, *_10 = NULL, *_12 = NULL, *_20 = NULL, *_21 = NULL, *_22 = NULL, *_23 = NULL, *_26 = NULL;
	zend_bool _4;
	HashTable *_2, *_7, *_17;
	HashPosition _1, _6, _16;
	zval *extensions = NULL, *extension = NULL, *alias = NULL, *name = NULL, *path = NULL, *component = NULL, *bootstrap = NULL, *class = NULL, *bootstrap_component = NULL, *pos = NULL, *_0, **_3, *_5, **_8, *_11, *_13 = NULL, *_14 = NULL, *_15 = NULL, **_18, *_19 = NULL, _24 = zval_used_for_init, *_25 = NULL, *_27 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("extensions"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(extensions, _0);
	if (Z_TYPE_P(extensions) != IS_ARRAY) {
		ZEPHIR_INIT_VAR(extensions);
		array_init(extensions);
	}
	zephir_is_iterable(extensions, &_2, &_1, 0, 0);
	for (
	  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
	  ; zephir_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HVALUE(extension, _3);
		_4 = zephir_array_isset_string(extension, SS("alias"));
		if (_4) {
			zephir_array_fetch_string(&_5, extension, SL("alias"), PH_NOISY | PH_READONLY TSRMLS_CC);
			_4 = !ZEPHIR_IS_EMPTY(_5);
		}
		if (_4) {
			ZEPHIR_OBS_NVAR(alias);
			zephir_array_fetch_string(&alias, extension, SL("alias"), PH_NOISY TSRMLS_CC);
			if (Z_TYPE_P(alias) == IS_ARRAY) {
				zephir_is_iterable(alias, &_7, &_6, 0, 0);
				for (
				  ; zephir_hash_get_current_data_ex(_7, (void**) &_8, &_6) == SUCCESS
				  ; zephir_hash_move_forward_ex(_7, &_6)
				) {
					ZEPHIR_GET_HMKEY(name, _7, _6);
					ZEPHIR_GET_HVALUE(path, _8);
					ZEPHIR_CALL_CE_STATIC(NULL, yii_baseyii_ce, "setalias", &_9, name, path);
					zephir_check_call_status();
				}
			}
		}
		if (zephir_array_isset_string(extension, SS("bootstrap"))) {
			zephir_array_fetch_string(&_11, extension, SL("bootstrap"), PH_NOISY | PH_READONLY TSRMLS_CC);
			ZEPHIR_CALL_CE_STATIC(&component, yii_baseyii_ce, "createobject", &_10, _11);
			zephir_check_call_status();
			if (Z_TYPE_P(component) == IS_OBJECT) {
				if (zephir_instance_of_ev(component, yii_base_bootstrapinterface_ce TSRMLS_CC)) {
					ZEPHIR_INIT_NVAR(_13);
					zephir_get_class(_13, component, 0 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_14);
					ZEPHIR_CONCAT_SVS(_14, "Bootstrap with ", _13, "::bootstrap()");
					ZEPHIR_INIT_NVAR(_15);
					ZVAL_STRING(_15, "Application:bootstrap", 0);
					ZEPHIR_CALL_CE_STATIC(NULL, yii_baseyii_ce, "trace", &_12, _14, _15);
					zephir_check_temp_parameter(_15);
					zephir_check_call_status();
					ZEPHIR_CALL_METHOD(NULL, component, "bootstrap", NULL, this_ptr);
					zephir_check_call_status();
				}
			} else {
				ZEPHIR_INIT_NVAR(_13);
				zephir_get_class(_13, component, 0 TSRMLS_CC);
				ZEPHIR_INIT_LNVAR(_14);
				ZEPHIR_CONCAT_SV(_14, "Bootstrap with ", _13);
				ZEPHIR_INIT_NVAR(_15);
				ZVAL_STRING(_15, "Application:bootstrap", 0);
				ZEPHIR_CALL_CE_STATIC(NULL, yii_baseyii_ce, "trace", &_12, _14, _15);
				zephir_check_temp_parameter(_15);
				zephir_check_call_status();
			}
		}
	}
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("bootstrap"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(bootstrap, _0);
	if (Z_TYPE_P(bootstrap) != IS_ARRAY) {
		ZEPHIR_INIT_VAR(bootstrap);
		array_init(bootstrap);
	}
	zephir_is_iterable(bootstrap, &_17, &_16, 0, 0);
	for (
	  ; zephir_hash_get_current_data_ex(_17, (void**) &_18, &_16) == SUCCESS
	  ; zephir_hash_move_forward_ex(_17, &_16)
	) {
		ZEPHIR_GET_HVALUE(class, _18);
		ZEPHIR_INIT_NVAR(bootstrap_component);
		ZVAL_NULL(bootstrap_component);
		if (Z_TYPE_P(class) == IS_STRING) {
			ZEPHIR_CALL_METHOD(&_19, this_ptr, "has", &_20, class);
			zephir_check_call_status();
			if (zephir_is_true(_19)) {
				ZEPHIR_CALL_METHOD(&bootstrap_component, this_ptr, "get", &_21, class);
				zephir_check_call_status();
			} else {
				ZEPHIR_CALL_METHOD(&_19, this_ptr, "hasmodule", &_22, class);
				zephir_check_call_status();
				if (zephir_is_true(_19)) {
					ZEPHIR_CALL_METHOD(&bootstrap_component, this_ptr, "getmodule", &_23, class);
					zephir_check_call_status();
				} else {
					ZEPHIR_SINIT_NVAR(_24);
					ZVAL_STRING(&_24, "\\", 0);
					ZEPHIR_INIT_NVAR(pos);
					zephir_fast_strpos(pos, class, &_24, 0 );
					if (Z_TYPE_P(pos) == IS_BOOL) {
						ZEPHIR_INIT_LNVAR(_14);
						object_init_ex(_14, yii_base_invalidconfigexception_ce);
						ZEPHIR_INIT_LNVAR(_25);
						ZEPHIR_CONCAT_SV(_25, "Unknown bootstrap component ID: ", class);
						ZEPHIR_CALL_METHOD(NULL, _14, "__construct", &_26, _25);
						zephir_check_call_status();
						zephir_throw_exception_debug(_14, "yii/base/Application.zep", 315 TSRMLS_CC);
						ZEPHIR_MM_RESTORE();
						return;
					}
				}
			}
		}
		if (Z_TYPE_P(bootstrap_component) == IS_NULL) {
			ZEPHIR_CALL_CE_STATIC(&bootstrap_component, yii_baseyii_ce, "createobject", &_10, class);
			zephir_check_call_status();
		}
		if (Z_TYPE_P(bootstrap_component) == IS_OBJECT) {
			if (zephir_instance_of_ev(bootstrap_component, yii_base_bootstrapinterface_ce TSRMLS_CC)) {
				ZEPHIR_INIT_NVAR(_13);
				zephir_get_class(_13, component, 0 TSRMLS_CC);
				ZEPHIR_INIT_LNVAR(_14);
				ZEPHIR_CONCAT_SVS(_14, "Bootstrap with ", _13, "::bootstrap()");
				ZEPHIR_INIT_NVAR(_15);
				ZVAL_STRING(_15, "Application:bootstrap", 0);
				ZEPHIR_CALL_CE_STATIC(NULL, yii_baseyii_ce, "trace", &_12, _14, _15);
				zephir_check_temp_parameter(_15);
				zephir_check_call_status();
				ZEPHIR_CALL_METHOD(NULL, bootstrap_component, "bootstrap", NULL, this_ptr);
				zephir_check_call_status();
			} else {
				ZEPHIR_INIT_NVAR(_15);
				zephir_get_class(_15, component, 0 TSRMLS_CC);
				ZEPHIR_INIT_LNVAR(_25);
				ZEPHIR_CONCAT_SV(_25, "Bootstrap with ", _15);
				ZEPHIR_INIT_NVAR(_27);
				ZVAL_STRING(_27, "Application:bootstrap", 0);
				ZEPHIR_CALL_CE_STATIC(NULL, yii_baseyii_ce, "trace", &_12, _25, _27);
				zephir_check_temp_parameter(_27);
				zephir_check_call_status();
			}
		}
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Registers the errorHandler component as a PHP error handler.
 */
PHP_METHOD(yii_base_Application, registerErrorHandler) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *config, *_0, *_1, *_2, *_3, *_4, *components, *_5 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &config);

	Z_SET_ISREF_P(config);


	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_GET_CONSTANT(_0, "YII_ENABLE_ERROR_HANDLER");
	if (zephir_is_true(_0)) {
		zephir_array_fetch_string(&_1, config, SL("components"), PH_NOISY | PH_READONLY TSRMLS_CC);
		zephir_array_fetch_string(&_2, _1, SL("errorHandler"), PH_NOISY | PH_READONLY TSRMLS_CC);
		if (!(zephir_array_isset_string(_2, SS("class")))) {
			php_printf("Error: no errorHandler component is configured.\n");
			ZEPHIR_INIT_BNVAR(_0);
			ZVAL_LONG(_0, 1);
			ZEPHIR_CALL_FUNCTION(NULL, "exit", NULL, _0);
			zephir_check_call_status();
		}
		zephir_array_fetch_string(&_3, config, SL("components"), PH_NOISY | PH_READONLY TSRMLS_CC);
		zephir_array_fetch_string(&_4, _3, SL("errorHandler"), PH_NOISY | PH_READONLY TSRMLS_CC);
		ZEPHIR_INIT_BNVAR(_0);
		ZVAL_STRING(_0, "errorHandler", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "set", NULL, _0, _4);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
		ZEPHIR_OBS_VAR(components);
		zephir_array_fetch_string(&components, config, SL("components"), PH_NOISY TSRMLS_CC);
		zephir_array_unset_string(&components, SS("errorHandler"), PH_SEPARATE);
		zephir_array_update_string(&config, SL("components"), &components, PH_COPY | PH_SEPARATE);
		ZEPHIR_CALL_METHOD(&_5, this_ptr, "geterrorhandler",  NULL);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(NULL, _5, "register", NULL);
		zephir_check_call_status();
	}
	Z_UNSET_ISREF_P(config);
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns an ID that uniquely identifies this module among all modules within the current application.
 * Since this is an application instance, it will always return an empty string.
 * @return string the unique ID of the module.
 */
PHP_METHOD(yii_base_Application, getUniqueId) {


	RETURN_STRING("", 1);

}

/**
 * Sets the root directory of the application and the @app alias.
 * This method can only be invoked at the beginning of the constructor.
 * @param string $path the root directory of the application.
 * @property string the root directory of the application.
 * @throws InvalidParamException if the directory does not exist.
 */
PHP_METHOD(yii_base_Application, setBasePath) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL, *_1 = NULL;
	zval *path, *_2 = NULL, *_3;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &path);



	ZEPHIR_CALL_PARENT(NULL, yii_base_application_ce, this_ptr, "setbasepath", &_0, path);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "getbasepath",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_3);
	ZVAL_STRING(_3, "@app", 0);
	ZEPHIR_CALL_CE_STATIC(NULL, yii_baseyii_ce, "setalias", &_1, _3, _2);
	zephir_check_temp_parameter(_3);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

/**
 * Runs the application.
 * This is the main entrance of an application.
 * @return integer the exit status (0 means normal, non-zero values mean abnormal)
 */
PHP_METHOD(yii_base_Application, run) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *response = NULL, *_0, *_1, *_2 = NULL, *_3;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 2);
	zephir_update_property_this(this_ptr, SL("state"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "beforeRequest", 0);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "trigger", NULL, _1);
	zephir_check_temp_parameter(_1);
	zephir_check_call_status();
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 3);
	zephir_update_property_this(this_ptr, SL("state"), _0 TSRMLS_CC);
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "getrequest",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&response, this_ptr, "handlerequest", NULL, _2);
	zephir_check_call_status();
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 4);
	zephir_update_property_this(this_ptr, SL("state"), _0 TSRMLS_CC);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_STRING(_1, "afterRequest", 0);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "trigger", NULL, _1);
	zephir_check_temp_parameter(_1);
	zephir_check_call_status();
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 5);
	zephir_update_property_this(this_ptr, SL("state"), _0 TSRMLS_CC);
	ZEPHIR_CALL_METHOD(NULL, response, "send", NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 6);
	zephir_update_property_this(this_ptr, SL("state"), _0 TSRMLS_CC);
	ZEPHIR_OBS_VAR(_3);
	zephir_read_property(&_3, response, SL("exitStatus"), PH_NOISY_CC);
	RETURN_CCTOR(_3);

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
PHP_METHOD(yii_base_Application, handleRequest) {

}

/**
 * Returns the directory that stores runtime files.
 * @return string the directory that stores runtime files.
 * Defaults to the "runtime" subdirectory under [[basePath]].
 */
PHP_METHOD(yii_base_Application, getRuntimePath) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1 = NULL, *_2;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(_0);
	zephir_read_property_this(&_0, this_ptr, SL("_runtimePath"), PH_NOISY_CC);
	if (Z_TYPE_P(_0) == IS_NULL) {
		ZEPHIR_CALL_METHOD(&_1, this_ptr, "getbasepath",  NULL);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(_2);
		ZEPHIR_CONCAT_VSS(_2, _1, "/", "runtime");
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "setruntimepath", NULL, _2);
		zephir_check_call_status();
	}
	RETURN_MM_MEMBER(this_ptr, "_runtimePath");

}

/**
 * Sets the directory that stores runtime files.
 * @param string $path the directory that stores runtime files.
 */
PHP_METHOD(yii_base_Application, setRuntimePath) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL, *_2 = NULL;
	zval *path, *_0 = NULL, *_3, *_4;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &path);



	ZEPHIR_CALL_CE_STATIC(&_0, yii_baseyii_ce, "getalias", &_1, path);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("_runtimePath"), _0 TSRMLS_CC);
	_3 = zephir_fetch_nproperty_this(this_ptr, SL("_runtimePath"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(_4);
	ZVAL_STRING(_4, "@runtime", 0);
	ZEPHIR_CALL_CE_STATIC(NULL, yii_baseyii_ce, "setalias", &_2, _4, _3);
	zephir_check_temp_parameter(_4);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns the directory that stores vendor files.
 * @return string the directory that stores vendor files.
 * Defaults to "vendor" directory under [[basePath]].
 */
PHP_METHOD(yii_base_Application, getVendorPath) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1 = NULL, *_2;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_vendorPath"), PH_NOISY_CC);
	if (Z_TYPE_P(_0) == IS_NULL) {
		ZEPHIR_CALL_METHOD(&_1, this_ptr, "getbasepath",  NULL);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(_2);
		ZEPHIR_CONCAT_VSS(_2, _1, "/", "vendor");
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "setvendorpath", NULL, _2);
		zephir_check_call_status();
	}
	RETURN_MM_MEMBER(this_ptr, "_vendorPath");

}

/**
 * Sets the directory that stores vendor files.
 * @param string $path the directory that stores vendor files.
 */
PHP_METHOD(yii_base_Application, setVendorPath) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL, *_2 = NULL;
	zval *path, *_0 = NULL, *_3, *_4;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &path);



	ZEPHIR_CALL_CE_STATIC(&_0, yii_baseyii_ce, "getalias", &_1, path);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("_vendorPath"), _0 TSRMLS_CC);
	_3 = zephir_fetch_nproperty_this(this_ptr, SL("_vendorPath"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(_4);
	ZVAL_STRING(_4, "@vendor", 0);
	ZEPHIR_CALL_CE_STATIC(NULL, yii_baseyii_ce, "setalias", &_2, _4, _3);
	zephir_check_temp_parameter(_4);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns the time zone used by this application.
 * This is a simple wrapper of PHP function date_default_timezone_get().
 * If time zone is not configured in php.ini or application config,
 * it will be set to UTC by default.
 * @return string the time zone used by this application.
 * @see http://php.net/manual/en/function.date-default-timezone-get.php
 */
PHP_METHOD(yii_base_Application, getTimeZone) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_FUNCTION("date_default_timezone_get", NULL);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Sets the time zone used by this application.
 * This is a simple wrapper of PHP function date_default_timezone_set().
 * Refer to the [php manual](http://www.php.net/manual/en/timezones.php) for available timezones.
 * @param string $value the time zone used by this application.
 * @see http://php.net/manual/en/function.date-default-timezone-set.php
 */
PHP_METHOD(yii_base_Application, setTimeZone) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *value;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &value);



	ZEPHIR_CALL_FUNCTION(NULL, "date_default_timezone_set", NULL, value);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns the database connection component.
 * @return \yii\db\Connection the database connection
 */
PHP_METHOD(yii_base_Application, getDb) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "db", 0);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the log dispatcher component.
 * @return \yii\log\Dispatcher the log dispatcher component
 */
PHP_METHOD(yii_base_Application, getLog) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "log", 0);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the error handler component.
 * @return \yii\web\ErrorHandler|\yii\console\ErrorHandler the error handler application component.
 */
PHP_METHOD(yii_base_Application, getErrorHandler) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "errorHandler", 0);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the cache component.
 * @return \yii\caching\Cache the cache application component. Null if the component is not enabled.
 */
PHP_METHOD(yii_base_Application, getCache) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "cache", 0);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, _0, ZEPHIR_GLOBAL(global_false));
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the formatter component.
 * @return \yii\base\Formatter the formatter application component.
 */
PHP_METHOD(yii_base_Application, getFormatter) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "formatter", 0);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the request component.
 * @return \yii\web\Request|\yii\console\Request the request component
 */
PHP_METHOD(yii_base_Application, getRequest) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "request", 0);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the response component.
 * @return \yii\web\Response|\yii\console\Response the response component
 */
PHP_METHOD(yii_base_Application, getResponse) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "response", 0);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the view object.
 * @return View|\yii\web\View the view object that is used to render various view files.
 */
PHP_METHOD(yii_base_Application, getView) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "view", 0);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the URL manager for this application.
 * @return \yii\web\UrlManager the URL manager for this application.
 */
PHP_METHOD(yii_base_Application, getUrlManager) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "urlManager", 0);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the internationalization (i18n) component
 * @return \yii\i18n\I18N the internationalization component
 */
PHP_METHOD(yii_base_Application, getI18n) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "i18n", 0);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the mailer component.
 * @return \yii\mail\MailerInterface the mailer interface
 */
PHP_METHOD(yii_base_Application, getMail) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "mail", 0);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the auth manager for this application.
 * @return \yii\rbac\ManagerInterface the auth manager for this application.
 * Null is returned if auth manager is not configured.
 */
PHP_METHOD(yii_base_Application, getAuthManager) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "authManager", 0);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, _0, ZEPHIR_GLOBAL(global_false));
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the asset manager.
 * @return \yii\web\AssetManager the asset manager component
 */
PHP_METHOD(yii_base_Application, getAssetManager) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "assetManager", 0);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the core application components.
 * @see set
 */
PHP_METHOD(yii_base_Application, coreComponents) {

	zval *_0, *_1, *_2, *_3, *_4, *_5, *_6;
	zval *elements;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(elements);
	array_init(elements);

	ZEPHIR_INIT_VAR(_0);
	array_init_size(_0, 2);
	add_assoc_stringl_ex(_0, SS("class"), SL("yii\\log\\Dispatcher"), 1);
	zephir_array_update_string(&elements, SL("log"), &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(_1);
	array_init_size(_1, 2);
	add_assoc_stringl_ex(_1, SS("class"), SL("yii\\web\\View"), 1);
	zephir_array_update_string(&elements, SL("view"), &_1, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(_2);
	array_init_size(_2, 2);
	add_assoc_stringl_ex(_2, SS("class"), SL("yii\\base\\Formatter"), 1);
	zephir_array_update_string(&elements, SL("formatter"), &_2, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(_3);
	array_init_size(_3, 2);
	add_assoc_stringl_ex(_3, SS("class"), SL("yii\\i18n\\I18N"), 1);
	zephir_array_update_string(&elements, SL("i18n"), &_3, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(_4);
	array_init_size(_4, 2);
	add_assoc_stringl_ex(_4, SS("class"), SL("yii\\swiftmailer\\Mailer"), 1);
	zephir_array_update_string(&elements, SL("mail"), &_4, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(_5);
	array_init_size(_5, 2);
	add_assoc_stringl_ex(_5, SS("class"), SL("yii\\web\\UrlManager"), 1);
	zephir_array_update_string(&elements, SL("urlManager"), &_5, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(_6);
	array_init_size(_6, 2);
	add_assoc_stringl_ex(_6, SS("class"), SL("yii\\web\\AssetManager"), 1);
	zephir_array_update_string(&elements, SL("assetManager"), &_6, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(elements);

}

/**
 * Terminates the application.
 * This method replaces the `exit()` function by ensuring the application life cycle is completed
 * before terminating the application.
 * @param integer $status the exit status (value 0 means normal exit while other values mean abnormal exit).
 * @param Response $response the response to be sent. If not set, the default application [[response]] component will be used.
 * @throws ExitException if the application is in testing mode
 */
PHP_METHOD(yii_base_Application, end) {

	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool _1, _5;
	zval *status = NULL, *response = NULL, *_0, *_2, *_3, *_4 = NULL, *_6, *_7, *_8;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &status, &response);

	if (!status) {
		ZEPHIR_INIT_VAR(status);
		ZVAL_LONG(status, 0);
	}
	if (!response) {
		ZEPHIR_CPY_WRT(response, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(response);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("state"), PH_NOISY_CC);
	_1 = ZEPHIR_IS_LONG(_0, 2);
	if (!(_1)) {
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("state"), PH_NOISY_CC);
		_1 = ZEPHIR_IS_LONG(_2, 3);
	}
	if (_1) {
		ZEPHIR_INIT_ZVAL_NREF(_3);
		ZVAL_LONG(_3, 4);
		zephir_update_property_this(this_ptr, SL("state"), _3 TSRMLS_CC);
		ZEPHIR_INIT_VAR(_4);
		ZVAL_STRING(_4, "afterRequest", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "trigger", NULL, _4);
		zephir_check_temp_parameter(_4);
		zephir_check_call_status();
	}
	_3 = zephir_fetch_nproperty_this(this_ptr, SL("state"), PH_NOISY_CC);
	_5 = !ZEPHIR_IS_LONG(_3, 5);
	if (_5) {
		_6 = zephir_fetch_nproperty_this(this_ptr, SL("state"), PH_NOISY_CC);
		_5 = !ZEPHIR_IS_LONG(_6, 6);
	}
	if (_5) {
		ZEPHIR_INIT_ZVAL_NREF(_7);
		ZVAL_LONG(_7, 6);
		zephir_update_property_this(this_ptr, SL("state"), _7 TSRMLS_CC);
		if (Z_TYPE_P(response) == IS_NULL) {
			ZEPHIR_CALL_METHOD(&response, this_ptr, "getresponse",  NULL);
			zephir_check_call_status();
		}
		ZEPHIR_CALL_METHOD(NULL, response, "send", NULL);
		zephir_check_call_status();
	}
	ZEPHIR_INIT_NVAR(_4);
	ZEPHIR_GET_CONSTANT(_4, "YII_ENV_TEST");
	if (zephir_is_true(_4)) {
		ZEPHIR_INIT_VAR(_8);
		object_init_ex(_8, yii_base_exitexception_ce);
		ZEPHIR_CALL_METHOD(NULL, _8, "__construct", NULL, status);
		zephir_check_call_status();
		zephir_throw_exception_debug(_8, "yii/base/Application.zep", 658 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	} else {
		ZEPHIR_CALL_FUNCTION(NULL, "exit", NULL, status);
		zephir_check_call_status();
	}
	ZEPHIR_MM_RESTORE();

}

