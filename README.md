# CNN Ripple

![CNN-ripple](https://open-ephys.github.io/gui-docs/_images/cnnripple.png)

A 1D convolutional neural network (CNN) operating over high-density LFP recordings to detect hippocampal SWR both offline and online. It works with recordings from several types of recordings (e.g. linear arrays, high-density probes, ultradense Neuropixels).


## Installation

This plugin can be added via the Open Ephys GUI Plugin Installer. To access the Plugin Installer, press **ctrl-P** or **⌘P** from inside the GUI. Once the installer is loaded, browse to the "CNN Ripple" plugin and click "Install."

## Usage

Instructions for using the CNN Ripple plugin are available [here](https://open-ephys.github.io/gui-docs/User-Manual/Plugins/CNN-Ripple.html).

## Building from source 

1. Clone this plugin repository inside of a directory that is at the same level as the plugin-GUI's source code. For example, if you have the plugin-GUI source code in `~/code/plugin-GUI`, you should clone this repository in `~/code/OEPlugins/cnn-ripple`. The directory structure should look like this:
    ```
    ~/code
    ├── plugin-GUI
    │   ├── Build
    │   ├── Source
    │   └── ...
    ├── OEPlugins
    │   └── cnn-ripple
    │       ├── Build
    │       ├── Source
    │       ├── libs
    │       ├── model
    │       └── ...
    ```

2. Navigate to the cnn-ripple's ```libs``` directory and open the folder with your current operating system's name

3. Download the tensorflow C libraries from the link below and extract the entire contents into the platform-specifc folder you opened in step 2 above. 

    - [Windows](https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-cpu-windows-x86_64-2.3.0.zip)
    - [Linux](https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-cpu-linux-x86_64-2.3.0.tar.gz)
    - [MacOS](https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-cpu-darwin-x86_64-2.3.0.tar.gz)


4. Navigate to the cnn-ripple's ```Build``` directory and run the following commands:

    Linux:
    ```bash
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release  ..
    make install -j4    
    ```

    Windows:
    ```bash
    cmake -G "Visual Studio 16 2019" -A x64  ..
    # Build and run the generated solution file in Visual Studio using the INSTALL target in Release configuration
    ```

    MacOS:
    ```bash
    # Intel based Mac:
    cmake -G "Xcode" -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=x86_64 ..

    # Apple Silicon based Mac (coming soon)
    cmake -G "Xcode" -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64 ..

    # Build and run the generated Xcode project using the INSTALL target
    ```
