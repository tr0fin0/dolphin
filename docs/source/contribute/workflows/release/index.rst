`release <.github/workflows/release.yml>`_
==========================================

This workflow analyzes and create a new release if suitable with use of `semantic-release <https://github.com/semantic-release/semantic-release>`_. It's possible to locally evaluate the behavior with the following:

..  code-block:: bash

    curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.3/install.sh | bash

    npm install
    npx dotenv-cli -e .env -- npx semantic-release --dry-run
