(in-package :cl-user)
(define-foreign-type device-buffer-result-type () () (:actual-type :pointer))
(define-parse-method device-buffer-result () (make-instance 'device-buffer-result-type))
(defclass device-buffer-result () ((device-buffer-result-ptr :initarg :device-buffer-result-ptr)))
(defmethod translate-to-foreign (x (type device-buffer-result-type)) (slot-value x 'device-buffer-result-ptr))
(defmethod translate-from-foreign (x (type device-buffer-result-type)) (make-instance 'device-buffer-result :device-buffer-result-ptr x))
(defcfun (device-buffer-open "fae_device_buffer_open") device-buffer (a :int32))
(defcfun (device-buffer-close "fae_device_buffer_close") :void (a device-buffer))
(defcfun (device-buffer-run "fae_device_buffer_run") device-buffer-result (a device-buffer) (b processor) (c device-buffer))