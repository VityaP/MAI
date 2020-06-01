(defun copy-array (array)
    (let ((dims (array-dimensions array)))
        (adjust-array (make-array dims :displaced-to array) dims)))

(defun transpose (matrix)
(let*   ((n (array-dimension matrix 0))
        (result (copy-array matrix))
        (i 0) (j 0))
        (loop while (< i n)
            do
            (setq j 0)
            (loop while (< j n)
                do
                (setf (aref result i j) (aref matrix j i))
                (setq j (+ j 1)))
            (setq i (+ i 1)))
        (return-from transpose result)))

(defun execute (nums)
    (print nums)
    (print (transpose nums))
    (print nums)
    (print "---------------------------------------------------"))

(defvar first-test (make-array '(3 3) :initial-contents '((1 2 3)(4 5 6)(7 8 9))))
(defvar second-test (make-array '(4 4) :initial-contents '((100 5 6 9)(1 2 3 8)(5 10 12 1)(1 5 13 10))))
(defvar third-test (make-array '(1 1) :initial-contents '((31415))))

(execute first-test)
(execute second-test)
(execute third-test)