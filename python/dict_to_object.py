from types import SimpleNamespace

# Converting dictionary into an object
d = {'a': 1, 'b': 'a string', 'c': [1, 2, 3, 4]}
o = SimpleNamespace(**d)

# Parsing JSON into an object
import json

j = '{"a": 1, "b": "a string", "c": [1, 2, 3, 4]}'
o = json.loads(j, object_hook=lambda d: SimpleNamespace(**d))

# same works with types, too.
from dataclasses import dataclass
from typing import List

@dataclass
class MyDataClass:
    a: int
    b: str
    c: List[int]

o = json.loads(j, object_hook=lambda d: MyDataClass(**d))

# or
from typing import NamedTuple

class MyNamedTuple(NamedTuple):
    a: int
    b: str
    c: List[int]

o = json.loads(j, object_hook=lambda d: MyNamedTuple(**d))
