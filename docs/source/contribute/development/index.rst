Development
===========

The workflow to be follow when suggesting modifications designed with `Mermaid <https://mermaid.js.org/intro/>`_ is:

.. mermaid::

    ---
    config:
    flowchart:
        curve: basis
    title: Development Workflow
    ---
    flowchart LR
        classDef local  fill:#003300, stroke: #, stroke-width:1px
        classDef remote fill:#000033, stroke: #, stroke-width:1px

        A((open Issue)):::remote ==> B[create Branch]:::local
        B ==> C[add modifications]:::local
        C ==> D{validate<br>Branch}:::local
        D -->|workflows fail| C
        D ==>|workflows pass| E[push Branch]:::local
        E ==> F[create Pull Request]:::remote
        F ==> G{validate<br>Pull Request}:::remote
        G -->|workflows fail| C
        G ==>|workflows pass| H([merge Pull Request]):::remote

.. raw:: html

    <p>With local side operations in <span style="color:#77ff77"><b>green</b></span> and remote side operations in <span style="color:#7777ff"><b>blue</b></span>.</p>

Branch Naming
-------------

Each branch name **should** respect the following naming structure:

.. code-block:: text

    <type>/<description>

With the same accepted types as those for the `Commit Messages`_ and the description words separated by ``-``.

.. tip::
    Labels are automatically added by the ``cicd.yml`` workflow if this convention is followed.

.. _commit-messages:

Commit Messages
---------------

Each commit message **must** respect the `conventional commits <https://www.conventionalcommits.org/en/v1.0.0/>`_ structure to ensure releases created by `semantic-release <https://github.com/semantic-release/semantic-release>`_ follow the `semantic versioning <https://semver.org/>`_ convention. Therefore the following commit message structure is required:

.. code-block:: text

    <type>[optional scope]: <description>

    [optional body]

    [optional footer(s)]

.. dropdown:: Accepted Types
    :animate: fade-in-slide-down
    :open:

    Based on `Angular <https://github.com/angular/angular/blob/22b96b9/CONTRIBUTING.md#-commit-message-guidelines>`_ commit guidelines:

    ============ =================================================================
    Types        Application
    ============ =================================================================
    ``build``    modifications affecting the build system or external dependencies
    ``ci``       modifications affecting CI configuration files and scripts
    ``docs``     modifications only on documentation
    ``feat``     addition of a new feature
    ``fix``      correction of a bug
    ``perf``     modifications bringing performance improvements
    ``refactor`` modifications that neither fixes a bug nor adds a feature
    ``style``    modifications not affecting code behavior
    ``test``     addition or correction of tests
    ============ =================================================================

.. note::
    Breaking API change commit message must include either:

    - a footer ``BREAKING CHANGE:``
    - a ``!`` after the type/scope

    Adapted from the `conventional commits specification <https://www.conventionalcommits.org/en/v1.0.0/#specification>`_.

.. tip::
    **Read** the `conventional commits specification <https://www.conventionalcommits.org/en/v1.0.0/#specification>`_ for details on the do's and dont's.

Issues Creation
---------------

Each modification **must** be described on an Issue using either a `Bug <.github/ISSUE_TEMPLATE/bug.md>`_ or a `Feature <.github/ISSUE_TEMPLATE/feature.md>`_ Issue template. At least one of the following accepted `labels <.github/labels.yml>`_ **must** be included:

.. dropdown:: Accepted Labels
    :animate: fade-in-slide-down
    :open:

    ==================== ==========================================
    Label                Application
    ==================== ==========================================
    ``bug``              something isn't working
    ``dependencies``     related with project dependencies
    ``documentation``    improvements or additions to documentation
    ``enhancement``      new features or code improvements
    ``good first issue`` easy to solve for newcomers
    ``maintenance``      package and maintenance related
    ``security``         anything related to security advisories
    ``testing``          anything related to tests
    ==================== ==========================================

Pull Requests Creation
----------------------

Each working branch should be linked to a single Pull Request where it's title **must** respect the `Commit Messages`_ structure. When merging, the squash approach is used and the title will be used as default commit message.

..  warning::
    A Pull Request will be accepted if, and only if, the naming structure is respected and pass the `cicd.yml <.github/workflows/cicd.yml>`_ workflow.

    If disrespected, `semantic-release <https://github.com/semantic-release/semantic-release>`_ will disconsider the Pull Request and incorrectly determine the next release version.
