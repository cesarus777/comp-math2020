# lab1

## Description
In this lab the error between analytical and numerical derivative is investigated.

Used functions: 
1. ![sin(x^2)](https://latex.codecogs.com/png.latex?%5Cdpi%7B150%7D%20%5Cbg_white%20%5Ctiny%20f%28x%29%20%3D%20sin%28x%5E2%29 "function 1")
1. ![cos(sin(x))](https://latex.codecogs.com/png.latex?%5Cdpi%7B150%7D%20%5Cbg_white%20%5Ctiny%20f%28x%29%20%3D%20cos%28sin%28x%29%29 "function 2")
1. ![exp(sin(cos(x)))](https://latex.codecogs.com/png.latex?%5Cdpi%7B150%7D%20%5Cbg_white%20%5Ctiny%20f%28x%29%20%3D%20exp%28sin%28cos%28x%29%29%29 "function 3")
1. ![ln(x + 3)](https://latex.codecogs.com/png.latex?%5Cdpi%7B150%7D%20%5Cbg_white%20%5Ctiny%20f%28x%29%20%3D%20ln%28x%20&plus;%203%29 "function 4")
1. ![sqrt(x + 3)](https://latex.codecogs.com/png.latex?%5Cdpi%7B150%7D%20%5Cbg_white%20%5Ctiny%20f%28x%29%20%3D%20%5Csqrt%7Bx%20&plus;%203%7D "function 5")

Formulas for numerical derivatives as they are mentioned in the code:  
 ![formula (2)](https://latex.codecogs.com/gif.latex?%5Cdpi%7B150%7D%20%5Ctiny%20%5Cbullet%20%5Cquad%20%7Bf%7D%27_%7Bnum%7D%28x_0%29%20%3D%20%5Cfrac%7Bf%28x_0&plus;h%29-f%28x_0%29%7D%7Bh%7D%20%5Cqquad%20%282%29 "formula (2)")  
![formula (3)](https://latex.codecogs.com/png.latex?%5Cdpi%7B150%7D%20%5Cbg_white%20%5Ctiny%20%5Cbullet%20%5Cquad%20%7Bf%7D%27_%7Bnum%7D%28x_0%29%20%3D%20%5Cfrac%7Bf%28x_0%29-f%28x_0-h%29%7D%7Bh%7D%20%5Cqquad%20%283%29 "formula (3)")  
![formula (4)](https://latex.codecogs.com/png.latex?%5Cdpi%7B150%7D%20%5Cbg_white%20%5Ctiny%20%5Cbullet%20%5Cquad%20%7Bf%7D%27_%7Bnum%7D%28x_0%29%20%3D%20%5Cfrac%7Bf%28x_0&plus;h%29-f%28x_0-h%29%7D%7B2h%7D%20%5Cqquad%20%284%29 "formula (4)")  
![formula (5)](https://latex.codecogs.com/png.latex?%5Cdpi%7B150%7D%20%5Cbg_white%20%5Ctiny%20%5Cbullet%20%5Cquad%20%7Bf%7D%27_%7Bnum%7D%28x_0%29%20%3D%20%5Cfrac%7B4%7D%7B3%7D%5Cfrac%7Bf%28x_0&plus;h%29-f%28x_0-h%29%7D%7B2h%7D%20-%20%5Cfrac%7B1%7D%7B3%7D%5Cfrac%7Bf%28x_0&plus;2h%29-f%28x_0-2h%29%7D%7B4h%7D%20%5Cqquad%20%285%29 "formula (5)")  
![formula (6)](https://latex.codecogs.com/png.latex?%5Cdpi%7B150%7D%20%5Cbg_white%20%5Ctiny%20%5Cbullet%20%5Cquad%20%7Bf%7D%27_%7Bnum%7D%28x_0%29%20%3D%20%5Cfrac%7B3%7D%7B2%7D%5Cfrac%7Bf%28x_0&plus;h%29-f%28x_0-h%29%7D%7B2h%7D%20-%20%5Cfrac%7B3%7D%7B5%7D%5Cfrac%7Bf%28x_0&plus;2h%29-f%28x_0-2h%29%7D%7B4h%7D%20&plus;%20%5Cfrac%7B1%7D%7B10%7D%5Cfrac%7Bf%28x_0&plus;3h%29-f%28x_0-3h%29%7D%7B6h%7D%20%5Cquad%20%286%29 "formula (6)")  

The errors are computing for the next ![x_0](https://latex.codecogs.com/gif.latex?%5Cdpi%7B150%7D%20%5Ctiny%20x_0) and  ![h](https://latex.codecogs.com/gif.latex?%5Cdpi%7B150%7D%20%5Ctiny%20h):  
![x_0=5](https://latex.codecogs.com/gif.latex?%5Cdpi%7B150%7D%20%5Ctiny%20x_0=5)  
![h_n](https://latex.codecogs.com/gif.latex?%5Cdpi%7B150%7D%20%5Ctiny%20h_n%3D%5Cfrac%7B1%7D%7B2%5E%7Bn-1%7D%7D%2C%5C%20n%3D1%2C...%2C21)

## Run
After [building and installing](../README.md) you can run `lab1` using combination of the following options:
- just `lab1` - compute errors and plot graphs in several windows
- `lab1 -o 'output-type'` - determine plot output type, can be `jpg`,`png`or`window`, `window` is default
- `lab1 -v` or `lab1 --verbose` - enable verbose logging, can be useful for debugging
- `lab1 -size 'width'x'height'` - set plotting image size, `'width'` and `'height'` must be unsigned integers, `1600x900` is default
- `lab1 --nonlinear` - plot with y-axis logarithmic scale
- `lab1 --dump-data` - dump errors data to .txt files into output directory
- `lab1 -od 'outdir'` - change output directory to `'outdir'`, `'outdir'`should be already created directory, `output/` is default
- `lab1 -q` - if output type is `window` don't plot graphs (quiet run)

