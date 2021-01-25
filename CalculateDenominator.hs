import GHC.Exts
import Data.Poly

coin n = (1 - X^n) :: VPoly Integer

coins = [1, 5, 10, 25, 50, 100]

den = foldr (*) 1 $ map coin coins

denL = toList p 
