(defun is_russian_lower (char)
  (position char "абвгдеёжзийклмнопрстуфхцчшщъыьэюя"))

(defun is_russian_upper (char)
  (position char "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"))

(defun compare-like-russian (symb1 symb2)
    (when (and  (equal  (is_russian_lower symb1) (is_russian_lower symb2)) 
                (not (equal Nil (is_russian_lower symb1))))
        (return-from compare-like-russian T))
    (when (and  (equal  (is_russian_upper symb1) (is_russian_lower symb2)) 
                (not (equal Nil (is_russian_upper symb1))))
        (return-from compare-like-russian T))
    (when (and  (equal  (is_russian_lower symb1) (is_russian_upper symb2)) 
                (not (equal Nil (is_russian_lower symb1))))
        (return-from compare-like-russian T))
    (when (and  (equal  (is_russian_upper symb1) (is_russian_upper symb2)) 
                (not (equal Nil (is_russian_upper symb1))))
        (return-from compare-like-russian T))
    (return-from compare-like-russian Nil))

(defun my-special-compare (symb1 symb2)
    (when (or (char-equal symb1 symb2) (compare-like-russian symb1 symb2))
        (return-from my-special-compare T))
    (return-from my-special-compare Nil))


(defun is-whitespace (c)
    (when (or (equal c #\Space) (equal c #\Tab) (equal c #\Newline))
        (return-from is-whitespace T))
    (return-from is-whitespace Nil))

(defun count-words-with-start-eq-end (str)
(let*   ((idx 0) (begin 0) (end -1)
        (answer 0) (n (length str)))

        (loop while (< idx n)
            do
            (if (is-whitespace (aref str idx))
                (progn
                    (when (my-special-compare (aref str begin) (aref str (max 0 end)))
                        (when (not (is-whitespace (aref str begin)))
                            (setq answer (+ answer 1))))
                    (loop while (and (< idx n) (is-whitespace (aref str idx)))
                        do
                        (setq idx (+ idx 1)))
                    (setq begin idx)
                    (setq end (- idx 1)))
                (progn
                    (setq end (+ end 1))
                    (setq idx (+ idx 1)))))
        (setq begin (min begin (- n 1)))
        (setq end (min end (- n 1)))
        (when (my-special-compare (aref str begin) (aref str (max 0 end)))
            (when (not (is-whitespace (aref str begin)))
                (setq answer (+ answer 1))))
        (return-from count-words-with-start-eq-end answer)))

(defun test (str)
    (print str)
    (print (count-words-with-start-eq-end str))
    (princ #\Newline))

(defvar a "а роза упала на лапу Азора")
(defvar b "     а роза упала на лапу Азора   ")
(defvar c " 0120 00 0 FAF Faf 3 14 15 ")
(test a)
(test b)
(test c)