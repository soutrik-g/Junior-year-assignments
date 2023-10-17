from ortools.linear_solver import pywraplp

def LinearProgrammingExample():
    """Linear programming sample."""
    solver = pywraplp.Solver.CreateSolver("SAT")
    if not solver:
        return
    
    f = solver.NumVar(0, solver.infinity(), "f")
    x = [solver.NumVar(0, solver.infinity(), "x[%i]" % i) for i in range(1, 7)]
    b = [solver.NumVar(0, solver.infinity(), "b[%i]" % i) for i in range(1, 3)]
    
    print("Number of variables =", solver.NumVariables())

    solver.Add(f + x[0] >= 6)
    solver.Add(f + x[0] + x[1] >= 5)
    solver.Add(f + x[0] + x[1] + x[2] >= 3)
    solver.Add(f + x[1] + x[2] + x[3] >= 3)
    solver.Add(f - b[0] + x[2] + x[3] + x[4] >= 5)
    solver.Add(f - b[1] + x[3] + x[4] + x[5] >= 4)
    solver.Add(f + x[4] + x[5] >= 6)
    solver.Add(f + x[5] >= 8)
    solver.Add(x[0] + x[1] + x[2] + x[3] + x[4] + x[5] <= 6)
    solver.Add(f >= 0)
    for i in range(6):
        solver.Add(x[i] >= 0)
    for i in range(2):
        solver.Add(b[i] >= 0)
    solver.Add(b[0] + b[1] - f == 0)
    solver.Minimize(80*f + 21*(x[0] + x[1] + x[2] + x[3] + x[4] + x[5]))
    print("Number of constraints =", solver.NumConstraints())

    status = solver.Solve()

    if status == pywraplp.Solver.OPTIMAL:
        print("Solution:")
        print("Objective value =", solver.Objective().Value())
        print("f =", f.solution_value())
        for i in range(6):
            print("x[%i] =" % i, x[i].solution_value())
        for i in range(2):
            print("b[%i] =" % i, b[i].solution_value())
    else:
        print("The problem does not have an optimal solution.")

    print("\nAdvanced usage:")
    print("Problem solved in %f milliseconds" % solver.wall_time())
    print("Problem solved in %d iterations" % solver.iterations())


LinearProgrammingExample()