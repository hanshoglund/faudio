(in-package :doremir)
(define-foreign-type processor-audio-unit-type () () (:actual-type :pointer))
(define-parse-method processor-audio-unit () (make-instance 'processor-audio-unit-type))
(defclass processor-audio-unit () ((processor-audio-unit-ptr :initarg :processor-audio-unit-ptr)))
(defmethod translate-to-foreign (x (type processor-audio-unit-type)) (slot-value x 'processor-audio-unit-ptr))
(defmethod translate-from-foreign (x (type processor-audio-unit-type)) (make-instance 'processor-audio-unit :processor-audio-unit-ptr x))