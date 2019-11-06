from setuptools import setup, Extension

setup(
    name='demo',
    version='1.0',
    description='Dummy python package with C extension',
    ext_modules=[
        Extension(
            'demo',
            sources=['demomodule.c'],
            py_limited_api=True)
    ],
)
