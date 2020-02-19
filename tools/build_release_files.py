#! /usr/bin/env python3

import os
import sys
import subprocess
import shutil
import build_board_info as build_info
import time

for port in build_info.SUPPORTED_PORTS:
    result = subprocess.run("rm -rf ../ports/{port}/build*".format(port=port), shell=True)

ROSIE_SETUPS = ["rosie-ci"]
rosie_ok = {}
for rosie in ROSIE_SETUPS:
    rosie_ok[rosie] = True

PARALLEL = "-j 5"
travis = False
if "TRAVIS" in os.environ and os.environ["TRAVIS"] == "true":
    PARALLEL="-j 2"
    travis = True

all_boards = build_info.get_board_mapping()
build_boards = list(all_boards.keys())
if "TRAVIS_BOARDS" in os.environ:
    build_boards = os.environ["TRAVIS_BOARDS"].split()

sha, version = build_info.get_version_info()

languages = build_info.get_languages()
exit_status = 0
for board in build_boards:
    bin_directory = "../bin/{}/".format(board)
    os.makedirs(bin_directory, exist_ok=True)
    board_info = all_boards[board]

    for language in languages:
        bin_directory = "../bin/{board}/{language}".format(board=board, language=language)
        os.makedirs(bin_directory, exist_ok=True)
        start_time = time.monotonic()

        # Normally different language builds are all done based on the same set of compiled sources.
        # But sometimes a particular language needs to be built from scratch, if, for instance,
        # CFLAGS_INLINE_LIMIT is set for a particular language to make it fit.
        clean_build_check_result = subprocess.run(
            "make -C ../ports/{port} TRANSLATION={language} BOARD={board} check-release-needs-clean-build | fgrep 'RELEASE_NEEDS_CLEAN_BUILD = 1'".format(
                port = board_info["port"], language=language, board=board),
            shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        clean_build = clean_build_check_result.returncode == 0

        build_dir = "build-{board}".format(board=board)
        if clean_build:
            build_dir += "-{language}".format(language=language)

        make_result = subprocess.run(
            "make -C ../ports/{port} TRANSLATION={language} BOARD={board} BUILD={build}".format(
                port = board_info["port"], language=language, board=board, build=build_dir),
            shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

        build_duration = time.monotonic() - start_time
        success = "\033[32msucceeded\033[0m"
        if make_result.returncode != 0:
            exit_status = make_result.returncode
            success = "\033[31mfailed\033[0m"

        other_output = ""

        for extension in board_info["extensions"]:
            temp_filename = "../ports/{port}/{build}/firmware.{extension}".format(
                port=board_info["port"], build=build_dir, extension=extension)
            for alias in board_info["aliases"] + [board]:
                bin_directory = "../bin/{alias}/{language}".format(
                    alias=alias, language=language)
                os.makedirs(bin_directory, exist_ok=True)
                final_filename = "adafruit-circuitpython-{alias}-{language}-{version}.{extension}".format(
                    alias=alias, language=language, version=version, extension=extension)
                final_filename = os.path.join(bin_directory, final_filename)
                try:
                    shutil.copyfile(temp_filename, final_filename)
                except FileNotFoundError:
                    other_output = "Cannot find file {}".format(temp_filename)
                    if exit_status == 0:
                        exit_status = 1

        if travis:
            print('travis_fold:start:adafruit-bins-{}-{}\\r'.format(language, board))
        print("Build {board} for {language}{clean_build} took {build_duration:.2f}s and {success}".format(
            board=board, language=language, clean_build=(" (clean_build)" if clean_build else ""),
            build_duration=build_duration, success=success))
        if make_result.returncode != 0:
            print(make_result.stdout.decode("utf-8"))
            print(other_output)
        # Only upload to Rosie if its a pull request.
        if travis:
            for rosie in ROSIE_SETUPS:
                if not rosie_ok[rosie]:
                    break
                print("Uploading to https://{rosie}.ngrok.io/upload/{sha}".format(rosie=rosie, sha=sha))
                #curl -F "file=@$final_filename" https://$rosie.ngrok.io/upload/$sha
        if travis:
            print('travis_fold:end:adafruit-bins-{}-{}\\r'.format(language, board))

        # Flush so travis will see something before 10 minutes has passed.
        print(flush=True)

sys.exit(exit_status)
