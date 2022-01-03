from distutils.core import setup, Extension

module = Extension('geom_computation',
                   sources=['py_geom_computation.c','geom_computation.cpp'],
                   libraries=['Geographic'])

setup(
    name='geom_computation',
    version='1.0',
    description='WGS84 geometric computation',
    author='Paul Rousse',
    author_email='paul.rousse@airbus.com',
    ext_modules=[module])
