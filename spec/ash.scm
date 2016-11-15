(include "spec/spec-cli")

(display "Basic mathematical operations.\n")

(it "can add numbers"
  '("(+ 1 1)"      "2")
  '("(+ 1 1 1)"    "3")
  '("(add 5 8 10)" "23"))

(it "can multiply numbers"
  '("(* 2 2)"          "4")
  '("(* 2 2 2)"        "8")
  '("(multiply 4 4 4)" "64"))

(it "can subtract numbers"
  '("(- 16 8)"           "8")
  '("(- 16 8 4)"         "4")
  '("(subtract 32 16 8)" "8"))


(it "can divide 16 in 4"
  '("(/ 16 4)"      "4")
  '("(divide 32 4)" "8"))

(it "can find the remainder"
  '("(% 16 5)"      "1")
  '("(modulo 32 3)" "2"))

(it "can nest expressions"
  '("(+ 2 5 6 (- 3 4 (* 23 53)) (* 934 34))" "30549"))

(it "can operate on floating-point numbers"
  '("(+ 2.2 3.3)"        "5.5")
  '("(add 2.2 3.3 1.1)"  "6.6")
  '("(- 5.5 1.1)"        "4.4")
  '("(subtract 9.9 2.2)" "7.7")
  '("(* 2.2 2.2)"        "4.84")
  '("(multiply 3.3 4.2)" "13.86")
  '("(/ 4.4 1.1)"        "4")
  '("(divide 4.5 5.0)"   "0.9")
  '("(/ 4.5 5.0 3.0)"    "0.3")
  '("(% 10.5 5.0)"       "0.5")
  '("(modulo 10.5 5.0)"  "0.5"))

(it "can mix integer and floating-point numbers"
  '("(+ 2.2 3)"        "5.2")
  '("(add 2 3 1.1)"    "6.1")
  '("(- 5.5 1)"        "4.5")
  '("(subtract 9.9 2)" "7.9")
  '("(* 2.2 2)"        "4.4")
  '("(multiply 3.3 4)" "13.2")
  '("(/ 4.4 2)"        "2.2")
  '("(divide 4.5 5)"   "0.9")
  '("(/ 4.5 5 3)"      "0.3")
  '("(% 10.5 5)"       "0.5")
  '("(modulo 10.5 5)"  "0.5"))
(newline)

(display "List operations.\n")

(it "can define q-expressions"
  '("{1 2 3 4}"          "{1 2 3 4}")
  '("(list 1 2 3 4 5)"   "{1 2 3 4 5}")
  '("(list (+ 1 2) 6 9)" "{3 6 9}")
  '("{abc def ghi}"      "{abc def ghi}"))

(it "can return the first element of a q-expression with 'head'"
  '("(head {1 2 3})"   "{1}")
  '("(head {abc def})" "{abc}")
  '("(head {1})"       "{1}"))

(it "can return all the elements except the first with 'tail'"
  '("(tail {1 2 3 4})"      "{2 3 4}")
  '("(tail {ab cd ef gh})"  "{cd ef gh}")
  '("(tail (list 1 3 5 8))" "{3 5 8}"))

(it "can prepend an item to a list with 'construct'"
  '("(construct 1 {2 3 4})" "{1 2 3 4}"))

(it "can get the length of a list with 'length'"
  '("(length {1 2 3 4})" "4")
  '("(length {1})"       "1")
  '("(length {})"        "0"))

(it "can join lists together with 'join'"
  '("(join {1 2 3} {4 5 6})"   "{1 2 3 4 5 6}")
  '("(join {1 2} {3 4} {5 6})" "{1 2 3 4 5 6}"))

(it "can remove the last item of a list with 'initial'"
  '("(initial {1 2 3 4})" "{1 2 3}")
  '("(initial {1})"       "{}"))
(newline)

(display "complex operations")

(it "can define constants"
  '("(define {x} 1) x" "1"))