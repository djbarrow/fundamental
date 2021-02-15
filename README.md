# What is fundamental
Fundamental is a c command line program, a rough diamond which barely works beautifully but is my most original program to date. I've recently found out that it uses a technique called [Symbolic Regression](https://en.wikipedia.org/wiki/Symbolic_regression). It is also being marketed as Dirac because it has the inhuman talent of Dirac the famous Relativistic Detached almost Autistic Strange Genius Feynmans hero to pull inhuman crazy equations out of the air which fit data better than curve fitting, it can curve fit a sin x yes it can!!!!!, primative traditional polynomial curve fitting can't, I'm a wanabee shaman & I think my program from the future gave him a hand in the cosmic consciousness..... thank you Jesus praise you Jesus for inspiring me to do something which might be significant .... prove me wrong!!!!!!!


Fundamental is a toolbox for finding relationships between

*   numbers, e.g. is there any relationship between the numbers PI, e, the square root of 2?
*   sequences of numbers, e.g. the sequence 2, 4, 6, 8, 10, ... It could replace/compliment this [encylopedia of integer sequences (OEIS)](http://oeis.org/) website.

It could have found the following formulas which started quantum physics had it been around in the 1860's & 1890's. [Balmer Series](http://www.colorado.edu/physics/2000/quantumzone/balmer.html)  

It could have found [Planck's Law](http://en.wikipedia.org/wiki/Planck%27s_law) if on a supercomputer or used SETI at home's computer network.  

It could with some improvements in pattern matching have found Mendeleev's periodic table of elements or did some of Murray Gell Mann's work for which he got a Nobel Prize in 1969 for developing the Standard Model in Physics.

To date I've had little luck in coming up with a fast formula for factorising large large numbers but God loves a tryer.
However it did find this gem which looks dimensionally correct
relating the constants of physics to the golden ratio.
A program I wrote called fundamental might have made a scientific discovery

Found match error=4.714556e-05 fundamental constant name=mass_of_neutron value=1.674929e-27
(mass_of_proton + (mass_of_electron + (mass_of_electron * golden_ratio)))
(1.672623e-27  + (9.109390e-31  + (9.109390e-31  * 1.618034e+00 )))

In short it's for hacking the laws of nature & mathematics

it is in some ways related to curve fitting ( or more percisely a program for finding integer relations ) where you have some data & are looking for a formula to describe it, curve fitting can usually be done by a human, fundamental is more a brute force method where you tell fundamental what operators are allowed + - x / bitwise and or etc. The constants, e PI, it starts with the most simple sums & progresses towards the more complex.

My original motivation for writing it was while reading Max Borns book on the theory or relativity I fell over a fantastic insight one of Maxwells equations.  

<center>sqrt(permability of material*permittivity of material ) = speed of light.  
</center>

They didn't at the time know that light was electromagnetic radiation. What I fell over was that there were redundant constants of physics & if a program mas made to search for interrelations between the constants of physics on might by brute force find redundancies, simplifications & potentially new laws of of physics, I was onto a winner which I knew I could pull off. I had the first prototype of "Fundamental" laws of Physics written in 36 hours around 300 lines of code. It found existing laws of physics but no new ones.

It uses reverse polish notation (RPN) explained [here along with some explanations of the internals of fundamental not covered anywhere else](http://sinscienceandspirituality.blogspot.ie/2018/05/the-joys-of-reverse-polish-notation.html), the beauty of reverse polish notation is that there is no need for brackets the sum 2+3x2=8 or 10 depending on whether you interpret the sum as (2+3)x2 or 2+(3x2) reverse polish notation gets us out of this bind because the order of the numbers & operators gaurantees a unique & unambiguous algorithmic way to do the sum. It also makes it very easy to sequence or "count" through legal sums algorithmically & be gauranteed that each sum is unique.  
e.g.  
2+3 is an illegal RPN sum as the + binary operator needs 2 numbers to its left.  
2 3 + in RPN is equal to 5.

This is the beautiful idea which made funamental possible. The only conditions you need to make in RPN to do any equation is that there needs to be one less binary operator than numbers in the equation. The first binary must have at least 2 numbers to it's left A unary operatior has to have at least 1 number to its left

e.g.  
5 ! ! ! 2 3 + 5 + + is a legal sum ! being the factorial unary operator.

Also the equations generated using RPN are easily countable. e.g. it is easily algorithmically possible to write a program for every possible RPN equation with constraints. ( an example of constraints are only numbers between 0 and 5 are allowed & only the + & * operators are allowed * being the multiplication operator.

One of the thing I've had no success with fundamental is finding a formula for the nth prime, primality testing & pulling out the smallest or largest factor from a very big number.

Fundamental is a step towards automatic algorithm generation. I am very proud of Fundamental & have great hopes for it, it's by far my best idea to date,

Although my program doesn't use genetic algorithms, it brute force searches a solution space of RPN sums, however, it can be configured to use successive approximation (I read about genetic programming before inventing fundamental and I'm sure i got inspired by wondering how data mining works).  My program is almost identical in functionality to Eureqa & I am sure can find sums Eureqa can't as Eureqa calculates fitnesses of guesses & tries to find genetically fitter answers which won't work in cases where fundamental will,if I'm doing a bad job at explaining how fundamental works please watch [the following beautiful videos of Eureqa](http://formulize.nutonian.com/documentation/eureqa/). [A Isralei professors Eureqa moment.](http://www.haaretz.com/israel-news/an-israeli-professor-s-eureqa-moment-1.410881) Tony also has some code written but his [open source eureqa](https://github.com/verdverm) seems too simple to be as mature as fundamental, it is designed for paralell processing something yet to be done in fundamental. Tony also implemented [PGE](https://www.lri.fr/~hansen/proceedings/2013/GECCO/proceedings/p1021.pdf) but in python which means it would be slow. One last link for reference [Quora thread for alternatives to Eureqa](https://www.quora.com/Are-there-any-good-alternatives-to-Nutonian-Eureqa-Formulize-for-curve-fitting), [here is the deep learning approach to curve fitting](http://neuralnetworksanddeeplearning.com/chap4.html)

It's open source & I hope someday it will help someone solve significant problems, if it does please give me some credit for helping you on your way. It may be of use in Mathematics, Chemistry, Physics & Economics for Modeling data without a known formula. The program could be enhanced to do automatic algorithm generation using an open source forth language engine like Paflof or else Java bytecode. however it will be about 20 years before computation speeds will make this possible. Here is a description of what it how it works & what it currently does.

Fundamental internally uses reverse polish notation algorithm to search a solution space for a formula given test data, it can search for formulas for sequences of numbers as well as relationships between constants. To date it was successfully able to find the following by a brute force solution space search for..  
generate the [Taylor Series for e(x)](http://en.wikipedia.org/wiki/Taylor_series)  
An estimate formula for the nth prime.  
Design an n bit adder using only boolean logic.  
It could have found the formula for the Balmier lines in the hydrogen spectrum

[The Miraculous Bailey-Borwein-Plouffe Pi Algorithm](http://en.wikipedia.org/wiki/Bailey%E2%80%93Borwein%E2%80%93Plouffe_formula) [Borweins Algorithm](http://en.wikipedia.org/wiki/Borwein's_algorithm) was found by a similar ( but fundamentally much different ) program.

## Example

When set up properly you can enter a sequence like  
5,8,11,14,17 as follows  
fundamental -h 5 -m 5 -i 5 5 8 11 14 17  
Setting -h to 5 sets the maximum integer in the solution space to 5.  
Setting -m to 5 sets the stack depth or the maximum complexity of the sum to 5 term.  
Setting -i to 5 tells fundamental that there are 5 terms following which contain the sequence.  
The output of the testrun is:  
5 3 n[0] * +  
(5 + (3 * n[0]))  
5 -3 -n[0] * +  
(5 + (-3 * -n[0]))  
So the sequence formula is y=f(n[0])=5+(3 x n[0]), n[0] being an integer. 

## Accolades & Delusions of Grandeur ;)

### Pio a Friend and Fundamental

Pio is a bright guy, speaks 5 languages, Degree in numerical computation & mathematics currently doing a hdip in cloud computing. I've been 6 years trying to convince Pio that Fundamental was more than curve fitting. After reading Carsten Otte's compliment he decided he'd better investigate himself. Now he's spouting things like GO FOR IT!!, Smart Cookie D.J., uv cracked it this time D.J. Google can't help but buy it. Now he is in disbelief that nothing like it has ever been done before. Stuart Russell & [Peter Norvig](http://norvig.com) <a>in Artificial Intelligence a modern approach hint that it can be done but is totally impractical, my program proves otherwise. Pio has agreed to write a fisher price web interface for fundamental & explore data mining possibilities which he is learning in the cloud computing course.... Thanks Pio.</a>

<a>

A colleage at Pace in West Yorkshire who knew Richard Feynman personally said Fundamental was bloody useful.

A colleague at IBM Boeblingen Carsten Otte had this to say...  
I browsed through the code of fundamental a bit.  
I find the idea brilliant, and I think this might be worth drilling deeper.  

</a>
<a>
 Andrea said this after examining the code and me fixing a minor bug to get it going for him.
Hello D.J.
sorry for the long time to answer...
i have had too many argument to study to understand well that ,,,
I think your work it's absolutely fabulous !
</a>

<a></a>[Fabrice Bellard](http://en.wikipedia.org/wiki/Fabrice_Bellard) <a>didn't directly sing it's praises but here is what he'd to say  
Someone told me about a similar project some time ago and I find the idea interesting. Of course the problem is to limit the exponential growth of the search space. I guess that an exhaustive search cannot lead to very interesting discoveries because the programs are too small. But there may be ways to explore a subspace of the "interesting" possible programs with a well chosen language and search algorithm.</a>

<a></a>

<a>I guess that you already know the</a> [PSLQ algorithm](http://en.wikipedia.org/wiki/Integer_relation_algorithm) [( very intersting pdf here on the topic )](http://arminstraub.com/files/pslq.pdf)used to find the BBP Pi formula you mention. It just finds a linear combination with small integer coefficients of constants giving a zero result. It is very limited but already quite effective to find interesting relations.

Most importantly is only 2k lines of code.  

This is not curve fitting as defined by Wikipedia, curve fitting can be done using pen & paper & it gauranteed to produce an answer for the class of problems it solves, fundamental typically does billions of calculations when used for curve fitting without gaurantee of an answer & can handle classes of problems not handleable by traditional curve fitting or Mathematica.

## Caveats

Yes the code is buggy if you aren't acquainted with the gdb debugger now is the time to get familiar, the code compiles using gcc & runs on Linux & possibly Cygwin for Windows.

The section of the code enabled loops ( e.g. needed to find the Taylor series expansion formula for e(x) ) by the MAX_NUM_LOOPVARS is hopelessly broken, don't use it & #ifdef it out, I'm currently rewriting fundamental so that it can develop code automatically for things like bubblesorts automatically rather than just the rather limited loop.

## Documentation

Fundamental now contains a Fundamental Overview.doc readme explaining most of the internals of fundamental, please feel welcome to send questions to me at barrow_dj@yahoo.com and I will add detail to the document explaining your concern.

P.S. Remember gdb is your friend when pulling out bugs use it you can call print_sum directly from the debugger to see how the sum trials are progressing.
