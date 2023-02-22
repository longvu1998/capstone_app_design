import base64
from PIL import Image

with open("Vedentium Logo.png", "rb") as image_file:
    encoded_string = base64.b64encode(image_file.read()).decode('utf-8')

print(encoded_string)
