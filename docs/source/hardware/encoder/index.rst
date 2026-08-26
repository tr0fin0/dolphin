Encoder
#######

:Version: v1
:Author: Guilherme Nunes Trofino

.. toctree::
    :maxdepth: 2

    files/index

Designed to precisely measure motor rotation using the `AS5147U <https://www.mouser.com/datasheet/3/5912/1/AS5147U_AS5247U_DS000639_4_00.pdf>`_ high-resolution redundant rotary magnetic position sensor for fast absolute angle measurement over a full
360-degree range with a reduced form factor.

Features
========

Communication
-------------

.. tab-set::

    .. tab-item:: SPI

        All resistors must remain **unpopulated** while programming over SPI.

    .. tab-item:: PWM

        All resistors must remain **populated** while reading over PWM.

Dimensions
----------

2 layer board with:

========== =========== ======
Coordinate Dimension   Name
========== =========== ======
x          ``20.0 mm`` length
y          ``25.0 mm`` width
z          ``03.1 mm`` height
========== =========== ======

Operating Voltages
------------------

.. tab-set::

    .. tab-item:: 3V3

        1. supply input voltage to the ``3V3`` pin
        2. leave the bottom jumper **soldered**
        3. leava the ``C1`` capacitor **unpopulated**

    .. tab-item:: 5V

        1. supply input voltage to the ``VDD`` pin
        2. leave the bottom jumper **unsoldered**
        3. leave the ``C1`` capacitor **populated**

Limitations
===========

**No Physical Connectors** headers are omitted due to board size constraints.
