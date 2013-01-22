(in-package :audio-engine)
; (defctype ptr (:pointer :void))
(defctype nullary (:pointer (:pointer :void)))
(defctype unary (:pointer (:pointer :void)))
(defctype binary (:pointer (:pointer :void)))
(defctype ternary (:pointer (:pointer :void)))
(defctype pred (:pointer (:pointer :void)))
(defctype char8 :char)
(defctype char16 :uint16)
(defctype char32 :uint32)
(defcfun (equal "doremir_equal") :boolean (a ptr) (b ptr))
(defcfun (not-equal "doremir_not_equal") :boolean (a ptr) (b ptr))
(defcfun (less-than "doremir_less_than") :boolean (a ptr) (b ptr))
(defcfun (greater-than "doremir_greater_than") :boolean (a ptr) (b ptr))
(defcfun (less-than-equal "doremir_less_than_equal") :boolean (a ptr) (b ptr))
(defcfun (greater-than-equal "doremir_greater_than_equal") :boolean (a ptr) (b ptr))
(defcfun (min "doremir_min") ptr (a ptr) (b ptr))
(defcfun (max "doremir_max") ptr (a ptr) (b ptr))
(defcfun (add "doremir_add") ptr (a ptr) (b ptr))
(defcfun (subtract "doremir_subtract") ptr (a ptr) (b ptr))
(defcfun (multiply "doremir_multiply") ptr (a ptr) (b ptr))
(defcfun (divide "doremir_divide") ptr (a ptr) (b ptr))
(defcfun (absolute "doremir_absolute") ptr (a ptr))
(defcfun (copy "doremir_copy") ptr (a ptr))
(defcfun (move "doremir_move") ptr (a ptr))
(defcfun (destroy "doremir_destroy") :void (a ptr))
(defcfun (print "doremir_print") :void (a (:pointer :char)) (b ptr))
(defcfun (dprint "doremir_dprint") :void (a (:pointer :char)) (b ptr))
(defcfun (print-ln "doremir_print_ln") :void (a ptr))
(defcfun (dprint-ln "doremir_dprint_ln") :void (a ptr))
(defctype equal (:pointer :void))
(defctype order (:pointer :void))
(defctype copy (:pointer :void))
(defctype destroy (:pointer :void))
(defctype number (:pointer :void))
(defcfun (type-str "doremir_type_str") (:pointer :char) (a ptr))
(defcfun (is-bool "doremir_is_bool") :boolean (a ptr))
(defcfun (is-int8 "doremir_is_int8") :boolean (a ptr))
(defcfun (is-int16 "doremir_is_int16") :boolean (a ptr))
(defcfun (is-int32 "doremir_is_int32") :boolean (a ptr))
(defcfun (is-int64 "doremir_is_int64") :boolean (a ptr))
(defcfun (is-float "doremir_is_float") :boolean (a ptr))
(defcfun (is-double "doremir_is_double") :boolean (a ptr))
(defcfun (is-ref "doremir_is_ref") :boolean (a ptr))
(defcfun (to-bool "doremir_to_bool") :boolean (a ptr))
(defcfun (to-int8 "doremir_to_int8") :int8 (a ptr))
(defcfun (to-int16 "doremir_to_int16") :int16 (a ptr))
(defcfun (to-int32 "doremir_to_int32") :int32 (a ptr))
(defcfun (to-int64 "doremir_to_int64") :int64 (a ptr))
(defcfun (to-float "doremir_to_float") :float (a ptr))
(defcfun (to-double "doremir_to_double") :double (a ptr))
(defcfun (peek-bool "doremir_peek_bool") :boolean (a ptr))
(defcfun (peek-int8 "doremir_peek_int8") :int8 (a ptr))
(defcfun (peek-int16 "doremir_peek_int16") :int16 (a ptr))
(defcfun (peek-int32 "doremir_peek_int32") :int32 (a ptr))
(defcfun (peek-int64 "doremir_peek_int64") :int64 (a ptr))
(defcfun (peek-float "doremir_peek_float") :float (a ptr))
(defcfun (peek-double "doremir_peek_double") :double (a ptr))
(defcfun (from-bool "doremir_from_bool") ptr (a :boolean))
(defcfun (from-int8 "doremir_from_int8") ptr (a :int8))
(defcfun (from-int16 "doremir_from_int16") ptr (a :int16))
(defcfun (from-int32 "doremir_from_int32") ptr (a :int32))
(defcfun (from-int64 "doremir_from_int64") ptr (a :int64))
(defcfun (from-float "doremir_from_float") ptr (a :float))
(defcfun (from-double "doremir_from_double") ptr (a :double))
(defctype id :int64)
(defctype impl (:pointer (:pointer :void)))
(defcfun (interface "doremir_interface") ptr (a id) (b ptr))