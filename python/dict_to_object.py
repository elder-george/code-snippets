from types import SimpleNamespace

# Converting dictionary into an object
d = {'a': 1, 'b': 'a string', 'c': [1, 2, 3, 4]}
o = SimpleNamespace(**d)

# Parsing JSON into an object
import json

j = '{"a": 1, "b": "a string", "c": [1, 2, 3, 4]}'
o = json.loads(j, object_hook=lambda d: SimpleNamespace(**d))
