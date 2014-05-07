/**
 * @link http://www.yiiframework.com/
 * @copyright Copyright (c) 2008 Yii Software LLC
 * @license http://www.yiiframework.com/license/
 */

namespace yii\base;

use yii\BaseYii;

/**
 * ErrorException represents a PHP error.
 *
 * @author Alexander Makarov <sam@rmcreative.ru>
 * @since 2.0
 */
class ErrorException extends \ErrorException
{
    /**
     * Constructs the exception.
     * @link http://php.net/manual/en/errorexception.construct.php
     * @param $message [optional]
     * @param $code [optional]
     * @param $severity [optional]
     * @param $filename [optional]
     * @param $lineno [optional]
     * @param $previous [optional]
     */
    public function __construct(message = "", code = 0, severity = 1, filename = null , lineno = null, previous = null)
    {

        if typeof filename == "null" {
            let filename = __FILE__;
        }

        if typeof lineno == "null" {
            let lineno  = __LINE__;
        }
                
        parent::__construct(message, code, severity, filename, lineno, previous);

        var trace;
        if function_exists("xdebug_get_function_stack") {
            let trace = array_slice(array_reverse(xdebug_get_function_stack()), 3, -1);

            var key, frame, temp_trace, trace_key;
            let temp_trace = trace;
            for key, frame in temp_trace {
                let trace_key = trace[key];
                if !isset frame["function"] {
                    let trace_key["funciton"] = "unknown";
                }

                // XDebug < 2.1.1: http://bugs.xdebug.org/view.php?id=695
                if !isset frame["type"] || frame["type"] == "static" {
                    let trace_key["type"] = "::";
                }
                else {
                    if frame["type"] == "dynamic" {
                        let trace_key["type"] = "->";
                    }
                }

                if isset frame["params"] && !isset frame["args"] {
                    var temp_args;

                    let temp_args = frame["params"],
                        trace_key["args"] = temp_args;
                }
                let trace[key] = trace_key;
            }

            var ref;
            let ref = new \ReflectionProperty("Exception", "trace");
            ref->setAccessible(true);
            ref->setValue(this, trace);
        }
    }

    /**
     * Returns if error is one of fatal type.
     *
     * @param array $error error got from error_get_last()
     * @return boolean if error is one of fatal type
     */
    public static function isFatalError(error)
    {
        var errors;

        let errors = [E_ERROR, E_PARSE, E_CORE_ERROR, E_CORE_WARNING, E_COMPILE_ERROR, E_COMPILE_WARNING];

        return isset error["type"] && in_array(error["type"], errors);
    }

    /**
     * @return string the user-friendly name of this exception
     */
    public function getName()
    {
        var names = [];
        
        let names[E_ERROR] = "PHP Fatal Error",
            names[E_PARSE] = "PHP Parse Error",
            names[E_CORE_ERROR] = "PHP Core Error",
            names[E_COMPILE_ERROR] = "PHP Compile Error",
            names[E_USER_ERROR] = "PHP User Error",
            names[E_WARNING] = "PHP Warning",
            names[E_CORE_WARNING] = "PHP Core Warning",
            names[E_COMPILE_WARNING] = "PHP Compile Warning",
            names[E_USER_WARNING] = "PHP User Warning",
            names[E_STRICT] = "PHP Strict Warning",
            names[E_NOTICE] = "PHP Notice",
            names[E_RECOVERABLE_ERROR] = "PHP Recoverable Error",
            names[E_DEPRECATED] = "PHP Deprecated Warning";

        var code;
        let code = this->getCode();
        return isset names[code] ? names[code] : "Error";
    }
}
