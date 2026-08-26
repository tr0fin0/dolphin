Workflows
=========

`act <https://nektosact.com/installation/index.html>`_ is used to locally run GitHub Workflows which requires `Docker Engine <https://docs.docker.com/engine/install/>`_ to create an environment suitable for the workflow execution. a workflow may be run with the following:

..  code-block:: bash

    sudo act -W '.github/workflows/<name>.yml' -P ubuntu-latest=-self-hosted

When running for the first time, it should pull an ubuntu image which could take some minutes. Following runs will reuse the same environment and therefore should start quickly.

..  tip::
    Run workflows locally before push changes to remote.

.. toctree::
   :maxdepth: 2
   :caption: Table of Contents

   cicd/index
   kicad/index
   release/index
