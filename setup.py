try:
    from setuptools import setup, Extension
except ImportError:
    from distutils.core import setup, Extension


long_description = """
scheddl is a python interface to the SCHED_DEADLINE scheduler in Linux.
SCHED_DEADLINE is a CPU scheduler available in the Linux kernel since version 3.14 based on the Earliest Deadline First
(EDF) and Constant Bandwidth Server (CBS) algorithms, supporting resource reservations: each task scheduled under such
policy is associated with a budget Q (aka runtime), and a period P, corresponding to a declaration to the kernel that
Q time units are required by that task every P time units, on any processor. This makes SCHED_DEADLINE particularly
suitable for real-time applications, like multimedia or industrial control, where P corresponds to the minimum time
elapsing between subsequent activations of the task, and Q corresponds to the worst-case execution time needed by each
activation of the task.
"""


setup(
    name="scheddl",
    version="0.0.1",
    description='Wrapper for Linux SCHED_DEADLINE scheduler',
    long_description=long_description,
    author="Dan Hallgren",
    author_email="dan.hallgren@gmail.com",
    license="MIT",
    url="https://github.com/dahallgren/scheddl",
    packages=[],
    classifiers=(
        # 'Development Status :: 1 - Planning',
        # 'Development Status :: 2 - Pre-Alpha',
         'Development Status :: 3 - Alpha',
        # 'Development Status :: 4 - Beta',
        # 'Development Status :: 5 - Production/Stable',
        # 'Development Status :: 6 - Mature',
        # 'Development Status :: 7 - Inactive',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Operating System :: POSIX :: Linux',
        'Programming Language :: Python',
    ),
    ext_modules=[Extension('scheddl', ['sched_dl.c', 'pyapi.c'])]
)
