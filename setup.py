from setuptools import setup, Extension

def main():
    setup(
        name='demo',
        version='1.0',
        description='Dummy python package with C extension',
        ext_modules=[
            Extension(
                'demo',
                sources=[
                    'demomodule.c',
                ],
                py_limited_api=True,
                language="c"
            )
        ],
    )

if __name__ == '__main__':
    main()
