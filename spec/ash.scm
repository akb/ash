(use irregex shell filepath posix)

(define interpreter
  (filepath:join-path (append (list (current-directory)) '("bin" "ash"))))

(define (check-condition conditions)
  (begin
    (if (car conditions)
      (display ".")
      (display "x"))
    (if (> (length conditions) 1)
      (check-condition (cdr conditions)))))

(define (it behavior . conditions)
  (format #t "- it ~A" behavior)
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
  (irregex-search (irregex-quote output) (ash-eval code)))

(it "can add numbers"
  (test-case "(+ 1 1)"      "2")
  (test-case "(+ 1 1 1)"    "3")
  (test-case "(add 5 8 10)" "23"))

(it "can multiply numbers"
  (test-case "(* 2 2)"          "4")
  (test-case "(* 2 2 2)"        "8")
  (test-case "(multiply 4 4 4)" "64"))

(it "can subtract numbers"
  (test-case "(- 16 8)"           "8")
  (test-case "(- 16 8 4)"         "4")
  (test-case "(subtract 32 16 8)" "8"))

(it "can divide 16 in 4"
  (test-case "(/ 16 4)"      "4")
  (test-case "(divide 32 4)" "8"))

(it "can nest expressions"
  (test-case "(+ 2 5 6 (- 3 4 (* 23 53)) (* 934 34))" "30549"))


(newline)
(display "List operations.\n")

(it "can define q-expressions"
  (test-case "{1 2 3 4}"          "{1 2 3 4}")
  (test-case "(list 1 2 3 4 5)"   "{1 2 3 4 5}")
  (test-case "{abc def ghi}"      "{abc def ghi}"))

(it "can return the first element of a q-expression with 'head'"
  (test-case "(head {1 2 3})"   "{1}")
  (test-case "(head {abc def})" "{abc}")
  (test-case "(head {1})"       "{1}"))

(it "can return all the elements except the first with 'tail'"
  (test-case "(tail {1 2 3 4})"      "{2 3 4}")
  (test-case "(tail {ab cd ef gh})"  "{cd ef gh}")
  (test-case "(tail (list 1 3 5 8))" "{3 5 8}"))

(it "can prepend an item to a list with 'construct'"
  (test-case "(construct 1 {2 3 4})" "{1 2 3 4}"))
