#define PY_SSIZE_T_CLEAN
#include <Python.h>		/* Should be put before any other header! */

static PyObject *ShortStringError = NULL;
static PyObject *SysCommandError  = NULL;
static PyObject *NotCallableError = NULL;

static PyObject *callback = NULL;

/* Functions to wrap */

int perform(char *string, char *filename) 
{
	FILE *fp = fopen(filename, "w");
	int bytes_copied = fputs(string, fp);
	fclose(fp);
	return bytes_copied;
}


/* Wrappers */

/* *self points to the module for module-level functions, whereas
 * for methods it points to the object to which it will be attached
 */

static PyObject *method_empty(PyObject *self, PyObject *args)
{
	/* No arguments: f() */

	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	printf("ok\n");
	Py_RETURN_NONE;
}

static PyObject *method_build_values(PyObject *seld, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;

	PyObject *none    = Py_BuildValue("");
	PyObject *integer = Py_BuildValue("i", 123);
	PyObject *string  = Py_BuildValue("s", "oculus");
	PyObject *bytes   = Py_BuildValue("y", "oculus");
	PyObject *tuple   = Py_BuildValue("(isis)", 666, "oculus", 777, "vidit");
	PyObject *list    = Py_BuildValue("[isis]", 666, "oculus", 777, "vidit");
	PyObject *combined = Py_BuildValue("[(is)is]", 666, "oculus", 777, "vidit");
	PyObject *dictionary = Py_BuildValue("{s:i, i:s}", "oculus", 666, 777, "vidit");
	
	PyObject *result = combined;

	return result;
}

static PyObject *method_fputs(PyObject *self, PyObject *args) {
	/**********************************************************************
     	* Function: PyObject *method_fputs(PyObject *self, PyObject *args)
     	* Input   : the module object (self) along with actual arguments (args)
     	* Output  : PyLong_FromLong
     	* Procedure: Python wrapper of the perform() function
     	***********************************************************************/

	char *string   = NULL;	/* string to be written to the file stream*/
	char *filename = NULL;	/* name of file to write to */

	if (!PyArg_ParseTuple(args, "ss", &string, &filename)) {
		return NULL;
	}

	if (strlen(string) < 8) {
		PyErr_SetString(ShortStringError, 
			"Length of provided string must be at least 8");
		// PyErr_SetString(PyExc_ValueError, "Length of provided string must be at least 8");
		return NULL;
	}

	int bytes_copied = -1;
	bytes_copied = perform(string, filename);
	return PyLong_FromLong(bytes_copied);
}


static PyObject *method_system(PyObject *self, PyObject *args) {
	
	const char *command;
	int sts;

	if (!PyArg_ParseTuple(args, "s", &command))
		return NULL;
	sts = system(command);
	if (sts < 0) {
		PyErr_SetString(SysCommandError, "System command failed");
		return NULL;
	}
	return PyLong_FromLong(sts);
}


static PyObject *set_callback(PyObject *dummy, PyObject *args) {
	
	PyObject *result = NULL;
	PyObject *temp;

	if (PyArg_ParseTuple(args, "0:set_callback", &temp)) {
		if (!PyCallable_Check(temp)) {
			PyErr_SetString(NotCallableError, "Parameter must be callable");
			return NULL;
		}
		Py_XINCREF(temp);	// Add reference to new callback
		Py_XDECREF(callback);	// Dispose of previous callback
		callback = temp;

		Py_INCREF(Py_None);
		result = Py_None;	
	}

	return result;
}


static PyObject *method_dv_example(PyObject *self, PyObject *args, PyObject *kwargs)
{
	int voltage;
	const char *alpha = "ALPHA";
	const char *beta = "BETA";
	const char *gamma = "GAMMA";

	static char *kwlist[] = {"alpha", "beta", "gamma", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i|sss", kwlist,
					&voltage, &alpha, &beta, &gamma))
		return NULL;

	printf("voltage: %i\talpha: %s\tbeta: %s\tgamma: %s\n",
			voltage, alpha, beta, gamma);	
	Py_RETURN_NONE;
}


	
/* Meta info:
 *
 * Structures initialized by PyModule_Create(), before
 * the latter returns reference to module object to
 * the Python interperter
 */

/* Module methods definition (method table) */

static PyMethodDef MethodTable[] = {
	{
		"empty",
		method_empty,
		METH_VARARGS,
		"Exhibits no args"
	},
	{
		"build_values",
		method_build_values,
		METH_VARARGS,
		"Exhibits how to build values"
	},
	{
		"fputs",			/* Python function name */
		method_fputs,			/* corresponding wrapper */
		METH_VARARGS,			/* Will do accept args of type PyObject */
		"Python interface for fputs"	/* Function docstring */
	},
	{
		"system",
		method_system,
		METH_VARARGS,
		"Execute a shell command"
	},
	{
		"dv_example",
		(PyCFunction)(void(*)(void))method_dv_example,	/* The cast of function is necessary
								 * since PyCFunction values only take
								 * two PyObject* parameters while
								 * this one takes three
								 */
		METH_VARARGS | METH_KEYWORDS,
		"Exhibits kwargs usage"
	},
	{
		NULL, NULL, 0, NULL
	}
};

/* Module definition */

static struct PyModuleDef fputsmodule = {
	PyModuleDef_HEAD_INIT,
	"fputs",						/* Python module name */
	"Python interface to the fputs C library function",	/* Module docstring */
	-1,							/* No sub-interpreters support (a non-negative
								 * value enables module re-initialization
								 * along with specifying memory requirement
								 * to be allocated on each subinterpreter
								 * session */
	MethodTable
};


PyMODINIT_FUNC PyInit_fputs(void) {
	/*********************************************************************
	 * 
	 * Module import definition (init function). Should be the only
	 * non-static item defined in this file
	 *
	 * PyMODINIT_FUNK invoked whenever a Python program imports 
	 * the module for the first time
	 *
	 * Declares the function as extern C. In case of C++, it
	 * tells the compiler not to do name-mangling on symbols
	 *
	 * 			  | PyInit_<module_name> ----> PyModule_Create
	 *	Python	    ----> |			       +++++++++++++++
	 *	interpreter <---- |			       PyModuleDef
	 *			  |			       PyMethodDef
	 *
	 *********************************************************************/

	PyObject *module; //=PyModule_Create(&fputsmodule);
	module = PyModule_Create(&fputsmodule);
	// if (module == NULL)
	//	return NULL;
	
	/* Initialize exception objects (if any) and add them to module */
	ShortStringError = PyErr_NewException("fputs.ShortStringError", NULL, NULL);
	Py_INCREF(ShortStringError);
	PyModule_AddObject(module, "ShortStringError", ShortStringError);
	
	SysCommandError = PyErr_NewException("fputs.SysCommandError", NULL, NULL);
	Py_INCREF(SysCommandError);
	PyModule_AddObject(module, "SysCommandError", SysCommandError);

	NotCallableError= PyErr_NewException("fputs.NotCallableError", NULL, NULL);
	Py_INCREF(NotCallableError);
	PyModule_AddObject(module, "NotCallableError", NotCallableError);
	
	/* Initialize constants (if any) and add them to module */
	PyModule_AddIntConstant(module, "CONST_1", 64);
	PyModule_AddStringConstant(module, "CONST_2", "vivere");

	/* Initiaize macros (if any) and add them to module */
	#define MACRO_1 256
	#define MACRO_2 "oculus"
	PyModule_AddIntMacro(module, MACRO_1);
	PyModule_AddStringMacro(module, MACRO_2);	

	return module;	// so that it get inserted into sys.modules by the functions caller
}


