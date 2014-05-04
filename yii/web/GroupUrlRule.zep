/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */

namespace yii\web;

use yii\BaseYii;
use yii\base\InvalidConfigException;

/**
 * GroupUrlRule represents a collection of URL rules sharing the same prefix in their patterns and routes.
 *
 * GroupUrlRule is best used by a module which often uses module ID as the prefix for the URL rules.
 * For example, the following code creates a rule for the `admin` module:
 *
 * ```php
 * new GroupUrlRule([
 *     'prefix' => 'admin',
 *     'rules' => [
 *         'login' => 'user/login',
 *         'logout' => 'user/logout',
 *         'dashboard' => 'default/dashboard',
 *     ],
 * ]);
 *
 * // the above rule is equivalent to the following three rules:
 *
 * [
 *     'admin/login' => 'admin/user/login',
 *     'admin/logout' => 'admin/user/logout',
 *     'admin/dashboard' => 'admin/default/dashboard',
 * ]
 * ```
 *
 * The above example assumes the prefix for patterns and routes are the same. They can be made different
 * by configuring [[prefix]] and [[routePrefix]] separately.
 *
 * Using a GroupUrlRule is more efficient than directly declaring the individual rules it contains.
 * This is because GroupUrlRule can quickly determine if it should process a URL parsing or creation request
 * by simply checking if the prefix matches.
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
class GroupUrlRule extends CompositeUrlRule
{
    /**
     * @var array the rules contained within this composite rule. Please refer to [[UrlManager::rules]]
     * for the format of this property.
     * @see prefix
     * @see routePrefix
     */
    public rules;
    /**
     * @var string the prefix for the pattern part of every rule declared in [[rules]].
     * The prefix and the pattern will be separated with a slash.
     */
    public prefix;
    /**
     * @var string the prefix for the route part of every rule declared in [[rules]].
     * The prefix and the route will be separated with a slash.
     * If this property is not set, it will take the value of [[prefix]].
     */
    public routePrefix;
    /**
     * @var array the default configuration of URL rules. Individual rule configurations
     * specified via [[rules]] will take precedence when the same property of the rule is configured.
     */
    public ruleConfig;
    // = ['class' => 'yii\web\UrlRule'];


    /**
     * @inheritdoc
     */
    public function init()
    {
        if typeof this->routePrefix == "null" {
            let this->routePrefix = this->prefix;
        }
        var slash = "/";
        if typeof this->prefix == "string" {
            let this->prefix = trim(this->prefix, slash);
        }
        if typeof this->routePrefix == "string" {
            let this->routePrefix = trim(this->routePrefix, slash);
        }
        parent::init();
    }

    /**
     * @inheritdoc
     */
    protected function createRules()
    {
        var rules = [], this_rules, key, rule, temp_rule, slash;
        let slash = "/";
        let this_rules = this->rules;
        for key, temp_rule in this_rules {
            let rule = temp_rule;
            if typeof temp_rule != "array" {
                let rule = [];
                let rule["pattern"] = ltrim(this->prefix . slash . key, slash),
                    rule["route"] = ltrim(this->routePrefix . slash . temp_rule, slash);
            } else {
                if isset rule["pattern"] && isset rule["route"] {
                    let rule["pattern"] = ltrim(this->prefix . slash . rule["pattern"], slash),
                        rule["route"] = ltrim(this->routePrefix . slash . rule["route"], slash);
                }
            }

            var merge_array;
            let merge_array = array_merge(this->ruleConfig, rule),
                rule = BaseYii::createObject(merge_array);

            if !(rule instanceof UrlRuleInterface) {
                throw new InvalidConfigException("URL rule class must implement UrlRuleInterface.");
            }
            let rules[] = rule;
        }
        return rules;
    }

    /**
     * @inheritdoc
     */
    public function parseRequest(manager, request)
    {
        var pathInfo, slash, pos;
        let slash = "/",
            pathInfo = request->getPathInfo();

        if typeof this->prefix == "string" {
            let pos = strpos(pathInfo . slash, this->prefix . slash);
            if this->prefix == "" || (typeof pos != "boolean" && pos == 0) {
                return parent::parseRequest(manager, request);
            }
        }
        return false;
    }

    /**
     * @inheritdoc
     */
    public function createUrl(manager, route, params)
    {
        var pos;
        if typeof this->prefix == "string" {
            let pos = strpos(route, this->routePrefix . "/");
            if this->routePrefix == "" || (typeof pos != "boolean" && pos == 0) {
                return parent::createUrl(manager, route, params);
            }
        }
        return false;
    }
}
