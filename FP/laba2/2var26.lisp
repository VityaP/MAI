(defun makeSquare (x) (* x x))

(defun square-tree-recursive (seq)
    (if (not (null seq))
        (if (listp seq)
            (progn
                (setf (first seq) (square-tree-recursive (first seq)))
                (square-tree-recursive (rest seq))
                (return-from square-tree-recursive seq))
            (progn
                (setf seq (makeSquare seq))
                (return-from square-tree-recursive seq)))))

(defun square-tree (seq)
    (setq answer (copy-tree seq))
    (return-from square-tree (square-tree-recursive answer)))

(setq tree '(1 (2 (3 4) 5) (6 7)))
(print tree)
(print (square-tree tree))
(print tree)