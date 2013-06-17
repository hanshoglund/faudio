(in-package :cl-user)
(define-foreign-type processor-vst-type () () (:actual-type :pointer))
(define-parse-method processor-vst () (make-instance 'processor-vst-type))
(defclass processor-vst () ((processor-vst-ptr :initarg :processor-vst-ptr)))
(defmethod translate-to-foreign (x (type processor-vst-type)) (slot-value x 'processor-vst-ptr))
(defmethod translate-from-foreign (x (type processor-vst-type)) (make-instance 'processor-vst :processor-vst-ptr x))