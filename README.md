# Optimization Template Library (OTL)

OTL is a cross-platform C++ template library for multi-objective optimization, and the type of decision variable can be any data structures. The OTL contains various ready-to-use modules (multi-objective optimization problems (MOPs), operators, optimization algorithms, and quality indicators), which can be arbitrarily combined through a unified interface. Since generic programming is adopted in the OTL, the optimization algorithms can be used to solve the MOPs of different encodings.

The goal of the OTL is to implement various modules for multi-objective optimization and maximize their running efficiency. However, it does not provide an experimental platform or any engineering project, because the C++ has a low development efficiency. To improve the development efficiency, a feasible solution is using [Python](http://www.python.org). The project [PyOTL](https://github.com/O-T-L/PyOTL) is created to convert C++ programs of the OTL into Python modules, which are used to implement the project [PyOptimization](https://github.com/O-T-L/PyOptimization) to carry out scientific experiments.

Notably, since the standard C++11 is used in the OTL, please check if your compiler supports it.

# License

This project is released as the open source software with the GNU Lesser General Public License version 3 ([LGPL v3](http://www.gnu.org/licenses/lgpl-3.0.html)).

# Acknowledgements

OTL is originated in professor [Jinhua Zheng](http://daoshi.xtu.edu.cn/index.php?id=725)'s lab since 2009. Professor Jinhua Zheng is my master's and doctoral tutor. I'm grateful for his support to let me spend 6 years to develop this project.

The following tool and library are used in this project:

* [CMake](http://www.cmake.org).
* [Boost](http://www.boost.org).

# Related Works

* Programming language independent interface for search algorithms ([PISA](http://www.tik.ee.ethz.ch/sop/pisa)).
* [jMetal](http://jmetal.sourceforge.net).
* [MOEA Framework](www.moeaframework.org).
* [Opt4J](http://opt4j.sourceforge.net).
* [ECJ](http://cs.gmu.edu/~eclab/projects/ecj).
* [Watchmaker Framework](http://watchmaker.uncommons.org).
* [Paradiseo](http://paradiseo.gforge.inria.fr).