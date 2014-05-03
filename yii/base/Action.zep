/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */

namespace yii\base;

use yii\BaseYii;

/**
 * Action is the base class for all controller action classes.
 *
 * Action provides a way to divide a complex controller into
 * smaller actions in separate class files.
 *
 * Derived classes must implement a method named `run()`. This method
 * will be invoked by the controller when the action is requested.
 * The `run()` method can have parameters which will be filled up
 * with user input values automatically according to their names.
 * For example, if the `run()` method is declared as follows:
 *
 * ~~~
 * public function run($id, $type = 'book') { ... }
 * ~~~
 *
 * And the parameters provided for the action are: `['id' => 1]`.
 * Then the `run()` method will be invoked as `run(1)` automatically.
 *
 * @property string $uniqueId The unique ID of this action among the whole application. This property is
 * read-only.
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
class Action extends Component
{
    /**
     * @var string ID of the action
     */
    public id;
    /**
     * @var Controller the controller that owns this action
     */
    public controller;

    /**
     * Constructor.
     *
     * @param string $id the ID of this action
     * @param Controller $controller the controller that owns this action
     * @param array $config name-value pairs that will be used to initialize the object properties
     */
    public function __construct(string id, controller, $config = [])
    {
        let this->id = id;
        let this->controller = controller;
        parent::__construct(config);
    }

    /**
     * Returns the unique ID of this action among the whole application.
     *
     * @return string the unique ID of this action among the whole application.
     */
    public function getUniqueId()
    {
        return this->controller->getUniqueId() . "/" . this->id;
    }

    /**
     * Runs this action with the specified parameters.
     * This method is mainly invoked by the controller.
     *
     * @param array $params the parameters to be bound to the action's run() method.
     * @return mixed the result of the action
     * @throws InvalidConfigException if the action class does not have a run() method
     */
    public function runWithParams(params)
    {
        var args, requestedParams;
        if !method_exists(this, "run") {
            throw new InvalidConfigException(get_class(this) . " must define a \"run()\" method.");
        }

        let args = this->controller->bindActionParams(this, params);
        var app, result;

        let app = BaseYii::$app,
            requestedParams = app->requestedParams;

        BaseYii::trace("Running action: " . get_class(this) . "::run()", __METHOD__);
        if typeof requestedParams == "null" {
            let app->requestedParams = args;
            let BaseYii::$app = app;
        }
        if this->beforeRun() {
            var elements = [], temp_args;
            let elements[] = this,
                elements[] = "run",
                temp_args = args;

            let result = call_user_func_array(elements, temp_args);
            this->afterRun();

            return result;
        } else {
            return null;
        }
    }

    /**
     * This method is called right before `run()` is executed.
     * You may override this method to do preparation work for the action run.
     * If the method returns false, it will cancel the action.
     *
     * @return boolean whether to run the action.
     */
    protected function beforeRun()
    {
        return true;
    }

    /**
     * This method is called right after `run()` is executed.
     * You may override this method to do post-processing work for the action run.
     */
    protected function afterRun()
    {
    }
}
