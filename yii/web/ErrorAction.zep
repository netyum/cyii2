/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */

namespace yii\web;

use yii\BaseYii;
use yii\base\Action;
use yii\base\Exception;
use yii\base\UserException;

/**
 * ErrorAction displays application errors using a specified view.
 *
 * To use ErrorAction, you need to do the following steps:
 *
 * First, declare an action of ErrorAction type in the `actions()` method of your `SiteController`
 * class (or whatever controller you prefer), like the following:
 *
 * ```php
 * public function actions()
 * {
 *     return [
 *         'error' => ['class' => 'yii\web\ErrorAction'],
 *     ];
 * }
 * ```
 *
 * Then, create a view file for this action. If the route of your error action is `site/error`, then
 * the view file should be `views/site/error.php`. In this view file, the following variables are available:
 *
 * - `$name`: the error name
 * - `$message`: the error message
 * - `$exception`: the exception being handled
 *
 * Finally, configure the "errorHandler" application component as follows,
 *
 * ```php
 * 'errorHandler' => [
 *     'errorAction' => 'site/error',
 * ]
 * ```
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
class ErrorAction extends Action
{
    /**
     * @var string the view file to be rendered. If not set, it will take the value of [[id]].
     * That means, if you name the action as "error" in "SiteController", then the view name
     * would be "error", and the corresponding view file would be "views/site/error.php".
     */
    public view;
    /**
     * @var string the name of the error when the exception name cannot be determined.
     * Defaults to "Error".
     */
    public defaultName;
    /**
     * @var string the message to be displayed when the exception message contains sensitive information.
     * Defaults to "An internal server error occurred.".
     */
    public defaultMessage;

    public function run()
    {
        var exception, app, code, name, message;

        let app = BaseYii::$app,
            exception = app->errorHandler->exception;

        if typeof exception == "null" {
            return "";
        }

        if exception instanceof HttpException {
            let code = exception->statusCode;
        } else {
            let code = exception->getCode();
        }
        if exception instanceof Exception {
            let name = exception->getName();
        } else {
            let name = this->defaultName;
            if typeof name == "null" {
                let name = BaseYii::t("yii", "Error");
            }
        }
        if !empty code {
            let name .= " (#". code .")";
        }

        if exception instanceof UserException {
            let message = exception->getMessage();
        } else {
            let message = this->defaultMessage;
            if typeof message == "null" {
                let message = BaseYii::t("yii", "An internal server error occurred.");
            } 
        }

        var request, is_ajax;

        let request = app->getRequest(),
            is_ajax = request->getIsAjax();

        if is_ajax == true {
            return name . ": ". message;
        } else {
            var elements = [];
            let elements["name"] = name,
                elements["message"] = message,
                elements["exception"] = exception;

            var retval;
            if typeof this->view == "null" {
                let retval = this->controller->render(this->id, elements);
            } 
            else {
                let retval = this->controller->render(this->view, elements);
            }
            return retval;
        }
    }
}
