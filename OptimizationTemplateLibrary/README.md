# Optimization Template Library (OTL)

OTL is a cross-platform C++ template library for multi-objective optimization, and the type of decision variable can be any data structures. The OTL contains various ready-to-use modules (multi-objective optimization problems (MOPs), operators, optimization algorithms, and quality indicators), which can be arbitrarily combined through a unified interface. Since generic programming is adopted in the OTL, the optimization algorithms can be used to solve the MOPs of different encodings.

The goal of the OTL is to implement various modules for multi-objective optimization and maximize their running efficiency. However, it does not provide an experimental platform or any engineering project, because C++ has a low development efficiency. A feasible solution is using Python. the project [PyOTL](https://github.com/O-T-L/PyOTL) is created to convert C++ programs of the OTL into Python modules, which are used to implement the project [PyOptimization](https://github.com/O-T-L/PyOptimization) to carry out scientific experiments.

# Acknowledgements

* [Boost](http://www.boost.org/).
* [CMake](http://www.cmake.org/).

# Related Works

* Programming language independent interface for search algorithms ([PISA](http://www.tik.ee.ethz.ch/sop/pisa)).
* [jMetal](http://jmetal.sourceforge.net).
* [MOEA Framework](www.moeaframework.org).
* [Opt4J](http://opt4j.sourceforge.net/).
* [ECJ](http://cs.gmu.edu/~eclab/projects/ecj/).
* [Watchmaker Framework](http://watchmaker.uncommons.org/).
* [Paradiseo](http://paradiseo.gforge.inria.fr/).