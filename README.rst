DigiSim
=======
|MIT| |APPLE| |Apple2TS| |Itch|

.. |MIT| image:: https://img.shields.io/badge/License-MIT-yellow.svg
   :target: https://opensource.org/licenses/MIT

.. |APPLE| image:: https://img.shields.io/badge/Apple%20II-ProDOS-0000C0.svg?logo=apple&logoColor=ee0000
   :target: https://github.com/AppleWin/AppleWin

.. image:: banner.png
   :alt: Splash screen banner
   :align: center


Overview
--------
This is an old logic simulator written back in 1986 for the Apple II.  It
made it possible to play around with simple networks of basic one and two
input logic gates. Up to 8 inputs are supported and the simulation is run
for 8 clocks.  Note: execution is synchronous, that is gate delay is not
part of the simulation.  Thus, entities such as flip-flops may not run
in deterministic fashion, but they will generally resolve as expected.

In 2024, the original source code to the simulator was unearthed and the original
programmer decided to fix up the project enough so that it could be rebuilt
from source and run.   Originally, the program was written for DOS 3.3
but was converted over to ProDOS recently as can be seen in the github 
history.  The resulting source code is in this repo and one can
download a generated disk image from itch.io.

Details
-------
The program is written in a combination of AppleSoft and Merlin 6502 assembly.

There is a build script in this repo that is capable of generating a .po file 
from the sources.  It requires several tools to be installed:

- Python
- `Merlin32 Assembler <https://brutaldeluxe.fr/products/crossdevtools/merlin/>`_
- `CiderPress II <https://ciderpress2.com/>`_

If one places the CiderPress CLI in a subdirectory named 'ciderpress' (ciderpress/cp2.exe)
and places the Merlin package in a subdirectory named 'merlin32' 
(merlin32\\Windows\\Merlin32.exe), then the following commands will build
the `DigiSim_Release.po` file:

.. code::

   python -m virtualenv venv
   .\venv\Scripts\activate.ps1
   python -m install requirements.txt
   python build.py


One can adjust the pathnames to CiderPress and Merlin at the top of the build.py file.

Documentation and Issues
------------------------
The simulator allows for the placement of gates and waveform sources on a 32x32
grid.  One can use IJKM or the arrow keys to move about the grid.  This can be
via single cell or by "pages" of 8 cells (the 'S' key will toggle between modes).

There are 8 sources and both the raw and inverse value of a source can be placed 
in the network.  A fairly complete list of one and two input gates are supported
including: NOT, AND, NAND, OR, NOR, EOR, NEOR.  There is also a "NONE" gate that
is used to remove a gate or waveform from a grid cell.  The specific wave or gate
to be placed are displayed on the right side of the display and there are keys 
to select between them.  

Highlighting a placed gate or source will display their simulated or setup values
respectively.  Gates can be edited to specify the coordinates of the inputs to 
the gate.  A waveform can be redefined using the number keys to select the waveform
and then the 1&0 keys to enter the 8 values.  The simulation can be updated using
the 'A' key which recomputes all of the gate outputs.

All of the valid keystrokes can be seen by pressing '?' to display a help screen.
Other options (loading, saving and quitting the program) are accessed using the 
utilities menu via the 'U' key.  Generally, simulation files have been saved
using the '.DS' suffix, but this is not enforced by the program.  All files are
8200 byte files of type BIN. 

Normally, one would download the `.po` file and use it with an emulator or 
burn a 5.25" disk with the image.  Thanks to the great work by Chris Torrence
and Michael Morrison on the `Apple2TS <https://github.com/ct6502/apple2ts>`_ browser 
hosted Apple II emulator, one can run the program via a web browser.  

`Run DigiSim in a browser <https://apple2ts.com/?appmode=game&theme=dark#https://github.com/randall-frank/digisim/releases/latest/download/DigiSim_Release.po>`_

Please feel free to post issues and other questions at `DigiSim Issues
<https://github.com/randall-frank/digisim/issues>`_. This is the best place
to post questions and code.

The program is also hosted on `itch.io <https://myleftgoat.itch.io/digisim>`_ which provides
a simpler download option and forum to discuss more execution related issues.


Things To Do
~~~~~~~~~~~~
The display of wires is extremely crude.  It needs to be reworked to avoid gates
and attempt to bend around each other a bit.


License
-------
`DigiSim` is licensed under the MIT license.
