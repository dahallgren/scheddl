scheddl - SCHED_DEADLINE for Python
===================================
SCHED_DEADLINE is a CPU scheduler available in the Linux kernel since version 3.14 based on the Earliest Deadline First
(EDF) and Constant Bandwidth Server (CBS) algorithms, supporting resource reservations: each task scheduled under such
policy is associated with a budget Q (aka runtime), and a period P, corresponding to a declaration to the kernel that
Q time units are required by that task every P time units, on any processor. This makes SCHED_DEADLINE particularly
suitable for real-time applications, like multimedia or industrial control, where P corresponds to the minimum time
elapsing between subsequent activations of the task, and Q corresponds to the worst-case execution time needed by each
activation of the task.

scheddl is a module that provides a Python API for the Linux SCHED_DEADLINE scheduler.

Requirements
------------
* Linux kernel >= 3.14

Example
-------
Here is how to set the current scheduler to SCHED_DEADLINE and run a periodic task each 50 ms.

.. code-block:: python

    import scheddl

    dl_args = (
        20 * 1000 * 1000, # runtime in nanoseconds
        50 * 1000 * 1000, # deadline in nanoseconds
        50 * 1000 * 1000  # time period in nanoseconds
    )

    scheddl.set_deadline(*dl_args)

    while True:
        periodic_task(*args, **kwargs)
        scheddl.sched_yield()

The SCHED_RESET_ON_FORK flag could be passed with `scheddl.RESET_ON_FORK` as an optional fourth argument
to `scheddl.set_deadline`.

.. code-block:: python

    scheddl.set_deadline(20000000, 50000000, 50000000, scheddl.RESET_ON_FORK)

When this flag is set, children do not inherit privileged scheduling policies,
SCHED_DEADLINE in this case.

Documentation
-------------
See man pages for sched(7), sched_setattr(2)
