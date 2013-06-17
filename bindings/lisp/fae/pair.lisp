(in-package :cl-user)
(define-foreign-type pair-type () () (:actual-type :pointer))
(define-parse-method pair () (make-instance 'pair-type))
(defclass pair () ((pair-ptr :initarg :pair-ptr)))
(defmethod translate-to-foreign (x (type pair-type)) (slot-value x 'pair-ptr))
(defmethod translate-from-foreign (x (type pair-type)) (make-instance 'pair :pair-ptr x))
(defcfun (pair-create "fae_pair_create") pair (a ptr) (b ptr))
(defcfun (pair-copy "fae_pair_copy") pair (a pair))
(defcfun (pair-destroy "fae_pair_destroy") :void (a pair))
(defcfun (pair-fst "fae_pair_fst") ptr (a pair))
(defcfun (pair-snd "fae_pair_snd") ptr (a pair))
(defcfun (pair-dup "fae_pair_dup") pair (a ptr))
(defcfun (pair-swap "fae_pair_swap") pair (a pair))
(defcfun (pair-assoc "fae_pair_assoc") pair (a pair))
(defcfun (pair-unassoc "fae_pair_unassoc") pair (a pair))
(defcfun (pair-to-list "fae_pair_to_list") list (a pair))