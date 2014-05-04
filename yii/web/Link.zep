/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */

namespace yii\web;

/**
 * Link represents a link object as defined in [JSON Hypermedia API Language](https://tools.ietf.org/html/draft-kelly-json-hal-03).
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
class Link extends \yii\base\Object
{
    /**
     * The self link.
     */
    const REL_SELF = "self";

    /**
     * @var string a URI [RFC3986](https://tools.ietf.org/html/rfc3986) or
     * URI template [RFC6570](https://tools.ietf.org/html/rfc6570). This property is required.
     */
    public href;
    /**
     * @var string a secondary key for selecting Link Objects which share the same relation type
     */
    public name;
    /**
     * @var string a hint to indicate the media type expected when dereferencing the target resource
     */
    public type;
    /**
     * @var boolean a value indicating whether [[href]] refers to a URI or URI template.
     */
    public templated = false;
    /**
     * @var string a URI that hints about the profile of the target resource.
     */
    public profile;
    /**
     * @var string a label describing the link
     */
    public title;
    /**
     * @var string the language of the target resource
     */
    public hreflang;

    /**
     * Serializes a list of links into proper array format.
     * @param array $links the links to be serialized
     * @return array the proper array representation of the links.
     */
    public static function serialize(links)
    {
        var temp_links, link, rel, i, l, temp_link, elements = [];
        let temp_links = links;

        for rel, temp_link in temp_links {
            let link = temp_link;

            if typeof temp_link == "array" {
                for i, l in temp_link {
                    var temp_l;
                    if typeof l == "object" {
                        if l instanceof Link {
                            //convert_to_array(l);
                            //array_filter(l);
                        }
                        else {
                            let temp_l = l,
                                elements["href"] = l,
                                link[i] = elements;
                        }
                    }
                }
                let links[rel] = link;
            } else {
                if typeof temp_link == "object" {
                    if !(temp_link instanceof Link) {
                        let elements["href"] = link,
                            links[rel] = elements;
                    }
                }
            }
        }

        return $links;
    }
}
