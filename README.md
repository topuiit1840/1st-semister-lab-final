# 🎨 Image Manipulation Software in C

![C](https://img.shields.io/badge/Language-C-blue.svg)
![OS](https://img.shields.io/badge/OS-Ubuntu%2FDebian-orange.svg)
![GUI](https://img.shields.io/badge/GUI-IUP-lightgrey.svg)

A graphical Image Manipulation Software built using the C programming language and the IUP toolkit. This application allows users to open 24-bit uncompressed BMP images, perform manual pixel manipulations (such as grayscale, blurring, cropping, and flipping), and save the modified results.

## 👨‍💻 Author

- **Name:** MD Toriqul Islam Topu
- **Course/ID:** [Final Lab Project]
- **GitHub:** [@Topu](https://github.com/topuiit1840)
- **Project Link:** [@Lab_Final_Project](https://github.com/topuiit1840/1st-semister-lab-final)
- **IUP Documentation:** [@IUP](https://www.tecgraf.puc-rio.br/iup/)

---

## ⚙️ System Requirements

Ensure your environment meets the following dependencies before building:

- **OS:** Ubuntu / Debian
- **Compiler:** `gcc` (version >= 14.2.0)
- **Version Control:** `git` (version >= 2.47.3)

---


## Screenshots

![1](./ui_images/1.png)
![2](./ui_images/2.png)
![3](./ui_images/3.png)
![4](./ui_images/4.png)
![5](./ui_images/5.png)
![6](./ui_images/6.png)
![7](./ui_images/7.png)
![8](./ui_images/8.png)
![9](./ui_images/9.png)
![8](./ui_images/10.png)


## 🚀 Setup & Installation

Run the following commands in your terminal to clone the repository, install the necessary system dependencies, and set up the isolated library environment.

```bash
# Clone the repo
git clone https://github.com/topuiit1840/1st-semister-lab-final.git
cd image_manipulation

# Install system dependencies
sudo apt update
sudo apt install build-essential libgtk-3-dev libx11-dev pkg-config

# Create isolated library directories
mkdir ./iup
mkdir ./im

# Extract precompiled libraries into the isolated folders
tar -zxvf iup-3.32_Linux515_64_lib.tar.gz -C ./iup
tar -zxvf im-3.15_Linux515_64_lib.tar.gz -C ./im

```
