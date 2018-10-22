from distutils.core import setup, Extension
from glob import glob

import os, sys, setuptools
import shutil
import platform

#increase this number for PIP
VERSION="0.0.0"

WIN32=platform.system()=="Windows" or platform.system()=="win32"
APPLE=platform.system()=="Darwin"
BDIST_WHEEL="bdist_wheel" in sys.argv

# ////////////////////////////////////////////////////////////////////
def cleanAll():
    shutil.rmtree('./build', ignore_errors=True)
    shutil.rmtree('./Xidx.egg-info', ignore_errors=True)
    shutil.rmtree('./__pycache__', ignore_errors=True)

# ////////////////////////////////////////////////////////////////////
def runSetupTools():

    setuptools.setup(
        name = "Xidx",
        description = "Xidx extensible IDX library",
        version=VERSION,
        url="https://github.com/sci-visus/XIDX",
        author="visus.org",
        author_email="support@visus.org",
        packages=["Xidx"],
        package_dir={"Xidx":'.'},
        package_data={"Xidx": ['lib/_xidx.so','lib/xidx.py']},
        classifiers=[
            "Programming Language :: Python :: 3",
            "License :: OSI Approved :: BSD License",
            'Operating System :: MacOS :: MacOS X',
            'Operating System :: Microsoft :: Windows',
            'Operating System :: POSIX',
            'Operating System :: Unix',
        ],
        platforms=['Linux', 'OS-X', 'Windows'],
        license = "BSD",
    )

# ////////////////////////////////////////////////////////////////////
def autoRenameSDist():

    import glob

    for ext in (".tar.gz", ".zip"):

        sdist_filename = glob.glob('dist/*'+ext);
        wheel_filename = glob.glob('dist/*.whl')

        if len(sdist_filename)==1 and len(wheel_filename)==1 and os.path.isfile(sdist_filename[0]):
            os.rename(sdist_filename[0], os.path.splitext(wheel_filename[0])[0]+ext)

# ////////////////////////////////////////////////////////////////////
if __name__ == "__main__":

    cleanAll()
    runSetupTools()
    autoRenameSDist()