(in-package :faudio)
(defctype ratio-num :int32)
(defctype ratio-denom :int32)
(define-foreign-type ratio-type () () (:actual-type :pointer))
(define-parse-method ratio () (make-instance 'ratio-type))
(defclass ratio () ((ratio-ptr :initarg :ratio-ptr)))
(defmethod translate-to-foreign (x (type ratio-type)) (slot-value x 'ratio-ptr))
(defmethod translate-from-foreign (x (type ratio-type)) (make-instance 'ratio :ratio-ptr x))
(defcfun (ratio-create "fa_ratio_create") ratio (a ratio-num) (b ratio-denom))
(defcfun (ratio-num "fa_ratio_num") ratio-num (a ratio))
(defcfun (ratio-denom "fa_ratio_denom") ratio-denom (a ratio))
(defcfun (ratio-match "fa_ratio_match") :void (a ratio) (b (:pointer ratio-num)) (c (:pointer ratio-denom)))
(defcfun (ratio-copy "fa_ratio_copy") ratio (a ratio))
(defcfun (ratio-destroy "fa_ratio_destroy") :void (a ratio))
(defcfun (ratio-add "fa_ratio_add") ratio (a ratio) (b ratio))
(defcfun (ratio-subtract "fa_ratio_subtract") ratio (a ratio) (b ratio))
(defcfun (ratio-multiply "fa_ratio_multiply") ratio (a ratio) (b ratio))
(defcfun (ratio-divide "fa_ratio_divide") ratio (a ratio) (b ratio))
(defcfun (ratio-succ "fa_ratio_succ") ratio (a ratio))
(defcfun (ratio-pred "fa_ratio_pred") ratio (a ratio))
(defcfun (ratio-negate "fa_ratio_negate") ratio (a ratio))
(defcfun (ratio-recip "fa_ratio_recip") ratio (a ratio))
(defcfun (ratio-absolute "fa_ratio_absolute") ratio (a ratio))
(defcfun (ratio-normalize "fa_ratio_normalize") ratio (a ratio))
(defcfun (ratio-to-mixed "fa_ratio_to_mixed") :void (a ratio) (b (:pointer ratio-num)) (c (:pointer ratio)))

