from ortools.linear_solver import pywraplp


def LinearProgrammingExample():
    """Linear programming sample."""
    solver = pywraplp.Solver.CreateSolver("SAT")
    if not solver:
        return

    x1 = solver.NumVar(0, solver.infinity(), "x1")
    x2 = solver.NumVar(0, solver.infinity(), "x2")
    x3 = solver.NumVar(0, solver.infinity(), "x3") 
    
    print("Number of variables =", solver.NumVariables())

    solver.Add(70*x1 + 100*x2 + 50*x3 <= 60000)
    solver.Add(2*x1 + 4*x2 + 2*x3 <= 600)
    solver.Add(x1 >= 50)
    solver.Add(x2 >= 20)
    solver.Add(x3 >= 30)    
    solver.Add(x1 >= 0)
    solver.Add(x2 >= 0)
    solver.Add(x3 >= 0)
    solver.Maximize(130*x1 + 50*x2 + 70*x3)

    print("Number of constraints =", solver.NumConstraints())
    
    status = solver.Solve()

    if status == pywraplp.Solver.OPTIMAL:
        print("Solution:")
        print("Objective value =", solver.Objective().Value())
        print("x1 =", x1.solution_value())
        print("x2 =", x2.solution_value())
        print("x3 =", x3.solution_value())
    else:
        print("The problem does not have an optimal solution.")

    print("\nAdvanced usage:")
    print("Problem solved in %f milliseconds" % solver.wall_time())
    print("Problem solved in %d iterations" % solver.iterations())


LinearProgrammingExample()