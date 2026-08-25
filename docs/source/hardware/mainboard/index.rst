Mainboard
#########

:Version: v2
:Author: Guilherme Nunes Trofino

.. toctree::
    :maxdepth: 2

    files/index

Designed for both autonomous and radio-controlled competitive robotics sumo categories using the `ESP32-S3 <https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/index.html>`_ microcontroller with closed-loop motor control using independent encoders and up to 16 sensors via an onboard `CD74HC4067M <https://www.ti.com/lit/ds/symlink/cd74hc4067.pdf>`_ multiplexer.

Features
========

Communication
-------------

.. tab-set::

    .. tab-item:: I2C

        Pull-up resistors on the bottom with the SCK and SDA pins.

    .. tab-item:: SPI

        SCK, MISO and MOSI pins on the center connector of the board.


Dimensions
----------

2 layer board with:

========== =========== ======
Coordinate Dimension   Name
========== =========== ======
x          ``50.0 mm`` length
y          ``70.0 mm`` width
z          ``21.5 mm`` height
========== =========== ======

Operating Voltages
------------------

.. tab-set::

    .. tab-item:: 4.2V to 25.2V

        1. leave the ``U1`` **soldered**
        2. leave the ``D1`` LED and ``R18`` resistor **populated**
        3. supply input voltage to the ``VIN`` pin via external power

    .. tab-item:: 5V

        1. leave the ``U1`` **unsoldered**
        2. leave the ``D1`` LED and ``R18`` resistor **unpopulated**
        3. supply input voltage to the ``5V0`` pin via ESCs

Limitations
===========

Resistive voltage dividers used only allow **unidirectional communication** on most inputs pins due to the ESP32-S3 3V3 operating voltage.
