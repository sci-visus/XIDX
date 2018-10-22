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

src_path=sys.argv[0]+"/../../"
#print("args",sys.argv)

# Xidx_module = Extension('_Xidx',
#                       sources=[src_path+'/include/xidx/xidx.i'],
#                       include_dirs=[src_path+'/include/xidx',src_path+'/include/xidx/elements'],#,'/usr/include/libxml2/'],
#                       libraries=['xml2'],
#                       swig_opts=["-c++"],
#                       extra_compile_args=['-std=c++11'],
#                       language="c++")
#
# setup (name = 'Xidx',
#    version = '0.0',
#    author      = "University of Utah",
#    description = """Xidx Python module""",
#   # ext_modules = [Xidx_module],
#    py_modules = ["_Xidx"],
#   )
#

# ////////////////////////////////////////////////////////////////////
def findFilesInCurrentDirectory():

    ret=[]

    for dirpath, __dirnames__, filenames in os.walk("."):
        for it in filenames:

            filename= os.path.abspath(os.path.join(dirpath, it))

            if filename.startswith(os.path.abspath('./dist')):
                continue

            if os.path.basename(filename).startswith(".git"):
                continue

            if "__pycache__" in filename:
                continue

            # for bdist_wheel I don't need to add files for compilation
            if BDIST_WHEEL:

                if filename.startswith(os.path.abspath('./lib')):
                    continue

                if filename.startswith(os.path.abspath('./include')):
                    continue

                if WIN32:

                    if filename.startswith(os.path.abspath('./win32/python')):
                        continue

                    if filename.endswith(".pdb"):
                        continue

            ret.append(filename)

    return ret

# ////////////////////////////////////////////////////////////////////
def cleanAll():
    shutil.rmtree('./build', ignore_errors=True)
    shutil.rmtree('./Xidx.egg-info', ignore_errors=True)
    shutil.rmtree('./__pycache__', ignore_errors=True)

# ////////////////////////////////////////////////////////////////////
def runSetupTools():

    setuptools.setup(
        name = "Xidx",
        description = "Xidx extensible metadata",
        version=VERSION,
        url="https://github.com/sci-visus/XIDX",
        author="visus.org",
        author_email="support@visus.org",
        packages=["Xidx"],
        package_dir={"Xidx":'.'},
        package_data={"Xidx": '_xidx.so, xidx.py'},
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