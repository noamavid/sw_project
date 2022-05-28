from setuptools import setup, find_packages, Extension

setup(
    name="kmeans_capi",
    version=1,
    description="c api for kmeans algorithm",
    packages= find_packages(),
    classifiers=[
            'Development Status :: 3 - Alpha',
            'License :: OSI Approved :: GNU General Public License v2 (GPLv2)',
            'Natural Language :: English',
            'Programming Language :: Python :: 3 :: Only',
            'Programming Language :: Python :: Implementation :: CPython',
        ],
    py_modules = [],
    package_dir = {"":"src"},
    ext_modules= [
        Extension(
            "kmeans_capi",
            ['main.c'],
        ),
    ]
)
