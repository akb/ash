(include "spec/spec")

(display "Basic mathematical operations.\n")

(it "can add numbers"
  '("2"  "(+ 1 1)")
  '("3"  "(+ 1 1 1)")
  '("23" "(add 5 8 10)"))

(it "can multiply numbers"
  '("4"  "(* 2 2)")
  '("8"  "(* 2 2 2)")
  '("64" "(multiply 4 4 4)"))

(it "can subtract numbers"
  '("8" "(- 16 8)")
  '("4" "(- 16 8 4)")
  '("8" "(subtract 32 16 8)"))

(it "can divide 16 in 4"
  '("4" "(/ 16 4)")
  '("8" "(divide 32 4)"))

(it "can find the remainder"
  '("1" "(% 16 5)")
  '("2" "(modulo 32 3)"))

(it "can nest expressions"
  '("30549" "(+ 2 5 6 (- 3 4 (* 23 53)) (* 934 34))"))

(it "can operate on floating-point numbers"
  '("5.5"   "(+ 2.2 3.3)")
  '("6.6"   "(add 2.2 3.3 1.1)")
  '("4.4"   "(- 5.5 1.1)")
  '("7.7"   "(subtract 9.9 2.2)")
  '("4.84"  "(* 2.2 2.2)")
  '("13.86" "(multiply 3.3 4.2)")
  '("4"     "(/ 4.4 1.1)")
  '("0.9"   "(divide 4.5 5.0)")
  '("0.3"   "(/ 4.5 5.0 3.0)")
  '("0.5"   "(% 10.5 5.0)")
  '("0.5"   "(modulo 10.5 5.0)"))

(it "can mix integer and floating-point numbers"
  '("5.2"  "(+ 2.2 3)")
  '("6.1"  "(add 2 3 1.1)")
  '("4.5"  "(- 5.5 1)")
  '("7.9"  "(subtract 9.9 2)")
  '("4.4"  "(* 2.2 2)")
  '("13.2" "(multiply 3.3 4)")
  '("2.2"  "(/ 4.4 2)")
  '("0.9"  "(divide 4.5 5)")
  '("0.3"  "(/ 4.5 5 3)")
  '("0.5"  "(% 10.5 5)")
  '("0.5"  "(modulo 10.5 5)"))
(newline)

(display "List operations.\n")

(it "can define q-expressions"
  '("{1 2 3 4}"     "{1 2 3 4}")
  '("{1 2 3 4 5}"   "(list 1 2 3 4 5)")
  '("{3 6 9}"       "(list (+ 1 2) 6 9)")
  '("{abc def ghi}" "{abc def ghi}"))

(it "can return the first element of a q-expression with 'head'"
  '("{1}"   "(head {1 2 3})")
  '("{abc}" "(head {abc def})")
  '("{1}"   "(head {1})"))

(it "can return all the elements except the first with 'tail'"
  '("{2 3 4}"    "(tail {1 2 3 4})")
  '("{cd ef gh}" "(tail {ab cd ef gh})")
  '("{3 5 8}"    "(tail (list 1 3 5 8))"))

(it "can prepend an item to a list with 'construct'"
  '("{1 2 3 4}" "(construct 1 {2 3 4})"))

(it "can get the length of a list with 'length'"
  '("4" "(length {1 2 3 4})")
  '("1" "(length {1})")
  '("0" "(length {})"))

(it "can join lists together with 'join'"
  '("{1 2 3 4 5 6}" "(join {1 2 3} {4 5 6})")
  '("{1 2 3 4 5 6}" "(join {1 2} {3 4} {5 6})"))

(it "can remove the last item of a list with 'initial'"
  '("{1 2 3}" "(initial {1 2 3 4})")
  '("{}"      "(initial {1})"))
(newline)

(display "complex operations\n")

(it "can define constants"
  '("1" "(define {x} 1)" "x"))

(it "can evaluate a list"
  '("10" "(evaluate {add 1 2 3 4})")
  '("30" "(evaluate (join {add} (list 10 10 10)))"))

(it "can exit"
  '("Exiting. Code 0" "(exit)")
  '("Exiting. Code 1" "(exit 1)"))