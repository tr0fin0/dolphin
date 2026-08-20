KiCAD
=====

.. dropdown:: Full Workflow
    :animate: fade-in-slide-down

    .. literalinclude:: ../../../../../.github/workflows/kicad.yml
        :language: yaml
        :linenos:

What It Does
------------

This workflow ensures that every KiCAD project passes the **Electrical Rules Check (ERC)** for the schematics and the **Design Rules Check (DRC)** for the PCBs via the `kicad-actions <https://github.com/actions-for-kicad/kicad-actions>`_ before proceeding to export the following files:

- SCH as PDF
- PCB as STEP
- PCB from the Top perspective as PNG
- PCB from the Bottom perspective as PNG
- Gerbers and Drill files as ZIP

Those files will be then added to the repository via the `git-auto-commit-action <https://github.com/stefanzweifel/git-auto-commit-action>`_ with the following message:

.. code-block:: bash

    chore(ci): update generated KiCAD files [skip ci]

The ``[skip ci]`` flag tells GitHub to skip most Continuous Integrations workflows avoiding an endless loop of GitHub Actions triggers.

.. tip::

    If either of the **ERC** or the **DRC** fail, their reports will be available in the workflow summary explaining what went wrong according to the project file rules.

When It Does
------------

.. literalinclude:: ../../../../../.github/workflows/kicad.yml
    :language: yaml
    :start-after: name:
    :end-before: env:

This workflow runs whenever a push or pull request to the main branch of the repository affects either the ``kicad.yml`` workflow file or any file the ``hardware/`` folder.

How to ... Add Projects
-----------------------

.. literalinclude:: ../../../../../.github/workflows/kicad.yml
    :language: yaml
    :start-after: verify-project:
    :end-before: steps:

Whenever a new project is added to the repository ``hardware/projects/`` folder it **must** be included in the GitHub Actions strategy matrix project list to ensure it will be verified.
