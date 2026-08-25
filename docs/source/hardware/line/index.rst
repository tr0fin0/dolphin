Sensor Line
###########

:Version: v1
:Author: Guilherme Nunes Trofino

.. toctree::
    :maxdepth: 2

    files/index

Designed to precisely detects white edge lines on a black arena using the `QRE1113 <https://www.onsemi.com/pdf/datasheet/qre1113-d.pdf>`_ miniature reflective object sensor with a reduced form factor.

Features
========

Communication
-------------

.. tab-set::

    .. tab-item:: Analogic

        Without any external capacitors between ``VCC`` and transistor collector it behaves **digitally**.

Dimensions
----------

2 layer board with:

========== =========== ======
Coordinate Dimension   Name
========== =========== ======
x          ``07.5 mm`` length
y          ``15.0 mm`` width
z          ``04.0 mm`` height
========== =========== ======

Operating Voltages
------------------

.. tab-set::

    .. tab-item:: 5V

        1. supply input voltage to the ``VDD`` pin

Limitations
===========

**No Physical Connectors** headers are omitted due to board size constraints.
