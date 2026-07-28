# Fuzzy

Motion controller for a four-motor mobile platform based on **fuzzy logic**, Bluetooth serial communication, and pulse-width modulation (PWM).

The project includes an automatic controller that processes position and velocity errors along the **X** and **Y** axes, as well as an alternative program for directly controlling the vehicle through Bluetooth commands.

## Main Features

- Two-input fuzzy controller for each axis:
  - Position error: `ΔX` and `ΔY`.
  - Error variation or velocity: `ΔẊ` and `ΔẎ`.
- Five membership functions per input.
- A 25-rule fuzzy rule base for each axis.
- Defuzzification using a weighted sum of singleton outputs.
- Independent control of four motors.
- Communication through `Serial3` at 38400 baud.
- PWM output for speed and direction control.
- Alternative manual-control mode through Bluetooth.

## Repository Structure

```text
Fuzzy/
├── Fuzzy.ino       # Main program with the fuzzy controller
├── bluetooth.cpp        # Bluetooth command reading and processing
├── bluetooth.h          # Bluetooth module declarations
├── control.cpp          # Motor PWM and direction control
├── control.h            # Pins, actions, and control functions
└── Carrito/
    └── Carrito.ino      # Alternative program for manual control
```

## General Operation

```mermaid
flowchart LR
    A[Reference or external system] -->|ΔX, ΔẊ, ΔY, ΔẎ| B[Bluetooth / Serial3]
    B --> C[Membership functions]
    C --> D[Fuzzy rules]
    D --> E[Control output]
    E --> F[Direction selection]
    F --> G[PWM and H-bridges]
    G --> H[Four motors]
```

The `FuzzyChido.ino` program performs the following process:

1. Receives four values through `Serial3`.
2. Converts the received data into error values for the X and Y axes.
3. Evaluates the membership functions for each input.
4. Applies 25 fuzzy rules to each axis.
5. Calculates the control outputs `out1` and `out2`.
6. Selects the axis with the largest absolute error.
7. Defines the movement direction and PWM duty cycle.
8. Updates the four motors.

## Intended Hardware

The code requires an Arduino board that provides a `Serial3` port, for example:

- Arduino Mega 2560 or a compatible board.
- Four DC motors.
- Two or four motor drivers with `RPWM` and `LPWM` inputs.
- A serial Bluetooth module, such as an HC-05 or equivalent.
- A suitable power supply for the motors.
- A common ground between the Arduino, Bluetooth module, and motor drivers.

> Verify the voltage, maximum current, and H-bridge wiring before powering the system.

## Motor Connections

The pins are defined in `control.h`:

| Motor | RPWM | LPWM |
|---|---:|---:|
| Front left | 11 | 10 |
| Front right | 9 | 8 |
| Rear left | 7 | 6 |
| Rear right | 5 | 4 |

A code comment indicates that the Arduino `RPWM` signal may need to be connected to the motor driver's `LPWM` input, and vice versa. Confirm this correspondence using the electrical diagram and the datasheet of the specific motor driver being used.

## Serial Communication

### Serial Monitor

```text
Baud rate: 9600
```

### Bluetooth

```text
Port: Serial3
Baud rate: 38400
Terminator: newline (\n)
```

## Fuzzy Controller Input Format

The message must contain four values identified by the characters `m`, `n`, `o`, and `p`:

```text
m<delta_x>n<velocity_x>o<delta_y>p<velocity_y>
```

Example:

```text
m0.20n-0.10o0.30p0.05
```

Interpretation:

| Field | Example value |
|---|---:|
| `ΔX` | 0.20 |
| `ΔẊ` | -0.10 |
| `ΔY` | 0.30 |
| `ΔẎ` | 0.05 |

The program internally multiplies each input by `100` before evaluating the membership functions.

## Fuzzy Control

Each axis uses two inputs and five fuzzy sets per input, resulting in a matrix of **25 rules**.

### X Axis

Approximate configured range:

```text
-1000 to 1000
```

### Y Axis

Approximate configured range:

```text
-200 to 200
```

The singleton outputs used by both controllers are:

```text
-10, -5, -2, -1, 0,
 -5, -2, -1,  0, 1,
 -2, -1,  0,  1, 2,
 -1,  0,  1,  2, 5,
  0,  1,  2,  5, 10
```

The implemented functions are:

- `FuzT`: triangular membership function.
- `FuzZ`: Z-shaped membership function.
- `FuzS`: S-shaped membership function.

## Available Actions

The actions are defined in `control.h`:

| Action | Code |
|---|---:|
| Reverse | `-1` |
| Stop | `0` |
| Forward | `1` |
| Left | `2` |
| Right | `3` |
| Rotate left | `4` |
| Rotate right | `5` |

The main fuzzy controller automatically selects forward, reverse, left, and right movements. Rotation actions are available in the motor-control module and can be triggered through external commands.

## Manual Control through Bluetooth

The `Carrito/Carrito.ino` file uses `f_read_bth()` to directly receive an action and a duty-cycle value.

Expected format:

```text
<command>,<duty_cycle>.
```

Example for moving forward with a 60% duty cycle:

```text
1,60.
```

Example for stopping the vehicle:

```text
0,0.
```

> To compile `Carrito.ino` as an independent sketch, also place `control.cpp`, `control.h`, `bluetooth.cpp`, and `bluetooth.h` inside the `Carrito` folder, or adapt the project structure so the build environment can locate those files.

## Installation and Upload

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Download or clone this repository.
3. Open `FuzzyChido.ino` in the Arduino IDE.
4. Select a board compatible with `Serial3`.
5. Select the corresponding serial port.
6. Verify the motor and Bluetooth-module connections.
7. Compile and upload the program.
8. Open the Serial Monitor at 9600 baud to observe the controller outputs.

```bash
git clone <REPOSITORY-URL>
cd Fuzzy
```

## Debug Output

The main program periodically prints:

```text
out1 out2 delta_x delta_y command
```

Conceptual example:

```text
127 85 120 -40 2
```

These values make it possible to review the controller response, the received errors, and the selected action.

## Controller Tuning

The main parameters that can be modified are:

- Membership-function limits `C1_MF*` and `C2_MF*`.
- Output matrices `C1_Out` and `C2_Out`.
- Movement thresholds for the X and Y axes.
- Scaling applied to the inputs.
- PWM ratio between the front and rear motors.
- Update delay in `f_update_motors_cmd()`.

Before modifying the rules, record the system response and perform low-power tests to avoid unexpected movements.

## Current Considerations

- The code does not require external libraries beyond the Arduino core.
- The controller assumes that received messages follow the expected format.
- Validation of incomplete or corrupted messages is not implemented.
- The duty cycle should remain within a valid interval, preferably from `0` to `100`.
- The current configuration reduces the front-motor PWM to one fifth of the rear-motor PWM.
- The fuzzy parameters and thresholds must be calibrated for the actual platform geometry, mass, motors, and operating surface.

## Safety

Initially test the system with the wheels lifted and with a current-limited power supply. Keep a physical emergency-stop switch available and avoid working close to the wheels while the controller is running.


