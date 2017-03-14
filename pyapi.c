/*
 Copyright (c) 2017 Dan Hallgren  <dan.hallgren@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 */
#include <Python.h>
#include <linux/sched.h>
#include "sched_dl.h"

#define SCHED_DL_RESET_ON_FORK SCHED_FLAG_RESET_ON_FORK

static PyObject* set_deadline (PyObject* self, PyObject *args)
{
    __u64 runtime;
    __u64 deadline;
    __u64 period;
    unsigned int flags = 0;
    int ret;

    if (!PyArg_ParseTuple (args, "lll|I", &runtime, &deadline, &period, &flags)) {
        return NULL;
    }

    if ((flags != 0) && (flags != SCHED_DL_RESET_ON_FORK)) {
        return PyErr_Format (PyExc_AttributeError, "Flags must be 0 or RESET_ON_FORK");
    }

    ret = set_current_tid_to_deadline (runtime, deadline, period, flags);
    if (ret < 0) {
        return PyErr_SetFromErrno (PyExc_OSError);
    }

    Py_RETURN_NONE;
}

static PyObject* deadline_yield (PyObject* self)
{
    sched_dl_yield ();

    Py_RETURN_NONE;
}

static char set_deadline_docs[] = "set_deadline(runtime_ns, deadline_ns, period_ns, flags=0 ): Set current process to deadline scheduler\n"
        "    The kernel requires that:\n"
        "    runtime_ns <= deadline_ns <= period_ns\n"
        "    and all of the parameter values must be at least 1024 (i.e., just over one microsecond, which is the\n"
        "    resolution of the implementation), and less than 2^63.";

static char deadline_yield_docs[] = "sched_yield(): yield process\n";

static PyMethodDef sched_dl_funcs[] = { { "set_deadline", (PyCFunction) set_deadline, METH_VARARGS, set_deadline_docs }, { "sched_yield",
        (PyCFunction) deadline_yield, METH_NOARGS, deadline_yield_docs }, { NULL } };

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "scheddl",
    "Wrapper module for linux SCHED_DEADLINE",
    0,
    sched_dl_funcs,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC PyInit_scheddl(void)
#else
void initscheddl (void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule3 ("scheddl", sched_dl_funcs, "Wrapper module for linux SCHED_DEADLINE");
#endif
    (void) PyModule_AddIntConstant (module, "RESET_ON_FORK", SCHED_DL_RESET_ON_FORK);

#if PY_MAJOR_VERSION >= 3
    return module;
#endif

}
