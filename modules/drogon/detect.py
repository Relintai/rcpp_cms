import os
import platform
import sys


def is_active():
    return True


def get_name():
    return "drogon"


def can_build():
    if os.name == "posix" or sys.platform == "darwin":
        err = os.system("pkg-config --version > /dev/null")
        if err:
            return False

        err = os.system("pkg-config uuid --modversion --silence-errors > /dev/null ")

        if err:
            print("Package uuid not found..")
            return False

        err = os.system("pkg-config jsoncpp --modversion --silence-errors > /dev/null ")

        if err:
            print("Package jsoncpp not found..")
            return False

        err = os.system("pkg-config libcares --modversion --silence-errors > /dev/null ")

        if err:
            print("Package libcares not found..")
            return False

    return True


def get_opts():
    return []

def get_flags():

    return []


def configure(env):
    err = os.system("pkg-config uuid --modversion --silence-errors > /dev/null ")

    if not err:
        env.ParseConfig("pkg-config uuid --cflags --libs")

    err = os.system("pkg-config jsoncpp --modversion --silence-errors > /dev/null ")

    if not err:
        env.ParseConfig("pkg-config jsoncpp --cflags --libs")

    err = os.system("pkg-config libcares --modversion --silence-errors > /dev/null ")

    if not err:
        env.ParseConfig("pkg-config libcares --cflags --libs")

    err = os.system("pkg-config openssl --modversion --silence-errors > /dev/null ")

    if not err:
        env.ParseConfig("pkg-config openssl --cflags --libs")
        env.Append(CPPDEFINES=["OPENSSL_FOUND"])

    env.Prepend(CPPPATH=["#modules/drogon/drogon/lib/inc"])
    env.Prepend(CPPPATH=["#modules/drogon"])

    env.Prepend(CPPPATH=["#modules/drogon/trantor"])
    env.Prepend(CPPPATH=["#modules/drogon/trantor/net"])
    env.Prepend(CPPPATH=["#modules/drogon/trantor/net/inner"])
    env.Prepend(CPPPATH=["#modules/drogon/trantor/utils"])
    
