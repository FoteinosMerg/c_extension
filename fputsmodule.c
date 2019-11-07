#define PY_SSIZE_T_CLEAN
#include <Python.h>		/* Should be put before any other header! */

static PyObject *ShortStringError = NULL;

/* Function to wrap */

int perform(char *string, char *filename) 
{
	FILE *fp = fopen(filename, "w");
	int bytes_copied = fputs(string, fp);
	fclose(fp);
	return bytes_copied;
}


/* Wrapper */

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


/* Meta info:
 *
 * Structures initialized by PyModule_Create(), before
 * the latter returns reference to module object to
 * the Python interperter
 */

/* Module methods definition (method table) */

static PyMethodDef MethodTable[] = {
	{
		"fputs",			/* Python function name */
		method_fputs,			/* corresponding wrapper */
		METH_VARARGS,			/* Will do accept args of type PyObject */
		"Python interface for fputs"	/* Function docstring */
	},
	{
		NULL,
		NULL,
		0,
		NULL
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
	 * Module import definition (init function)
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

	PyObject *module = PyModule_Create(&fputsmodule);
	
	/* Initialize exception objects (if any) and add them to module */
	ShortStringError = PyErr_NewException("fputs.ShortStringError", NULL, NULL);
	PyModule_AddObject(module, "ShortStringError", ShortStringError);

	/* Initialize constants (if any) and add them to module */
	PyModule_AddIntConstant(module, "CONST_1", 64);
	PyModule_AddStringConstant(module, "CONST_2", "vivere");

	/* Initiaize macros (if any) and add them to module */
	#define MACRO_1 256
	#define MACRO_2 "oculus"
	PyModule_AddIntMacro(module, MACRO_1);
	PyModule_AddStringMacro(module, MACRO_2);	

	return module;
}


