#!/usr/bin/env python
# coding: utf-8

import os, sys, unittest

sys.path.insert(0, '.')
from pinktrace import trace

UNAME = os.uname()

class TestTrace_01_Invalid(unittest.TestCase):

    def test_01_me(self):
        self.assertRaises(TypeError, trace.me, 1)

    def test_02_cont(self):
        self.assertRaises(TypeError, trace.cont)
        self.assertRaises(TypeError, trace.cont, 1, 2, 3, 4)
        self.assertRaises(TypeError, trace.cont, 'pink')
        self.assertRaises(TypeError, trace.cont, 0, 'pink')
        self.assertRaises(OSError, trace.cont, 0)

    def test_02_resume(self):
        self.assertRaises(TypeError, trace.resume)
        self.assertRaises(TypeError, trace.resume, 1, 2, 3)
        self.assertRaises(TypeError, trace.resume, 'pink')
        self.assertRaises(TypeError, trace.resume, 0, 'pink')
        self.assertRaises(OSError, trace.resume, 0)

    def test_02_kill(self):
        self.assertRaises(TypeError, trace.kill)
        self.assertRaises(TypeError, trace.kill, 'pink')
        self.assertRaises(OSError, trace.kill, 0)

    def test_03_singlestep(self):
        self.assertRaises(TypeError, trace.singlestep)
        self.assertRaises(TypeError, trace.singlestep, 'pink')
        self.assertRaises(TypeError, trace.singlestep, 0, 'pink')
        self.assertRaises(OSError, trace.singlestep, 0)

    def test_04_syscall(self):
        self.assertRaises(TypeError, trace.syscall)
        self.assertRaises(TypeError, trace.syscall, 'pink')
        self.assertRaises(TypeError, trace.syscall, 0, 'pink')
        self.assertRaises(OSError, trace.syscall, 0)

    def test_05_syscall_entry(self):
        if UNAME[0] != 'FreeBSD': return

        self.assertRaises(TypeError, trace.syscall_entry)
        self.assertRaises(TypeError, trace.syscall_entry, 'pink')
        self.assertRaises(TypeError, trace.syscall_entry, 0, 'pink')
        self.assertRaises(OSError, trace.syscall_entry, 0)

    def test_06_syscall_exit(self):
        if UNAME[0] != 'FreeBSD': return

        self.assertRaises(TypeError, trace.syscall_exit)
        self.assertRaises(TypeError, trace.syscall_exit, 'pink')
        self.assertRaises(TypeError, trace.syscall_exit, 0, 'pink')
        self.assertRaises(OSError, trace.syscall_exit, 0)

    def test_05_geteventmsg(self):
        if UNAME[0] != 'Linux': return

        self.assertRaises(TypeError, trace.geteventmsg)
        self.assertRaises(TypeError, trace.geteventmsg, 'pink')
        self.assertRaises(OSError, trace.geteventmsg, 0)

    def test_06_setup(self):
        if UNAME[0] != 'Linux': return

        self.assertRaises(TypeError, trace.setup)
        self.assertRaises(TypeError, trace.setup, 'pink')
        self.assertRaises(TypeError, trace.setup, 0, 'pink')
        self.assertRaises(OSError, trace.setup, 0)

    def test_07_attach(self):
        self.assertRaises(TypeError, trace.attach)
        self.assertRaises(TypeError, trace.attach, 'pink')
        self.assertRaises(TypeError, trace.attach, 0, 'pink')
        self.assertRaises(OSError, trace.attach, 0)

    def test_08_detach(self):
        self.assertRaises(TypeError, trace.detach)
        self.assertRaises(TypeError, trace.detach, 'pink')
        self.assertRaises(TypeError, trace.detach, 0, 'pink')
        self.assertRaises(OSError, trace.detach, 0)

if __name__ == '__main__':
    unittest.main()
