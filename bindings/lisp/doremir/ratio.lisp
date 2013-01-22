(in-package :audio-engine)
(defctype ratio-num :int32)
(defctype ratio-denom :int32)
(define-foreign-type ratio-type () () (:actual-type :pointer))
(define-parse-method ratio () (make-instance 'ratio-type))
(defclass ratio () ((ratio-ptr :initarg :ratio-ptr)))
(defmethod translate-to-foreign (x (type ratio-type)) (slot-value x 'ratio-ptr))
(defmethod translate-from-foreign (x (type ratio-type)) (make-instance 'ratio :ratio-ptr x))
(defcfun (ratio-create "doremir_ratio_create") ratio (a ratio-num) (b ratio-denom))
(defcfun (ratio-num "doremir_ratio_num") ratio-num (a ratio))
(defcfun (ratio-denom "doremir_ratio_denom") ratio-denom (a ratio))
(defcfun (ratio-match "doremir_ratio_match") :void (a ratio) (b (:pointer ratio-num)) (c (:pointer ratio-denom)))
(defcfun (ratio-destroy "doremir_ratio_destroy") :void (a ratio))
(defcfun (ratio-add "doremir_ratio_add") ratio (a ratio) (b ratio))
(defcfun (ratio-subtract "doremir_ratio_subtract") ratio (a ratio) (b ratio))
(defcfun (ratio-multiply "doremir_ratio_multiply") ratio (a ratio) (b ratio))
(defcfun (ratio-divide "doremir_ratio_divide") ratio (a ratio) (b ratio))
(defcfun (ratio-remainder "doremir_ratio_remainder") ratio (a ratio) (b ratio))
(defcfun (ratio-succ "doremir_ratio_succ") ratio (a ratio))
(defcfun (ratio-pred "doremir_ratio_pred") ratio (a ratio))
(defcfun (ratio-negate "doremir_ratio_negate") ratio (a ratio))
(defcfun (ratio-recip "doremir_ratio_recip") ratio (a ratio))