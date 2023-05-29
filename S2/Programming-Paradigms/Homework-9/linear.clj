(defn compatible? [cmp? & args]
  (and (every? cmp? args) (apply == (map count args))))

(defn linear-vector? [type? v] (and (vector? v) (every? type? v)))
(defn linear-matrix? [type? m] (linear-vector?
                                 #(compatible? (partial linear-vector? type?) (first m) %)
                                 m))

(defn coordinate-op [valid? op]
  (fn [& args]
    {:pre [(every? (partial valid? (first args)) args)] :post [(valid? (first args) %)]}
    (apply mapv op args)))

(def vector-op (partial coordinate-op (partial compatible? (partial linear-vector? number?))))

(def v+ (vector-op +'))
(def v- (vector-op -'))
(def v* (vector-op *'))
(def vd (vector-op /))

(defn reduce-op [op]
  (fn [& args]
    (reduce op args)))

(defn scalar [& args] (reduce + (apply v* args)))

(def vect
  (reduce-op
    (fn [v-1 v-2]
      {:pre [(compatible? (partial linear-vector? number?) v-1 v-2)] :post [(linear-vector? number? %)]}
      (let [[x-1 y-1 z-1] v-1 [x-2 y-2 z-2] v-2]
        [(- (* y-1 z-2) (* y-2 z-1))
         (- (* z-1 x-2) (* z-2 x-1))
         (- (* x-1 y-2) (* x-2 y-1))]))))

(defn v*s [v & scalars]
  {:pre [(and (linear-vector? number? v) (every? number? scalars))] :post [(linear-vector? number? %)]}
  (let [scalar (reduce * scalars)]
    (mapv (partial * scalar) v)))

(def matrix-op
  (partial coordinate-op
           #(and
              (compatible? (partial linear-matrix? number?) %1 %2)
              (compatible? (partial linear-vector? number?) (first %1) (first %2)))))

(def m+ (matrix-op v+))
(def m- (matrix-op v-))
(def m* (matrix-op v*))
(def md (matrix-op vd))

(defn m*s [m & scalars]
  {:pre [(and (linear-matrix? number? m) (every? number? scalars))] :post [(linear-matrix? number? %)]}
  (let [scalar (reduce * scalars)]
    (mapv #(v*s % scalar) m)))

(defn m*v [m v]
  {:pre [(and (linear-matrix? number? m) (compatible? (partial linear-vector? number?) v (first m)))]
   :post [(linear-vector? number? %)]}
  (mapv (partial scalar v) m))

(defn transpose [m]
  {:pre [(linear-matrix? number? m)] :post [(linear-matrix? number? %)]}
  (apply mapv vector m))

(def m*m (reduce-op
           (fn [m-1 m-2]
             {:pre [(and (linear-matrix? number? m-1) (linear-matrix? number? m-2) (== (count m-2) (count (first m-1))))]
              :post [(linear-matrix? number? %)]}
             (mapv (partial m*v (transpose m-2)) m-1))))

(defn tensor-op [op]
  (fn [& args]
    {:pre [(or (every? number? args) (apply (partial compatible? vector?) args))]
     :post [(or (number? %) (vector? %))]}
    (if (every? number? args)
      (apply op args) (apply (partial mapv (tensor-op op)) args))))

(def t+ (tensor-op +'))
(def t- (tensor-op -'))
(def t* (tensor-op *'))
(def td (tensor-op /))