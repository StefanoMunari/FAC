/* example: arithmetic expression */

fract a = [1|2];
fract b = [2|3];
fract c = [5|3];
fract d = [5|3];

c = a + -b + (d*c + b/a);

print(c);