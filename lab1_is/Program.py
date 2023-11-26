# -*- coding: utf-8 -*-
from PIL import Image
import os
import numpy as np
import fnmatch
from BitmapExt import remove_empty_spaces
from Corners import Corners

original_path = 'выборка'
save_path = 'обрезанные'
search_pattern = '*.bmp'

def main():
    classify_all_shapes(crop_images(get_images(original_path, search_pattern)))

def get_images(path, search_pattern):
    images = {}
    for filename in os.listdir(path):
        if fnmatch.fnmatch(filename, search_pattern):
            image = Image.open(os.path.join(path, filename))
            images[filename] = image
    return images

def crop_images(images):
    for filename, image in images.items():
        images[filename] = remove_empty_spaces(image)
    return images

def classify_all_shapes(images):
    for filename, image in images.items():
        corners = count_corners(np.array(image))
        # print(np.array(image))
        result = classify_figure(corners, is_shape_closed(image), is_width_equal_to_height(image))
        print(filename + ': ' + result + ', ' + str(corners) + '\n')

def count_corners(image):
    total_corners = 0
    window_size = 3
    x, y = 0, 0
    width, height = image.shape

    # линия 
    if (image == 0).all():
        return total_corners

    while y <= height - window_size:
        window = translate_window(image, x, y)

        # ускоряем работу
        if is_corner(
            window, 
            [
            [0, 1, 1],
            [0, 1, 1],
            [0, 0, 0]
            ]):
            total_corners = 4
        return total_corners
        
        for template in Corners.Templates:
            if is_corner(window, template):
                total_corners += 1
                x += window_size
                break

        if x < width - window_size:
            x += 1
        else:
            x = 0
            y += 1

    return total_corners

def translate_window(image, x, y):
    window_size = 3
    window = image[y:y + window_size, x:x + window_size]
    return window

def is_shape_closed(image):
    matrix = np.array(image)
    is_shape_closed_go_around_figure(matrix)
    numRows, numCols = matrix.shape

    if (matrix == 0).all():
        return False

    for row in range(numRows):
        for col in range(numCols):
            if matrix[row, col] == 1:
                if not has_at_least_two_neighbors(matrix, row, col):
                    return False

    return True

def is_shape_closed_go_around_figure(matrix):
    x0, y0 = 0, 0
    numRows, numCols = matrix.shape

    for row in range(numRows):
        for col in range(numCols):
            if matrix[row, col] == 1:
                x0, y0 = col, row
                break

    return check_closure(matrix, x0, y0)

# ищем первую точку фигуры, предполгагая, что это начало
def check_closure(image, start_x, start_y):
    rows, cols = image.shape
    visited = np.zeros_like(image, dtype=bool)  # матрица такого же размера этой же формы
    queue = [(start_x, start_y)]
    visited[start_y, start_x] = True

    while queue:
        x, y = queue.pop(0)
        dx = [-1, -1, -1, 0, 0, 1, 1, 1]
        dy = [-1, 0, 1, -1, 1, -1, 0, 1]

        for i in range(8):
            nx, ny = x + dx[i], y + dy[i]

            if 0 <= nx < cols and 0 <= ny < rows:
                if image[ny, nx] == 1 and not visited[ny, nx]:
                    queue.append((nx, ny))
                    visited[ny, nx] = True

    return visited[start_y, start_x]

#  проверяем можно ли дойти до первой точки поиском в ширину
def has_at_least_two_neighbors(matrix, row, col):   
    num_neighbors = 0
    numRows, numCols = matrix.shape

    if row > 0 and matrix[row - 1, col] != 0:
        num_neighbors += 1
    if row < numRows - 1 and matrix[row + 1, col] != 0:
        num_neighbors += 1
    if col > 0 and matrix[row, col - 1] != 0:
        num_neighbors += 1
    if col < numCols - 1 and matrix[row, col + 1] != 0:
        num_neighbors += 1

    if row > 0 and col > 0 and matrix[row - 1, col - 1] != 0:
        num_neighbors += 1
    if row > 0 and col < numCols - 1 and matrix[row - 1, col + 1] != 0:
        num_neighbors += 1
    if row < numRows - 1 and col > 0 and matrix[row + 1, col - 1] != 0:
        num_neighbors += 1
    if row < numRows - 1 and col < numCols - 1 and matrix[row + 1, col + 1] != 0:
        num_neighbors += 1

    return num_neighbors >= 2

def is_corner(window, template):
    for _ in range(4):
        if np.array_equal(window, template):
            return True
        else:
            window = rotate_matrix(window)
    return False

def rotate_matrix(matrix):
    return np.rot90(matrix)

def is_width_equal_to_height(image):
    return image.width == image.height

def classify_figure(corners, is_closed=True, is_width_equal_to_height=False):
    if corners == 4:
        if is_width_equal_to_height:
            return 'Square'
        return 'Rectangle'
    if corners == 3:
        return 'Triangle'
    if corners == 0 or corners > 4:
        if is_closed:
            if is_width_equal_to_height:
                return 'Circle'
            return 'Ellipse'
        else:
            return 'Line'
    return 'Undefined'

if __name__ == '__main__':
    main()
