#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    test_configure.py                                  :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/04 14:59:06 by gtoubol           #+#    #+#              #
#    Updated: 2022/11/26 00:04:43 by gtoubol          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

import pytest
import os
import uuid
import subprocess

BASE_PATH = "/srcs/config/"


class TestConfigure:
    path = f"{os.getcwd()}{BASE_PATH}"
    exec_test = f"{path}test_configure.test"

    def run_error(
        self,
        filename: str,
        expected_log: str,
        expected_returncode: int
        ) -> None:
        """Run the test executable and compare the error output and returncode

        """
        with subprocess.Popen([self.exec_test, filename], stderr=subprocess.PIPE) as proc:
            errorlog = proc.stderr.read().decode()
            returncode = proc.wait()
        assert errorlog == expected_log
        assert returncode == expected_returncode


    def test_inputfile_file_no_exist(self):
        filename = "does_not_exist"
        self.run_error(filename, f"Error: {filename}: cannot read the file.\n", 1)


    def test_inputfile_permission_denied_status_1(self, tmp_path):
        """Test the configuration status depending on the configuration file status.

        """

        tmp = tmp_path / str(uuid.uuid4())
        tmp.write_text("")

        # No rights
        os.chmod(tmp, 0000)
        self.run_error(tmp, f"Error: {tmp}: cannot read the file.\n", 1)
        # Exec only
        os.chmod(tmp, 0o100)
        self.run_error(tmp, f"Error: {tmp}: cannot read the file.\n", 1)
        # Write only
        os.chmod(tmp, 0o277)
        self.run_error(tmp, f"Error: {tmp}: cannot read the file.\n", 1)
        # Exec or write
        os.chmod(tmp, 0o377)
        self.run_error(tmp, f"Error: {tmp}: cannot read the file.\n", 1)


    def test_inputfile_allowed_status_0(self, tmp_path):

        tmp = tmp_path / str(uuid.uuid4())
        tmp.write_text("")

        # Read only
        os.chmod(tmp, 0o400)
        self.run_error(tmp, "", 0)
        # All rights
        os.chmod(tmp, 0o777)
        self.run_error(tmp, "", 0)


    def test_inputfile_folder_status_2(self, tmp_path):
        tmp = tmp_path / str(uuid.uuid4())
        tmp.mkdir()
        self.run_error(tmp, f"Error: {tmp}: unreadable.\n", 1)

    def test_inputfile_levelformating(self, tmp_path):
        tmp = tmp_path / str(uuid.uuid4())
        tmp.write_text("""
 server
   server
server
# Coucou
  server
 # test test
""")
        self.run_error(
        tmp,
        """Bad config: line 2: bad block level
Bad config: line 3: bad block level
""",
        1)

    def test_inputfile_server(self, tmp_path):
        tmp = tmp_path / str(uuid.uuid4())
        tmp.write_text("""  server
server:
server coucou
server : coucou
server:


server:
server: test
""")

        self.run_error(
            tmp,
            """Bad config: line 1: bad block level
"""
            , 1)


    def test_inputfile_listen(self, tmp_path):
        tmp = tmp_path / str(uuid.uuid4())
        tmp.write_text("""
# listen out of server block
listen: 127.0.0.2:80

server:
  # listen with options
  listen: 80
    bad listen
    bad listen
  listen: 80
  listen: 80

# bad listen blocks
server:
  listen: 183  ;
  # bad to high
  listen: 65536
  # last port ok
  listen: 65535
  #check overflow
  listen: 9223372036854775809
  listen: 18446744073709551616

# bad host
server
  listen: 1sdf23
  listen: aze123
  listen: 127..0.0:80
  listen: etaset:80

# valid hosts
server:
  listen: 127.0.0.2:80
  listen: 80
  listen: ::1:80
  listen: localhost:80

server:
  listen
  listen: 255.255.255.255:80

""")
        self.run_error(
            tmp,
        """Bad config: `listen`: bad key level
Bad config: listen block don't have son properties
Bad config: listen: bad port format
Bad config: listen: bad port format
Bad config: listen: bad port format
Bad config: listen: bad port format
Bad config: listen: bad format
Bad config: listen: bad format
Bad config: listen: could not resolve `127..0.0`
Bad config: listen: could not resolve `etaset`
Bad config: listen: bad format
""",
            1)

    def test_inputfile_root(self, tmp_path):
        tmp = tmp_path / str(uuid.uuid4())
        tmp.write_text("""
root: test
server:
    root
  root
  root /test
  root: /test
  root: /test

server:
  root: test/test
""")

        self.run_error(
            tmp,
            """Bad config: line 4: bad block level
""",
            1)

    def test_inputfile_server_name(self, tmp_path):
        tmp = tmp_path / str(uuid.uuid4())
        tmp.write_text("""
server_name: coucou
server:
server_name: coucou
server:
  server_name test123
  server_name: test
  server_name: test
server:
   server_name: test
# coucou
   # coucou
""")
        self.run_error(
            tmp,
            """Bad config: line 10: bad block level
""",
            1)
