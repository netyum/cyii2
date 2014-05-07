/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */

namespace yii\base;

/**
 * Event is the base class for all event classes.
 *
 * It encapsulates the parameters associated with an event.
 * The [[sender]] property describes who raises the event.
 * And the [[handled]] property indicates if the event is handled.
 * If an event handler sets [[handled]] to be true, the rest of the
 * uninvoked handlers will no longer be called to handle the event.
 *
 * Additionally, when attaching an event handler, extra data may be passed
 * and be available via the [[data]] property when the event handler is invoked.
 *
 * @author Qiang Xue <qiang.xue@gmail.com>
 * @since 2.0
 */
class Event extends $Object
{
    /**
     * @var string the event name. This property is set by [[Component::trigger()]] and [[trigger()]].
     * Event handlers may use this property to check what event it is handling.
     */
    protected name;
    /**
     * @var object the sender of this event. If not set, this property will be
     * set as the object whose "trigger()" method is called.
     * This property may also be a `null` when this event is a
     * class-level event which is triggered in a static context.
     */
    protected sender;
    /**
     * @var boolean whether the event is handled. Defaults to false.
     * When a handler sets this to be true, the event processing will stop and
     * ignore the rest of the uninvoked event handlers.
     */
    public handled = false;
    /**
     * @var mixed the data that is passed to [[Component::on()]] when attaching an event handler.
     * Note that this varies according to which event handler is currently executing.
     */
    public data;

    protected static _events;

    /**
     * Attaches an event handler to a class-level event.
     *
     * When a class-level event is triggered, event handlers attached
     * to that class and all parent classes will be invoked.
     *
     * For example, the following code attaches an event handler to `ActiveRecord`'s
     * `afterInsert` event:
     *
     * ~~~
     * Event::on(ActiveRecord::className(), ActiveRecord::EVENT_AFTER_INSERT, function ($event) {
     *     Yii::trace(get_class($event->sender) . ' is inserted.');
     * });
     * ~~~
     *
     * The handler will be invoked for EVERY successful ActiveRecord insertion.
     *
     * For more details about how to declare an event handler, please refer to [[Component::on()]].
     *
     * @param string $class the fully qualified class name to which the event handler needs to attach.
     * @param string $name the event name.
     * @param callable $handler the event handler.
     * @param mixed $data the data to be passed to the event handler when the event is triggered.
     * When the event handler is invoked, this data can be accessed via [[Event::data]].
     * @see off()
     */
    public static function on($class, string name, handler, data = null)
    {
        var events, elements = [], event, event_class;

        let $class = ltrim($class, "\\");

        let events = self::$_events;

        if typeof events != "array" {
            let events = [];
        }

        let elements[] = handler,
            elements[] = data;

        let event = events[name],
            event_class = event[$class],
            event_class[] = elements,
            event[$class] = event_class,
            events[name] = event;
            self::_events = events;
 
        return elements;
       
    }

    /**
     * Detaches an event handler from a class-level event.
     *
     * This method is the opposite of [[on()]].
     *
     * @param string $class the fully qualified class name from which the event handler needs to be detached.
     * @param string $name the event name.
     * @param callable $handler the event handler to be removed.
     * If it is null, all handlers attached to the named event will be removed.
     * @return boolean whether a handler is found and detached.
     * @see on()
     */
    public static function off($class, string name, handler = null)
    {
        var events, event, temp_event, temp_event2, removed_temp_event;

        let $class = ltrim($class, "\\");
        if !isset self::_events[name][$class] || empty self::_events[name][$class] {
            return false;
        }
        if typeof handler == "null" {
            let events = self::_events,
                event = events[name];
            unset event[$class];
            let events[name] = event,
                self::_events = events;
            return true;
        } else {
            var removed, i;
            let removed = false;
            let events = self::_events,
                event = events[name];

            if isset event[$class] && typeof event[$class] == "array" {
                for i, temp_event in event[$class] {
                    if temp_event[0] == handler {

                        let temp_event2 = event[$class];
                        unset temp_event2[i];

                        let event[$class] = temp_event2,
                            removed = true;
                    } 
                }
            }
            if (removed) {
                let removed_temp_event = event[$class],
                    removed_temp_event = array_values(removed_temp_event),
                    event[$class] = removed_temp_event,
                    events[name] = event[$class],
                    self::_events = events;
            }

            return removed;
        }
    }

    /**
     * Returns a value indicating whether there is any handler attached to the specified class-level event.
     * Note that this method will also check all parent classes to see if there is any handler attached
     * to the named event.
     * @param string|object $class the object or the fully qualified class name specifying the class-level event.
     * @param string $name the event name.
     * @return boolean whether there is any handler attached to the event.
     */
    public static function hasHandlers($class, string name) -> bool
    {
        if !isset self::_events[name] || empty self::_events[name] {
            return false;
        }

        if typeof $class == "object" {
            let $class = get_class($class);
        } else {
            let $class = ltrim($class, "\\");
        }

        while typeof $class != "boolean" {
            if isset self::_events[name][$class] && !empty self::_events[name][$class] {
                return true;
            }
            let $class = get_parent_class($class);
        }
        return false;
    }

    /**
     * Triggers a class-level event.
     * This method will cause invocation of event handlers that are attached to the named event
     * for the specified class and all its parent classes.
     * @param string|object $class the object or the fully qualified class name specifying the class-level event.
     * @param string $name the event name.
     * @param Event $event the event parameter. If not set, a default [[Event]] object will be created.
     */
    public static function trigger($class, string name, event = null)
    {
        var events;
        let events = self::_events;

        if !isset events[name] || empty events[name]{
            return;
        }

        var temp_event = null;
        let temp_event = event;
        if typeof temp_event == "null" {
            let temp_event = new $static;
        }

        let temp_event->handled = false;
        let temp_event->name = name;

        if typeof $class == "object" {
            if typeof temp_event->sender == "null" {
                let temp_event->sender = $class;
            }
            let $class = get_class($class);
        } else {
            let $class = ltrim($class, "\\");
        }

        while typeof $class != "boolean" {
            if isset events[name][$class] && !empty events[name][$class] {
                var handler, data, call, class_handers;
                let class_handers = events[name][$class];

                for handler in class_handers {
                    let data = handler[1],
                        call = handler[0];

                    let temp_event->data = data;
                    call_user_func(call, temp_event);
                    if temp_event->handled == true {
                        return;
                    }
                }
            }
            let $class = get_parent_class($class);
        }
    }
}
