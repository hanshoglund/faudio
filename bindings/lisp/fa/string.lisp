(in-package :faudio)
(defctype string-utf8 (:pointer char8))
(defctype string-utf16 (:pointer char16))
(defctype string-utf32 (:pointer char32))
(define-foreign-type string-type () () (:actual-type :pointer))
(define-parse-method string () (make-instance 'string-type))
(defclass string () ((string-ptr :initarg :string-ptr)))
(defmethod translate-to-foreign (x (type string-type)) (slot-value x 'string-ptr))
(defmethod translate-from-foreign (x (type string-type)) (make-instance 'string :string-ptr x))
(defcfun (string-empty "fa_string_empty") string)
(defcfun (string-single "fa_string_single") string (a char16))
(defcfun (string-repeat "fa_string_repeat") string (a :int) (b char16))
(defcfun (string-copy "fa_string_copy") string (a string))
(defcfun (string-append "fa_string_append") string (a string) (b string))
(defcfun (string-dappend "fa_string_dappend") string (a string) (b string))
(defcfun (string-destroy "fa_string_destroy") :void (a string))
(defcfun (string-length "fa_string_length") :int (a string))
(defcfun (string-char-at "fa_string_char_at") char16 (a :int) (b string))
(defctype string-show (:pointer :void))
(defcfun (string-show "fa_string_show") string (a ptr))
(defcfun (string-to-string "fa_string_to_string") string (a ptr))
(defcfun (string-to-json "fa_string_to_json") string (a ptr))
(defcfun (string-from-json "fa_string_from_json") ptr (a string))
(defcfun (string-to-utf8 "fa_string_to_utf8") string-utf8 (a string))
(defcfun (string-to-utf16 "fa_string_to_utf16") string-utf16 (a string))
(defcfun (string-to-native "fa_string_to_native") ptr (a string))
(defcfun (string-from-utf8 "fa_string_from_utf8") string (a string-utf8))
(defcfun (string-from-utf16 "fa_string_from_utf16") string (a string-utf16))
(defcfun (string-from-native "fa_string_from_native") string (a ptr))
(defcfun (string-matches "fa_string_matches") :boolean (a string) (b string))
(defcfun (string-format-integral "fa_string_format_integral") string (a (:pointer :char)) (b :long))
(defcfun (string-format-floating "fa_string_format_floating") string (a (:pointer :char)) (b :double))

