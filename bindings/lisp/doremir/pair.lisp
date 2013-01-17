(in-package :doremir)
(define-foreign-type pair-type () () (:actual-type :pointer))
(define-parse-method pair () (make-instance 'pair-type))
(defclass pair () ((pair-ptr :initarg :pair-ptr)))
(defmethod translate-to-foreign (x (type pair-type)) (slot-value x 'pair-ptr))
(defmethod translate-from-foreign (x (type pair-type)) (make-instance 'pair :pair-ptr x))
(defcfun (pair-create "doremir_pair_create") pair (a ptr) (b ptr))
(defcfun (pair-copy "doremir_pair_copy") pair (a pair))
(defcfun (pair-destroy "doremir_pair_destroy") :void (a pair))
(defcfun (pair-fst "doremir_pair_fst") ptr (a pair))
(defcfun (pair-snd "doremir_pair_snd") ptr (a pair))
(defcfun (pair-dup "doremir_pair_dup") pair (a ptr))
(defcfun (pair-swap "doremir_pair_swap") pair (a pair))
(defcfun (pair-assoc "doremir_pair_assoc") pair (a pair))
(defcfun (pair-unassoc "doremir_pair_unassoc") pair (a pair))