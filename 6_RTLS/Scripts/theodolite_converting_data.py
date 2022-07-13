import math as op
import numpy as np

ANCHORS_0_ON_X = 0  # select the anchor that works as 0 referece on X axis
ANCHORS_0_ON_Y = 2  # select the anchor that works as 0 referece on Y axis

COORDINATES = 3

title_coord = ["Anchor 0", "Anchor 1", "Anchor 2", "Anchor 3", "Anchor 4", "Anchor 5", "Anchor 6", "Anchor 7",]

# [range diag, V, Hz, h]
Coords = np.array([
    [3373, 68.2898, 300.2326, 1312],    # anchor 0 Yellow
    [22688, 95.5185, 9.2528, 1312],    # anchor 1 Red
    [22478, 95.4276, 390.2912, 1312],     # anchor 2 Blue
    [3707, 71.1970, 93.3848, 1312],    # anchor 3 Green
    [8074, 87.2225, 372.8219, 1312],     # anchor 4 Orange
    [14600, 92.9770, 14.3665, 1312],   # anchor 5 Black
    [14488, 92.9093, 385.2126, 1312],     # anchor 6 Pink
    [7724, 86.8533, 28.8558, 1312],   # anchor 7 Purple
])

num_acoord = np.size(Coords, 0)
print(f"Number of measures: {num_acoord}")
result = np.zeros((num_acoord, COORDINATES))

for i in range(np.size(Coords, 0)):
    result[i, 0] = Coords[i, 0] * op.sin(((90 * Coords[i, 1]) / 100) * (
        op.pi) / 180) * op.cos(((90 * Coords[i, 2]) / 100) * (op.pi) / 180)
    result[i, 1] = Coords[i, 0] * op.sin(((90 * Coords[i, 1]) / 100) * (
        op.pi) / 180) * op.sin(((90 * Coords[i, 2]) / 100) * (op.pi) / 180)
    result[i, 2] = Coords[i, 0] * \
        op.cos(((90 * Coords[i, 1]) / 100) * (op.pi) / 180) + Coords[i, 3]

result = result.astype(int)
result = np.array(result)
print("Coordinates from Theodolite:")
print(np.array2string(result, separator=','))

# Matrix transformation
new_result = np.zeros((num_acoord, COORDINATES))
new_result = np.copy(result)

# X and Y value to correct coordinates
dist_x_correction = op.fabs(result[ANCHORS_0_ON_X, 0])
dist_y_correction = op.fabs(result[ANCHORS_0_ON_Y, 1])

last_result = np.zeros((num_acoord, COORDINATES))
last_result[:, 0] = new_result[:, 0] - dist_x_correction
last_result[:, 1] = new_result[:, 1] + dist_y_correction
last_result[:, 2] = new_result[:, 2]

print("Matrix Transformed form")
print(np.array2string(last_result, separator=','))

print("Matrix Script form")
for a, i in enumerate(last_result):
    print(f"{int(i[0])}, {int(i[1])}, {int(i[2])} - {title_coord[a]}")
