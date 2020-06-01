(defun compose (f g x) 
            (funcall f (funcall g x)))

(print (sin (cos 5)));contol value to test my function

(print (compose 'sin 'cos 5))
(print (funcall 'compose #'sin #'cos 5))
;; (print (compose 'sqrt 'sqrt 625))
;; (print (compose 'log 'exp 5))