/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */

namespace yii\web;

use yii\BaseYii;

/**
 * CompositeUrlRule is the base class for URL rule classes that consist of multiple simpler rules.
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
abstract class CompositeUrlRule extends \yii\base\Object implements UrlRuleInterface
{
    /**
     * @var UrlRuleInterface[] the URL rules contained in this composite rule.
     * This property is set in [[init()]] by the return value of [[createRules()]].
     */
    protected rules;

    /**
     * Creates the URL rules that should be contained within this composite rule.
     * @return UrlRuleInterface[] the URL rules
     */
    abstract protected function createRules();

    /**
     * @inheritdoc
     */
    public function init()
    {
        parent::init();
        var rules;
        let rules = this->createRules(),
            this->rules = rules;
    }

    /**
     * @inheritdoc
     */
    public function parseRequest(manager, request)
    {
        var rules, rule, result;
        let rules = this->rules;

        if typeof rules == "array" {
            for rule in rules {
                let result = rule->parseRequest(manager, request);
                if typeof result == "boolean" {
                    BaseYii::trace("Request parsed with URL rule: " . rule->name , __METHOD__);
                    return result;
                }
            }
        }
        return false;
    }

    /**
     * @inheritdoc
     */
    public function createUrl(manager, route, params)
    {
        var rules, rule, url;
        let rules = this->rules;
        for rule in rules {
            let url = rule->createUrl(manager, route, params);
            if typeof url != "boolean" {
                return url;
            }
        }
        return false;
    }
}
