.. _coordinate:

===========================
Useful coordinating library
===========================

Introduction
============

This page is used to present all the coordinating libraries used in the Ensmasteel bot. This description includes :

* :ref:`The communication library <communication>` which includes useful tools to set up communications with the bot.
* :ref:`The enum library <enumeration>`, containing all the enumerations used along the main code
* :ref:`The logger library <logger>`, containing all the informations used to log information, debugging infos and useful error messages.
* :ref:`The vector library <vector>`, defining vector type, used to model position, speed and acceleration.
* :ref:`The encoder library <encoder>`, used to encode message and information in order to communicate them through
  the communication port.
* :ref:`The ErrorManager library <error_manager>`, which combined with the logger one enables you to correctly handle errors raised by the code.

.. note::

    This page is only for coordinating libraries, for actuators please see :ref:`actuators`, and for robot functionalities
    please have a look to :ref:`coordinate`.

.. _communication:

Communication
=============

This refers to the Communication.h file, which defines lot of functionalities and models to set up communication
in the robot and with the environnement, whether its a computer or not.

.. note::
    Please note that all functions in this API can help you setting up useful tools such as communication ports.

.. doxygenfile:: Communication.h


.. _enumeration:

Enumerations
============

This library includes all the enumerations used in the robot code. They are useful to type some class and give some
precious information about what to do with message for example.

.. note::
    Please note that these enumerations may be updated every year because of specificities of the new cup regulation.

MessageID enumeration
+++++++++++++++++++++
This enumerations enables to characterize messages with useful informations such as actuator `Action_order`.

.. doxygenfile:: MessageID.h


MoveProfile enumeration
+++++++++++++++++++++++
This enumeration enables to characterize the robot movements and to invoke the right command at the right time
during movement operations.

.. doxygenfile:: MoveProfile.h


SequenceName enumeration
++++++++++++++++++++++++
This enumeration enables you to characterize each sequence. That enables you to ensure which sequence is running.

.. doxygenfile:: SequenceName.h


.. _logger:

Logger
======

This API enables you to create a logger to log errors and informations messages during tests and robot running.

.. doxygenfile:: Logger.h


.. _vector:

Vector
======
The Vector class enables you to creates vectors to plot positions, uses velocity and kinetics to places and move the
robot, targets and ghost. It overrides some classic operator to work with two vectors.

.. doxygenfile:: Vector.h


.. _encoder:

Encoder
=======
The encoder class enables to encode message to transfer them into message port and communication BUS.

.. doxygenfile:: Encoder.h


.. _error_manager:

Error Manager
=============

The ErrorManager class enables you to handle errors without crashing the all system down. This class is useful in case
of risky functionalities that may raise errors and bugs.

.. doxygenfile:: ErrorManager.h


Other Libraries and informations about Ensmasteel association
=============================================================

For further informations about other libraries please have a look to :ref:`the documentation home page <home>`.
To learn more about us, have a look to our `GitHub page ! <https://github.com/Ensmasteel>`_