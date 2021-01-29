# Introduction

This repo is a response to the challenge from the youtuber [__Mathologer__](https://www.youtube.com/channel/UC1_uAIS3r8Vu6JjXWvastJg), 
in [this](https://www.youtube.com/watch?v=VLbePGBOVeg) video.

This type of idea has already gone through my mind once or twice, but never 
implemented something like it, this challenge was a great push :).

Note that in the video there was given a polynomial to calculate the final answer, 
which is super nice, but this challenge was given with the purpose of not using it,
so I'm not gonna use it here.


# The problem

How many ways there are to make change 
(with infinite the coins of type 1, 5, 10, 25, 50 and 100), of $2000 = 200000 cents.

With the extra of what is the highest n, such that, `$2*10^n` can be calculated, 
with that algorithm, within 1 hour on my setup.


## Generalization

This problem can be seen as, finding the term m of the infinite polynomial (generating function):

    (1 + x + x² + ...) *
    (1 + x⁵ + x¹⁰ + ...) *
    (1 + x¹⁰ + x²⁰ + ...) *
    (1 + x²⁵ + x⁵⁰ + ...) *
    (1 + x⁵⁰ + x¹⁰⁰ + ...) *
    (1 + x¹⁰⁰ + x²⁰⁰ + ...)


# Idea

The obvious way of solving this problem is by limiting each infinite polynomial and then multiplying.
That way is a bit unconfortable, because the limite would be on the same order as the input, and then
we have to multiply 5 times, which each of them have time complexity `O(n²)` even with FFT with time 
complexity `O(n*log(n))` is still unconfortable.

My approach is a math idea that I had for other topic, unrelated math topic.
First we can use the closed form of each infinite polynomial, like so:

    (1 - x)⁻¹ *
    (1 - x⁵)⁻¹ *
    (1 - x¹⁰)⁻¹ *
    (1 - x²⁵)⁻¹ *
    (1 - x⁵⁰)⁻¹ *
    (1 - x¹⁰⁰)⁻¹

We can see that after this multiplication we would have a 1/p(x), where p(x) is

    (1 - x) *
    (1 - x⁵) *
    (1 - x¹⁰) *
    (1 - x²⁵) *
    (1 - x⁵⁰) *
    (1 - x¹⁰⁰)

and this polynomial p(x) can be easilly calculated, being only a polynomial of degree 191.
In the code the polynomial p is called den, because it is the denominator of the expression.

So the problem now is to find the term of degree m of 1/p(x).

If we see 1/p(x) as 

    a0 + a1*x + a2*x² + ...

to get the term of degree 0 is easy, only evaluate `1/p(x)` with `x = 0`;  

to get the term of degree 1 we can subtract a0 (`1/p(x) - a0`), then divide by x (`(1/p(x) - a0)/x`), then evaluate with `x = 0`;

    (1/p(x) - a0)/x = (1 - a0*p(x))/(x*p(x))

...

to get the term of degree m we can subtract a(m-1), then divide by x, then evaluate with `x = 0`, from the previous expression.

With this approach we can get the solution with a time complexity `O(n)` Which is awesome.

With a careful implementation, like I did, we can even see that the space complexity can be `O(1)` 
if we close our eyes to the multiple precision integers, which is surprising.

# Code

To calculate the polynomial p(x), I used a simple Haskell program in `CalculateDenominator.hs`.

I have 2 programs, one with `long int` which was the first attempt, called `moneych.c`, 
then with multiple precision integers of gmp `mpz_t` , called `moneych_gmp.c`.

Both do the same thing, but the first has a limit to the output, and the second hasn't.

## Calculate p(x)

This is done in a very direct way only multiply all of the polynomials:

    den = (1 - x) *
        (1 - x⁵) *
        (1 - x¹⁰) *
        (1 - x²⁵) *
        (1 - x⁵⁰) *
        (1 - x¹⁰⁰)

Then we can retrieve with `denL` the format we are going to use in the c program.

## Calculate the term with degree m

This program is manipulating an expression which is a fraction of 2 polinomials of degree 191.
If we only manipulate these 2 polynomials we can have a space complexity of `O(1)`, 
which is exactly what we are going to do.

The expression can be seen as `num/den` where the initial expression is
`1/den`, where the `den` is the previews calculated polynomial.

With each step we calculate one term, one step as 3 parts.

### First part (calculate the constant term)

The first part is responsable to calculate the constant term of the 
infinite polynomial, which is simply `num(0)/den(0)`, because both parts
are polynomials, when evaluating them with 0 is the same as cutting out 
all the terms wich have x^n, with n > 1, fancy way of saying we only 
keep the constant term. The constant term can be accessed by getting 
the first element of the array, that can be done like `pol[0]`. So
basically

    num(0)/den(0) = num[0]/den[0]

Because den doesn't change we can calculate den(0), for every step as 
only 1. Leaving us with

    num(0)/den(0) = num[0]/den[0] = num[0]

### Second part (removing the constant term)

Here we are going to basically do

    num/den - c = (num - c*den)/den

### Third part (Dividing by x)

Here we are going to basically shift the `num` 
polynomial to the left

    (num/x)/den

# Running it

A simple time command can give enough precision for this purpose.

    $ time ./moneych_gmp 2000
    53995291

    real    0m0,019s
    user    0m0,018s
    sys 0m0,001s

    $ time ./moneych_gmp 20000
    4371565890901

    real    0m0,100s
    user    0m0,098s
    sys 0m0,002s

    $ time ./moneych_gmp 200000
    427707562988709001

    real    0m0,915s
    user    0m0,910s
    sys 0m0,006s

    $ time ./moneych_gmp 2000000
    42677067562698867090001

    real    0m9,332s
    user    0m9,310s
    sys 0m0,021s

    $ time ./moneych_gmp 20000000
    4266770667562669886670900001

    real    1m31,882s
    user    1m31,674s
    sys 0m0,200s

I didn't run it but, because it is linear could answer for

    200000000 cents = $2000000 = $2*10⁶

and that is the limit in my setup, which would take 15-16min.

6 is very far from 100 but this approach could be used for other generating functions.

# After Thought

After some thought the strategy of rewriting the numerator of a fraction, 
with the same procedure, makes me think of a linear operator, like a matrix.

In This section I'm going to extend on this idea, and maybe derive an expression
to answer this question. Which would mean a kinda constant time complexity program.

The polynimials we are talking are of degree 191, i.e. not a simple task to do by hand,
so to explain I'm gonna use a simpler example.

    (a + bx + cx²)/(1 + dx + ex²) -> 
    ( ((a + bx + cx²) - a*(1 + dx + ex²)) / x )/(1 + dx + ex²) =
    ( (b - ad) + (c - ae)x )/(1 + dx + ex²)
    
    |-d 1 0| |a| = |b - ad|
    |-e 0 1| |b|   |c - ae|
    | 0 0 0| |c|   |0     |
    
It can be seen that with the denominator as the polynomial 
    a0 + a1*x + a2*x² + ... + an*x^n
this process is the same as applying the matrix (n+1)x(n+1)

    A = |-a1 1 0 0 ... 0|
        |-a2 0 1 0 ... 0|
        |-a3 0 0 1 ... 0|
        |... . . . ... .|
        |-an 0 0 0 ... 1|
        |  0 0 0 0 ... 0|

To derive the expression that answers the problem we would have to 
calculate for some `m` the `A^m` and that can be done easily if we 
have an Eigendecomposition of A, 

    A = PA * DA * PA^T
    A^m = PA * DA^m * PA^T

DA^m is easy to calculate, basically a `^m` pointwise.

The hard part is to calculate the eigenvalues in DA.
Especially in a symbolic way, because we don't want an approximation,
because we are calculating for a number `m` with an arbitrary precision.
