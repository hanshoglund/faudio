(in-package :audio-engine)
(defctype system-event-type :int)
(defcfun (system-event-mouse-move "doremir_system_event_mouse_move") event)
(defcfun (system-event-mouse-drag "doremir_system_event_mouse_drag") event)
(defcfun (system-event-mouse-up "doremir_system_event_mouse_up") event)
(defcfun (system-event-mouse-down "doremir_system_event_mouse_down") event)
(defcfun (system-event-key-up "doremir_system_event_key_up") event)
(defcfun (system-event-key-down "doremir_system_event_key_down") event)
(defcfun (system-event-select "doremir_system_event_select") event (a list))
(defcfun (system-event-select-sender "doremir_system_event_select_sender") message-sender (a list))