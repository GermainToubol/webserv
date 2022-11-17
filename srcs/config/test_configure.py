#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    test_configure.py                                  :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/04 14:59:06 by gtoubol           #+#    #+#              #
#    Updated: 2022/11/17 13:16:51 by gtoubol          ###   ########.fr        #
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
            """Bad config: line 1: server level needs to be 0
Bad config: line 3: missing delimiter
Bad config: line 4: missing delimiter
Bad config: line 9: configuration file error
"""
            , 1)


    def test_inputfile_listen(self, tmp_path):
        tmp = tmp_path / str(uuid.uuid4())
        tmp.write_text("""
listen: 127.0.0.2:80

server:
    listen: 80;
  listen: 80;
  listen: 80;
""")
        self.run_error(
            tmp,
        """Bad config: line 2: listen block should be in server block
Bad config: line 5: listen level needs to be 1
Bad config: line 7: server blocks have only one listen
""",
            1)
