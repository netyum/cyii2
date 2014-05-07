/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */

namespace yii\web;

use yii\BaseYii;
use yii\base\InvalidRouteException;

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
class Application extends \yii\base\Application
{
    /**
     * @var string the default route of this application. Defaults to 'site'.
     */
    public defaultRoute = "site";
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
    public catchAll;
    /**
     * @var Controller the currently active controller instance
     */
    public controller;

    protected _homeUrl;

    /**
     * @inheritdoc
     */
    protected function bootstrap()
    {
        var request;
        let request = this->getRequest();
        BaseYii::setAlias("@webroot", dirname(request->getScriptFile()));
        BaseYii::setAlias("@web", request->getBaseUrl());

        parent::bootstrap();
    }

    /**
     * Handles the specified request.
     * @param Request $request the request to be handled
     * @return Response the resulting response
     * @throws NotFoundHttpException if the requested route is invalid
     */
    public function handleRequest($request)
    {
        var route, params, resolve, catchAll, result, response;
        let catchAll = this->catchAll;
        if typeof catchAll == "null" {
            let resolve = request->resolve(),
                route = resolve[0],
                params = resolve[1];
        } else {
            if typeof catchAll == "array" {
                let route = catchAll[0],
                    params = array_splice(catchAll, 1);
            }
        }
        //try {
            BaseYii::trace("Route requested: '". $route ."'", __METHOD__);
            let this->requestedRoute = route,
                result = this->runAction(route, params);

            if typeof result == "object" {
                if result instanceof Response {
                    return result;
                }
            }
            else {
                let response = this->getResponse();
                if typeof result != "null" {
                    let response->data = result;
                }

                return response;
            }
        //} catch (InvalidRouteException $e) {
        //    throw new NotFoundHttpException($e->getMessage(), $e->getCode(), $e);
        //}
    }

    /**
     * @return string the homepage URL
     */
    public function getHomeUrl()
    {
        if typeof this->_homeUrl == "null" {
            if this->getUrlManager()->showScriptName {
                return this->getRequest()->getScriptUrl();
            } else {
                return this->getRequest()->getBaseUrl() . "/";
            }
        } else {
            return this->_homeUrl;
        }
    }

    /**
     * @param string $value the homepage URL
     */
    public function setHomeUrl(value)
    {
        let this->_homeUrl = value;
    }

    /**
     * Returns the session component.
     * @return Session the session component
     */
    public function getSession()
    {
        return this->get("session");
    }

    /**
     * Returns the user component.
     * @return User the user component
     */
    public function getUser()
    {
        return this->get("user");
    }

    /**
     * @inheritdoc
     */
    public function coreComponents()
    {
        var parent_elements, elements = [], coreComponents;

        let parent_elements = parent::coreComponents(),
            elements["request"] = ["class" : "yii\\web\\Request"],
            elements["response"] = ["class" : "yii\\web\\Response"],
            elements["session"] = ["class" : "yii\\web\\Session"],
            elements["user"] = ["class" : "yii\\web\\User"],
            coreComponents = array_merge(parent_elements, elements);

        return coreComponents;
    }

    /**
     * Registers the errorHandler component as a PHP error handler.
     */
    //protected function registerErrorHandler(&$config)
    protected function registerErrorHandler(config)
    {
        if !isset config["components"]["errorHandler"]["class"] {
            let config["components"]["errorHandler"]["class"] = "yii\\web\\ErrorHandler";
        }
        parent::registerErrorHandler(config);
    }
}
