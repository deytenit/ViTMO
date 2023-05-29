(defn double-division
  ([arg] (/ 1 (double arg)))
  ([f-arg & r-args] (reduce #(/ (double %1) (double %2)) f-arg r-args)))

(defn make-parser [operation-list constant-kind variable-kind]
    (letfn [(parse-token [token]
              (cond
                (number? token) (constant-kind token)
                (symbol? token) (variable-kind (str token))
                (list? token) (apply (operation-list (first token)) (mapv parse-token (rest token)))))]
      (comp parse-token read-string)))

;; -----------functional-----------

(defn operation [function]
  (fn [& expressions]
     (fn [args]
       (apply function (map #(% args) expressions)))))

(def add (operation +'))
(def subtract (operation -'))
(def multiply (operation *'))
(def divide (operation double-division))

(def sumexp (operation (fn [& args]
                         (apply + (map #(Math/exp %) args)))))
(def lse (operation (fn [& args]
                      (Math/log (apply + (map #(Math/exp %) args))))))
(defn negate [expression] (fn [arg] (- (expression arg))))

(defn constant [value] (constantly value))
(defn variable [name] (fn [args] (args name)))

(def functional-operations {'+ add '- subtract
                            '* multiply '/ divide
                            'sumexp sumexp 'lse lse
                            'negate negate})

(def parseFunction (make-parser functional-operations constant variable))

;; ----------object-----------

(declare Constant Variable)

(declare Negate Add Subtract Multiply Divide Meansq RMS)
(declare diff)
(declare ZERO ONE TWO)

(definterface IExpression
  (evaluate [args])
  (diff [var])
  (toString [])
  (toStringInfix []))

(defn evaluate [that args] (.evaluate that args))
(defn diff [that variable] (.diff that variable))
(defn toString [that] (.toString that))
(defn toStringInfix [that] (.toStringInfix that))

(deftype ConstantConstructor [value]
  IExpression
  (evaluate [this args] (.value this))
  (diff [this variable] ZERO)
  (toStringInfix [this] (toString this))
  (toString [this] (str (.value this))))

(defn Constant [value] (ConstantConstructor. value))

(def ZERO (Constant 0))
(def ONE (Constant 1))
(def TWO (Constant 2))

(deftype VariableConstructor [name]
  IExpression
  (evaluate [this args] (args ((comp clojure.string/lower-case str first) (.name this))))
  (diff [this variable] (if (= variable (.name this)) ONE ZERO))
  (toStringInfix [this] (toString this))
  (toString [this] (.name this)))

(defn Variable [name] (VariableConstructor. name))

(deftype OperationConstructor [title operation-fn diff-fn infix-fmt expressions]
  IExpression
  (evaluate [this args] (apply operation-fn (map #(evaluate % args) (.expressions this))))
  (diff [this variable] (apply diff-fn variable (.expressions this)))
  (toStringInfix [this] (format infix-fmt ((comp toStringInfix first) (.expressions this)) (.title this) ((comp toStringInfix last) (.expressions this))))
  (toString [this] (str "(" (.title this) " " (clojure.string/join " " (map toString (.expressions this))) ")")))

(defn Operation [title operation-fn diff-fn infix-fmt]
  (fn [& expressions] (OperationConstructor. title operation-fn diff-fn infix-fmt expressions)))

(defn BooleanOperation [title operation-fn diff-fn infix-fmt]
  (fn [& expressions] (OperationConstructor. title #(if (apply operation-fn %&) 1 0) diff-fn infix-fmt expressions)))

(def Negate (Operation
              "negate"
              -'
              #(Negate (diff %2 %1))
              "%2$s %1$s"))

(def Not (BooleanOperation
           "!"
           (comp not pos?)
           #(Not (diff %2 %1))
           "%2$s %1$s"))

(defn diff-all [variable expressions]
  (map #(diff % variable) expressions))
(def Add (Operation
           "+"
           +'
           #(apply Add (diff-all % %&))
           "(%1$s %2$s %3$s)"))
(def Subtract (Operation
                "-"
                -'
                #(apply Subtract (diff-all % %&))
                "(%1$s %2$s %3$s)"))

(defn diff-multiply
  ([variable expression]
   (diff expression variable))
  ([variable f-expr & r-exprs]
   (Add (Multiply (diff f-expr variable) (apply Multiply r-exprs))
        (Multiply f-expr (apply diff-multiply variable r-exprs)))))
(def Multiply (Operation
                "*"
                *'
                diff-multiply
                "(%1$s %2$s %3$s)"))

(defn diff-divide
  ([variable expression]
   (Divide (Negate (diff expression variable)) (Multiply expression expression)))
  ([variable f-expr & r-exprs]
   (Divide (Subtract (Multiply (diff f-expr variable)
                               (apply Multiply r-exprs))
                     (Multiply
                       f-expr
                       (apply diff-multiply variable r-exprs)))
           (Multiply
             (apply Multiply r-exprs)
             (apply Multiply r-exprs)))))
(def Divide (Operation
              "/"
              double-division
              diff-divide
              "(%1$s %2$s %3$s)"))

(defn meansq-op [& args]
  (/ (apply + (map #(* % %) args)) (count args)))
(defn diff-meansq
  ([variable f-expr & r-exprs]
   (Divide
     (apply Add (diff-multiply variable f-expr f-expr) (map #(diff-multiply variable % %) r-exprs))
     (Constant (inc (count r-exprs))))))
(def Meansq (Operation
              "meansq"
              meansq-op
              diff-meansq
              "(%1$s %2$s %3$s)"))

(defn diff-rms [variable & expressions]
  (Divide (apply diff-meansq variable expressions) (Multiply TWO (apply RMS expressions))))
(def RMS (Operation
           "rms"
           #(Math/sqrt (apply meansq-op %&))
           diff-rms
           "(%1$s %2$s %3$s)"))

(def And (BooleanOperation
           "&&"
           #(every? pos? %&)
           #(apply And (diff-all % %&))
           "(%1$s %2$s %3$s)"))

(def Or (BooleanOperation
          "||"
          #(some pos? %&)
          #(apply And (diff-all % %&))
          "(%1$s %2$s %3$s)"))

(def Xor (BooleanOperation
           "^^"
           #(reduce (fn [x y] (not= x (pos? y))) false %&)
           #(apply And (diff-all % %&))
           "(%1$s %2$s %3$s)"))



(def object-operations {'negate Negate
                        '+ Add '- Subtract
                        '* Multiply '/ Divide
                        'meansq Meansq 'rms RMS
                        '! Not '&& And
                        '|| Or (symbol "^^") Xor})

(def parseObject (make-parser object-operations Constant Variable))

;; ----------parser-----------

(load-file "parser.clj")

(declare >base-low >parser)

(def *ws
  (let [*space (+char " \t\n\r")]
    (+ignore (+star *space))))

(def *constant
  (let [*digit (+char "0123456789")]
    (+map
      (comp Constant read-string)
      (+str (+seqf #(flatten %&) (+opt (+char "-")) (+plus *digit) (+opt (+char ".")) (+opt (+plus *digit)))))))

(def *variable
  (let [*letter (+char "xyzXYZ")]
    (+map (comp Variable str) (+str (+plus *letter)))))

(defn *unary-operation [parser post-parser]
  (letfn [(resolver [action term] ((object-operations ((comp symbol str) action)) term))]
    (+seqf resolver *ws parser *ws post-parser)))

(defn *binary-operation [parser pre-parser]
  (letfn [(resolver [term & actions] (reduce (fn [x y] ((object-operations ((comp symbol str) (nth y 0))) x (nth y 1))) term actions))]
       (+seqf (comp (partial apply resolver) cons) *ws pre-parser (+star (+seq *ws parser *ws pre-parser)))))

(def >base-low
  (let [*sub-expression (+seqn 1 (+char "(") (delay >parser) (+char ")"))]
    (+or (*unary-operation (+or (+string "negate") (+char "!")) (delay >base-low)) *variable *constant *sub-expression)))
(def >base-mid (*binary-operation (+char "*/") >base-low))
(def >base-top (*binary-operation (+char "+-") >base-mid))
(def >bool-low (*binary-operation (+string "&&") >base-top))
(def >bool-mid (*binary-operation (+string "||") >bool-low))
(def >bool-top (*binary-operation (+string "^^") >bool-mid))
(def >parser (+seqn 0 *ws >bool-top *ws))

(def parseObjectInfix (+parser >parser))