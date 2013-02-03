(in-package :audio-engine)
(define-foreign-type atomic-type () () (:actual-type :pointer))
(define-parse-method atomic () (make-instance 'atomic-type))
(defclass atomic () ((atomic-ptr :initarg :atomic-ptr)))
(defmethod translate-to-foreign (x (type atomic-type)) (slot-value x 'atomic-ptr))
(defmethod translate-from-foreign (x (type atomic-type)) (make-instance 'atomic :atomic-ptr x))
(defcfun (atomic-create "doremir_atomic_create") atomic)
(defcfun (atomic-copy "doremir_atomic_copy") atomic (a atomic))
(defcfun (atomic-swap "doremir_atomic_swap") :void (a atomic) (b atomic))
(defcfun (atomic-destroy "doremir_atomic_destroy") :void (a atomic))
(defcfun (atomic-exchange "doremir_atomic_exchange") :boolean (a atomic) (b ptr) (c ptr))
(defcfun (atomic-get "doremir_atomic_get") ptr (a atomic))
(defcfun (atomic-set "doremir_atomic_set") :void (a atomic) (b ptr))
(defcfun (atomic-modify "doremir_atomic_modify") :void (a atomic) (b unary) (c ptr))