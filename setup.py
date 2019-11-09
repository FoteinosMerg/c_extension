from distutils.core import setup, Extension

module_1 = Extension('demo',
                sources=['demomodule.c'],
                py_limited_api=True,
                language="c"
            )

def main():
    setup(
        name='demo',
        version='1.0',
        description='Dummy python package with C extension',
        ext_modules=[module_1],
    )

if __name__ == '__main__':
    main()
