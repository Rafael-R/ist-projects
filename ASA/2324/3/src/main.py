from pulp import LpProblem, LpVariable, LpMaximize, LpInteger, lpSum, value, GLPK

toys = {}
packs = {}
used_in = {}

# Read Input
t, p, max = [int(_) for _ in input().split()]

for toy in range(1, t + 1):
    l, c = [int(_) for _ in input().split()]
    toys[toy] = [l, c]
    used_in[toy] = []

for pack in range(1, p + 1):
    i, j, k, packs[pack] = [int(_) for _ in input().split()]
    for _ in [i, j, k]:
        used_in[_] += [pack]

# Problem Definition
problem = LpProblem('problem', LpMaximize)

# Decision Variables: Toys to produce
toy_vars = LpVariable.dicts("Toy", toys, 0, None, LpInteger)

# Decision Variables: Packs to produce
pack_vars = LpVariable.dicts("Pack", packs, 0, None, LpInteger)

# Add objective function: maximize profit
problem += (lpSum([toy_vars[t] * toys[t][0] for t in toys] + \
                  [pack_vars[p] * packs[p] for p in packs]), 
            "objective")

# Add constraint for factory's maximum production
problem += (lpSum([toy_vars] + ([pack_vars] * 3)) <= max, 
            "total of toys")

# Add constraints for individual toy production
for t in toys:
    problem += (lpSum(toy_vars[t] + \
                [pack_vars[p] for p in used_in[t]]) <= toys[t][1], 
                f"total of toy {t}")

# Solve the problem
problem.solve(GLPK(msg=False))

# Print the result
print(value(problem.objective))