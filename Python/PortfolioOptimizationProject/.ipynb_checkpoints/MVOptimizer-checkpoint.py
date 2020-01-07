

# Note the use of matrix type in model cvxopt and np.matrix, these two types are different
# cvxopt only accept matrix type defined in cvxopt
# check cvxopt's documentation for more information
# http://cvxopt.org/userguide/solvers.html#problems-with-linear-objectives
# https://cvxopt.org/documentation/index.html

from cvxopt import matrix, solvers
import numpy as np

class MVOptimizer():
    
    def __init__(self, maxiter=1000, show_progress=False):
        self.show_progress = show_progress
        self.maxiter = maxiter

    def optimize(self, ret, cov):
        """
        Max return, constrains on risk
        """
            
        cov = np.matrix(cov)
        
        # positive definite check
        if not np.all(np.linalg.eigvals(cov) > 0):
            print('Not positive definite.')
            mat = cov * cov.T
            w, v = np.linalg.eig(mat)
            sqrt_w, v = np.diag(np.sqrt(w)), np.matrix(v)
            cov =  v * sqrt_w * np.linalg.inv(v)
        
        # positive definite check may yield a plural value matrix
        # thus use cov.real, note plural part is tiny e^-10
        ret, cov, N = matrix(-ret), matrix(cov.real), len(ret)
        
        ## nonlinear constraints
        def func(x=None, z=None):
            if x is None:
                return 1, matrix(1/N, (N, 1))
            
            # risk constraints and its matrix derivative
            f = x.T * cov * x - 0.0064
            df = x.T * (cov + cov.T)
            if z is None:
                return f, df
            return f, df, z[0, 0] * (cov + cov.T)
        
        ## linear constrains
        ## note all the constraints are 'less and equal'
        
        # no shortselling
        g1 = matrix(np.diag(np.ones(N) * -1))
        h1 = matrix(np.zeros(N))
        
        # weights upperlimits
        g2 = matrix(np.diag(np.ones(N)))
        h2 = matrix(np.ones(N) * 0.05)
        g, h = matrix([g1, g2]), matrix([h1, h2])
        
        # weights sum equals 1
        a = matrix(np.ones(N)).T
        b = matrix(1.0, (1, 1))
        
        ## option for solver
        
        # print convergence table
        solvers.options['show_progress'] = self.show_progress
        
        # max iteration
        solvers.options['maxiters'] = self.maxiter
        
        sol = solvers.cpl(ret, func, g, h, A=a, b=b)
        return sol['x']