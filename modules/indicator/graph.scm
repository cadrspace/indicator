;;; graph.scm -- Indicator graphic primitives.

(define-module (indicator graph)
  #:use-module (indicator config)
  #:use-module (rnrs bytevectors)
  #:export (fill! draw-point! draw-line! draw-random-image!
                  draw-rectangle!))


(define* (fill! bv #:optional (value 0))
  "Fill a bytevevtor BV with a VALUE."
  (bytevector-fill! bv value))

(define* (draw-point! bv x y #:optional (value 0))
  (bytevector-u8-set! bv
                      (+ x (* y %led-panel-heigh))
                      value))


(define (slope x1 y1 x2 y2)
  (/ (exact->inexact (- y2 y1))
     (exact->inexact (- x2 x1))))

(define* (draw-line! bv x1 y1 x2 y2 #:optional (color 0))
  (define (draw x1 y1 x2 y2)
    (cond
     ((= x1 x2)
      ;; Handle vertical lines
      (do ((y y1 (1+ y)))
          ((> y y2))
        (draw-point! bv x1 y color)))
     ((= y1 y2)
      ;; Handle horizontal lines
      (do ((x x1 (1+ x)))
          ((> x x2))
        (draw-point! bv x y1 color)))
     (else
      ;; Handle lines with slope
      (do ((x x1 (1+ x)))
          ((> x x2))
        (let* ((s (slope x1 y1 x2 y2))
               (y (round (inexact->exact
                          (+ (* s (- x x1))
                             y1)))))
          ;; (format #t " --- 1: ~a,~a; slope: ~a; ~%"
          ;;         x y s)
          (draw-point! bv
                       (logand (if (< x 0) 0 x)
                               (1- %led-panel-width))
                       (logand (if (< y 0) 0 y)
                               (1- %led-panel-heigh))
                       color))))))
  ;; (format #t "~% draw-line!: x1:~a; y1:~a; x2:~a; y2:~a~%"
  ;;         x1 y1 x2 y2)
  (if (> x1 x2)
      (draw x2 y2 x1 y1)
      (draw x1 y1 x2 y2)))

(define (draw-rectangle! bv x1 y1 x2 y2 color)
  (do ((y y1 (1+ y)))
      ((> y y2))
    (draw-line! bv x1 y x2 y color)))


(define (draw-random-image! bv)
  "Generate a random image of SIZE pixels, return a bytevector."
  (do ((i 0 (1+ i)))
      ((= i (bytevector-length bv)))
    (bytevector-u8-set! bv i (random 7))))

;;; graph.scm ends here.
