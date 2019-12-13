import pandas as pd


def maxdrawdown(ret):
    dd, curmax, cumret = float('inf'), 1, 1
    for r in ret:
        cumret *= 1 + r
        dd = min(dd, cumret / curmax)
        curmax = max(curmax, cumret)

    return 1 - dd


def IR(ret, benchmark=None):

    if benchmark is not None:
        alpha = ret - benchmark
        return alpha.mean() / alpha.std(ddof=1)
    else:
        return ret.mean() / ret.std(ddof=1)


class PerformanceEvaluator:
    def __init__(self, ret, benchmark=None, freq='M'):
        self.ret = ret
        self.benchmark = benchmark

        if freq == 'D':
            self.annualize_factor = 250 ** 0.5
        elif freq == 'M':
            self.annualize_factor = 12 ** 0.5
        elif freq == 'Y':
            self.annualize_factor = 1

    def evaluate(self):
        return {
            'IR': IR(self.ret, self.benchmark) * self.annualize_factor,
            'MaxDD': maxdrawdown(self.ret)
        }
