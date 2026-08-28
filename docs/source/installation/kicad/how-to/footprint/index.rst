Add Project Footprint Library
=============================

Follow the steps below if the Footprint library in the project PCB isn't correctly referenced:

1. Open the Preferences > Manage Footprint Libraries menu.
2. In the Project Specific Libraries verify the ``dolphin`` project is defined:

    * Nick Name: ``dolphin``.
    * Library Path: ``${KIPRJMOD}/../../library/footprints``.

.. note::

    The ``${KIPRJMOD}`` variable automatically provide the absolute path from the current location.

More information is available in the `KiCAD Docs <https://docs.kicad.org/10.0/en/pcbnew/pcbnew.html#managing-footprint-libraries>`_ page.
