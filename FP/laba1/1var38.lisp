(defun compose (f g)
    #'(lambda (x) (funcall f (funcall g x))))
(print (funcall (compose #'sin #'cos) 20))
(print (sin (cos 20)))