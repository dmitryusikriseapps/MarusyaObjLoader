![](logo.jpg)

# MarusyaObjLoader

A small library for fast Wavefront .obj and .mtl files loading. As .obj parser was used this fantastic library [https://github.com/syoyo/tinyobjloader](https://github.com/syoyo/tinyobjloader) 

## Getting Started

These instructions will help you to install the package and use it.

### Installing

Using gradle:

```gradle
implementation 'com.riseapps.marusyaobjloader:marusyaobjloader:1.0.0'
```

Using maven:
```xml
<dependency>
  <groupId>com.riseapps.marusyaobjloader</groupId>
  <artifactId>marusyaobjloader</artifactId>
  <version>1.0.0</version>
  <type>pom</type>
</dependency>
```

Or download it as a file [here](https://github.com/dmitryusikriseapps/MarusyaObjLoader/releases)

### Usage

Use this code for the minimum functionality: 
```java
import com.riseapps.marusyaobjloader.MarusyaObjLoader;
import com.riseapps.marusyaobjloader.MarusyaObjLoaderImpl;
import com.riseapps.marusyaobjloader.model.ResultModel;

import java.io.File;
import java.io.FileNotFoundException;

private static final File FILES_PATH = new File(Environment.getExternalStorageDirectory(), "jni_test");
private static final File OBJ = new File(FILES_PATH, "luxury_house_interior.obj");
private static final File MTL = new File(FILES_PATH, "luxury_house_interior.mtl");

public void load() {
    final MarusyaObjLoader marusyaObjLoader = new MarusyaObjLoaderImpl();
    ResultModel resultModel = null;
    try {
      resultModel = marusyaObjLoader.load(
        OBJ,
        MTL,
        1.0f,
        true);
    } catch (FileNotFoundException e) {
      e.printStackTrace();
      }
    }
}
```

## Example

The full example of using is located [here](https://github.com/dmitryusikriseapps/MarusyaObjLoader/tree/master/app/src/main/java/com/riseapps/objloaderjni)

## Documentation

### Method

```java
ResultModel load(File obj, boolean flipTextureCoordinates) throws FileNotFoundException;
```

#### Arguments

obj - .obj file in a storage

flipTextureCoordinates - reverse a texture or not

#### Return

ResultModel - an object which contains shapes, materials, error and warning messages

### Method

```java
ResultModel load(File obj, File mtl, boolean flipTextureCoordinates) throws FileNotFoundException;
```

#### Arguments

obj - .obj file in a storage

mtl - .mtl file in a storage

flipTextureCoordinates - reverse a texture or not

#### Return

ResultModel - an object which contains shapes, materials, error and warning messages

### Method

```java
ResultModel load(File obj, float normalizeCoefficient, boolean flipTextureCoordinates) throws FileNotFoundException;
```

#### Arguments

obj - .obj file in a storage

normalizeCoefficient - allows normalizing vertices

flipTextureCoordinates - reverse a texture or not

#### Return

ResultModel - an object which contains shapes, materials, error and warning messages

### Method

```java
ResultModel load(File obj, File mtl, float normalizeCoefficient, boolean flipTextureCoordinates) throws FileNotFoundException;
```

#### Arguments

obj - .obj file in a storage

mtl - .mtl file in a storage

normalizeCoefficient - allows normalizing vertices

flipTextureCoordinates - reverse a texture or not

#### Return

ResultModel - an object which contains shapes, materials, error and warning messages

### Method

```java
void enableLog();
```

You will see a log like this:
```
D/MarusyaObjLoader: ***********************************************************************************
D/MarusyaObjLoader: Start parsing -> obj: /storage/emulated/0/jni_test/luxury_house_interior.obj, mtl: /storage/emulated/0/jni_test/luxury_house_interior.mtl
D/MarusyaObjLoader: End parsing -> /storage/emulated/0/jni_test/luxury_house_interior.obj
D/MarusyaObjLoader: shapes size -> 116
D/MarusyaObjLoader: materials size -> 25
D/MarusyaObjLoader: vertices size -> 390021
D/MarusyaObjLoader: normals size -> 392763
D/MarusyaObjLoader: texcoords size -> 130796
D/MarusyaObjLoader: colors size -> 390021
D/MarusyaObjLoader: indices size -> 773121
D/MarusyaObjLoader: Time to parse -> 1364 ms
D/MarusyaObjLoader: ***********************************************************************************
```

### Method

```java
void disableLog();
```

## Performance

For the performance tests were read 50 3-D models of .obj format. It was done using pure Java and JNI. For pure Java was used this cool library [https://github.com/javagl/Obj](https://github.com/javagl/Obj). The table below shows gain in speed for devices with different power when reading 3-D models of .obj format using pure Java and JNI.

| Device  | File size versus read time |
| :--- | :---: |
| **Google Pixel 2**  Qualcom Snapdragon 835 (4x2.35 GGz + 4x1.9 GGz), RAM 4 GB | ![](charts/google%20pixel%202.png) |
| **Huawei HRY - LX1** HiSiliconKirin 710 (4x2.2 GGz + 4x1.7 GGz), RAM 3 GB) | ![](charts/huawei%20hry-lx1.png) |
| **LGE Nexus 5X** Qualcom Snapdragon 808 (2x1.44 GGz + 2x1.82 GGz), RAM 2 GB) | ![](charts/lge%20nexus%205x.png) |
| **Samsung SM - G930F** Samsung Exynos 9 Octa 8890 (8x2.3 GGz), RAM 4 GB) | ![](charts/samsung%20sm-g930f.png) |
| **Samsung SM - N950F** Samsung Exynos 9 Octa 8895 (4x2.3 GGz + 4x1.7 ГГц), RAM 6 GB) | ![](charts/samsung%20sm-n950f.png) |
| **Samsung SM - T820** Qualcomm Snapdragon 820 (2x2.15 GGz + 2x1.6 GGz), RAM 4 GB) | ![](charts/samsung%20sm-t820.png) |
| **Xiaomi Mi A1** Qualcomm Snapdragon 625 (4x2.0 GGz), RAM 4 GB) | ![](charts/xiaomi%20mi%20a1.png) |
| **Xiaomi Redmi Note 5** Qualcom Snapdragon 636 (8x1.8 GGz), RAM 3 GB) | ![](charts/xioami%20redmi%20note%205.png) |

On average, the download speed using JNI is **4.71** times higher.

## License

This project is licensed under the Apache 2.0 License - see the [LICENSE](LICENSE) file for details.
