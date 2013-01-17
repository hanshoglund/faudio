(in-package :doremir)
(defctype type-frames :int32)
(defctype type-simple :int)
(define-foreign-type type-type () () (:actual-type :pointer))
(define-parse-method type () (make-instance 'type-type))
(defclass type () ((type-ptr :initarg :type-ptr)))
(defmethod translate-to-foreign (x (type type-type)) (slot-value x 'type-ptr))
(defmethod translate-from-foreign (x (type type-type)) (make-instance 'type :type-ptr x))
(defcfun (type-simple "doremir_type_simple") type (a type-simple))
(defcfun (type-pair "doremir_type_pair") type (a type) (b type))
(defcfun (type-vector "doremir_type_vector") type (a type) (b :int32))
(defcfun (type-frame "doremir_type_frame") type (a type))
(defcfun (type-copy "doremir_type_copy") type (a type))
(defcfun (type-destroy "doremir_type_destroy") :void (a type))
(defcfun (type-is-simple "doremir_type_is_simple") :boolean (a type))
(defcfun (type-is-pair "doremir_type_is_pair") :boolean (a type))
(defcfun (type-is-vector "doremir_type_is_vector") :boolean (a type))
(defcfun (type-is-frame "doremir_type_is_frame") :boolean (a type))
(defcfun (type-size-of "doremir_type_size_of") :int32 (a type-frames) (b type))
(defcfun (type-align-of "doremir_type_align_of") :int32 (a type))