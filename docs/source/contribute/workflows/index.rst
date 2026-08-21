Workflows
=========

`act <https://nektosact.com/installation/index.html>`_ is used to locally run GitHub Workflows which requires `Docker Engine <https://docs.docker.com/engine/install/>`_ to create an environment suitable for the workflow execution. a workflow may be run with the following:

git clone https://github.com/nektos/act.git
cd act/
sudo apt install golang-go
make build

sudo apt-get update
sudo apt install ./docker-desktop-amd64.deb

gpg --generate-key
pass init B515D918DF8D536A64C9EADBA576E3F46EF59845


act --workflows="./.github/workflows/kicad.yml"
echo 'export DOCKER_HOST="unix://$HOME/.docker/desktop/docker.sock"' >> ~/.bashrc

DOCKER_HOST="unix://$HOME/.colima/docker.sock"
export DOCKER_HOST="unix://$HOME/.docker/desktop/docker.sock"


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
