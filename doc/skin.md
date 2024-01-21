# Skin Definition

There three options for main display: brewpi, all information, and custom. The custom skin is loaded from the file system, and
is defined by the same rules as embedded ones.

## A.Global options
Global options. They are all **optional**. You don't need to specify them when not needed.
| Field | code/key | Description |
|--| -- |--|
|Background image for main screen| **bg** | specifiy the background image file.|
|Background image for control screen| **cbg** | specifiy the background image file.|
|Background image for setting screen| **sbg** | specifiy the background image file.|
|Static Texts | **stxt** | A list of static texts. |
|Static Text Style | **stxs** | Default the default style of static texts. | 
|Rectangle Blocks | **cbs** | Define rectangels. |
| Click Area | **click** | to specify the areas to input or setting screen.|
| Custom Font 1 | **c1** | file path for custom font 1. | 
| Custom Font 2 | **c2** | file path for custom font 2. | 
| Custom Font 3 | **c3** | file path for custom font 3. | 

### A.1 Background Images
Specify the background images for the screens. Optional. The value can be **Image File** or **Embedded Image File**. See following sections for more detail. These options provide a way to localize the display for non western European languages, given the fact that default fonts include only ASCII table alphabets.

### A.2 Static Texts and styles
To display static texts on main screen, specify the text and text properties.

Static Texts can have a common Style object, specified by **stxs**. All static text labels will share the same style, inclding

| Field | key | Description |
|--| -- |--|
|text color | **tc** | Text color, specified as hex code. eg. "FFFFFF" for white color.|
|text font | **tf** | text font, can be : "**s**": small system font(default), "**m**": mideum font, "**l**": large system font. "**c1**": custom font 1 if available, "**c2**": custom font 2 if available, "**c3**": custom font 3 if available |
|text decoration | **td** | Decoration. Can be "u" for underlined, and "s" for Strikethrough. |

| Field | key | Description |
|--| -- |--|
|text  | **tx** | The text string in Static Texts definitions. |

The postion and text properties are specified as **Common style specifiers** and **Text type specififier**. The style specification, like font and color, will overwrite the common styles.


### A.3 Rectangle Blocks
To specify some color rectangle blocks on screen. 
| Field | key | Description |
|--| -- |--|
|Fill Color  | **fc** | Fill Color, default to black color. |
|Border corner | **r** | integer value. To specify rounded corner. |
|Border width | **bw** | border width. |
|Border color | **bc** | Border color. |

### A.4 Click Area

The click areas define the area which, when cliked, will go to the setting/control screens.
|Control type | key | description |
|--| -- |--|
|Temperature Control | **tc** | to seitch Temperature control screen |
|Network settings | **set** | to switch to Network settings |
|Input Gravity | **ig** | to input Current gravity |
|Input OriginalGravity | **iog** | to input Original Gravity|

The area is defined as a retangle on the screen by specifiying x, y, width, and height, as well as alignment. (see below)
example:
```
    "click":{
    "set":{"w":165,"h":88,"a":"tr"},
    "ig":{"w":132,"h":88,"x":0,"y":230,"a":"tr"},
    "iog":{"w":94,"h":88,"x":-151,"y":224,"a":"tr"},
    "tc":{"w":274,"h":158}
    }
```
### A.5 Custom Fonts

To specify font files used by text widgets.

## B. Fields/Values
The following fields are available. Only fields present in the JSON file are shown.
Note: keys are **case sensitive**.

| Field | code/key | Type |
|--| -- |--|
| Mode | **mode** | enumeration(text or icons) |
| State | **state** | enumeration(text or icons) |
| Status time | **st** | text(time) |
| Beer Temperature| **bt** | text(number) |
| Beer Set|  **bs** | text(number) |
|Fridge Temperature| **ft** | text(number) |
|Fridge Set | **fs** | text(number) |
|Room Tempperature | **rt** | text(number)|
| Temperature Unit | **tu** | text, &deg;C or &deg;F |
| WiFi RSSI | **rx** | text or widget |
| Hydrometer related items | |
| Last update time | **lu** | text(time)|
| Temperature | **gt** | text(number)|
| RSSI | **grx** | text or widget |
| Tilt Angle | **tilt** | text(number) |
| Battery| **bat** | text(number), "V" appended, or "%" for Pill. |
| Gravity  | **sg** | text(number) |
| Original Gravity | **og** | text(number) |
| ABV | **abv** | text(number), % appended |
| ATT | **att** | text(number),% appended |
| Pressure | **psi** | text(number) |
| Wireless Hydrometer name | **hn** | text. For Pill and Tilt, it will be "Pill" and "Tile". |

### B.1 Common style specifiers
All the elements share the following common attributes, which defines the postion on screen. They are derived from CSS concept.
| Field | key | Description |
|--| -- |--|
| Position alignment| **a** | position alignment. can be "**c**"(center), "**tl**"(top left), "**bl**"(bottom left), "**lm**"(left middle), "**tr**"(top right), "**br**"(bottom right), "**rm**"(right middle). <u>Default to **tl**(top left)</u>.|
| x | **x** | postion of x, <u>default to 0</u>. The position is related to position alignment. |
| y | **y** | postio of y, <u>default to 0</u> |
| width | **w**| integer value. strait numbers represent "pixel", appended "%" means percentage of parent(the screen), and "c" means content size. <u>Default to **c**(content size).</u> |
| height | **h**|Height, same as above |

### B.2 Text type specififier
All fields are by default text types. The style are defined by the speicifiers followed.
| Field | key | Description |
|--| -- |--|
|text color | **tc** | Text color, specified as hex code. eg. "FFFFFF" for white color, which is default value.|
|text alignment | **ta** | Text Alignment, can be "**c**":center, "**l**":left(Default), or "**r**":right|
|text font | **tf** | text font, can be : "**s**": small system font(default), "**m**": mideum font, "**l**": large system font. "**x**": extra large for numbers only. "**c1**": custom font 1 if available, "**c2**": custom font 2 if available, "**c3**": custom font 3 if available |

### B.3 Text(time) specifications
For **Status time**("st") and **iSpindel Last update time**("lu"). The format can be specified.
| Field | key | Description |
|--| -- |--|
| Time format | **fmt** | specifiy time format |

| Specifier | Meaning | Description |
|--| -- |--|
| %y | 4 digit year | eg. 2023 |
| %Y | 2 digit year | eg. 89 as 1989 |
| %M | month | 1 ~ 12 |
| %d | month day | 1~31 |
| %D | week of the day | "SUN", "MON" .. |
| %h | hour | two digit, 24hr format |
| %m | minutes | 0-59|
| %s | seconds | 0-59 |

example: "%y/%M/%d %h:%m%s" will generate something like "2023/11/12 21:45:39".

For **Status time** time, only hour, minute, and seconds are available.

### B.4 enumeration
"Mode" and "State" are enumeration type. By default, they are presented as a text type. The displayed texts can be customized, too. 

Possible values for "Mode":
| Value | Meaning | Description |
|--| -- |--|
| 0 | Off | temperature control is not running |
| 1  | Fridge Constant | |
| 2  | Beer Constant | |
| 3  | Beer Profile | |
| 4  | Invalid mode | |

Possible values for "State":
| Value | Meaning | Description |
|--| -- |--|
| 0 | Idle | |
| 1 | Off | |
| 2 | Door Open | |
| 3 | Heating | |
| 4 | Cooling | |
| 5 | Waiting to cool | |
| 6 | Waiting to heat | |
| 7 | Waiting for peak detected | |
| 8 | cooling, wait for minimum time ||
| 9 | heating, wait for minimum time | |
| 10 | invalid state | |

To specify text representation, add the following into the field definition.(text is default value, so it is not necessary to specify.):
```
"rep":"txt"
```
To specify customized texts, use **"texts"**. For example, you can use localized texts(only for ASCII alphabets) like:
```
"texts":["Apagado","Nevera Const","Cerveza Const.Cerveza Perfil","Invalido"]
```

A fancier way to present the values is by icons. All icons should be in a single file with the same size. "rep" should be specified as "**ico**", and the path of the image file as well as the offets of the icons should be specified. Offsets are specified as [x0,y0,x1,y1, ...].

example:
Usig the image file
![](img/state_icons_4bit.png)

```
"mode":{"w":60,"h":16,"x":12,"y":8,"a":"tl","rep":"ico","icons":"A:\modec_zh.bin","off":[0,0,0,16,0,32,0,48,0,64]}
```

The icons(picture) are displayed in a 60x16 pixels area. The whole image file of "mode_zh.bin" is a image of size 60x80. According to the offset specification, if the value of mode is 2(Beer constant), (0,32) will be applied.


### B.5 RSSI
RSSI can be displayed as text(number, default value) or widgets that draws bars as signal strength. Specify "rep" to "wgt" to display widget type representation. Three additional parameters can be specified:

| key/code | Meaning | Description |
|--| -- |--|
| **bg** | Backgroud color | |
| **fc** | Forgroud color, for "solid" bar. | |
| **sc** | shadow color, for "invisible" bar | |

The colors are customizable to be better blending into the background color.

```
"rx":{"w":24,"h":24,"x":-4,"y":2,"a":"tr","rep":"wgt","bg":"E6E6E6","fc":"000000","sc":"DDDDDD"},
```
## C. Fonts and Images
Custom fonts and images in the file system(LittleFS) of BPL can be used. To use files from LittleFS, speicify the path starting from "A:/".

### C.1 Fonts

![Fonts](fonts.jpg)

There are four fonts available: 14pt(s), 32pt(m), 48pt(l), and 108pt(x). The 108pt font includes only numbers. At most three custom fonts can be used. The font file should be converted to binary format by lv_font_conv tool.

Git hub of lv_font_conv:[https://github.com/lvgl/lv_font_conv](https://github.com/lvgl/lv_font_conv)

If the font is used for number only, specify the range 0x2D-0x39(0x3a for ":" if used in time format) to save space.

## C.2 Image files

Same as font, images should be converted. Use [LVGL tool](https://lvgl.io/tools/imageconverter) to convert image into binary format.
To save space and processing/loading time, use as little colors as possible.

## C.3 Embedded Image files

The embedded images are also available for custom skin. There is only one embedded image available. 

Use "I:" prefix to specify embeded images. For example, the following JSON use embeded state icons, as displayed above.

```
"state":{"w":24,"h":24,"x":128,"y":4,"rep":"ico","icons":"I:istate","off":[0,0,24,0,48,0,72,0,96,0,120,0,144,0,168,0,192,0,216,0,240,0]}
```

The state icons:
![State Icons](img/state_icons_4bit.png)
From left to right, they represents states of "Idle", "Off","Door Open","Heating", "Cooling," Wait to Cool", "Wait to Heat","Wait for Peak", "Cooling minimum time","Heating minimum Time", "Invalid".


# D. Add Embdeded Skin files

Loading file from file system is relative slow, and memory consuming. If you want to replace the embedded skins, you can edit the embedded JSON files, in "src/skins/" folder.

TBD.

# E. Guideline

- Loading files are resource intensive. Avoid if possible.
- The limit of maximum size of JSON files is 3k. Omit values that are default.
- Reduce image size by reducing the color depth. Using black-white is the best. Use as small files as possible.

# F. Example

Embedded BrewPi screen:
```
{"cbs":[{"w":320,"h":36,"fc":"00AAAA"},
{"w":159,"h":36,"y":204,"fc":"00AAAA"},
{"w":159,"h":36,"x":161,"y":204,"fc":"00AAAA"}],
"stxt":[{"tx":"Target","x":110,"y":180},
{"tx":"Fridge","x":14,"y":214,"fc":"FFFFFF"},
{"tx":"FridgeSet","x":166,"y":214,"fc":"FFFFFF"}],
"mode":{"w":120,"x":8,"y":8},
"state":{"w":24,"h":24,"x":128,"y":4,"rep":"ico","icons":"I:istate","off":[0,0,24,0,48,0,72,0,96,0,120,0,144,0,168,0,192,0,216,0,240,0]},
"st":{"w":"120","x":162,"y":8,"ta":"l","tc":"FFFFFF"},            
"rx":{"w":24,"h":24,"x":-4,"y":2,"a":"tr","rep":"wgt","bg":"00AAAA","fc":"DDDDDD","sc":"0f7a79"},
"bt":{"a":"c","tc":"00FF00","ta":"c","tf":"x"},
"tu":{"x":114,"y":64,"a":"c","tf":"m"},
"bs":{"x":58,"y":64,"a":"c","tc":"FF3300","ta":"r","tf":"m"},
"ft":{"x":-70,"y":102,"a":"c","tc":"33FF00","ta":"r","tf":"s"},
"fs":{"x":104,"y":102,"a":"c","tc":"FF0000","ta":"r","tf":"s"},
"click":{"set":{"w":88,"h":88,"a":"tr"},
"tc":{"w":160,"h":24,"x":110,"y":174}}
}

```

Embedded Information screen
```
{
    "mode":{"w":100},
    "bt":{"w":80,"x":4,"y":44,"tc":"03EE03","ta":"r","tf":"m"},
    "bs":{"w":80,"x":112,"y":44,"tc":"FF3333","ta":"r","tf":"m"},
    "rt":{"w":80,"x":212,"y":44,"tc":"00AAAA","ta":"r","tf":"m"},
    "tu":{"w":20,"x":296,"y":58,"tc":"00AAAA","ta":"r"},
    "ft":{"w":80,"x":4,"y":96,"tc":"03EE03","ta":"r","tf":"m"},
    "fs":{"w":80,"x":112,"y":96,"tc":"FF3333","ta":"r","tf":"m"},
    "state":{"w":114,"x":102,"y":0,"ta":"r"},
    "st":{"w":80,"x":220,"y":0},
    "psi":{"w":80,"x":212,"y":96,"tc":"00AAAA","ta":"r","tf":"m"},
    "sg":{"w":100,"x":208,"y":196,"tc":"03EE03","ta":"r","tf":"m"},
    "gt":{"w":48,"x":64,"y":172},
    "tilt":{"w":48,"x":64,"y":194},
    "bat":{"w":48,"x":64,"y":220},
    "og":{"w":42,"x":164,"y":172,"ta":"r"},
    "abv":{"w":42,"x":164,"y":194,"ta":"r"},
    "att":{"w":42,"x":164,"y":220,"ta":"r"},
    "grx":{"w":36,"x":280,"y":148},
    "lu":{"w":80,"x":124,"y":148,"ta":"l"},
    "rx":{"w":16,"h":16,"x":1,"y":1,"a":"tr","rep":"wgt"},
    "hn":{"w":120,"x":2,"y":148,"ta":"c"},
    "stxs":{"tc":"AAAAAA"},
    "stxt":[
        {"tx":"Beer","x":36,"y":30},
        {"tx":"BeerSet","x":136,"y":30},
        {"tx":"Room","x":242,"y":30},
        {"tx":"Fridge","x":30,"y":82},
        {"tx":"FridgeSet","x":132,"y":82},
        {"tx":"Pressure","x":234,"y":82},
        {"tx":"psi","x":294,"y":112},
        {"tx":"RSSI","x":238,"y":148},
        {"tx":"Temp","x":16,"y":172},
        {"tx":"Angle","x":16,"y":194},
        {"tx":"Battery","x":6,"y":220},
        {"tx":"OG","x":128,"y":172},
        {"tx":"ABV","x":120,"y":194},
        {"tx":"ATT","x":123,"y":220},

        {"tx":"Gravity","x":234,"y":172}
    ],
    "click":{
    "set":{"w":100,"h":50,"a":"tr"},
    "ig":{"w":100,"h":80,"a":"br"},
    "iog":{"w":100,"h":80,"x":-100,"y":0,"a":"br"},
    "tc":{"w":200,"h":120}
    }
}
```

Pill Information:
```
{"stxt":[{"tx":"RAPT Pill Hydrometer","w":280,"x":8,"y":8,"ta":"l"},
{"tx":"Gravity","x":60,"y":64},
{"tx":"Temp","x":6,"y":108},
{"tx":"Angle","x":168,"y":108},
{"tx":"OG","x":22,"y":146},
{"tx":"ABV","x":14,"y":176},
{"tx":"ATT","x":20,"y":206},
{"tx":"Battery","x":156,"y":146},
{"tx":"RSSI","x":176,"y":176},
{"tx":"Last Seen","x":138,"y":206},
{"tx":"TOPLEFT"},
{"tx":"BOTTMRIGHT","a":"br","x":-10},
{"tx":"Center","a":"c"}
],
"sg":{"x":128,"y":36,"tf":"l","tc":"33FF33"},
"gt":{"x":54,"y":94,"w":86,"tf":"m","ta":"r","tc":"FF3333"},
"tu":{"w":"c","x":145,"y":110,"tc":"FF3333"},
"tilt":{"x":218,"y":94,"tf":"m","tc":"5555DD"},
"og":{"x":64,"y":146,"tc":"00AAAA"},
"abv":{"x":64,"y":176,"tc":"00AAAA"},
"att":{"x":64,"y":206,"tc":"00AAAA"},
"bat":{"x":228,"y":146,"tc":"00AAAA"},
"grx":{"x":228,"y":176,"tc":"00AAAA"},
"lu":{"x":228,"y":206,"fmt":"%h:%m","tc":"00AAAA"},
"rx":{"w":16,"h":16,"x":1,"y":1,"a":"tr","rep":"wgt"},
"click":{"set":{"w":88,"h":88,"a":"tr"},
"iog":{"w":160,"h":94,"x":22,"y":146,"a":"tl"}}
}
```