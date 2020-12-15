# Barcode Intermediate Results
The C/C++ sample demonstrates how to get grayscale image, binary image, and predicted multi-barcode bounding boxes by configuring parameters for intermediate results.

## Installation
- [CMake](https://cmake.org/download/)
- [OpenCV 4.5.0](https://opencv.org/releases/)

    Configuration for CMake:

    ```md
    # Find OpenCV, you may need to set OpenCV_DIR variable
    # to the absolute path to the directory containing OpenCVConfig.cmake file
    # via the command line or GUI
    find_package(OpenCV REQUIRED)
    ```
- [Dynamsoft Barcode Reader 8.0](https://www.dynamsoft.com/Downloads/Dynamic-Barcode-Reader-Download.aspx)

## How to Build and Run
Before building the project, you need to get a [trial license](https://www.dynamsoft.com/customer/license/trialLicense) and update the following code:

```cpp
ret = reader.InitLicense("LICENSE-KEY");
```

### Windows

```
mkdir build
cd build

// x86
cmake -DCMAKE_GENERATOR_PLATFORM=x86 ..

// x64
cmake -DCMAKE_GENERATOR_PLATFORM=x64 ..

cmake --build . --config release

Release\BarcodeReaderDemo.exe
```

![multi-barcode grayscale image](https://www.dynamsoft.com/codepool/wp-content/uploads/2020/12/multi-barcode-grayscale-image.jpg)

![multi-barcode binary image](https://www.dynamsoft.com/codepool/wp-content/uploads/2020/12/multi-barcode-binary-image.jpg)

![multi-barcode predicted bounding boxes](https://www.dynamsoft.com/codepool/wp-content/uploads/2020/12/multi-barcode-predicted-bounding-box.jpg)

![multi-barcode final results](https://www.dynamsoft.com/codepool/wp-content/uploads/2020/12/multi-barcode-final-results.jpg)

### Linux

```
mkdir build
cd build
cmake ..
cmake --build . --config release 

./BarcodeReaderDemo
```

### ARM32

```
mkdir build
cd build
cmake -DBUILD_ARM=arm32 .. barcode-intermediate-results
cmake --build . --config release 

./BarcodeReaderDemo
```

## References
- https://www.dynamsoft.com/barcode-reader/parameters/scenario-settings/intermediate-result.html
- https://www.dynamsoft.com/barcode-reader/parameters/scenario-settings/terminate.html