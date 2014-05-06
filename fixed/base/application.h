
extern zend_class_entry *yii_base_application_ce;

ZEPHIR_INIT_CLASS(yii_base_Application);

PHP_METHOD(yii_base_Application, __construct);
PHP_METHOD(yii_base_Application, preInit);
PHP_METHOD(yii_base_Application, init);
PHP_METHOD(yii_base_Application, bootstrap);
PHP_METHOD(yii_base_Application, registerErrorHandler);
PHP_METHOD(yii_base_Application, getUniqueId);
PHP_METHOD(yii_base_Application, setBasePath);
PHP_METHOD(yii_base_Application, run);
PHP_METHOD(yii_base_Application, handleRequest);
PHP_METHOD(yii_base_Application, getRuntimePath);
PHP_METHOD(yii_base_Application, setRuntimePath);
PHP_METHOD(yii_base_Application, getVendorPath);
PHP_METHOD(yii_base_Application, setVendorPath);
PHP_METHOD(yii_base_Application, getTimeZone);
PHP_METHOD(yii_base_Application, setTimeZone);
PHP_METHOD(yii_base_Application, getDb);
PHP_METHOD(yii_base_Application, getLog);
PHP_METHOD(yii_base_Application, getErrorHandler);
PHP_METHOD(yii_base_Application, getCache);
PHP_METHOD(yii_base_Application, getFormatter);
PHP_METHOD(yii_base_Application, getRequest);
PHP_METHOD(yii_base_Application, getResponse);
PHP_METHOD(yii_base_Application, getView);
PHP_METHOD(yii_base_Application, getUrlManager);
PHP_METHOD(yii_base_Application, getI18n);
PHP_METHOD(yii_base_Application, getMail);
PHP_METHOD(yii_base_Application, getAuthManager);
PHP_METHOD(yii_base_Application, getAssetManager);
PHP_METHOD(yii_base_Application, coreComponents);
PHP_METHOD(yii_base_Application, end);

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_base_application___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_base_application_preinit, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_base_application_registererrorhandler, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_base_application_setbasepath, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_base_application_handlerequest, 0, 0, 1)
	ZEND_ARG_INFO(0, request)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_base_application_setruntimepath, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_base_application_setvendorpath, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_base_application_settimezone, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_base_application_end, 0, 0, 0)
	ZEND_ARG_INFO(0, status)
	ZEND_ARG_INFO(0, response)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(yii_base_application_method_entry) {
	PHP_ME(yii_base_Application, __construct, arginfo_yii_base_application___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(yii_base_Application, preInit, arginfo_yii_base_application_preinit, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, init, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, bootstrap, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(yii_base_Application, registerErrorHandler, arginfo_yii_base_application_registererrorhandler, ZEND_ACC_PROTECTED)
	PHP_ME(yii_base_Application, getUniqueId, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, setBasePath, arginfo_yii_base_application_setbasepath, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, run, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, handleRequest, arginfo_yii_base_application_handlerequest, ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getRuntimePath, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, setRuntimePath, arginfo_yii_base_application_setruntimepath, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getVendorPath, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, setVendorPath, arginfo_yii_base_application_setvendorpath, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getTimeZone, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, setTimeZone, arginfo_yii_base_application_settimezone, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getDb, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getLog, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getErrorHandler, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getCache, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getFormatter, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getRequest, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getResponse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getView, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getUrlManager, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getI18n, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getMail, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getAuthManager, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, getAssetManager, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, coreComponents, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_base_Application, end, arginfo_yii_base_application_end, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
