(import chicken scheme)
(use irregex shell filepath posix s)

(define (fatal . rest) (apply format (cons #t rest)) (exit 1))

(define (it behavior . conditions)
  (let ((results (check-conditions conditions)))
    (format #t "- it ~A ~A\n" behavior (format-results results))
    (map print-error results)))

(define (format-results results)
  (s-join "" (map (lambda (r) (if (boolean? r) "." "x")) results)))

(define (print-error result)
  (if (not (boolean? result))
    (apply format (append '(#t "[FAIL] '~A', expected '~A' got '~A'\n") result))))

(define (check-conditions conditions) (map check-condition conditions))

(define (check-condition condition)
  (let ((actual   (last (s-split "\n" (s-chomp (ash-eval (cdr condition))))))
        (expected (irregex-quote (car condition))))
    (if (irregex-match expected actual)
        #t
        (list (cadr condition) expected actual))))

(define (ash-eval expressions)
  (let ((stdout (capture ,(format-command expressions))))
    (if (= (string-length stdout) 0)
        (fatal "Failed to execute \n~A\n" command)
        (last (s-split "\n" stdout)))))

(define (format-command expressions)
  (format #f "echo \"~A\" | ~A -s" (s-join "\n" expressions) interpreter))

(define interpreter
  (filepath:join-path (append (list (current-directory)) '("bin" "ash"))))
