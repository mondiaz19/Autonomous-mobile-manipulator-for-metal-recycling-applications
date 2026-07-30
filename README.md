# Autonomous Mobile Manipulator for Metal Recycling Applications

This repository contains the publicly available resources associated with the research work:

> **“Intelligent Autonomous Mobile Manipulator for Metal Recycling Applications with Automatic Identification of Target Objects via Image Deep Learning Methods.”**

The research system combines autonomous mobile robotics, computer vision, deep learning, fuzzy-logic control, and robotic manipulation to support the identification and collection of recyclable metal objects.

The current public release focuses on two main components:

1. Dataset preparation and augmentation for an aluminum-object detector.
2. Fuzzy-logic motion control for a four-motor mobile robotic platform.

---

## Repository Contents

```text
Autonomous-mobile-manipulator-for-metal-recycling-applications/
│
├── Dataset/
│   ├── README.md
│   └── prepareAugmentedDataset.m.txt
│
├── Fuzzy/
│   ├── Carrito.ino
│   ├── Fuzzy.ino
│   ├── README.md
│   ├── bluetooth.cpp
│   ├── bluetooth.h
│   ├── control.cpp
│   └── control.h
│
└── README.md
```

---

## System Overview

The complete research workflow includes:

1. Image acquisition of recyclable metal objects.
2. Manual image annotation.
3. Dataset conversion to YOLO format.
4. Dataset division into training, validation, and test subsets.
5. Image augmentation for the training and validation subsets.
6. Deep-learning-based detection of aluminum objects.
7. Estimation of the position error between the robot and the target.
8. Transmission of position and velocity errors to the mobile platform.
9. Fuzzy-logic-based motion control.
10. Robot movement toward the detected target.

The resources included in this repository provide the dataset-preparation and mobile-platform-control components of this workflow.

---

# Dataset Preparation

The [`Dataset`](Dataset) directory contains the MATLAB resources used to prepare and augment the image dataset for the aluminum-object detector.

Detailed instructions are available in:

- [`Dataset/README.md`](Dataset/README.md)
- [`Dataset/prepareAugmentedDataset.m.txt`](Dataset/prepareAugmentedDataset.m.txt)

## Dataset Workflow

The dataset-preparation process follows four main stages:

```text
Image collection
       ↓
Manual labeling with MATLAB Image Labeler
       ↓
Export to YOLO format and dataset splitting
       ↓
Image and bounding-box augmentation
```

Images are manually labeled using the MATLAB Image Labeler application. The annotations are then exported to the normalized YOLO format:

```text
class x_center y_center width height
```

All bounding-box coordinates are normalized between `0` and `1`.

The class identifier used for the aluminum objects is:

```text
class = 0
```

---

## Expected Dataset Structure

Before running the augmentation script, the dataset must be organized as follows:

```text
datasets/
│
├── images/
│   ├── train/
│   ├── val/
│   └── test/
│
├── labels/
│   ├── train/
│   ├── val/
│   └── test/
│
└── data.yaml
```

Each image must have a corresponding YOLO label file with the same base filename.

Example:

```text
images/train/sample01.jpg
labels/train/sample01.txt
```

Example label:

```text
0 0.593250 0.288375 0.085500 0.178750
```

---

## Dataset Split

The original dataset is divided into:

| Subset | Percentage |
|---|---:|
| Training | 60% |
| Validation | 10% |
| Test | 30% |

A fixed random seed is used to improve reproducibility:

```matlab
rng(42)
```

---

## Data Augmentation

The MATLAB script performs augmentation only on the training and validation subsets.

The test subset remains unchanged so that the final performance metrics are calculated using clean, non-augmented images.

The augmentation operations include:

- Random horizontal reflection.
- Random rotation between approximately `10°` and `80°`.
- Random scaling between approximately `1.0×` and `1.25×`.
- Hue modification.
- Saturation modification.
- Brightness modification.
- Automatic transformation of the bounding boxes.
- Removal of bounding boxes that are mostly outside the transformed image.

By default, the script generates five augmented copies of each training and validation image.

The augmented files follow this naming convention:

```text
original_name-aug1.jpg
original_name-aug1.txt
original_name-aug2.jpg
original_name-aug2.txt
```

---

## Running the MATLAB Script

The script is currently stored in GitHub as:

```text
prepareAugmentedDataset.m.txt
```

After downloading or cloning the repository, rename it to:

```text
prepareAugmentedDataset.m
```

Then:

1. Open MATLAB.
2. Navigate to the directory containing the script.
3. Place the `datasets` directory in the expected location.
4. Open `prepareAugmentedDataset.m`.
5. Review the configuration section.
6. Adjust the dataset path if necessary.
7. Select the desired number of augmentations.
8. Enable or disable fixed-size resizing.
9. Run the script.

The augmented images and labels will be saved inside their corresponding `train` and `val` directories.

---

## MATLAB Requirements

The dataset-preparation module requires:

- MATLAB.
- Computer Vision Toolbox.
- Deep Learning Toolbox.

The script uses functions related to:

- Bounding-box transformation.
- Random affine transformations.
- Image resizing.
- HSV color modification.
- Image and annotation management.

---

# Fuzzy-Logic Mobile Platform Controller

The [`Fuzzy`](Fuzzy) directory contains the Arduino programs used to control a four-motor mobile robotic platform.

Detailed documentation is available in:

- [`Fuzzy/README.md`](Fuzzy/README.md)

The controller combines:

- Fuzzy logic.
- Bluetooth serial communication.
- Pulse-width modulation.
- Independent control of four DC motors.
- Position-error processing.
- Velocity-error processing.

---

## Fuzzy Controller Features

The controller uses two independent fuzzy systems:

- One controller for the X axis.
- One controller for the Y axis.

Each controller receives two inputs:

1. Position error.
2. Error variation or velocity.

For the X axis:

```text
ΔX
ΔẊ
```

For the Y axis:

```text
ΔY
ΔẎ
```

Each input uses five membership functions, resulting in:

```text
5 × 5 = 25 fuzzy rules per axis
```

The fuzzy output is converted into a motor PWM command.

---

## Fuzzy Controller Files

### `Fuzzy.ino`

Main automatic fuzzy-control program.

It:

- Receives X- and Y-axis errors.
- Evaluates the membership functions.
- Applies the fuzzy rule base.
- Calculates the control output.
- Selects the required movement direction.
- Generates the PWM motor command.

### `Carrito.ino`

Alternative program for direct manual control of the mobile platform through Bluetooth commands.

### `bluetooth.cpp` and `bluetooth.h`

Implement Bluetooth initialization, message reception, and command processing.

### `control.cpp` and `control.h`

Implement:

- Motor initialization.
- Motor direction control.
- PWM generation.
- Forward and reverse motion.
- Lateral motion.
- Rotation.
- Robot stopping.

---

## Fuzzy Controller Communication

### Debug Serial Port

```text
Port: Serial
Baud rate: 9600
```

### Bluetooth Serial Port

```text
Port: Serial3
Baud rate: 38400
Terminator: newline (\n)
```

Because the program uses `Serial3`, a compatible Arduino board is required.

An Arduino Mega 2560 or an equivalent board with multiple hardware serial ports is recommended.

---

## Automatic Controller Input Format

The automatic fuzzy controller expects four values identified by the characters `m`, `n`, `o`, and `p`:

```text
m<delta_x>n<velocity_x>o<delta_y>p<velocity_y>
```

Example:

```text
m0.20n-0.10o0.30p0.05
```

This message represents:

| Variable | Value |
|---|---:|
| `ΔX` | 0.20 |
| `ΔẊ` | -0.10 |
| `ΔY` | 0.30 |
| `ΔẎ` | 0.05 |

The received inputs are multiplied internally by `100` before they are evaluated by the fuzzy membership functions.

---

## Manual-Control Input Format

The manual-control program expects:

```text
<command>,<duty_cycle>.
```

Example for moving forward at a duty cycle of `60`:

```text
1,60.
```

Example for stopping:

```text
0,0.
```

---

## Available Motion Commands

| Action | Command |
|---|---:|
| Reverse | `-1` |
| Stop | `0` |
| Forward | `1` |
| Left | `2` |
| Right | `3` |
| Rotate left | `4` |
| Rotate right | `5` |

---

## Motor Connections

The motor-control pins are defined in `control.h`.

| Motor | RPWM | LPWM |
|---|---:|---:|
| Front left | 11 | 10 |
| Front right | 9 | 8 |
| Rear left | 7 | 6 |
| Rear right | 5 | 4 |

Before powering the platform, verify:

- Motor-driver voltage.
- Maximum motor current.
- PWM input compatibility.
- H-bridge connections.
- Common electrical ground.
- Battery polarity.
- Emergency stopping procedure.

Depending on the motor driver, the Arduino `RPWM` and `LPWM` signals may need to be connected to the opposite driver inputs. Confirm the connection using the driver datasheet and the robot electrical diagram.

---

## Recommended Hardware

The fuzzy-control module was designed for hardware equivalent to:

- Arduino Mega 2560.
- Four DC motors.
- Motor drivers with `RPWM` and `LPWM` inputs.
- HC-05 Bluetooth module or equivalent.
- External motor power supply.
- Common ground between all electronic modules.
- Four-wheel mobile robotic platform.

---

## Preparing the Arduino Sketches

`Fuzzy.ino` and `Carrito.ino` are alternative main programs. They should be compiled as separate Arduino sketches.

Do not compile both main `.ino` files together in the same Arduino sketch.

### Automatic fuzzy controller

Create a folder such as:

```text
FuzzyController/
```

Place the following files inside it:

```text
FuzzyController/
├── FuzzyController.ino
├── bluetooth.cpp
├── bluetooth.h
├── control.cpp
└── control.h
```

Copy the contents of `Fuzzy.ino` into `FuzzyController.ino`.

### Manual controller

Create another folder:

```text
Carrito/
```

Place the following files inside it:

```text
Carrito/
├── Carrito.ino
├── bluetooth.cpp
├── bluetooth.h
├── control.cpp
└── control.h
```

This separation prevents duplicate `setup()` and `loop()` definitions during compilation.

---

## Uploading the Arduino Program

1. Install the Arduino IDE.
2. Prepare either the automatic or manual sketch.
3. Connect the Arduino board to the computer.
4. Open the corresponding `.ino` file.
5. Select the correct Arduino board.
6. Select the corresponding serial port.
7. Verify the Bluetooth and motor-driver connections.
8. Compile the program.
9. Upload the program.
10. Open the Serial Monitor at `9600` baud.

---

# Installation

Clone the repository using:

```bash
git clone https://github.com/mondiaz19/Autonomous-mobile-manipulator-for-metal-recycling-applications.git
```

Enter the repository directory:

```bash
cd Autonomous-mobile-manipulator-for-metal-recycling-applications
```

The repository can also be downloaded as a ZIP file from GitHub.

---

# Reproducibility Notes

For reproducible dataset preparation:

- Preserve the original train, validation, and test division.
- Do not apply augmentation to the test subset.
- Preserve the random seed unless a different experimental configuration is being evaluated.
- Record all changes to the augmentation parameters.
- Preserve the YOLO label format.
- Keep image and label filenames synchronized.

For reproducible robot experiments:

- Record the Arduino board and firmware version.
- Record the Bluetooth-module configuration.
- Record the motor-driver model.
- Record the PWM limits.
- Record the power-supply voltage.
- Verify the motor direction before autonomous operation.
- Test the stop command before enabling the complete system.

---

# Research Applications

The resources in this repository may support research in:

- Autonomous mobile robotics.
- Intelligent recycling systems.
- Robotic waste collection.
- Metal-object detection.
- Computer vision.
- Deep learning.
- YOLO-based object detection.
- Fuzzy-logic control.
- Embedded robotic systems.
- Bluetooth-based robot communication.
- Human–robot and environment–robot interaction.

---

# Citation

When using this repository in academic work, please cite the associated publication:

> **“Intelligent Autonomous Mobile Manipulator for Metal Recycling Applications with Automatic Identification of Target Objects via Image Deep Learning Methods.”**

The complete author list, journal information, publication year, volume, pages, and DOI should be added to this section once the final bibliographic information is publicly available.

The repository can be referenced using:

```text
Autonomous Mobile Manipulator for Metal Recycling Applications.
GitHub repository:
https://github.com/mondiaz19/Autonomous-mobile-manipulator-for-metal-recycling-applications
```

---

# License and Use

The resources in this repository are intended primarily for research and educational purposes.

No separate `LICENSE` file is currently included in the repository. Contact the repository owner before commercial use, redistribution, or incorporation into another publicly distributed project.

---

# Contact

For questions, corrections, or collaboration proposals:

- Open an issue in this GitHub repository.
- Contact the repository owner through the GitHub profile [`mondiaz19`](https://github.com/mondiaz19).

---

## Repository Link

```text
https://github.com/mondiaz19/Autonomous-mobile-manipulator-for-metal-recycling-applications
```
