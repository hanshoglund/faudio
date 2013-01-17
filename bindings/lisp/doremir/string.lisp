(in-package :doremir)
(defctype string-utf8 (:pointer char8))
(defctype string-utf16 (:pointer char16))
(defctype string-utf32 (:pointer char32))
(define-foreign-type string-type () () (:actual-type :pointer))
(define-parse-method string () (make-instance 'string-type))
(defclass string () ((string-ptr :initarg :string-ptr)))
(defmethod translate-to-foreign (x (type string-type)) (slot-value x 'string-ptr))
(defmethod translate-from-foreign (x (type string-type)) (make-instance 'string :string-ptr x))
(defctype string-file-path string)
(defcfun (string-empty "doremir_string_empty") string)
(defcfun (string-single "doremir_string_single") string (a char16))
(defcfun (string-copy "doremir_string_copy") string (a string))
(defcfun (string-append "doremir_string_append") string (a string) (b string))
(defcfun (string-dappend "doremir_string_dappend") string (a string) (b string))
(defcfun (string-destroy "doremir_string_destroy") :void (a string))
(defcfun (string-length "doremir_string_length") :int (a string))
(defcfun (string-char-at "doremir_string_char_at") char16 (a :int) (b string))
(defcfun (string-to-utf8 "doremir_string_to_utf8") string-utf8 (a string))
(defcfun (string-to-utf16 "doremir_string_to_utf16") string-utf16 (a string))
(defcfun (string-to-utf32 "doremir_string_to_utf32") string-utf32 (a string))
(defcfun (string-to-cf-string "doremir_string_to_cf_string") (:pointer :void) (a string))
(defcfun (string-from-utf8 "doremir_string_from_utf8") string (a string-utf8))
(defcfun (string-from-utf16 "doremir_string_from_utf16") string (a string-utf16))
(defcfun (string-from-utf32 "doremir_string_from_utf32") string (a string-utf32))
(defcfun (string-from-cf-string "doremir_string_from_cf_string") string (a (:pointer :void)))
(defcfun (string-format-integer "doremir_string_format_integer") string (a (:pointer :char)) (b :long))
(defctype string-show (:pointer :void))
(defctype string-to-json (:pointer :void))
(defctype string-from-json (:pointer :void))
(defctype string-severity :int)
(defctype string-error (:pointer :void))
(defcfun (string-check "doremir_string_check") :boolean (a ptr))
(defcfun (string-show "doremir_string_show") string (a ptr))