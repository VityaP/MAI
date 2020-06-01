(defun print-n-randoms (n max-limit)
    (dotimes (i n)
        (format t "~r~%" (random max-limit))))
(print-n-randoms 10 100)