(in-package :audio-engine)
(define-foreign-type time-type () () (:actual-type :pointer))
(define-parse-method time () (make-instance 'time-type))
(defclass time () ((time-ptr :initarg :time-ptr)))
(defmethod translate-to-foreign (x (type time-type)) (slot-value x 'time-ptr))
(defmethod translate-from-foreign (x (type time-type)) (make-instance 'time :time-ptr x))
(defcfun (time-create "doremir_time_create") time (a :int32) (b :int32) (c :int32) (d ratio))
(defcfun (time-copy "doremir_time_copy") time (a time))
(defcfun (time-destroy "doremir_time_destroy") :void (a time))
(defcfun (time-days "doremir_time_days") :int32 (a time))
(defcfun (time-hours "doremir_time_hours") :int32 (a time))
(defcfun (time-minutes "doremir_time_minutes") :int32 (a time))
(defcfun (time-seconds "doremir_time_seconds") :int32 (a time))
(defcfun (time-divisions "doremir_time_divisions") ratio (a time))
(defcfun (time-to-iso "doremir_time_to_iso") string (a time))
(defcfun (time-to-seconds "doremir_time_to_seconds") :int32 (a time))
(defcfun (time-to-milliseconds "doremir_time_to_milliseconds") :int32 (a time))
(define-foreign-type time-system-type () () (:actual-type :pointer))
(define-parse-method time-system () (make-instance 'time-system-type))
(defclass time-system () ((time-system-ptr :initarg :time-system-ptr)))
(defmethod translate-to-foreign (x (type time-system-type)) (slot-value x 'time-system-ptr))
(defmethod translate-from-foreign (x (type time-system-type)) (make-instance 'time-system :time-system-ptr x))
(define-foreign-type time-cpu-type () () (:actual-type :pointer))
(define-parse-method time-cpu () (make-instance 'time-cpu-type))
(defclass time-cpu () ((time-cpu-ptr :initarg :time-cpu-ptr)))
(defmethod translate-to-foreign (x (type time-cpu-type)) (slot-value x 'time-cpu-ptr))
(defmethod translate-from-foreign (x (type time-cpu-type)) (make-instance 'time-cpu :time-cpu-ptr x))
(defcfun (time-from-system "doremir_time_from_system") time (a time-system))
(defcfun (time-from-cpu "doremir_time_from_cpu") time (a time-cpu))
(defcfun (time-system "doremir_time_system") time-system)
(defcfun (time-cpu "doremir_time_cpu") time-cpu)
(defctype time-clock-interface (:pointer :void))
(define-foreign-type time-clock-type () () (:actual-type :pointer))
(define-parse-method time-clock () (make-instance 'time-clock-type))
(defclass time-clock () ((time-clock-ptr :initarg :time-clock-ptr)))
(defmethod translate-to-foreign (x (type time-clock-type)) (slot-value x 'time-clock-ptr))
(defmethod translate-from-foreign (x (type time-clock-type)) (make-instance 'time-clock :time-clock-ptr x))
(defcfun (time-time "doremir_time_time") time (a time-clock))
(defcfun (time-tick-rate "doremir_time_tick_rate") :double (a time-clock))
(defcfun (time-ticks "doremir_time_ticks") :int64 (a time-clock))
(defcfun (time-get-system-clock "doremir_time_get_system_clock") time-clock)
(defcfun (time-get-system-prec-clock "doremir_time_get_system_prec_clock") time-clock)
(defcfun (time-get-cpu-clock "doremir_time_get_cpu_clock") time-clock)