#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject* show_message(PyObject *self, PyObject *args)
{
	const char *message;
	if (!PyArg_ParseTuple(args, "s", &message))
	{
		return NULL;
	}
	printf("%s: it works\n", message);
	Py_RETURN_NONE;
}

static PyMethodDef DemoMethods[] = {
	{
		"show",			/* Python function */ 
		show_message, 		/* C function */
		METH_VARARGS,		/* if without args: METH_NOARGS */ 
		"Show a message..."	
	},
	{
		NULL, 
		NULL, 
		0, 
		NULL
	}
};

static struct PyModuleDef demo = 
{
	PyModuleDef_HEAD_INIT,
	"demo",				/* Python module name */
	"",				/* module documentation */
	-1,				/* size of per-interpreter state of the module, or -1 
					if the module keeps state in global variables. */
	DemoMethods
};

PyMODINIT_FUNC PyInit_demo(void)
{
	return PyModule_Create(&demo);
}
