(in-package :cl-user)
(define-foreign-type device-file-result-type () () (:actual-type :pointer))
(define-parse-method device-file-result () (make-instance 'device-file-result-type))
(defclass device-file-result () ((device-file-result-ptr :initarg :device-file-result-ptr)))
(defmethod translate-to-foreign (x (type device-file-result-type)) (slot-value x 'device-file-result-ptr))
(defmethod translate-from-foreign (x (type device-file-result-type)) (make-instance 'device-file-result :device-file-result-ptr x))
(defcfun (device-file-open "fae_device_file_open") device-file (a string-file-path))
(defcfun (device-file-close "fae_device_file_close") :void (a device-file))
(defcfun (device-file-run "fae_device_file_run") device-file-result (a device-file) (b processor) (c device-file))