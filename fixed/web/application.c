
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
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/operators.h"


/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */
/**
 * Application is the base class for all web application classes.
 *
 * @property string $homeUrl The homepage URL.
 * @property Session $session The session component. This property is read-only.
 * @property User $user The user component. This property is read-only.
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
ZEPHIR_INIT_CLASS(yii_web_Application) {

	ZEPHIR_REGISTER_CLASS_EX(yii\\web, Application, yii, web_application, yii_base_application_ce, yii_web_application_method_entry, 0);

	/**
	 * @var string the default route of this application. Defaults to 'site'.
	 */
	zend_declare_property_string(yii_web_application_ce, SL("defaultRoute"), "site", ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * @var array the configuration specifying a controller action which should handle
	 * all user requests. This is mainly used when the application is in maintenance mode
	 * and needs to handle all incoming requests via a single action.
	 * The configuration is an array whose first element specifies the route of the action.
	 * The rest of the array elements (key-value pairs) specify the parameters to be bound
	 * to the action. For example,
	 *
	 * ~~~
	 * [
	 *     'offline/notice',
	 *     'param1' => 'value1',
	 *     'param2' => 'value2',
	 * ]
	 * ~~~
	 *
	 * Defaults to null, meaning catch-all is not used.
	 */
	zend_declare_property_null(yii_web_application_ce, SL("catchAll"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * @var Controller the currently active controller instance
	 */
	zend_declare_property_null(yii_web_application_ce, SL("controller"), ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_property_null(yii_web_application_ce, SL("_homeUrl"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

/**
 * @inheritdoc
 */
PHP_METHOD(yii_web_Application, bootstrap) {

	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL, *_3 = NULL, *_6 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *request = NULL, *_1 = NULL, *_2 = NULL, *_4, *_5 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&request, this_ptr, "getrequest",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_1, request, "getscriptfile",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_FUNCTION(&_2, "dirname", &_3, _1);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_4);
	ZVAL_STRING(_4, "@webroot", 0);
	ZEPHIR_CALL_CE_STATIC(NULL, yii_baseyii_ce, "setalias", &_0, _4, _2);
	zephir_check_temp_parameter(_4);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_5, request, "getbaseurl",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_BNVAR(_4);
	ZVAL_STRING(_4, "@web", 0);
	ZEPHIR_CALL_CE_STATIC(NULL, yii_baseyii_ce, "setalias", &_0, _4, _5);
	zephir_check_temp_parameter(_4);
	zephir_check_call_status();
	ZEPHIR_CALL_PARENT(NULL, yii_web_application_ce, this_ptr, "bootstrap", &_6);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

/**
 * Handles the specified request.
 * @param Request $request the request to be handled
 * @return Response the resulting response
 * @throws NotFoundHttpException if the requested route is invalid
 */
PHP_METHOD(yii_web_Application, handleRequest) {

	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL, *_3 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *request, *route = NULL, *params = NULL, *resolve = NULL, *catchAll = NULL, *result = NULL, *response = NULL, *_0, *_1 = NULL, *_4;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &request);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("catchAll"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(catchAll, _0);
	if (Z_TYPE_P(catchAll) == IS_NULL) {
		ZEPHIR_CALL_METHOD(&resolve, request, "resolve",  NULL);
		zephir_check_call_status();
		ZEPHIR_OBS_VAR(route);
		zephir_array_fetch_long(&route, resolve, 0, PH_NOISY TSRMLS_CC);
		ZEPHIR_OBS_VAR(params);
		zephir_array_fetch_long(&params, resolve, 1, PH_NOISY TSRMLS_CC);
	} else {
		if (Z_TYPE_P(catchAll) == IS_ARRAY) {
			ZEPHIR_OBS_NVAR(route);
			zephir_array_fetch_long(&route, catchAll, 0, PH_NOISY TSRMLS_CC);
			ZEPHIR_INIT_VAR(_1);
			ZVAL_LONG(_1, 1);
			Z_SET_ISREF_P(catchAll);
			ZEPHIR_CALL_FUNCTION(&params, "array_splice", &_2, catchAll, _1);
			Z_UNSET_ISREF_P(catchAll);
			zephir_check_call_status();
		}
	}
	ZEPHIR_INIT_VAR(_4);
	ZEPHIR_CONCAT_SVS(_4, "Route requested: '", route, "'");
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_STRING(_1, "Application:handleRequest", 0);
	ZEPHIR_CALL_CE_STATIC(NULL, yii_baseyii_ce, "trace", &_3, _4, _1);
	zephir_check_temp_parameter(_1);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("requestedRoute"), route TSRMLS_CC);
	ZEPHIR_CALL_METHOD(&result, this_ptr, "runaction", NULL, route, params);
	zephir_check_call_status();
	if (Z_TYPE_P(result) == IS_OBJECT) {
		if (zephir_is_instance_of(result, SL("yii\\web\\Response") TSRMLS_CC)) {
			RETURN_CCTOR(result);
		}
	} else {
		ZEPHIR_CALL_METHOD(&response, this_ptr, "getresponse",  NULL);
		zephir_check_call_status();
		if (Z_TYPE_P(result) != IS_NULL) {
			zephir_update_property_zval(response, SL("data"), result TSRMLS_CC);
		}
		RETURN_CCTOR(response);
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * @return string the homepage URL
 */
PHP_METHOD(yii_web_Application, getHomeUrl) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1 = NULL, *_2, *_3 = NULL, *_4 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(_0);
	zephir_read_property_this(&_0, this_ptr, SL("_homeUrl"), PH_NOISY_CC);
	if (Z_TYPE_P(_0) == IS_NULL) {
		ZEPHIR_CALL_METHOD(&_1, this_ptr, "geturlmanager",  NULL);
		zephir_check_call_status();
		ZEPHIR_OBS_VAR(_2);
		zephir_read_property(&_2, _1, SL("showScriptName"), PH_NOISY_CC);
		if (zephir_is_true(_2)) {
			ZEPHIR_CALL_METHOD(&_3, this_ptr, "getrequest",  NULL);
			zephir_check_call_status();
			ZEPHIR_RETURN_CALL_METHOD(_3, "getscripturl", NULL);
			zephir_check_call_status();
			RETURN_MM();
		} else {
			ZEPHIR_CALL_METHOD(&_3, this_ptr, "getrequest",  NULL);
			zephir_check_call_status();
			ZEPHIR_CALL_METHOD(&_4, _3, "getbaseurl",  NULL);
			zephir_check_call_status();
			ZEPHIR_CONCAT_VS(return_value, _4, "/");
			RETURN_MM();
		}
	} else {
		RETURN_MM_MEMBER(this_ptr, "_homeUrl");
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * @param string $value the homepage URL
 */
PHP_METHOD(yii_web_Application, setHomeUrl) {

	zval *value;

	zephir_fetch_params(0, 1, 0, &value);



	zephir_update_property_this(this_ptr, SL("_homeUrl"), value TSRMLS_CC);

}

/**
 * Returns the session component.
 * @return Session the session component
 */
PHP_METHOD(yii_web_Application, getSession) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "session", 0);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the user component.
 * @return User the user component
 */
PHP_METHOD(yii_web_Application, getUser) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "user", 0);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * @inheritdoc
 */
PHP_METHOD(yii_web_Application, coreComponents) {

	zval *_1, *_2, *_3, *_4;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *parent_elements = NULL, *elements, *coreComponents;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(elements);
	array_init(elements);

	ZEPHIR_CALL_PARENT(&parent_elements, yii_web_application_ce, this_ptr, "corecomponents", &_0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_1);
	array_init_size(_1, 2);
	add_assoc_stringl_ex(_1, SS("class"), SL("yii\\web\\Request"), 1);
	zephir_array_update_string(&elements, SL("request"), &_1, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(_2);
	array_init_size(_2, 2);
	add_assoc_stringl_ex(_2, SS("class"), SL("yii\\web\\Response"), 1);
	zephir_array_update_string(&elements, SL("response"), &_2, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(_3);
	array_init_size(_3, 2);
	add_assoc_stringl_ex(_3, SS("class"), SL("yii\\web\\Session"), 1);
	zephir_array_update_string(&elements, SL("session"), &_3, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(_4);
	array_init_size(_4, 2);
	add_assoc_stringl_ex(_4, SS("class"), SL("yii\\web\\User"), 1);
	zephir_array_update_string(&elements, SL("user"), &_4, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(coreComponents);
	zephir_fast_array_merge(coreComponents, &(parent_elements), &(elements) TSRMLS_CC);
	RETURN_CCTOR(coreComponents);

}

/**
 * Registers the errorHandler component as a PHP error handler.
 */
PHP_METHOD(yii_web_Application, registerErrorHandler) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_3 = NULL;
	zval *config, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &config);

    Z_SET_ISREF_P(config);


	zephir_array_fetch_string(&_0, config, SL("components"), PH_NOISY | PH_READONLY TSRMLS_CC);
	zephir_array_fetch_string(&_1, _0, SL("errorHandler"), PH_NOISY | PH_READONLY TSRMLS_CC);
	if (zephir_array_isset_string(_1, SS("class"))) {
		ZEPHIR_INIT_VAR(_2);
		ZVAL_STRING(_2, "yii\\web\\ErrorHandler", 1);
		zephir_array_update_multi(&config, &_2 TSRMLS_CC, SL("sss"), 6, SL("components"), SL("errorHandler"), SL("class"));
	}
	ZEPHIR_CALL_PARENT(NULL, yii_web_application_ce, this_ptr, "registererrorhandler", &_3, config);
	zephir_check_call_status();
    Z_UNSET_ISREF_P(config);
	ZEPHIR_MM_RESTORE();

}

