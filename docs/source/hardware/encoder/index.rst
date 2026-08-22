Encoder
#######

:Version: v1
:Author: Equipe Paralela

.. toctree::
    :maxdepth: 2

    files/index

Purpose
=======

Designed to precisely measure motor rotation using small-diameter magnets.


Features
========

* **Compact Design:** reduced board footprint and height to fit tight assemblies.
* **Dual Power Modes:** supports both 3.3V and 5V operating voltages.
* **SMD Connection:** solder pads used in place of physical connectors.
* **SPI Reconfigurable:** settings can be modified via SPI communication pins.

Operating Voltages
------------------

.. tab-set::

    .. tab-item:: 3V3

        1. supply input voltage to the ``3V3`` pin
        2. solder the bottom jumper
        3. remove the ``C1`` capacitor.

    .. tab-item:: 5V

        1. supply input voltage to the ``VDD`` pin
        2. unsolder the bottom jumper
        3. add the ``C1`` capacitor.

.. tip::
    Ensure power sources and capacitor configurations before applying power.

Programming Configuration
-------------------------

.. warning::
    Do not populate mode resistors during programming.

1. **SPI Reprogramming:** keep configuration resistors **unpopulated** while programming over SPI.
2. **PWM Operation:** after programming, populate the resistors with their required values.

Limitations
===========

* **No Connectors:** physical header connectors are omitted due to board size constraints.
