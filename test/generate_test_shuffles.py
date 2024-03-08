x = ("x", "-1.0f")
y = ("y", "2.66669667f")
z = ("z", "3.4567896f")
w = ("w", "-4e-4f")

zips1 = [w, x, z, y]
zips2 = [z, y, x, w]
zips3 = [w, z, y, x]
zips4 = [y, w, x, z]

print(f"  Vec4 vec({x[1]}, {y[1]}, {z[1]}, {w[1]});\n")

for p1 in zips1:
    for p2 in zips2:
        for p3 in zips3:
            for p4 in zips4:
                print(f"  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::{p1[0]}{p2[0]}{p3[0]}{p4[0]}>(vec), {p1[1]}, {p2[1]}, {p3[1]}, {p4[1]});")

print("\* ############ *\\\n\n\n\n")

zips1 = [x, z, y]
zips2 = [z, y, x]
zips3 = [y, x, z]

print(f"  LVec3 vec({x[1]}, {y[1]}, {z[1]});\n")

for p1 in zips1:
    for p2 in zips2:
        for p3 in zips3:
            print("  {")
            print(f"    LVec3 shuffled = shuffle<Shuf3::{p1[0]}{p2[0]}{p3[0]}>(vec);")
            print(f"    FSFV_CHECK_COMP3(shuffled, {p1[1]}, {p2[1]}, {p3[1]});")
            print(f"    FSFV_CHECK_COMPONENTS(Vec4(shuffled), {p1[1]}, {p2[1]}, {p3[1]}, 0);")
            print("  }")

print("\* ############ *\\\n\n\n\n")

r = ("r", "pi")
g = ("g", "-.66669667f")
b = ("b", "-inv_pi")
a = ("a", "4e7f")

zips1 = [a, r, b, g]
zips2 = [b, g, r, a]
zips3 = [a, b, g, r]
zips4 = [g, a, r, b]

print(f"\n  Vec4 vec({r[1]}, {g[1]}, {b[1]}, {a[1]});\n")

for p1 in zips1:
    for p2 in zips2:
        for p3 in zips3:
            for p4 in zips4:
                print(f"  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::{p1[0]}{p2[0]}{p3[0]}{p4[0]}>(vec), {p1[1]}, {p2[1]}, {p3[1]}, {p4[1]});")

print("\* ############ *\\\n\n\n\n")

zips1 = [r, b, g]
zips2 = [b, g, r]
zips3 = [g, r, b]

print(f"  LVec3 vec({r[1]}, {g[1]}, {b[1]});\n")

for p1 in zips1:
    for p2 in zips2:
        for p3 in zips3:
            print("  {")
            print(f"    LVec3 shuffled = shuffle<Shuf3::{p1[0]}{p2[0]}{p3[0]}>(vec);")
            print(f"    FSFV_CHECK_COMP3(shuffled, {p1[1]}, {p2[1]}, {p3[1]});")
            print(f"    FSFV_CHECK_COMPONENTS(Vec4(shuffled), {p1[1]}, {p2[1]}, {p3[1]}, 0);")
            print("  }")
