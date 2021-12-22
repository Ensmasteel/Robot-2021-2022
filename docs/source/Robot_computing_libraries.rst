========================================
Robot computing and simulating libraries
========================================

Introduction
============

On this page will be presented all libraries used to instantiate and simulate the robot behavior. Moreover, this page
presents the simulation library documentation.

You may find below the information about :

* :ref:`The Robot libraries <robot>`, that can instantiate a robot reference. This includes the Robot and the RobotSimu libraries.
* :ref:`The ghost library <ghost>`, which enables you to create a ghost image of the robot, in order to whether simulate
  robot behavior or just play the model role for robot.
* :ref:`The Action library <action>`, that enables you to define actions to be done by the robot or the ghost, whether simulated or not.
* :ref:`The Functions library <function>` that presents all function done in robot.
* :ref:`The Sequence library <sequence>`, that presents all sequences that can be done during the match. By the way, you'll find all
  operations that can be done on sequences, like adding actions or operating on those already implemented.
* :ref:`The Math_Functions library <math-function>` that includes all the mathematics functions useful in calculating trajectories and target
  positions for the robot. It also implements math operators about vectors and other custom structures.
* :ref:`The PID library <pid>`, that defines all the parameters for PIDs and calculators.
* :ref:`The Simulator library <simulator>`

.. note::
    This page contains all about the APIs about moving the robot, but only in code lines. For actuators you may refer
    to the :ref:`actuators`, and for coordinating functions you may refer to :ref:`coordinate`.


.. _robot:

Robot Library
=============
This section includes the robot entity itself and the robotSimu library. Those two are precised below.

Robot
+++++

This library enables to add all functionalities and actuators to a robot entity. This is this robot entity that will
command each actuator and handle each message, before sending them to the right calculator electronic card.

.. doxygenfile:: Robot.h


.. robot-simu:

Robot simulated library
+++++++++++++++++++++++
This library enables you to simulate the robot behavior inside a simulator of your own, or created with the
:ref:`Simulator Library <simulator>` described below.

.. doxygenfile:: RobotSimu.h


.. _ghost:

Ghost Library
=============

This library enables you to create a ghost entity to mimic the robot behavior. Moreover, this is the class used to
make the robot move. In fact, controlling and recalibrating a ghost entity is easier than trying so on a physical bot.

.. warning::
    This class may not completely mimic the robot behavior because of lot of physics parameters not implemented in this
    class. For further simulation please see the :ref:`simulator class <simulator>`.

.. doxygenfile:: Ghost.h


.. _action:

Action Library
==============

The action library defines a primary abstract class defining the principal behavior of an action. This abstract class
may **NEVER** be instantiated.

The other action classes extend this primary class, though you must use those ones and create your own custom action
class for specific actions required by the actual cup.


.. doxygenfile:: Actions.h


.. _function:

Functions Library
=================

The function library enables you to use and create custom functions in order to control the robot behavior.


.. doxygenfile:: Functions.h


.. _sequence:

Sequence Library
================

The sequence library enables you to create custom sequences of actions and behaviors.


.. doxygenfile:: Sequence.h


.. _math-function:

Math functions Library
======================

The math function library defines the math functions useful to control the robot trajectory and velocity.


.. doxygenfile:: Math_functions.h


.. _pid:

PID Library
===========

The PID defines the parameters of the PID corrector inside the robot. This is this API to use when positioning and
moving the robot is becoming harder because of miscalculation or bad calibration.


.. doxygenfile:: PID.h


.. _simulator:

Simulator Library
=================

This library enables you to simulate a complete match with a simulated robot. This is useful in case of final tests.

.. doxygenfile:: Simulator.h



Other Libraries and informations about Ensmasteel association
=============================================================

For further informations about other libraries please have a look to :ref:`the documentation home page <home>`.
To learn more about us, have a look to our `GitHub page ! <https://github.com/Ensmasteel>`_