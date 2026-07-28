# Aluminum Detection Dataset — Preparation & Augmentation

This document describes the dataset preparation pipeline used to train the
YOLOv8n aluminum-piece detector reported in the manuscript, and how to run
`prepareAugmentedDataset.m` to reproduce the augmented training/validation
sets.

## 1. Pipeline overview

```
[1] Image collection          [2] Manual labeling          [3] Export + split           [4] Augmentation
    (arbitrary user-       →      MATLAB Image Labeler  →      YOLO .txt format,     →     prepareAugmentedDataset.m
     defined folder)                app (groundTruth)           train/val/test split         (this script)
```

1. **Image collection** — raw images are stored wherever the user originally
   saved them; this location is project-dependent and is *not* fixed by any
   script in this repository.
2. **Manual labeling** — images were manually labeled using MATLAB's
   **Image Labeler** app, producing a `groundTruth` object per the
   manuscript's labeling procedure (Section X).
3. **Export + split** — the `groundTruth` bounding boxes are exported to
   normalized YOLO `.txt` format and the dataset is split into
   train/validation/test (60/10/30%, fixed random seed) prior to
   augmentation.
4. **Augmentation** (this script) — operates on the split, YOLO-formatted
   dataset produced by step 3.

## 2. Expected input structure

`prepareAugmentedDataset.m` expects the dataset already organized as:

```
datasets/
├── images/
│   ├── train/
│   ├── val/
│   └── test/
├── labels/
│   ├── train/
│   ├── val/
│   └── test/
└── data.yaml
```

Each label file follows the standard YOLO format — one line per object:

```
class x_center y_center width height
```

All values normalized to `[0, 1]` relative to image size, e.g.:

```
0 0.593250 0.288375 0.085500 0.178750
```

(`class = 0` corresponds to the single "Aluminium" class used in this
study.)

## 3. What the script does

- **Train and validation splits only**: generates `numAugmentsPerImage`
  (default: 5) augmented copies of every image using random horizontal
  flip, rotation (10°–80°), scale (1.0×–1.25×), and HSV color jitter
  (Hue 0.1, Saturation 0.2, Brightness 0.2). Bounding boxes are
  transformed and re-normalized accordingly; boxes warped mostly out of
  frame (overlap < 0.25 IoU) are dropped.
- **Test split is left untouched.** This is intentional: it ensures the
  Precision/Recall/F1/mAP metrics reported in the manuscript (and in our
  responses to Reviewer Comments 3.1 and 3.2) are computed on clean,
  unaugmented data, consistent with the described evaluation protocol.
- **Optional resizing** — if `RESIZE_TO_FIXED_SIZE` is set to `true`,
  original images/boxes are also resized to a fixed input size before
  augmentation.
- **Reproducibility** — a fixed random seed (`rng(42)`) is used throughout,
  matching the seed used in the rest of the training/evaluation pipeline.

## 4. Usage

1. Place the split, YOLO-formatted dataset under `datasets/` as described
   in Section 2.
2. Open `prepareAugmentedDataset.m` and adjust the `CONFIGURATION` block
   if needed (dataset path, number of augmentations, resizing option).
3. Run the script from the folder containing `datasets/`.
4. Augmented images/labels are written alongside the originals in the same
   `train`/`val` folders, named `<original_name>-aug<N>.jpg` /
   `.txt`.

## 5. Requirements

- MATLAB with the Computer Vision Toolbox and Deep Learning Toolbox
  (for `bboxwarp`, `randomAffine2d`, `jitterColorHSV`, `bboxresize`).

## 6. Dataset license & citation

If this dataset is released publicly, please cite the associated
publication:

> [Full citation to be added upon publication / DOI assignment.]

Unless noted otherwise, this dataset is intended for research and
educational use. Please contact the corresponding author regarding any
commercial use or redistribution.
