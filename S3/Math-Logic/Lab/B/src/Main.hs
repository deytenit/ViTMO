module Main where

import qualified Data.List as List
import qualified Data.Map.Strict as Map
import Data.Maybe
import qualified Data.Set as Set
import Lexer
import Parser

-- Utils

normalizeProblem (Problem context expression) = (Problem (List.sort context) expression)

deductProblem (Problem context (Impl a b)) = deductProblem (Problem (a : context) b)
deductProblem problem = normalizeProblem problem

scan content = map (parser . alexScanTokens) (lines content)

proofToStr problem = "[" ++ (show . fst) problem ++ "] " ++ (show . snd) problem

listJoin delim list = List.intercalate delim (map show list)

isImpossible that = case that of
  (Impossible a) -> True
  _ -> False

isImpl that = case that of
  (Impl a b) -> True
  _ -> False


-- Grammar

instance Show Problem where
  show (Problem context expression) = (listJoin "," context) ++ "|-" ++ (show expression)

instance Show Expression where
  show (Impl disjunction expression) = "(" ++ (show disjunction) ++ "->" ++ (show expression) ++ ")"
  show (Or disjunction conjunction) = "(" ++ (show disjunction) ++ "|" ++ (show conjunction) ++ ")"
  show (And conjunction negation) = "(" ++ (show conjunction) ++ "&" ++ (show negation) ++ ")"
  show (Not negation) = "!" ++ (show negation)
  show (Var string) = string

instance Show Proof where
  show (Hypothesis index line) = (show line) ++ " [Hyp. " ++ (show index) ++ "]"
  show (Axiom index line) = (show line) ++ " [Ax. sch. " ++ (show index) ++ "]"
  show (ModusPonens fromIndex toIndex line) = (show line) ++ " [M.P. " ++ (show fromIndex) ++ ", " ++ (show toIndex) ++ "]"
  show (Deduction index line) = (show line) ++ " [Ded. " ++ (show index) ++ "]"
  show (Impossible line) = (show line) ++ " [Incorrect]"

showFormalization (index, proof) = "[" ++ (show index) ++ "] " ++ (show proof)


-- Hypothesis

checkHypothesis problem = case List.elemIndex (getExpression problem) (getContext problem) of
  Just index -> Hypothesis (index + 1) problem
  Nothing -> Impossible problem


-- Axioms

checkAxiom (Problem context expression@(Impl a (Impl b a'))) | a == a' = Axiom 1 (Problem context expression)
checkAxiom (Problem context expression@(Impl (Impl a b) (Impl (Impl a' (Impl b' c)) (Impl a'' c')))) | a == a' && a' == a'' && b == b' && c == c' = Axiom 2 (Problem context expression)
checkAxiom (Problem context expression@(Impl a (Impl b (And a' b')))) | a == a' && b == b' = Axiom 3 (Problem context expression)
checkAxiom (Problem context expression@(Impl (And a b) a')) | a == a' = Axiom 4 (Problem context expression)
checkAxiom (Problem context expression@(Impl (And a b) b')) | b == b' = Axiom 5 (Problem context expression)
checkAxiom (Problem context expression@(Impl a (Or a' b))) | a == a' = Axiom 6 (Problem context expression)
checkAxiom (Problem context expression@(Impl b (Or a b'))) | b == b' = Axiom 7 (Problem context expression)
checkAxiom (Problem context expression@(Impl (Impl a c) (Impl (Impl b c') (Impl (Or a' b') c'')))) | a == a' && b == b' && c == c' && c' == c'' = Axiom 8 (Problem context expression)
checkAxiom (Problem context expression@(Impl (Impl a b) (Impl (Impl a' (Not b')) (Not a'')))) | a == a' && a' == a'' && b == b' = Axiom 9 (Problem context expression)
checkAxiom (Problem context expression@(Impl (Not (Not a)) a')) | a == a' = Axiom 10 (Problem context expression)
checkAxiom problem = Impossible problem


-- Modus Ponens

checkModusPonens (Problem context expression) prbmMap implMap =
  let fromList = fromMaybe [] (Map.lookup expression implMap)
      fromExpression' = List.find (\l -> isJust (Map.lookup (normalizeProblem (Problem context l)) prbmMap)) fromList
   in if isJust fromExpression'
        then
          let fromExpression = fromJust fromExpression'
              fromIndex = fromJust (Map.lookup (normalizeProblem (Problem context fromExpression)) prbmMap)
              toIndex = fromJust (Map.lookup (normalizeProblem (Problem context (Impl fromExpression expression))) prbmMap)
           in ModusPonens fromIndex toIndex (Problem context expression)
        else Impossible (Problem context expression)


-- Deduction

checkDeduction problem hyptMap =
  let hyptIndex' = Map.lookup (deductProblem problem) hyptMap
   in if isJust hyptIndex'
        then Deduction (fromJust hyptIndex') problem
        else Impossible problem


-- Maps

updatePrbmMap problem' index prbmMap =
  let problem = normalizeProblem problem'
   in Map.insert problem index prbmMap

updateHyptMap problem' index hyptMap =
  let problem = deductProblem problem'
   in Map.insert problem index hyptMap

updateImplMap problem implMap =
  if isImpl (getExpression problem)
    then
      let from = getT1 (getExpression problem)
          to = getT2 (getExpression problem)
       in Map.insertWith (++) to [from] implMap
    else implMap


-- Proof

makeProof problem prbmMap hyptMap implMap = min (checkAxiom problem) (min (checkHypothesis problem) (min (checkDeduction problem hyptMap) (checkModusPonens problem prbmMap implMap)))

resolve [] index prbmMap hyptMap implMap result = result
resolve (problem : rest) index prbmMap hyptMap implMap result =
  let proof = makeProof problem prbmMap hyptMap implMap
      newPrbmMap = updatePrbmMap problem index prbmMap
      newHyptMap = updateHyptMap problem index hyptMap
      newImplMap = updateImplMap problem implMap
      newResult = result ++ [proof]
   in resolve rest (index + 1) newPrbmMap newHyptMap newImplMap newResult


-- Main

main :: IO ()
main = do
  content <- Prelude.getContents
  let problems = scan content
  let results = resolve problems 1 Map.empty Map.empty Map.empty []
  mapM_ (putStrLn . showFormalization) (zip [1 ..] results)
