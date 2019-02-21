# SimplexAlgo

*Author: Lucas Turci*

This is a project made by me with the purpose of solving linear programming problems such as this one:  
> Maximize f(x) = c<sub>1</sub> x<sub>1</sub> +  c<sub>2</sub> x<sub>2</sub> + ... + c<sub>m</sub> x<sub>m</sub> <br>
> Subject to: <br>
> a<sub>11</sub> x<sub>1</sub> +  a<sub>12</sub> x<sub>2</sub> + ... + a<sub>1n</sub> x<sub>n</sub> ≤ b<sub>1</sub> <br>
> a<sub>21</sub> x<sub>1</sub> +  a<sub>22</sub> x<sub>2</sub> + ... + a<sub>2n</sub> x<sub>n</sub> ≤ b<sub>2</sub> <br>
> ... <br>
> a<sub>n1</sub> x<sub>1</sub> +  a<sub>n2</sub> x<sub>2</sub> + ... + a<sub>nn</sub> x<sub>n</sub> ≤ b<sub>n</sub> <br>
>

My crappy implementation of *simplex* algorithm solves such problems easily, but not so fast. The assimptotic complexity of *simplex* best implementation is still exponential, but in average it runs very fast. This implementation does not care to make any major optimization such as to cut some restrictions or stuff like that. In addition, to this moment **this program requires that all b<sub>i</sub> are non-negative.**

### How to use
*Requirements: g++ compiler with c++ 11 or above*

* Clone this repository to your computer, using `git clone https://github.com/lucasturci/SimplexAlgo`
* Then enter the directory, `cd SimplexAlgo`
* There is a Makefile in the directory, just write `make` in your command line to compile.
* To run, `./simplex file`, in which file is a text input file described below 

### Input File
This is where you describe your linear problem. 

* The first line contains the function you want to maximize, in the following format: <br>
[number *] \<first variable\> {+|-} [number *] \<second variable\> {+|-} ... {+|-} [number *] \<m-th variable\> <br>
* The second line **must be** an empty line <br>
The following n lines will be in format: <br>
* [number *] \<first variable\> {+|-} [number *] \<second variable\> {+|-} ... {+|-} [number *] \<m-th variable\> <= \<number\> <br>

*Variables* can be any string with alphanumeric characters (but they mustn't start with a digit), and *number* can be any real number.

For example, 
> file.in
> ------------------
> 2 * a + 3 * c - x <br>
> <br>
> a + c <= 15 <br>
> 3 * a - y <= 100 <br>
>
>
