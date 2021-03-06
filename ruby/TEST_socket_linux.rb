#!/usr/bin/env ruby
# coding: utf-8

%w{socket test/unit}.each {|m| require m}

$:.insert(0, '.libs')
require 'PinkTrace'

class TestPinkSocket < Test::Unit::TestCase
  def test_socket_decode_call_esrch
    assert_raise Errno::ESRCH do
      PinkTrace::Socket.decode_call 0
    end
  end

  def test_socket_decode_fd_esrch
    assert_raise Errno::ESRCH do
      PinkTrace::Socket.decode_fd 0
    end
  end
end

class TestPinkSocket
  TEST_SOCKET = './TEST_unix_socket'

  def teardown
    begin
      File.unlink TEST_SOCKET
    rescue Errno::ENOENT
    end
  end

  def test_decode_socket_call
    pid = fork do
      PinkTrace::Trace.me
      Process.kill 'STOP', Process.pid

      UNIXServer.new TEST_SOCKET
    end
    Process.waitpid pid
    PinkTrace::Trace.setup pid, PinkTrace::Trace::OPTION_SYSGOOD


    # Loop until we get to the socket() system call as there's no guarantee
    # that other system calls won't be called beforehand.
    event = -1
    while event != PinkTrace::Event::EVENT_EXIT_GENUINE
      PinkTrace::Trace.syscall pid
      Process.waitpid pid

      event = PinkTrace::Event.decide
      if event == PinkTrace::Event::EVENT_SYSCALL then
        scno = PinkTrace::Syscall.get_no pid
        name = PinkTrace::Syscall.name scno
        if name == 'socketcall' or name == 'socket' then
          subcall = PinkTrace::Socket.decode_call pid
          if name == 'socketcall' then
            # The call must have given the decoded socket call.
            subname = PinkTrace::Socket.name subcall
          else
            subname = PinkTrace::Syscall.name subcall
          end
          assert(subname == 'socket', "Wrong subcall name, expected: socket got: '#{subname}'")
          break
        end
      end
    end

    begin PinkTrace::Trace.kill pid
    rescue Errno::ESRCH ;end
  end

  def test_decode_socket_fd
  end
end
