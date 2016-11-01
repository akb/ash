(use irregex shell filepath posix)

(define interpreter
  (filepath:join-path (append (list (current-directory)) '("bin" "ash"))))

(define (it behavior . conditions)
  (format #t "- it ~A" behavior)
  (define (check-condition conditions)
    (begin
      (if (car conditions)
        (display ".")
        (display "x"))
      (if (> (length conditions) 1)
        (check-condition (cdr conditions)))))
  (check-condition conditions)
  (newline))

(define (ash-eval code)
  (let ((stdout (capture ,(format #f "echo \"~A\" | ~A" code interpreter))))
    (if (= (string-length stdout) 0)
      (begin
        (format #t "Failed to execute \n~A\n" command)
        (exit))
      stdout)))

(display "Basic mathematical operations.\n")

(define (test-case code output)
  (irregex-search output (ash-eval code)))

(it "can add one and one"
  (test-case "(+ 1 1)" "2"))

(it "can add three 1s"
  (test-case "(+ 1 1 1)" "3"))

(it "can multiply two by two"
  (test-case "(* 2 2)" "4"))

(it "can multiply two by two by two"
  (test-case "(* 2 2 2)" "8"))

(it "can subtract 8 from 16"
  (test-case "(- 16 8)" "8"))

(it "can subtract 4 from 8 from 16"
  (test-case "(- 16 8 4)" "4"))

(it "can divide 16 in 4"
  (test-case "(/ 16 4)" "4"))

(it "can nest expressions"
  (test-case "(+ 2 5 6 (- 3 4 (* 23 53)) (* 934 34))" "30549"))

(it "defines textual aliases of each operator"
  (test-case "(add 5 8 10)" "23"))
