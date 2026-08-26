Add Project Symbol Library
==========================

Follow the steps below if the Symbol library in the project schematics isn't correctly referenced:

1. Open the Preferences > Manage Symbol Libraries menu.
2. In the Project Specific Libraries verify the ``dolphin`` project is defined:

    * Nick Name: ``dolphin``.
    * Library Path: ``${KIPRJMOD}/../../library/symbols/dolphin.kicad_sym``.

.. note::

    The ``${KIPRJMOD}`` variable automatically provide the absolute path from the current location.

More information is available in the `KiCAD Docs <https://docs.kicad.org/10.0/en/eeschema/eeschema.html#managing-symbol-libraries>`_ page.
