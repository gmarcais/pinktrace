/* vim: set cino= fo=croql sw=8 ts=8 sts=0 noet cin fdm=syntax : */

/*
 * Copyright (c) 2010 Ali Polatel <alip@exherbo.org>
 *
 * This file is part of Pink's Tracing Library. pinktrace is free software; you
 * can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License version 2.1, as published by the Free Software
 * Foundation.
 *
 * pinktrace is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <Python.h>
#include <pinktrace/pink.h>

#include "pink-python-hacks.h"

PyMODINIT_FUNC
#if PY_MAJOR_VERSION > 2
PyInit_string(void);
#else
initstring(void);
#endif /* PY_MAJOR_VERSION > 2 */

static char pinkpy_string_decode_doc[] = ""
	"This function decodes the string at the argument of the given index.\n"
	"\n"
	"@param pid: Process ID of the traced child\n"
	"@param index: The index of the argument\n"
	"@param maxlen: Max length of the string\n"
	"@param bitness: The bitness of the traced child\n"
	"(Optional, defaults to C{pinktrace.bitness.DEFAULT_BITNESS})\n"
	"(Optional, defaults to -1, if smaller than zero, pinktrace tries to determine the string length itself)\n"
	"@raise IndexError: Raised if the index is not smaller than MAX_INDEX\n"
	"@raise ValueError: Raised if the given bitness is either unsupported or invalid\n"
	"@raise OSError: Raised when the underlying ptrace call fails.\n"
	"@rtype: str\n"
	"@return: The decoded string";
static PyObject *
pinkpy_string_decode(pink_unused PyObject *self, PyObject *args)
{
	pid_t pid;
	unsigned ind;
	int maxlen;
	pink_bitness_t bit;
	char *str;
	PyObject *obj;

	maxlen = -1;
	bit = PINKTRACE_DEFAULT_BITNESS;
	if (!PyArg_ParseTuple(args, PARSE_PID"I|iI", &pid, &ind, &maxlen, &bit))
		return NULL;

	switch (bit) {
	case PINK_BITNESS_64:
#if defined(I386) || defined(POWERPC)
		PyErr_SetString(PyExc_ValueError, "Unsupported bitness");
		return NULL;
#endif
		break;
	case PINK_BITNESS_32:
#if defined(IA64) || defined(POWERPC64)
		PyErr_SetString(PyExc_ValueError, "Unsupported bitness");
		return NULL;
#endif
		break;
	default:
		PyErr_SetString(PyExc_ValueError, "Invalid bitness");
		return NULL;
	}

	if (ind >= PINK_MAX_INDEX) {
		PyErr_SetString(PyExc_IndexError, "Invalid index");
		return NULL;
	}

	if (maxlen < 0) {
		str = pink_decode_string_persistent(pid, bit, ind);
		if (!str)
			return PyErr_SetFromErrno(PyExc_OSError);
#if PY_MAJOR_VERSION > 2
		obj = PyUnicode_FromString(str);
#else
		obj = PyString_FromString(str);
#endif /* PY_MAJOR_VERSION > 2 */
		free(str);
		return obj;
	}

	str = PyMem_Malloc(sizeof(char) * maxlen);
	if (!str)
		return PyErr_NoMemory();

	if (!pink_decode_string(pid, bit, ind, str, maxlen))
		return PyErr_SetFromErrno(PyExc_OSError);

#if PY_MAJOR_VERSION > 2
	obj = PyUnicode_FromStringAndSize(str, maxlen);
#else
	obj = PyString_FromStringAndSize(str, maxlen);
#endif /* PY_MAJOR_VERSION > 2 */
	PyMem_Free(str);
	return obj;
}

static char pinkpy_string_encode_doc[] = ""
	"Encode a string into the argument of the given index safely.\n"
	"\n"
	"@param pid: Process ID of the traced child\n"
	"@param index: The index of the argument\n"
	"@param string: The string to be encoded\n"
	"@param bitness: The bitness of the traced child\n"
	"(Optional, defaults to C{pinktrace.bitness.DEFAULT_BITNESS})\n"
	"@raise IndexError: Raised if the index is not smaller than C{MAX_INDEX}\n"
	"@raise ValueError: Raised if the given bitness is either unsupported or invalid\n"
	"@raise OSError: Raised when the underlying ptrace call fails.\n";
static PyObject *
pinkpy_string_encode(pink_unused PyObject *self, PyObject *args)
{
	pid_t pid;
	unsigned ind;
	pink_bitness_t bit;
	char *str;
	Py_ssize_t len;

	bit = PINKTRACE_DEFAULT_BITNESS;
	if (!PyArg_ParseTuple(args, PARSE_PID"Is#|I", &pid, &ind, &str, &len, &bit))
		return NULL;

	switch (bit) {
	case PINK_BITNESS_64:
#if defined(I386) || defined(POWERPC)
		PyErr_SetString(PyExc_ValueError, "Unsupported bitness");
		return NULL;
#endif
		break;
	case PINK_BITNESS_32:
#if defined(IA64) || defined(POWERPC64)
		PyErr_SetString(PyExc_ValueError, "Unsupported bitness");
		return NULL;
#endif
		break;
	default:
		PyErr_SetString(PyExc_ValueError, "Invalid bitness");
		return NULL;
	}

	if (ind >= PINK_MAX_INDEX) {
		PyErr_SetString(PyExc_IndexError, "Invalid index");
		return NULL;
	}

	if (!pink_encode_simple_safe(pid, bit, ind, str, len))
		return PyErr_SetFromErrno(PyExc_OSError);

	return Py_BuildValue("");
}

static char pinkpy_string_encode_unsafe_doc[] = ""
	"Encode a string into the argument of the given index without additional\n"
	"checking for writable areas.\n"
	"\n"
	"@param pid: Process ID of the traced child\n"
	"@param index: The index of the argument\n"
	"@param string: The string to be encoded\n"
	"@param bitness: The bitness of the traced child\n"
	"(Optional, defaults to C{pinktrace.bitness.DEFAULT_BITNESS})\n"
	"@raise IndexError: Raised if the index is not smaller than C{MAX_INDEX}\n"
	"@raise ValueError: Raised if the given bitness is either unsupported or invalid\n"
	"@raise OSError: Raised when the underlying ptrace call fails.\n";
static PyObject *
pinkpy_string_encode_unsafe(pink_unused PyObject *self, PyObject *args)
{
	pid_t pid;
	unsigned ind;
	pink_bitness_t bit;
	char *str;
	Py_ssize_t len;

	bit = PINKTRACE_DEFAULT_BITNESS;
	if (!PyArg_ParseTuple(args, PARSE_PID"Is#|I", &pid, &ind, &str, &len, &bit))
		return NULL;

	switch (bit) {
	case PINK_BITNESS_64:
#if defined(I386) || defined(POWERPC)
		PyErr_SetString(PyExc_ValueError, "Unsupported bitness");
		return NULL;
#endif
		break;
	case PINK_BITNESS_32:
#if defined(IA64) || defined(POWERPC64)
		PyErr_SetString(PyExc_ValueError, "Unsupported bitness");
		return NULL;
#endif
		break;
	default:
		PyErr_SetString(PyExc_ValueError, "Invalid bitness");
		return NULL;
	}

	if (ind >= PINK_MAX_INDEX) {
		PyErr_SetString(PyExc_IndexError, "Invalid index");
		return NULL;
	}

	if (!pink_encode_simple(pid, bit, ind, str, len))
		return PyErr_SetFromErrno(PyExc_OSError);

	return Py_BuildValue("");
}

static char string_doc[] = "Pink's string decoding and encoding functions";
static PyMethodDef string_methods[] = {
	{"decode", pinkpy_string_decode, METH_VARARGS, pinkpy_string_decode_doc},
	{"encode", pinkpy_string_encode, METH_VARARGS, pinkpy_string_encode_doc},
	{"encode_unsafe", pinkpy_string_encode_unsafe, METH_VARARGS, pinkpy_string_encode_unsafe_doc},
	{NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION > 2
static struct PyModuleDef string_module = {
	PyModuleDef_HEAD_INIT,
	"string",
	string_doc,
	-1,
	string_methods,
	NULL,
	NULL,
	NULL,
	NULL
};

PyMODINIT_FUNC
PyInit_string(void)
{
	PyObject *mod;

	mod = PyModule_Create(&string_module);
	if (!mod)
		return NULL;

	return mod;
}
#else
PyMODINIT_FUNC
initstring(void)
{
	PyObject *mod;
	mod = Py_InitModule3("string", string_methods, string_doc);
}
#endif /* PY_MAJOR_VERSION > 2 */
