# Long way:

import time

class MeasureTime:
    def __init__(self, operation: str):
        self.operation = operation
    def __enter__(self):
        self.start = time.perf_counter_ns()
    def __exit__(self, exc_type, exc_val, exc_tb):  # allows to handle exception
        print(f"{self.operation}: {time.perf_counter_ns() - self.start}ns")

with MeasureTime("sum(range(0, 1000000))"):
    print(sum(range(0, 1000000)))        

# simple way
from contextlib import contextmanager

# utility for profiling code block
@contextmanager
def measure_time(operation):
    import time
    start = time.perf_counter_ns()
    yield   # here the control is transition to the block
    print(f"{operation}: {time.perf_counter_ns() - start}ns")

# Use:
with measure_time("sum(range(0, 1000000))"):
    print(sum(range(0, 1000000)))

