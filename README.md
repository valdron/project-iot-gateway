<<<<<<< HEAD
## Welcome to GitHub Pages

You can use the [editor on GitHub](https://github.com/umadbro96/project-iot-gateway/edit/master/README.md) to maintain and preview the content for your website in Markdown files.

Whenever you commit to this repository, GitHub Pages will run [Jekyll](https://jekyllrb.com/) to rebuild the pages in your site, from the content in your Markdown files.

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/umadbro96/project-iot-gateway/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://help.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.
=======
# project-iot-gateway
A Project of students of the Hochschule Esslingen

## Installing Dependencies ##

### open62541 ###

```bash
git clone https://github.com/open62541/open62541.git
cd open62541
mkdir build
cd build
cmake ..
```
CMakeCache.txt bearbeiten   
UA\_ENABLE_AMALGAMATION:BOOL=OFF   
ersetzen durch  
UA\_ENABLE_AMALGAMATION:BOOL=ON  
```bash
cmake ..
sudo make install
```

### paho_mqtt ###

```bash
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
cmake .
sudo make install
```

## Creating CMakeLists.txt ##
```cmake
INCLUDE_DIRECTORIES("." dir1 dir2)
ADD_EXECUTABLE(NAME file1.c file2.c file3.c ...)
TARGET_LINK_LIBRARIES(TARGETNAME LIBRARY)
```

## Building ##

```bash
mkdir build && cd build
cmake ..
make
```

>>>>>>> upstream/master
