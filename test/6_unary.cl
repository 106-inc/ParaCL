x = ?;

neg_x = -x; // -10
not_x = !x; // 0
not_not_x = !!x > 0; // 1
andrey = -!-x; // 0
min_min_x = --x; // 10

print 5 -- -x;   // -5
print neg_x;     // -10 
print not_x;     // 0
print not_not_x; // 1
print andrey;    // 0
print min_min_x; // 10
