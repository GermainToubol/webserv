#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    test_configure.py                                  :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/04 14:59:06 by gtoubol           #+#    #+#              #
#    Updated: 2022/11/08 09:40:10 by gtoubol          ###   ########.fr        #
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

    def test_inputfile_permission_denied_status_1(self, tmp_path):
        """Test the configuration status depending on the configuration file status.

        """

        tmp = tmp_path / str(uuid.uuid4())
        tmp.write_text("")

        # No rights
        os.chmod(tmp, 0000)
        assert subprocess.run([self.exec_test, tmp]).returncode == 1
        # Exec only
        os.chmod(tmp, 0o100)
        assert subprocess.run([self.exec_test, tmp]).returncode == 1
        # Write only
        os.chmod(tmp, 0o277)
        assert subprocess.run([self.exec_test, tmp]).returncode == 1
        # Exec or write
        os.chmod(tmp, 0o377)
        assert subprocess.run([self.exec_test, tmp]).returncode == 1

    def test_inputfile_allowed_status_0(self, tmp_path):

        tmp = tmp_path / str(uuid.uuid4())
        tmp.write_text("")

        # Read only
        os.chmod(tmp, 0o400)
        assert subprocess.run([self.exec_test, tmp]).returncode == 0
        # All rights
        os.chmod(tmp, 0o777)
        assert subprocess.run([self.exec_test, tmp]).returncode == 0

    def test_inputfile_folder_status_2(self, tmp_path):
        tmp = tmp_path / str(uuid.uuid4())
        so.mkdir
