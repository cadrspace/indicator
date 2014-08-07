(define-module (indicator termios)
  #:export (cfgetispeed
            cfgetospeed
            cfsetispeed!
            cfsetospeed!
            cfsetspeed!
            cfmakeraw!
            tcgetattr
            tcsetattr!

            ;; Constants
            B0
            B50
            B75
            B110
            B134
            B150
            B200
            B300
            B600
            B1200
            B1800
            B2400
            B19200
            B38400
            B9600))


(load-extension "libguile-termios" "init_termios")


(define (tcgetattr fd)
  "Get the parameters associated with the object referred by fd and
stores them in the termios structure."
  (%tcgetattr fd))

(define (tcsetattr! termios fd . optional-actions)
  "Set the parameters associated with the terminal."
  (if (not (null? optional-actions))
      (%tcsetattr! termios fd optional-actions)
      (%tcsetattr! termios fd)))


(define (cfgetispeed termios)
  "Returns the input baud rate stored in the termios object."
  (%cfgetispeed termios))

(define (cfgetospeed termios)
  "Returns the output baud rate stored in the termios object."
  (%cfgetospeed termios))


(define (cfsetispeed! termios speed)
  "Set the input baud rate stored in the TERMIOS object to SPEED."
  (%cfsetispeed! termios speed))

(define (cfsetospeed! termios speed)
  "Set the input baud rate stored in the TERMIOS object to SPEED."
  (%cfsetospeed! termios speed))

(define (cfsetspeed! termios speed)
  "Set both input and output SPEED for the TERMIOS object."
  (cfsetispeed! termios speed)
  (cfsetospeed! termios speed))


(define (cfmakeraw! termios)
  "Set the terminal to something like the \"raw\" mode of the old
Version 7 terminal driver: input is available character by character,
echoing is disabled, and all special processing of terminal input and
output characters is disabled."
  (%cfmakeraw! termios))

;;; termios.scm ends here

