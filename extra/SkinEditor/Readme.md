# BPLGxEditor

It's a simple editor to layout **texts** of skins for BrewPiLessGx. It's not well designed neither well implemented, but it helps in designing skins. Only texts, color blocks, and RSSI bars are supported.

# Usage
## Import JSON file
It is simpler to start with an existing one. Find the "information" JSON file: `src/skins/skin_info.json`. Copy the content of the file, paste it to the input box at the bottom of the page, and then click "input" button.

## Edit
Changing the values of each items as you like. You can also move the text items by using mouse cursor. 

## Generate
Click "Generate" button, and the JSON content will be generated in the input box.

## Image
Try not to use images. They are resoure intensive. Currently, user-supplied images are not supported. You must edit the JSON content manually to use image. Please check the [document](/doc/skin.md) for detail. The embedded state icon is supported, though.