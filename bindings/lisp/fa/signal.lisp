(in-package :faudio)
(define-foreign-type signal-type () () (:actual-type :pointer))
(define-parse-method signal () (make-instance 'signal-type))
(defclass signal () ((signal-ptr :initarg :signal-ptr)))
(defmethod translate-to-foreign (x (type signal-type)) (slot-value x 'signal-ptr))
(defmethod translate-from-foreign (x (type signal-type)) (make-instance 'signal :signal-ptr x))
(defctype signal-unary-signal (:pointer (:pointer :void)))
(defctype signal-unary-double (:pointer (:pointer :void)))
(defctype signal-binary-double (:pointer (:pointer :void)))
(defcfun (signal-time "fa_signal_time") signal)
(defcfun (signal-random "fa_signal_random") signal)
(defcfun (signal-constant "fa_signal_constant") signal (a :double))
(defcfun (signal-lift "fa_signal_lift") signal (a string) (b signal-unary-double) (c ptr) (d signal))
(defcfun (signal-lift2 "fa_signal_lift2") signal (a string) (b signal-binary-double) (c ptr) (d signal) (e signal))
(defcfun (signal-loop "fa_signal_loop") signal (a signal-unary-signal) (b ptr))
(defcfun (signal-delay "fa_signal_delay") signal (a :int) (b signal))
(defcfun (signal-input "fa_signal_input") signal (a :int))
(defcfun (signal-output "fa_signal_output") signal (a :int) (b :int) (c signal))
(defcfun (signal-latter "fa_signal_latter") signal (a signal) (b signal))
(defcfun (signal-former "fa_signal_former") signal (a signal) (b signal))
(defcfun (signal-print "fa_signal_print") :void (a :int) (b signal))
(defcfun (signal-run "fa_signal_run") :void (a :int) (b signal) (c (:pointer :double)))
(defcfun (signal-run-buffer "fa_signal_run_buffer") buffer (a :int) (b signal))
(defcfun (signal-run-file "fa_signal_run_file") ptr (a :int) (b signal) (c string))
(defcfun (signal-add "fa_signal_add") signal (a signal) (b signal))
(defcfun (signal-multiply "fa_signal_multiply") signal (a signal) (b signal))
(defcfun (signal-cos "fa_signal_cos") signal (a signal))
(defcfun (signal-sin "fa_signal_sin") signal (a signal))
(defcfun (signal-to-tree "fa_signal_to_tree") pair (a signal))
(defcfun (signal-draw-tree "fa_signal_draw_tree") string (a pair))
(defcfun (signal-simplify "fa_signal_simplify") signal (a signal))
(defcfun (signal-impulse "fa_signal_impulse") signal)
(defcfun (signal-line "fa_signal_line") signal (a :double))
(defcfun (signal-low-pass "fa_signal_low_pass") signal (a signal) (b signal) (c signal) (d signal) (e signal))
(defcfun (signal-biquad "fa_signal_biquad") signal (a signal) (b signal) (c signal) (d signal) (e signal) (f signal))

