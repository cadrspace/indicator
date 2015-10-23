#!/usr/bin/guile \
-L . -e main -s
!#

(use-modules (ice-9 getopt-long)
             (ice-9 rdelim)
             (ice-9 binary-ports)
             (rnrs  io ports)
             (rnrs bytevectors)
             (termios)
             (termios system)
             (indicator config)
             (indicator graph)
             (indicator module clock))


(define (debug msg)
  (format #t "DEBUG: ~a~%" msg))


;;; Low-level procedures

(define* (show bv #:optional (port (current-output-port)))
  "Send a bytevector BV to a port."
  (put-u8 port %header)
  (put-bytevector port bv)
  (force-output port))


;;;

(define (show-random-image! tty bv count)
  "Show a random image."
  (do ((i 0 (1+ i)))
      ((> i count))
    (draw-random-image! bv)
    (show bv tty)
    (usleep 500)))

(define (show-fillin! tty bv count)
  "Fill the LED panel with a random color, pixel by pixel"
  (let* ((interval 125)                 ;us
         (fill (lambda (filling)
                 (do ((i 0 (1+ i)))
                     ((= i (bytevector-length bv)))
                   (bytevector-u8-set! bv i
                                       filling)
                   (show bv tty)
                   (usleep interval)))))
    (do ((i 0 (1+ i)))
        ((> i count))
      (fill (1+ (random 6))))))

(define (show-vfillin! tty bv count)
  "Fill the LED panel vertically."
  (let* ((interval 125)
         (fill     (lambda (filling)
                     (do ((offset 0 (1+ offset)))
                         ((= offset %led-panel-width))
                       (do ((i 0 (+ i %led-panel-width)))
                           ((>= i %led-panel-size))
                         (bytevector-u8-set! bv (+ i offset)
                                             filling)
                         (show bv tty)
                         (usleep interval))))))
    (do ((i 0 (1+ i)))
        ((> i count))
      (fill (1+ (random 6))))))

(define (show-cadr-logo! tty bv count)
  "Show the logo of CADR hackerspace."
  (let ((logo (list
               #x7 #x7 #x7 #x7 #x7 #x7 #x7 #x7
               #x7 #x0 #x0 #x0 #x0 #x0 #x0 #x7
               #x7 #x0 #x7 #x7 #x7 #x0 #x0 #x7
               #x7 #x0 #x7 #x0 #x7 #x7 #x7 #x7
               #x7 #x0 #x7 #x0 #x0 #x7 #x7 #x7
               #x7 #x0 #x7 #x7 #x7 #x0 #x0 #x7
               #x7 #x0 #x0 #x0 #x0 #x0 #x0 #x7
               #x7 #x7 #x7 #x7 #x7 #x7 #x7 #x7)))
    (do ((i 0 (1+ i)))
        ((> i count))
      (let* ((mask (random 6))
             (bv   (u8-list->bytevector
                    (map (lambda (x) (logxor x mask))
                         logo))))
        (show bv tty)
        (sleep 1)))))

(define (show-matrix! tty bv count)
  (let* ((interval 125)
         (fill     (lambda (filling)
                     (do ((offset 0 (1+ offset)))
                         ((= offset %led-panel-width))
                       (do ((i 0 (+ i %led-panel-width)))
                           ((>= i %led-panel-size))
                         (bytevector-u8-set! bv (+ i offset)
                                             filling)
                         (show bv tty)
                         (usleep interval))))))
    (do ((i 0 (1+ i)))
        ((> i count))
      (fill (1+ (random 6))))))

(define (show-rotating-line tty bv count)
  (define interval 0)
  (define (draw! x1 y1 x2 y2 color)
    "Draw a line."
    (fill! bv)
    (draw-line! bv x1 y1 x2 y2 color)
    (show bv tty)
    (usleep interval))
  (define (draw-rotating-line! offset size)
    "Show rotating line of a given SIZE using OFFSET."
    (let ((interval 0))
      (do ((x1 offset (1+ x1))
           (x2 size (1- x2)))
          ((> x1 (+ offset size)))
        (draw! x1 offset x2 (+ offset size) #x7))
      (do ((y1 (+ offset size) (1- y1))
           (y2 offset (1+ y2)))
          ((= y1 offset))
        (draw! offset y1 (+ offset size) y2 #x7))))
  (draw-rotating-line! 0 7))


;;;

;; List of all available animations
(define %animations
  (list show-random-image!
        show-random-image!
        show-fillin!
        show-vfillin!
        show-cadr-logo!))

(define (demo tty)
  "The demo mode."
  (let ((animations-count (length %animations))
        (bv               (make-bytevector %led-panel-size 0)))
    (while #t
      (let ((anim (list-ref %animations
                            (random animations-count))))
        (anim tty bv (1+ (random 3)))
        (usleep 125)))))


;;;

(define (logunset word bitmask)
  (logand word (logxor word bitmask)))

(define (configure-tty! tty)
  (let ((ts (make-termios-struct)))
    (tc-get-attr! tty ts)
    (debug "configure-tty!: debug 0")
    (let* ((ts-scm (parse-termios-struct ts))
           (new-cflag (logunset
                       (logand (get-field-from-termios ts-scm
                                                       'c-cflag)
                               termios-CSTOPB
                               termios-CS8)
                       termios-PARENB))
           (zzz (put-field-into-termios! ts-scm 'c-cflag new-cflag))
           (ts (make-termios-struct ts-scm)))
      (debug "configure-tty!: debug 1")
      (cf-make-raw! ts)
      (cf-set-speed! ts termios-B9600)
      (tc-set-attr tty ts))))


(define (main args)
  "Entry point of the program."
  (let* ((option-spec '((help   (single-char #\h) (value #f))
                        (device (single-char #\d) (value #t)
                                (required? #t))))
         (zzz (debug "debug 0"))
         (options     (getopt-long args option-spec))
         (zzz (debug "debug 1"))
         (help-wanted? (option-ref options 'help #f))
         (device       (option-ref options 'device #f))
         (zzz (debug "debug 2"))
         (tty          (open-io-file device)))
    (debug "configuring ...")
    (configure-tty! tty)
    (debug "configured.")
    (tc-flush tty termios-TCIFLUSH)
    (while #t
      (let ((bv (make-bytevector %led-panel-size 0)))
        (draw-clock! bv)
        (show bv tty)))
        ;; (show-rotating-line tty bv 1)))
        ;; (do ((i 0 (1+ i)))
        ;;     ((= i %led-panel-width))
        ;;   (fill! bv)
        ;;   (draw-point! bv i 4 #x7)
        ;;   (show bv tty)
        ;;   (usleep 125))
        ;; (do ((i (1- %led-panel-width) (1- i)))
        ;;     ((zero? i))
        ;;   (fill! bv)
        ;;   (draw-point! bv i 4 #x7)
        ;;   (show bv tty)
        ;;   (usleep 125))))
    ;; (demo tty)
    (close-port tty))
  (display "done\n"))

;;; indicator.scm ends here.
