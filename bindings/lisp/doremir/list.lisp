(in-package :doremir)
(define-foreign-type list-type () () (:actual-type :pointer))
(define-parse-method list () (make-instance 'list-type))
(defclass list () ((list-ptr :initarg :list-ptr)))
(defmethod translate-to-foreign (x (type list-type)) (slot-value x 'list-ptr))
(defmethod translate-from-foreign (x (type list-type)) (make-instance 'list :list-ptr x))
(defcfun (list-empty "doremir_list_empty") list)
(defcfun (list-single "doremir_list_single") list (a ptr))
(defcfun (list-cons "doremir_list_cons") list (a ptr) (b list))
(defcfun (list-dcons "doremir_list_dcons") list (a ptr) (b list))
(defcfun (list-repeat "doremir_list_repeat") list (a :int) (b ptr))
(defcfun (list-enumerate "doremir_list_enumerate") list (a :int) (b :int))
(defcfun (list-copy "doremir_list_copy") list (a list))
(defcfun (list-destroy "doremir_list_destroy") :void (a list))
(defcfun (list-is-empty "doremir_list_is_empty") :boolean (a list))
(defcfun (list-is-single "doremir_list_is_single") :boolean (a list))
(defcfun (list-length "doremir_list_length") :int (a list))
(defcfun (list-head "doremir_list_head") ptr (a list))
(defcfun (list-tail "doremir_list_tail") list (a list))
(defcfun (list-dtail "doremir_list_dtail") list (a list))
(defcfun (list-init "doremir_list_init") list (a list))
(defcfun (list-dinit "doremir_list_dinit") list (a list))
(defcfun (list-last "doremir_list_last") ptr (a list))
(defcfun (list-append "doremir_list_append") list (a list) (b list))
(defcfun (list-dappend "doremir_list_dappend") list (a list) (b list))
(defcfun (list-reverse "doremir_list_reverse") list (a list))
(defcfun (list-dreverse "doremir_list_dreverse") list (a list))
(defcfun (list-sort "doremir_list_sort") list (a list))
(defcfun (list-dsort "doremir_list_dsort") list (a list))
(defcfun (list-take "doremir_list_take") list (a :int) (b list))
(defcfun (list-dtake "doremir_list_dtake") list (a :int) (b list))
(defcfun (list-drop "doremir_list_drop") list (a :int) (b list))
(defcfun (list-ddrop "doremir_list_ddrop") list (a :int) (b list))
(defcfun (list-index "doremir_list_index") ptr (a :int) (b list))
(defcfun (list-range "doremir_list_range") list (a :int) (b :int) (c list))
(defcfun (list-insert "doremir_list_insert") list (a :int) (b ptr) (c list))
(defcfun (list-dinsert "doremir_list_dinsert") list (a :int) (b ptr) (c list))
(defcfun (list-insert-range "doremir_list_insert_range") list (a :int) (b list) (c list))
(defcfun (list-dinsert-range "doremir_list_dinsert_range") list (a :int) (b list) (c list))
(defcfun (list-remove "doremir_list_remove") list (a :int) (b list))
(defcfun (list-dremove "doremir_list_dremove") list (a :int) (b list))
(defcfun (list-remove-range "doremir_list_remove_range") list (a :int) (b :int) (c list))
(defcfun (list-dremove-range "doremir_list_dremove_range") list (a :int) (b :int) (c list))
(defcfun (list-has "doremir_list_has") :boolean (a ptr) (b list))
(defcfun (list-find "doremir_list_find") ptr (a pred) (b ptr) (c list))
(defcfun (list-index-of "doremir_list_index_of") :int (a ptr) (b list))
(defcfun (list-find-index "doremir_list_find_index") :int (a pred) (b ptr) (c list))
(defcfun (list-filter "doremir_list_filter") list (a pred) (b ptr) (c list))
(defcfun (list-dfilter "doremir_list_dfilter") list (a pred) (b ptr) (c list))
(defcfun (list-map "doremir_list_map") list (a unary) (b ptr) (c list))
(defcfun (list-dmap "doremir_list_dmap") list (a unary) (b ptr) (c list))
(defcfun (list-concat-map "doremir_list_concat_map") list (a unary) (b ptr) (c list))
(defcfun (list-dconcat-map "doremir_list_dconcat_map") list (a unary) (b ptr) (c list))
(defcfun (list-fold-left "doremir_list_fold_left") ptr (a binary) (b ptr) (c ptr) (d list))
(defcfun (list-dfold-left "doremir_list_dfold_left") ptr (a binary) (b ptr) (c ptr) (d list))
(defcfun (list-concat "doremir_list_concat") list (a list))
(defcfun (list-dconcat "doremir_list_dconcat") list (a list))
(defcfun (list-to-list "doremir_list_to_list") list (a list))