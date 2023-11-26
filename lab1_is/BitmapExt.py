
def remove_empty_spaces(image):
    width, height = image.size
    min_x = width
    min_y = height
    max_x = 0
    max_y = 0

    for y in range(height):
        for x in range(width):
            pixel_color = image.convert('RGBA')
            r, g, b, a = pixel_color.getpixel((x, y))

            if a == 255 and r == 0 and g == 0 and b == 0:
                min_x = min(min_x, x)
                min_y = min(min_y, y)
                max_x = max(max_x, x)
                max_y = max(max_y, y)

    new_width = max_x - min_x + 1
    new_height = max_y - min_y + 1
    crop_box = (min_x, min_y, min_x + new_width, min_y + new_height)
    cropped_image = image.crop(crop_box)

    return cropped_image
