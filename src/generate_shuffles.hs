---- shuffle components
symbols_v  = [('x',"00"), ('y',"01"), ('z',"10"), ('w',"11")]
symbols_c  = [('r',"00"), ('g',"01"), ('b',"10"), ('a',"11")]
symbols3_v = take 3 symbols_v
symbols3_c = take 3 symbols_c

-- all words of n letters
zip_shuf_v  = [[a,b,c,d] | a <- symbols_v,  b <- symbols_v, c <- symbols_v, d <- symbols_v]
zip_shuf_c  = [[a,b,c,d] | a <- symbols_c,  b <- symbols_c, c <- symbols_c, d <- symbols_c]
zip_shuf3_v = [[a,b,c]   | a <- symbols3_v, b <- symbols3_v, c <- symbols3_v]
zip_shuf3_c = [[a,b,c]   | a <- symbols3_c, b <- symbols3_c, c <- symbols3_c]

-- paired left-to-right due to endianness: xyzw = 0b[w][z][y][x]
shuffles_v  = [ [x,y,z,w] ++ " = 0b" ++ d ++ "\'" ++ c ++ "\'" ++ b ++ "\'" ++ a ++ ",\n" | [(x,a),(y,b),(z,c),(w,d)] <- zip_shuf_v ]
shuffles_c  = [ [x,y,z,w] ++ " = 0b" ++ d ++ "\'" ++ c ++ "\'" ++ b ++ "\'" ++ a ++ ",\n" | [(x,a),(y,b),(z,c),(w,d)] <- zip_shuf_c ]
-- last component of n=3 always left untouched
shuffles3_v = [ [x,y,z] ++ " = 0b11" ++ "\'" ++ c ++ "\'" ++ b ++ "\'" ++ a ++ ",\n" | [(x,a),(y,b),(z,c)] <- zip_shuf3_v ]
shuffles3_c = [ [x,y,z] ++ " = 0b11" ++ "\'" ++ c ++ "\'" ++ b ++ "\'" ++ a ++ ",\n" | [(x,a),(y,b),(z,c)] <- zip_shuf3_c ]

main :: IO ()
main = do
    putStrLn "xyzw components"
    putStr $ concat shuffles_v
    putStrLn "\nrgba components"
    putStr $ concat shuffles_c
    putStrLn "\nxyz components"
    putStr $ concat shuffles3_v
    putStrLn "\nrgb components"
    putStr $ concat shuffles3_c
