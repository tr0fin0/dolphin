Mainboard
#########

:Version: v2
:Author: Equipe Paralela

.. toctree::
    :maxdepth: 2

    files/index

Purpose
=======

Designed to receive inputs for both autonomous and radio-controlled operation modes of a sumo category robot, controlling ESCs via PWM.

Features
========

* **Closed-Loop Control:** allows closed-loop motor control using independent encoders.
* **Communication Protocols:** exposed ``SPI`` and ``I2C`` pins available for peripheral devices.
* **External Peripheral Pins:** 12 available pins for external connections.
* **Flexible Power Supply:** supports external 1S to 6S LiPO batteries, as well as ESCs 5V.
* **Multiplexed Sensor Inputs:** handles up to 16 sensor inputs via an onboard multiplexer.
* **RGB LED Expansion:** features headers for addressable RGB LED integration.

Hardware Configuration
----------------------

.. tab-set::

    .. tab-item:: Buck Converter

        * Solder the buck converter on the back of the board only if external power is used.
        * The status LED and resistor are optional and not required for core functionality.

    .. tab-item:: I2C Bus

        * Solder the ``I2C`` pull-up resistors only if required by your bus setup.

    .. tab-item:: Multiplexer

        * Enable or disable the MUX using the solder jumpers accessible on the bottom of the board.

.. tip::
    Verify power source constraints before soldering the buck converter module onto the board.

Limitations
===========

* **Unidirectional Communication:** resistive voltage dividers used only allow unidirectional communication.
