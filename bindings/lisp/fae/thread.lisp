(in-package :cl-user)
(define-foreign-type thread-type () () (:actual-type :pointer))
(define-parse-method thread () (make-instance 'thread-type))
(defclass thread () ((thread-ptr :initarg :thread-ptr)))
(defmethod translate-to-foreign (x (type thread-type)) (slot-value x 'thread-ptr))
(defmethod translate-from-foreign (x (type thread-type)) (make-instance 'thread :thread-ptr x))
(define-foreign-type thread-mutex-type () () (:actual-type :pointer))
(define-parse-method thread-mutex () (make-instance 'thread-mutex-type))
(defclass thread-mutex () ((thread-mutex-ptr :initarg :thread-mutex-ptr)))
(defmethod translate-to-foreign (x (type thread-mutex-type)) (slot-value x 'thread-mutex-ptr))
(defmethod translate-from-foreign (x (type thread-mutex-type)) (make-instance 'thread-mutex :thread-mutex-ptr x))
(define-foreign-type thread-condition-type () () (:actual-type :pointer))
(define-parse-method thread-condition () (make-instance 'thread-condition-type))
(defclass thread-condition () ((thread-condition-ptr :initarg :thread-condition-ptr)))
(defmethod translate-to-foreign (x (type thread-condition-type)) (slot-value x 'thread-condition-ptr))
(defmethod translate-from-foreign (x (type thread-condition-type)) (make-instance 'thread-condition :thread-condition-ptr x))
(defctype thread-millis :int)
(defcfun (thread-create "fae_thread_create") thread (a nullary) (b ptr))
(defcfun (thread-sleep "fae_thread_sleep") :void (a thread-millis))
(defcfun (thread-join "fae_thread_join") :void (a thread))
(defcfun (thread-detach "fae_thread_detach") :void (a thread))
(defcfun (thread-main "fae_thread_main") thread)
(defcfun (thread-current "fae_thread_current") thread)
(defcfun (thread-create-mutex "fae_thread_create_mutex") thread-mutex)
(defcfun (thread-destroy-mutex "fae_thread_destroy_mutex") :void (a thread-mutex))
(defcfun (thread-lock "fae_thread_lock") :boolean (a thread-mutex))
(defcfun (thread-try-lock "fae_thread_try_lock") :boolean (a thread-mutex))
(defcfun (thread-unlock "fae_thread_unlock") :boolean (a thread-mutex))