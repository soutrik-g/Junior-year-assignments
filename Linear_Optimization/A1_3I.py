from ortools.linear_solver import pywraplp
from math import sqrt
from math import pi
def LinearProgrammingExample():
    """Linear programming sample."""
    solver = pywraplp.Solver.CreateSolver("SAT")
    if not solver:
        return
    
    a1 = solver.NumVar(0, solver.infinity(), "a1")
    a2 = solver.NumVar(0, solver.infinity(), "a2")
    a3 = solver.NumVar(0, solver.infinity(), "a3" )
    a4 = solver.NumVar(0, solver.infinity(), "a4")
    b = solver.NumVar(0, solver.infinity(), "b")
    
    print("Number of variables =", solver.NumVariables())

    solver.Add(100*a1 + 5*a2 + 10*a3 + a4 -b >= 1)
    solver.Add((-4)*a1 - pi*a2 - 50*a3 + (pi+5)*a4 -b >= 1)
    solver.Add((sqrt(2))*a1 + pi*a2 + 13*a3 + 7*a4 -b >= 1)
    solver.Add((-7)*a1 - (pi/2)*a2 + 13*a3 + (pi+4)*a4 -b >= 1)
    solver.Add(pi*a1 + a4 -b <= -1)
    solver.Add((sqrt(2))*a1 - pi*a2 + 10*a3 + a4 -b <= -1)
    solver.Add((-5)*a1 - pi*a2 + 14*a3 -b <= -1)
    solver.Add(a1 + 2*a2 + 11*a3 + 2*a4 -b <= -1)
    solver.Add(b >= 0)
    solver.Minimize(b)
    print("Number of constraints =", solver.NumConstraints())

    status = solver.Solve()

    if status == pywraplp.Solver.OPTIMAL:
        print("Solution:")
        print("Objective value =", solver.Objective().Value())
        print("a1 =", a1.solution_value())
        print("a2 =", a2.solution_value())
        print("a3 =", a3.solution_value())
        print("a4 =", a4.solution_value())
        print("b =", b.solution_value())
    else:
        print("The problem does not have an optimal solution.")

    print("\nAdvanced usage:")
    print("Problem solved in %f milliseconds" % solver.wall_time())
    print("Problem solved in %d iterations" % solver.iterations())


LinearProgrammingExample()