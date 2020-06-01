(defclass cart ()                
	((x :initarg :x :reader cart-x)
	 (y :initarg :y :reader cart-y)))

(defmethod print-object ((c cart) stream)
	(format stream "[CART x ~d y ~d]"
		(cart-x c) (cart-y c)))

(defclass polar ()
	((radius :initarg :radius :accessor radius)
		(angle  :initarg :angle  :accessor angle)))

(defmethod print-object ((p polar) stream)
	(format stream "[POLAR radius ~d angle ~d]"
		(radius p) (angle p)))

(defmethod radius ((c cart))
  (sqrt (+ (* (cart-x c) (cart-x c))
           (* (cart-y c) (cart-y c)))))

(defmethod angle ((c cart))
  (atan (cart-y c) (cart-x c)))

(defmethod cart-x ((p polar))
	(* (cos (angle p)) (radius p)))

(defmethod cart-y ((p polar))
	(* (sin (angle p)) (radius p)))

(defgeneric to-cart (arg)
	(:method ((c cart)) c)
	(:method ((p polar))
		(make-instance 'cart :x (cart-x p) :y (cart-y p))))

(defmethod add ((c1 cart) (c2 cart))
  	(make-instance 'cart 
		:x (+ (cart-x c1) (cart-x c2))
        :y (+ (cart-y c1) (cart-y c2))))

(defmethod add ((p1 polar) (p2 polar))
  	(make-instance 'cart 
		:x (+ (cart-x p1) (cart-x p2))
        :y (+ (cart-y p1) (cart-y p2))))

(defmethod make-half ((c1 cart))
	(make-instance 'cart 
		:x (/ (cart-x c1) 2)
		:y (/ (cart-y c1) 2)))

(defclass line ()
	((start :initarg :start :accessor line-start)
	 (end   :initarg :end   :accessor line-end)))

(defmethod print-object ((lin line) stream)
	(format stream "[ОТРЕЗОК ~s ~s]"
		(line-start lin) (line-end lin)))

(defclass triangle ()
	((vertex1 :initarg :1 :reader vertex1)
	 (vertex2 :initarg :2 :reader vertex2)
	 (vertex3 :initarg :3 :reader vertex3)))

(defmethod print-object ((tri triangle) stream)
	(format stream "[ТРЕУГ ~s ~s ~s]"
		(vertex1 tri) (vertex2 tri) (vertex3 tri)))

(defun median (tri)
	(make-instance 'line
           :start (to-cart (vertex1 tri))
           :end (make-half (add (vertex2 tri) (vertex3 tri)))))

(defvar triangleCartesian1 (make-instance 'triangle
           :1 (make-instance 'cart :x 0 :y 5)
           :2 (make-instance 'cart :x 0 :y 0)
           :3 (make-instance 'cart :x 4 :y 0)))
(print (median triangleCartesian1))

(defvar trianglePolar1 (make-instance 'triangle
           :1 (make-instance 'polar :radius (radius (vertex1 triangleCartesian1)) :angle (angle (vertex1 triangleCartesian1)))
           :2 (make-instance 'polar :radius (radius (vertex2 triangleCartesian1)) :angle (angle (vertex2 triangleCartesian1)))
           :3 (make-instance 'polar :radius (radius (vertex3 triangleCartesian1)) :angle (angle (vertex3 triangleCartesian1)))))
(print (median trianglePolar1))
(princ #\Newline)