cc65 Applesoft Floating Point Library
=====================================
|MIT| |APPLE| |Apple2TS| |cc65|

.. |MIT| image:: https://img.shields.io/badge/License-MIT-yellow.svg
   :target: https://opensource.org/licenses/MIT

.. |APPLE| image:: https://img.shields.io/badge/Apple%20II-ProDOS-0000C0.svg?logo=apple&logoColor=ee0000
   :target: https://github.com/AppleWin/AppleWin

.. |cc65| image:: https://img.shields.io/badge/cc65-compiler-de3234.svg?logo=cc65&logoColor=de3234
   :target: https://cc65.github.io/ 

.. |Apple2TS| image:: https://img.shields.io/badge/apple2ts-blue.svg?logo=data:image/svg%2bxml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiPz4NCjxzdmcgaWQ9IkxheWVyXzEiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyIgdmVyc2lvbj0iMS4xIiB2aWV3Qm94PSIwIDAgMjQuNSAyOC45Ij4NCiAgPCEtLSBHZW5lcmF0b3I6IEFkb2JlIElsbHVzdHJhdG9yIDMwLjAuMCwgU1ZHIEV4cG9ydCBQbHVnLUluIC4gU1ZHIFZlcnNpb246IDIuMS4xIEJ1aWxkIDEyMykgIC0tPg0KICA8ZGVmcz4NCiAgICA8c3R5bGU+DQogICAgICAuc3QwIHsNCiAgICAgICAgZmlsbDogI2U0MzgzOTsNCiAgICAgIH0NCg0KICAgICAgLnN0MSB7DQogICAgICAgIGZpbGw6ICMwMDlkZTA7DQogICAgICB9DQoNCiAgICAgIC5zdDIgew0KICAgICAgICBmaWxsOiAjZjg4MjAxOw0KICAgICAgfQ0KDQogICAgICAuc3QzIHsNCiAgICAgICAgZmlsbDogI2ZmMDsNCiAgICAgIH0NCg0KICAgICAgLnN0NCB7DQogICAgICAgIGZpbGw6ICNlMjM4Mzg7DQogICAgICB9DQoNCiAgICAgIC5zdDUgew0KICAgICAgICBmaWxsOiAjZTEzNzM4Ow0KICAgICAgfQ0KDQogICAgICAuc3Q2IHsNCiAgICAgICAgZmlsbDogIzllMzk5NTsNCiAgICAgIH0NCg0KICAgICAgLnN0NyB7DQogICAgICAgIGZpbGw6ICM1ZWJmM2Y7DQogICAgICB9DQoNCiAgICAgIC5zdDggew0KICAgICAgICBmaWxsOiAjZmViYTAxOw0KICAgICAgfQ0KICAgIDwvc3R5bGU+DQogIDwvZGVmcz4NCiAgPHBhdGggY2xhc3M9InN0NyIgZD0iTTE2LjIsMGMuOS41LjcsMS40LjcsMi4ycy40LjUuOC41Yy42LDAsMS4zLDAsMS42LjZzMCwxLDAsMS41aC04LjV2LTJjLjgsMCwxLjctLjIsMi42LS40Vi43Yy4zLS4yLjUtLjQuNy0uNmgyLjFaIi8+DQogIDxwYXRoIGNsYXNzPSJzdDEiIGQ9Ik0xOS4yLDI0LjF2My4yYzAsLjcsMCwxLjItLjksMS4ycy0uMi4xLS40LjJjLS4zLjEtLjUuMi0uNy4yaC0uMWMtLjItLjItLjQtLjQtLjYtLjQtLjUsMC0uNy0uMy0uNy0uOHYtMy41aDMuNFoiLz4NCiAgPHBhdGggY2xhc3M9InN0MiIgZD0iTTE2LjcsOS43di44aDJjLjUsMCwuNy4yLjYuNnYyaDQuOWMwLC42LjEsMS4xLjEsMS43aC01Ljh2LS45aC00LjNjMCwuMiwwLC41LS4xLjhoLTZjMC0xLjgtLjItMS45LDEuNy0xLjguMiwwLC4zLDAsLjYtLjEuMS0uNS4yLTEsLjEtMS41aC0zLjdzLS4yLjEtLjMuMmgwdjEuNWMwLC42LS4zLjktLjkuOWgtMS4yYy0uNy0uMS0uOS4yLTEsLjlILjF2LTEuMWMuMi0uNy43LS44LDEuNC0uOCwxLjIsMCwxLjEsMCwxLjItMS4xLDAtLjUuMy0xLC41LTEuNSwwLS4xLjMtLjIuNS0uMmgxLjNjMCwwLC4zLS4yLjUtLjRoMTEuMloiLz4NCiAgPHBhdGggY2xhc3M9InN0NiIgZD0iTTE2LjcsMTkuNWMwLC44LjMsMS4xLDEsMWguN2MuNiwwLC44LjMuOC44djNoLTMuM2MtLjQsMC0uOC0uMS0xLjMtLjItMS4xLDAtMS4zLS4yLTEuMy0xLjJzLS42LTEuNS0xLjgtMS40djIuNGMtLjgsMC0xLjUuMi0yLjMuMkgxLjljLTEuNCwwLTIuNC0xLjUtMS43LTIuOCwwLS4yLjItLjMuNS0uNC4zLDAsLjMsMCwuMywwaDYuNnMuMi0uMi4zLS4zYzAtLjMuMS0uNi4zLTFoOC42LDBaIi8+DQogIDxwYXRoIGNsYXNzPSJzdDgiIGQ9Ik0xNi43LDkuN0g1LjZjMC0uNiwwLTEuMS40LTEuN2g0LjJjLjUsMCwuNi0uMi42LS43di0yLjVoOC41djMuM2MwLC42LS4yLjgtLjguOGgtMS45di45aDBaIi8+DQogIDxwYXRoIGNsYXNzPSJzdDQiIGQ9Ik04LjIsMTQuNmg2YzAsLjguMSwxLjcsMCwyLjUsMCwuNy4yLjkuOS44aC41YzEsMCwxLjMuNiwxLjIsMS43aC04LjZ2LTVaIi8+DQogIDxwYXRoIGNsYXNzPSJzdDUiIGQ9Ik0wLDE0LjhoMy4zYzAsLjYsMCwxLjEtLjIsMS43LS45LDAtMS43LDAtMi41LS4xcS0uMi0uMS0uMy0uM2MwLS4yLDAsMCwwLDAsMC0uNC0uMi0uNy0uMy0xLjF2LS4yWiIvPg0KICA8cGF0aCBjbGFzcz0ic3QzIiBkPSJNMTAuMSw4aC00LjFjLjQsMCwuOS0uMSwxLjQtLjFoMi43WiIvPg0KICA8cGF0aCBjbGFzcz0ic3QwIiBkPSJNMTguNiwxNC44aDUuOGMuMiwxLjUsMCwxLjYtMS40LDEuNmgtMy4zYy0uNywwLTEuMS0uNC0xLjEtMS4xczAtLjMsMC0uNVoiLz4NCjwvc3ZnPg==
      :target: https://apple2ts.com/?appmode=game&theme=dark#https://github.com/randall-frank/cc65-applesoft-fp/releases/latest/download/a2fp_release.po


.. raw:: html

   <p align="center">
     <img src="docs/project_logo.jpg" style="width:600px; height:auto;">
   </p>


Overview
--------
The cc65 compiler suite does not include support for float or double types.
This project provides a library that adds support for floating point numbers
using the Applesoft ROM floating point routines running on Apple II hardware.
The system uses the Applesoft 40bit float representation and provides conversion
from standard C int, char and char * values.  

Support is included for:

- Addition, subtraction, multiplication, division
- Transcendental: sin, cos, tan, atan
- Logarithm, exponential, square root, power
- Comparison
- I/O to and from C strings
- Utilities: sgn, reciprocal, absolute value, etc.

Because this library leverages pre-existing routines in the Applesoft ROM,
it has a very small memory footprint (typically around 600 bytes).  However,
it does use the non-standard 40bit float format rather than the IEEE 754 standard.

Building
--------
The program is written in a combination of C and 6502 assembly, compiled
using the cc65 toolchain.

There is a build script in this repo that is capable of generating a .po file 
from the sources.  It requires several external tools:

- Python
- `cc65 C compiler <https://cc65.github.io/>`_
- `CiderPress II <https://ciderpress2.com/>`_
- `Doxygen <https://www.doxygen.nl/>`_
 
The build script will download the necessary tools, placing them in subdirectories
of the one containing the 'build.py' script.

The following commands will build the `a2fp_release.po` file:

.. code::

   python -m virtualenv venv
   .\venv\Scripts\activate.ps1
   python -m pip install -r requirements.txt
   python build.py build


Build Options
-------------
The build script is capable of performing a number of tasks:

- Build the library and .po image
- Generate library documentation
- Push documentation to github pages
- Convert Python (IEEE 754) floats to/from Applesoft binary representation

*build.py* has several options:

- clean [--full]
 
  - Remove the contents of the `build` and `html` directories.  `--full` removes the build tools as well.

- build, fullbuild [--debug] [--symbols]

  - rebuild the entire `build` directory. This does a `clean` followed by a build of the library
    `--debug` includes `BASIC.SYSTEM` in the disk image and boots to Applesoft. `--symbols` generates
    an assembly level listing file of the interface.

- ghpages [--ghmsg 'commit message']

  - ghpages will first execute a `docs` operation. It will then push the contents of
    the `build` directory to the `gh-pages` branch of the current git repository.  This 
    will make the story available on GitHub Pages.  The `--ghmsg` option allows you to 
    specify a commit message for the push.  The default is the current version number 
    of the story

- docs 

  - This command will generate the `html` directory contents using Doxygen.  It can be previewed
    by viewing the `html/index.html` file.

- flt2as [--varname VARNAME]

  - This command will convert a Python (IEEE 754) float to AppleSoft floating point format.  It will output
    the C source code needed to embed the constant into a C application. If --verbose is specified, the individual IEEE 754 fields will be displayed along with the mantissa in 
    binary.

    .. code::

      >  python build.py flt2as 0.5 --varname hello
      AS_FAC_FP hello = {0x80, {0x00, 0x00, 0x00, 0x00}}; /* 0.5 */


- as2flt

  - This command will convert an AppleSoft floating point value to a Python (IEEE 754) float.
    If --verbose is specified, the individual Applesoft float fields will be displayed along with the mantissa in binary.

    .. code::

      > python build.py as2flt 8000000000
      0.5


Documentation and Issues
------------------------
`Documentation <https://randall-frank.github.io/cc65-applesoft-fp/>`_ for the library is generated using Doxygen and is stored in the github pages for this project. 

Normally, one would download the `.po` file and use it with an emulator or 
burn a 5.25" disk with the image.  Thanks to the great work by Chris Torrence
and Michael Morrison on the `Apple2TS <https://github.com/ct6502/apple2ts>`_ browser 
hosted Apple II emulator, one can run the program via a web browser.  

`Run the test case in a browser <https://github.com/randall-frank/cc65-applesoft-fp/releases/latest/download/a2fp_release.po>`_

Please feel free to post issues and other questions at `a2fp Issues
<https://github.com/randall-frank/cc65-applesoft-fp/issues>`_. This is the best place
to post questions and code.


Things To Do
~~~~~~~~~~~~
- Add more examples from testmain.c

License
-------
`a2fp` is licensed under the MIT license.
