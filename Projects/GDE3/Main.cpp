/*!
Copyright (C) 2014, 申瑞珉 (Ruimin Shen)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cassert>
#include <iostream>
#include <vector>
#include <random>
#include <boost/format.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <OTL/Crossover/Real/DifferentialEvolution.h>
#include <OTL/Initial/Real/Uniform.h>
#include <OTL/Problem/DTLZ/DTLZ1.h>
#include <OTL/Problem/DTLZ/DTLZ2.h>
#include <OTL/Problem/DTLZ/DTLZ3.h>
#include <OTL/Problem/DTLZ/DTLZ4.h>
#include <OTL/Problem/DTLZ/DTLZ7.h>
#include <OTL/Optimizer/GDE3/GDE3.h>

int main(void)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::dtlz::DTLZ<_TReal> _TProblem;
	typedef otl::crossover::real::DifferentialEvolution<_TReal, _TRandom &> _TCrossover;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::optimizer::gde3::GDE3<_TReal, _TDecision, _TRandom &> _TOptimizer;
	_TRandom random(std::time(0));
	for (size_t repeat = 0; repeat < 30; ++repeat)
	{
		for (size_t nObjectives = 2; nObjectives < 4; ++nObjectives)
		{
			boost::ptr_vector<_TProblem> problems;
			problems.push_back(new otl::problem::dtlz::DTLZ1<_TReal>(nObjectives));
			problems.push_back(new otl::problem::dtlz::DTLZ2<_TReal>(nObjectives));
			problems.push_back(new otl::problem::dtlz::DTLZ3<_TReal>(nObjectives));
			problems.push_back(new otl::problem::dtlz::DTLZ4<_TReal>(nObjectives));
			problems.push_back(new otl::problem::dtlz::DTLZ7<_TReal>(nObjectives));
			std::vector<size_t> nEvaluationsList = {100000, 30000, 100000, 30000, 30000};
			std::vector<std::string> problemNames = {"DTLZ1", "DTLZ2", "DTLZ3", "DTLZ4", "DTLZ7"};
			assert(problems.size() == nEvaluationsList.size() && problems.size() == problemNames.size());
			for (size_t i = 0; i < problems.size(); ++i)
			{
				_TProblem &problem = problems[i];
				const size_t nEvaluations = nEvaluationsList[i];
				const clock_t start = clock();
				const std::vector<_TDecision> initial = otl::initial::real::BatchUniform(random, problem.GetBoundary(), 100);
				_TCrossover crossover(random, 1, problem.GetBoundary(), 0.5);
				_TOptimizer optimizer(random, problem, initial, crossover);
				for (size_t generation = 1; problem.GetNumberOfEvaluations() < nEvaluations; ++generation)
					optimizer();
				const clock_t end = clock();
				const _TReal duration = (_TReal)(end - start) / CLOCKS_PER_SEC;
				std::cout << duration << std::endl;

				boost::filesystem::path path = CMAKE_BINARY_DIR;
				path /= "Output";
				path /= "GDE3";
				path /= problemNames[i];
				path /= (boost::format("%u") % nObjectives).str();
				boost::filesystem::create_directories(path);
				{
					boost::filesystem::ofstream ofs(path / (boost::format("%u.pf") % repeat).str());
					const auto &population = optimizer.GetSolutionSet();
					for (size_t i = 0; i < population.size(); ++i)
					{
						const auto &objective = population[i].objective_;
						for (size_t j = 0; j < objective.size(); ++j)
							ofs << objective[j] << '\t';
						ofs << std::endl;
					}
					ofs.close();
				}
				{
					boost::filesystem::ofstream ofs(path / (boost::format("%u.duration") % repeat).str());
					ofs << duration;
					ofs.close();
				}
			}
		}
	}
	return 0;
}
