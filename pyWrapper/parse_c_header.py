import copy
import sys

sys.path.extend(['.', '..'])

from pycparser import c_parser, c_ast



def parse_function(func):
    name = func.name
    args = func.children()[0][1].children()[0][1]
    input_args = [{'name':a.type.declname,'type':a.type.type.names[0]} for a in args if type(a.type)==c_ast.TypeDecl]
    output_args = [{'name':a.type.type.declname,'type':a.type.type.type.names[0]} for a in args if type(a.type)==c_ast.PtrDecl]
    return {'name':name, 'input_args':input_args, 'output_args':output_args}

def parse_header(header_filename):
    with open(header_filename,'r') as header_file:
        parser = c_parser.CParser()
        try:
            node = parser.parse(header_file.read(), filename='<stdin>')
        except c_parser.ParseError:
            e = sys.exc_info()[1]
            print("Parse error:" + str(e))


    functions  = [n for n in node if type(n)==c_ast.Decl]


    parsed_functions = []
    for func in functions:
        parsed_functions.append(parse_function(func))

    return parsed_functions


