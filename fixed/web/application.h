
extern zend_class_entry *yii_web_application_ce;

ZEPHIR_INIT_CLASS(yii_web_Application);

PHP_METHOD(yii_web_Application, bootstrap);
PHP_METHOD(yii_web_Application, handleRequest);
PHP_METHOD(yii_web_Application, getHomeUrl);
PHP_METHOD(yii_web_Application, setHomeUrl);
PHP_METHOD(yii_web_Application, getSession);
PHP_METHOD(yii_web_Application, getUser);
PHP_METHOD(yii_web_Application, coreComponents);
PHP_METHOD(yii_web_Application, registerErrorHandler);

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_web_application_handlerequest, 0, 0, 1)
	ZEND_ARG_INFO(0, request)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_web_application_sethomeurl, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_yii_web_application_registererrorhandler, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(yii_web_application_method_entry) {
	PHP_ME(yii_web_Application, bootstrap, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(yii_web_Application, handleRequest, arginfo_yii_web_application_handlerequest, ZEND_ACC_PUBLIC)
	PHP_ME(yii_web_Application, getHomeUrl, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_web_Application, setHomeUrl, arginfo_yii_web_application_sethomeurl, ZEND_ACC_PUBLIC)
	PHP_ME(yii_web_Application, getSession, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_web_Application, getUser, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_web_Application, coreComponents, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(yii_web_Application, registerErrorHandler, arginfo_yii_web_application_registererrorhandler, ZEND_ACC_PROTECTED)
  PHP_FE_END
};
