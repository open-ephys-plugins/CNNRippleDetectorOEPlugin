# CNN Ripple

![CNN-ripple](cnn-ripple-plugin.png)

A 1D convolutional neural network (CNN) operating over high-density LFP recordings to detect hippocampal SWR both offline and online. It works with recordings from several types of recordings (e.g. linear arrays, high-density probes, ultradense Neuropixels).

This plugin allows the use of CNN-ripple as a real-time detection tool as part of the Open Ephys platform.

You can check our results in our manuscript ([Navas-Olive, Amaducci et al. eLife 2022](https://elifesciences.org/articles/77772))

## Installation

* NOTE: For now, you must manually install the relevant tensorflow libraries for your setup. We are working on a solution to automate this process.

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

3. Download the latest tensorflow C libraries from [here](https://www.tensorflow.org/install/lang_c) and extract the entire contents into the platform-specifc folder you opened in step 2 above. 

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

    # Apple Silicon based Mac: 
    cmake -G "Xcode" -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64 ..

    # Build and run the generated Xcode project using the INSTALL target
    ```

## Usage
The plugin receives 8 channels as input. You can use the `ChannelMap` plugin to select the channels.

- **File:** selector for the CNN model `.pb` file. The default model is located in the `cnn-ripple/model` directory.
- **Pulse duration:** duration of the TTL pulse sent when a ripple is detected (in milliseconds).
- **Timeout:** recovery time after a pulse is sent (in milliseconds).
- **Calibration:** calibration time before the experiment to setup the signals normalization (in seconds). One minute is usually enough.
- **Threshold:** probability threshold for the detections. Between 0 and 1.
- **Drift:** number of standard deviations above which the signal is considered to be dominated by extreme offset drift and the CNN will not predict.
- **Output:** output channel for TTL pulses.