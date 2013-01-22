(in-package :audio-engine)
(defctype processor-info (:pointer :void))
(defctype processor-samples ptr)
(defctype processor-interface (:pointer :void))
(define-foreign-type processor-type () () (:actual-type :pointer))
(define-parse-method processor () (make-instance 'processor-type))
(defclass processor () ((processor-ptr :initarg :processor-ptr)))
(defmethod translate-to-foreign (x (type processor-type)) (slot-value x 'processor-ptr))
(defmethod translate-from-foreign (x (type processor-type)) (make-instance 'processor :processor-ptr x))
(defcfun (processor-input-type "doremir_processor_input_type") type (a processor))
(defcfun (processor-output-type "doremir_processor_output_type") type (a processor))
(defcfun (processor-identity "doremir_processor_identity") processor (a type))
(defcfun (processor-constant "doremir_processor_constant") processor (a type) (b type) (c ptr))
(defcfun (processor-split "doremir_processor_split") processor (a type))
(defcfun (processor-seq "doremir_processor_seq") processor (a processor) (b processor))
(defcfun (processor-par "doremir_processor_par") processor (a processor) (b processor))
(defcfun (processor-loop "doremir_processor_loop") processor (a processor))
(defcfun (processor-unary "doremir_processor_unary") processor (a type) (b type) (c unary) (d ptr))
(defcfun (processor-binary "doremir_processor_binary") processor (a type) (b type) (c type) (d binary) (e ptr))
(defcfun (processor-delay "doremir_processor_delay") processor (a type) (b :int32))
(defcfun (processor-add "doremir_processor_add") processor (a type))
(defcfun (processor-subtract "doremir_processor_subtract") processor (a type))
(defcfun (processor-multiply "doremir_processor_multiply") processor (a type))
(defcfun (processor-power "doremir_processor_power") processor (a type))
(defcfun (processor-divide "doremir_processor_divide") processor (a type))
(defcfun (processor-modulo "doremir_processor_modulo") processor (a type))
(defcfun (processor-absolute "doremir_processor_absolute") processor (a type))
(defcfun (processor-not "doremir_processor_not") processor (a type))
(defcfun (processor-and "doremir_processor_and") processor (a type))
(defcfun (processor-or "doremir_processor_or") processor (a type))
(defcfun (processor-xor "doremir_processor_xor") processor (a type))
(defcfun (processor-bit-not "doremir_processor_bit_not") processor (a type))
(defcfun (processor-bit-and "doremir_processor_bit_and") processor (a type))
(defcfun (processor-bit-or "doremir_processor_bit_or") processor (a type))
(defcfun (processor-bit-xor "doremir_processor_bit_xor") processor (a type))
(defcfun (processor-shift-left "doremir_processor_shift_left") processor (a type))
(defcfun (processor-shift-right "doremir_processor_shift_right") processor (a type))
(defcfun (processor-equal "doremir_processor_equal") processor (a type))
(defcfun (processor-less-than "doremir_processor_less_than") processor (a type))
(defcfun (processor-greater-than "doremir_processor_greater_than") processor (a type))
(defcfun (processor-less-than-equal "doremir_processor_less_than_equal") processor (a type))
(defcfun (processor-greater-than-equal "doremir_processor_greater_than_equal") processor (a type))
(defcfun (processor-acos "doremir_processor_acos") processor (a type))
(defcfun (processor-asin "doremir_processor_asin") processor (a type))
(defcfun (processor-atan "doremir_processor_atan") processor (a type))
(defcfun (processor-cos "doremir_processor_cos") processor (a type))
(defcfun (processor-sin "doremir_processor_sin") processor (a type))
(defcfun (processor-tan "doremir_processor_tan") processor (a type))
(defcfun (processor-exp "doremir_processor_exp") processor (a type))
(defcfun (processor-log "doremir_processor_log") processor (a type))
(defcfun (processor-log10 "doremir_processor_log10") processor (a type))
(defcfun (processor-pow "doremir_processor_pow") processor (a type))
(defcfun (processor-sqrt "doremir_processor_sqrt") processor (a type))
(defcfun (processor-abs "doremir_processor_abs") processor (a type))
(defcfun (processor-min "doremir_processor_min") processor (a type))
(defcfun (processor-max "doremir_processor_max") processor (a type))
(defcfun (processor-fmod "doremir_processor_fmod") processor (a type))
(defcfun (processor-remainder "doremir_processor_remainder") processor (a type))
(defcfun (processor-floor "doremir_processor_floor") processor (a type))
(defcfun (processor-ceil "doremir_processor_ceil") processor (a type))
(defcfun (processor-rint "doremir_processor_rint") processor (a type))