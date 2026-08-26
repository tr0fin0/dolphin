Release
=======

.. dropdown:: Full Workflow
    :animate: fade-in-slide-down

    .. literalinclude:: ../../../../../.github/workflows/release.yml
        :language: yaml
        :linenos:

What It Does
------------

This workflow analyzes and create a new release if suitable with the use of `semantic-release <https://github.com/semantic-release/semantic-release>`_.


When It Does
------------

.. literalinclude:: ../../../../../.github/workflows/release.yml
    :language: yaml
    :start-after: name:
    :end-before: permissions:

This workflow runs whenever it's triggered by an user, whenever there is a push to the ``main`` branch and every sunday at midnight.


How to... Verify Locally
------------------------

It's possible to locally evaluate the behavior with the following:

.. code-block:: bash

    curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.3/install.sh | bash

    npm install
    npx dotenv-cli -e .env -- npx semantic-release --dry-run
