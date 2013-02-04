
(defvar *foreign-lib*)

; ---------------------------------------------------------------------------------------------------

(progn
  (asdf:load-system :audio-engine))

(progn
  (push "/Users/hans/audio/build/Frameworks/" cffi:*darwin-framework-directories*)
  (setf *foreign-lib* (cffi:load-foreign-library '(:framework "DoReMIRAudio")))  
    (audioengine-set-log-file "/Users/hans/Library/Logs/DoReMIRAudio.log")
    (audioengine-initialize)
    (plot-use-gnu))

; (close-foreign-library *foreign-lib*)

; ---------------------------------------------------------------------------------------------------

; Caveats:
;   - Calling a function on a destroyed object is undefined
;   - Calling (from-pointer) with the wrong type is undefined
;   - Calling a generic function on a type that does not implement a required interface is undefined

(in-package :audio-engine)


; For testing

(defvar x nil)
(defvar y nil)
(defvar z nil)
(defvar s nil)
(defvar d nil)
(defvar p nil)   

(setf p (id '(:frame (:f32 :f32 :f32 :f32 :f32 :f32 :f32 :f32 :f32 . :f32))))
(processor-buffer-size 256 p)


; ---------------------------------------------------------------------------------------------------

; AudioEngine

(audioengine-initialize)
(audioengine-terminate)
(audioengine-set-log-file "/Users/hans/Library/Logs/DoReMIRAudio.log")
(audioengine-set-log-std)
(audioengine-log-info "What to say?")
(audioengine-log-warning "Beware!")
(audioengine-log-error "Rattlesnakes!")
(audioengine-log-error-from "Rattlesnakes!" "Test.Lisp")


; ---------------------------------------------------------------------------------------------------

; Ratio

; Audio Engine ratios are converted to Lisp ratios and vice versa

(setf x (ratio-create 1 2))
(setf y (ratio-create 278 12))
(ratio-num x)
(ratio-denom x)
(ratio-add x y)
(ratio-subtract x y)
(ratio-multiply x y)
(ratio-divide x y)
;(ratio-remainder x y)
(ratio-succ x)
(ratio-pred x)
(ratio-negate x)
(ratio-recip x)
(cl:print x)
(ratio-destroy x)
(ratio-create 1 2)
(ratio-succ (/ 1 2))
(ratio-recip (/ 567 235))

; ---------------------------------------------------------------------------------------------------

; String

; Audio Engine strings are converted to Lisp strings and vice versa

(setf x (string-empty))
(setf x (string-single 104))
(string-append (string-single 104) (string-dappend (string-single (char-int #\a)) (string-single 110)))
(string-append "hans " "h�glund")
(string-length "h�gtalare")
(cl:print x)
;(string-destroy x)

; ---------------------------------------------------------------------------------------------------
; Pair

; Audio Engine pairs are NOT Lisp pairs
; They print as (1,2)

(setf x (pair-create 1 2))
(setf y (pair-copy x))
(pair-fst x)
(pair-snd x)
(pair-dup 3)
(pair-swap x)
(pair-assoc (pair-create 1 (pair-create 2 3)))
(pair-unassoc (pair-create (pair-create 1 2) 3))
(cl:print x)
(pair-destroy x)
(destroy x)

(pair-snd (from-pointer 'pair (to-pointer (pair-create 1 2))))
(pair-create (pair-create 1 2) (pair-create 3 4))
(pair-create (list-single 1) (set-single 2))

; ---------------------------------------------------------------------------------------------------

; List

; Audio Engine lists are NOT Lisp lists
; They print as [1,2,3..]

(setf x (list-empty))
(setf x (list-single 0))
(setf x (list-cons 1 x))
(setf x (list-cons (random 20) x))
(setf x (list-tail x))
(setf y (list-copy x))
(list-is-empty x)
(list-is-single x)
(list-length x)
(list-head x)
(list-tail x)
(list-init x)
(list-last x)
(list-append x x)
(list-reverse x)
(list-sort x)
(list-take 5 x)
(list-drop 5 x)
(list-index 2 x)
(list-range 2 3 x)
(list-insert 2 23 x)
(list-remove 2 x)
(list-insert-range 2 (list-single 56) x)
(list-remove-range 2 3 x)
(list-has 1 x)
; (list-find (lambda (x) t) x)
(list-index-of 1 x)
; (list-find-index (lambda (x) t) x)
; (list-filter (lambda (x) t) x)
; (list-map (lambda (x) (+ 1 x)) x)
; (list-fold-left (lambda (x y) (+ x y) nil 0 x)


(list-concat (list-empty))
(list-concat (list-single (list-single 1)))

(list-single (list-single 1))

(cl:print x)
(list-destroy x)

; (export-list# (cl:list 1 2 (export-list# (cl:list 1 3 4))))
; (import-list# (list-cons 1 (list-cons 2 (list-single 3))))

; ---------------------------------------------------------------------------------------------------

; Set

(setf x (set-empty))
(setf x (set-single 1))
(setf x (set-add (random 20) x))
(setf x (set-remove (random 20) x))
(setf y (set-copy x))
(set-size x)
(set-is-empty x)
(set-is-single x)
(set-has 1 x)
(set-get 1 x)
(set-is-subset-of y x)
(set-is-proper-subset-of y x)
(set-sum x y)
(set-intersection x y)
(set-product x y)
(set-difference x y)
; (set-power x)
; (set-from-list (list-empty))
(set-to-list x)
(cl:print x)
(set-destroy)

; ---------------------------------------------------------------------------------------------------

; Map

(setf x (map-empty))
(setf x (map-add "name" "hans" x))
(setf x (map-add "name" "sven" x))
(setf x (map-set "name" "sven" x))
(setf x (map-add "skills" (list-cons 1 (list-empty)) x))
(setf x (map-remove "name" x))
(setf x (map-remove "skills" x))
; (setf x (map-remove (random 20) x))
(setf y (map-copy x))
(map-size x)
(map-is-empty x)
(map-is-single x)
(from-pointer 'string (map-get "name" x))
(from-pointer 'list (map-get "skills" x))
(setf x (map-add-entry (pair-create "surname" "h�glund") x))
(setf x (map-remove-entry (pair-create "surname" "h�glund") x))
; (map-has-key "name" x)
; (map-has-elem "hans" x)
; (map-has-entry (pair-create "surname" "h�glund") x)
(map-is-submap-of x y)
(map-is-proper-submap-of x y)
(map-sum x y)
(map-product x y)
(map-difference x y)
(map-power x)
; (map-from-pair (pair-create 1 2))
; (map-from-list (list-single 1))
(map-to-list x)
(cl:print x)
(map-destroy x)

; ---------------------------------------------------------------------------------------------------

; Buffer

(setf x (buffer-create 1024))
(setf x (buffer-resize 2048 x))
(buffer-size x)
(buffer-peek x 1)
(buffer-poke x 1 10)
(dotimes (i 1024)
  (buffer-poke x i (mod i 256)))
(dotimes (i 1024)
  (buffer-poke x i 0))
(cl:print x)
(buffer-destroy x)


(setf x (buffer-read-audio "/Users/hans/Desktop/test.wav"))
(setf x (buffer-read-audio "/Users/hans/Desktop/Passager.wav"))
(error-check x)
(error-message (to-error x))
(error-severity (to-error x))
(error-origin (to-error x))
(error-log nil (to-error x))

(defun safe-buffer-read-audio (path)
  (setf res (buffer-read-audio path))
  (when (error-check x)
    (error "error"))
  res)


; TODO move below
(setf tp (from-pointer 'type (pair-fst x)))
(setf x (from-pointer 'buffer (pair-snd x)))

(plot-buffer-double x nil nil)
(plot-buffer-float x nil nil)

; ---------------------------------------------------------------------------------------------------

; Midi

(setf x (midi-create-simple #x9 60 127))
(setf x (midi-create-sysex (buffer-create 1024)))
(setf y (midi-copy x))
(midi-is-simple x)
(midi-is-sysex x)
(midi-status x)
(midi-channel x)
(midi-simple-data x)
(pair-fst (midi-simple-data x))
(midi-sysex-data x)
(midi-destroy x)

; ---------------------------------------------------------------------------------------------------

; Time

; TODO macro such as (make-time 2 :hours 3 :minutes)

(setf x (time-create 0 0 4 (rational 33.5))) ; days hours minutes seconds divs
(setf y (time-copy x))
(setf x (from-pointer 'time (add x y)))
(time-days x)
(time-hours x)
(time-minutes x)
(time-seconds x)
(time-divisions x)
(time-to-iso x)
(equal x y)
(destroy x)

; ---------------------------------------------------------------------------------------------------

; Types

; These expressions auto-convert to types
(setf x nil)
(setf x :unit)
(setf x :i8)
(setf x :i16)
(setf x :i32)
(setf x :i64)
(setf x :f32)
(setf x :f64)
(setf x :ptr)
(setf x '(:pair :f32 :i32))
(setf x '(:f32 . :f32))
(setf x '((:f32 . :f32) . (:f32 . :f32)))
(setf x '(:frame :f32))
(setf x '(:vector (:pair :i8 :i32) 24))
(setf x '(:vector :f32 1024))
(type-is-simple x)
(type-is-pair x)
(type-is-vector x)
(type-is-frame x)
(type-channels x)
(type-size-of 256 x)
(type-offset-of 256 x)
(type-align-of x)

; Multichannel
(setf x (type-repeat 4 x))

; We can also force conversion for nice printing
(setf x (make-type :unit))
(setf x (make-type '(:pair :i8 :i8)))
(setf x (make-type (make-type '(:pair :i8 :i8))))

; FIXME this should never return #<Pointer to VOID>
; That works but is confusing


; ---------------------------------------------------------------------------------------------------

(defcallback add-i8 :char ((c ptr) (x :char))
  (declare (ignore c))
  (+ x 1))
(defcallback add-f32 :float ((c ptr) (x :float))
  (declare (ignore c))
  (+ x 1))
(defcallback add-i8-i8 :char ((c ptr) (x :float) (y :float))
  (declare (ignore c))
  (+ x y))

(setf x (unary :i8 :i8 (callback add-i8) nil))
(setf x (unary :f32 :f32 (callback add-f32) nil))
(setf x (binary :i8 :i8 :i8 (callback add-i8-i8) nil))
(setf y x)

; ---------------------------------------------------------------------------------------------------

; Processor

; TODO combine defcallback/unary/binary into single macro
; i.e. define-processor

(type-offset-of 256 (input-type x))
(output-type x)

(setf y (id :i8))
(setf x (const :i16 :i8 0))
(setf x (sequence x y))
(setf x (parallel x y))
(setf x (loop x))
(setf x (split :f32))
; (setf x (delay :f32 44100))

(equal 
  (input-type (parallel (id :i8) (id :i8))) 
  (output-type (split :i8)))

(parallel 
  (sequence (id '(:frame :i8)) (const '(:frame :i8) '(:frame :i16) nil) (id '(:frame :i16))) 
  (id '(:vector :f32 1024))
  (id '(:frame :f32))) 

(sequence 
 (split :i8) 
 (parallel (id :i8) (id :i8))
 (id '(:i8 . :i8))
 (binary :i8 :i8 :f32 (callback add-i8-i8) nil))

; TODO short names
(setf x (processor-add i))
(setf x (processor-subtract i))
(setf x (processor-multiply i))
(setf x (processor-power i))
(setf x (processor-divide i))
(setf x (processor-modulo i))
(setf x (processor-absolute i))
(setf x (processor-not i))
(setf x (processor-and i))
(setf x (processor-or i))
(setf x (processor-xor i))
(setf x (processor-bit i))
(setf x (processor-bit i))
(setf x (processor-bit i))
(setf x (processor-bit i))
(setf x (processor-shift i))
(setf x (processor-shift i))
(setf x (processor-equal i))
(setf x (processor-less-than i))
(setf x (processor-greater-than i))
(setf x (processor-less-than-equal i))
(setf x (processor-greater-than-equal i))
(setf x (processor-acos i))
(setf x (processor-asin i))
(setf x (processor-atan i))
(setf x (processor-cos i))
(setf x (processor-sin i))
(setf x (processor-tan i))
(setf x (processor-exp i))
(setf x (processor-log i))
(setf x (processor-log10 i))
(setf x (processor-pow i))
(setf x (processor-sqrt i))
(setf x (processor-abs i))
(setf x (processor-min i))
(setf x (processor-max i))
(setf x (processor-fmod i))
(setf x (processor-remainder i))
(setf x (processor-floor i))
(setf x (processor-ceil i))
(setf x (processor-rint i))


; ---------------------------------------------------------------------------------------------------

; Signals

(type-offset-of 256 (signal-type x))
(signal-type x)

(setf x (constant :i8 1))
(setf x (apply (id :i8) x))
(setf x (apply (constant :i16 :i8 0)))

(let* (
  (input x)
  (freq x)
  (gain x)
  )
  output
  )

(let* 
  ((input x))      
  (feedback 
    delay 
    (+ input delay))
  ) 

; ==>  
(let* 
  ((input x))      
  (let* (
     (#fb# (signal-loop-create))
     (delay (loop-input #fb#))
    ) 
     (loop-output #fb# (+ input delay))
    )
  ) 



; TODO short names
(setf x (add x y))
(setf x (subtract x y))
(setf x (multiply x y))
(setf x (power x y))
(setf x (divide x y))
(setf x (modulo x y))
(setf x (absolute x y))
(setf x (not x y))
(setf x (and x y))
(setf x (or x y))
(setf x (xor x y))
(setf x (bit x y))
(setf x (bit x y))
(setf x (bit x y))
(setf x (bit x y))
(setf x (shift x y))
(setf x (shift x y))
(setf x (equal x y))
(setf x (less-than x y))
(setf x (greater-than x y))
(setf x (less-than-equal x y))
(setf x (greater-than-equal x y))
(setf x (acos x y))
(setf x (asin x y))
(setf x (atan x y))
(setf x (cos x y))
(setf x (sin x y))
(setf x (tan x y))
(setf x (exp x y))
(setf x (log x y))
(setf x (log10 x y))
(setf x (pow x y))
(setf x (sqrt x y))
(setf x (abs x y))
(setf x (min x y))
(setf x (max x y))
(setf x (fmod x y))
(setf x (remainder x y))
(setf x (floor x y))
(setf x (ceil x y))
(setf x (rint x y))



; ---------------------------------------------------------------------------------------------------

; Message stuff

(message-send)
(message-add-receiver)
(message-remove-receiver)
(message-dispatch)

; Move these to dispatcher module?

(setf x (message-simple))
(setf x (message-buffered))
(setf x (message-non-blocking))

; ---------------------------------------------------------------------------------------------------

; Scheduler

(setf x (scheduler-create))
(scheduler-destroy x)
; (scheduler-schedule x (lambda (x) ))
(scheduler-execute x)

; ---------------------------------------------------------------------------------------------------

; Audio devices

(setf s (device-audio-begin-session))
(error-check s)
(error-message (to-error s))
(device-audio-end-session s)
;(device-audio-with-session)


(device-audio-all s)
(setf x (device-audio-default-input s))
(setf y (device-audio-default-output s))
; (equal x y) ; FIXME

(device-audio-name x)
(device-audio-host-name x)
(device-audio-input-type x)
(device-audio-output-type x)
(device-audio-input-type y)
(device-audio-output-type y)
(type-channels (device-audio-input-type x))
(type-channels (device-audio-output-type x))
(type-size-of 1024 (device-audio-input-type x))

(type-channels 
 (device-audio-output-type 
  (from-pointer 'device-audio (list-head (device-audio-all s)))))

(setf p (processor-identity '(:pair :f32 :f32)))
(setf z (device-audio-open-stream x p y))
(device-audio-close-stream z)
;(device-audio-with-stream)

; ---------------------------------------------------------------------------------------------------

; Midi devices

; (device-midi-all)
; (device-midi-default)
; (device-midi-name)
; (device-midi-host-name)
; (device-midi-has-input)
; (device-midi-has-output)
; (device-midi-begin-session)
; (device-midi-restart-session)
; (device-midi-end-session)
; (device-midi-start-stream)
; (device-midi-restart-stream)
; (device-midi-stop-stream)
; (device-midi-with-session)
; (device-midi-with-stream)

; ---------------------------------------------------------------------------------------------------

; File devices

(device-file-open)
(device-file-close)
(device-file-run)

; ---------------------------------------------------------------------------------------------------

; Buffer devices

(device-buffer-open)
(device-buffer-close)
(device-buffer-run)


; ---------------------------------------------------------------------------------------------------

; Error

; check
; error-message
; error-module

; ---------------------------------------------------------------------------------------------------

; Priority queue

(setf x (priorityqueue-empty))
(priorityqueue-insert (random 1000) x)
(priorityqueue-peek x)
(priorityqueue-pop x)

; ---------------------------------------------------------------------------------------------------

; Atomic

(setf x (atomic-create))
(setf y (atomic-copy x))
(atomic-exchange x 0 1)      ; FIXME
(atomic-exchange x 1 0)
(atomic-get x)
(atomic-set x 1)
(atomic-add x 1)
; (atomic-modify (lambda (x) x) x)
(atomic-destroy x)

(setf x (atomic-queue-create))
(atomic-queue-destroy x)
(atomic-queue-write x (random 20))
(atomic-queue-read x)               ; FIXME <ptr 0> should be nil


; ---------------------------------------------------------------------------------------------------

; Plotting

(cffi:defcallback plot-cb ptr ((c ptr))
  (cl:print "Printing"))

(plot-show (callback plot-cb) nil)


; ---------------------------------------------------------------------------------------------------

; Misc generic functions

(equal              x y)
(less-than          x y)
(greater-than       x y)
(less-than-equal    x y)
(greater-than-equal x y)
(min                x y)
(max                x y)

(add                x y)
(multiply           x y)
(subtract           x y)
(divide             x y)
(absolute           x)

(copy               x)
(destroy            x)
(string-show        x)
(string-to-json     x)
(string-from-json   x)

(equal              0 0)
(equal              0 1)
(less-than          0 1)
(greater-than       0 1)
(less-than-equal    0 1)
(greater-than-equal 0 1)
(min                0 0)
(max                0 1)
(add                0 0)
(multiply           0 1)
(subtract           0 0)
(divide             0 1)
(absolute           -3)




