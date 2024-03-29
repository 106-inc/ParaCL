from os import path
import lit.formats

config.name = 'ParaCL testing'
config.test_format = lit.formats.ShTest(True)

config.suffixes = ['.cl']

config.test_source_root = path.dirname(__file__)
config.test_exec_root = path.join(path.dirname(__file__), '../build/')

config.substitutions.append(('%ParaCLi',
    path.join(path.dirname(__file__), '../build/ParaCLi')))

config.substitutions.append(('%ParaCL',
    path.join(path.dirname(__file__), '../build/ParaCL')))
