# butano-ldtk

[![Build examples status badge](https://github.com/copyrat90/butano-ldtk/actions/workflows/build-examples.yml/badge.svg)](https://github.com/copyrat90/butano-ldtk/actions/workflows/build-examples.yml)
![API Docs status badge](https://github.com/copyrat90/butano-ldtk/actions/workflows/doxygen.yml/badge.svg)

[butano-ldtk](https://github.com/copyrat90/butano-ldtk) is a [LDtk](https://ldtk.io/) level importer for the [Butano engine](https://github.com/GValiente/butano).

![](docs/images/parallax_level_bgs.gif)

## Features

* Render LDtk levels as GBA backgrounds with only one line of C++ code.
* Access *almost* all datas in the [LDtk JSON](https://ldtk.io/json) as `constexpr` objects.
    * Entities, Enums, Level fields, ... anything useful is imported.
* Provides [usage examples](examples/) and [API documentation](https://copyrat90.github.io/butano-ldtk).

## Getting started

### Prerequisites

* [Butano](https://gvaliente.github.io/butano/getting_started.html): 20.0.0 or later
* [LDtk](https://ldtk.io/download/): **exactly** 1.5.3
* [Python](https://www.python.org/downloads/): 3.10 or later

### Setup

1. Clone (or download) this repo to your desired location.
    ```bash
    git clone https://github.com/copyrat90/butano-ldtk.git
    ```
2. Install Python dependencies ([Pillow](https://pillow.readthedocs.io/en/stable/index.html)).
    ```bash
    cd /path/to/butano-ldtk
    pip install -r requirements.txt
    ```
3. Edit the `Makefile` of your own project.
    1. Add `BUILDLDTK` and `LIBBUTANOLDTK` right below the `LIBBUTANO`.
    ```diff
    BUILD       	:=  build
    LIBBUTANO   	:=  /path/to/butano/butano
    + BUILDLDTK   	:=  build_ldtk
    + LIBBUTANOLDTK	:=  /path/to/butano-ldtk
    ```
    2. Add butano-ldtk folders (including will-be-generated ones) to your `SOURCES`, `INCLUDES` and `GRAPHICS`.
    ```diff
    - SOURCES     	:=  src
    - INCLUDES    	:=  include
    + SOURCES     	:=  src $(LIBBUTANOLDTK)/src $(BUILDLDTK)/src
    + INCLUDES    	:=  include $(LIBBUTANOLDTK)/include $(BUILDLDTK)/include
    DATA        	:=
    - GRAPHICS    	:=  graphics
    + GRAPHICS    	:=  graphics $(BUILDLDTK)/graphics
    ```
    3. Add python script execution to convert your `*.ldtk` project file.
    ```diff
    - EXTTOOL     	:=
    + EXTTOOL     	:=  @$(PYTHON) -B $(LIBBUTANOLDTK)/tools/butano_ldtk.py --input=/path/to/your_project.ldtk --build=$(BUILDLDTK)
    ```
        * This will convert `your_project.ldtk` to every time you run `make -j$(nproc)`.
    4. Add `$(BUILDLDTK)` to `USERBUILD`
    ```diff
    - USERBUILD   	:=
    + USERBUILD   	:=  $(BUILDLDTK)
    ```
    * If not sure, you can take a look at any of the [examples](examples/)' Makefile.
4. Run `make -j$(nproc)` to see if your project builds.

### Basic usage

You can create a level as GBA backgrounds with this:

```cpp
// DO NOT use angular brackets <> for butano-ldtk headers!
#include "ldtk_core.h"
#include "ldtk_level.h"
#include "ldtk_project.h"

#include "ldtk_level_bgs_ptr.h"

// These are all the generated header files you might want to include
#include "ldtk_gen_project.h"
#include "ldtk_gen_enums.h"
#include "ldtk_gen_idents.h"
#include "ldtk_gen_iids.h"
#include "ldtk_gen_tags.h"

#include <bn_core.h>

int main()
{
    bn::core::init();
    // Initializes butano-ldtk, this should be after `bn::core::init();`
    ldtk::core::init();

    // Locate the constexpr `gen_project` instance.
    //
    // This corresponds to the "root" of LDtk JSON.
    // With its member functions, you can access most of datas.
    constexpr const ldtk::project& gen_project = ldtk::gen::gen_project;

    // Get the level with its identifier.
    // (Assuming your project has a level named "level_0")
    constexpr const ldtk::level& level_0 = gen_project.get_level(ldtk::gen::level_ident::level_0);

    // Create a `ldtk::level_bgs_ptr` with the level.
    //
    // Internally, each layer with visible tiles would create one `bn::regular_bg_ptr`
    ldtk::level_bgs_ptr level_0_bgs = level_0.create_bgs();

    while (true)
        bn::core::update();
}
```

Head over to the [`examples/`](examples/) directory for more sophisticated examples.

And don't forget to check out the [API documentation](https://copyrat90.github.io/butano-ldtk)!

## Notes

### Limitations

* butano-ldtk requires your LDtk project to use `Full lowercase` identifier convention.<br/>
  You can set it on your project settings.
    ![](docs/images/full_lowercase.png)
* Parallax scaling of layers is not supported.
    * If you set a parallax value, you need to **uncheck** the `[ ] Scale ?` box.
        ![](docs/images/uncheck_parallax_scale.png)
* `Save levels to seperate files` option is not supported.
* Importing external enum files is not supported.
* Some field types are not supported:
    * File path
    * Tile
* Tiles not aligned to the grid are not supported.
* Different layer opacity values are not supported.
    * This is because GBA only supports one global BG transparency alpha.
* If you have more than 4 layers with *visible* tiles, the converter will error out.
    * This is because GBA only supports 4 regular BGs visible.
    * You can have many layers without a *visible* tile, that won't create regular BG.
* Metadatas associated with LDtk Enums are not imported.
    * This is because enums are directly converted to C++ enums.
* Unused tiles in the tilesets are not imported.

### Caveats

* LDtk 1.5.3 has [a bug](https://github.com/deepnight/ldtk/issues/1188) that when you update the definitions of layers, entities, etc, it is not propagated properly to all the instances.<br/>
  When this happens, unexpected results might happen.
    * To avoid this, when you make any changes to the definitions, do this:
        1. Change the identifier convention to `Full uppercase` temporarily.
        2. Save the project.
        3. Change it back to `Full lowercase`
        4. Save the project again.
* While the layer opacity value enables the blending of the BG of that layer,<br/>
  the global transparency alpha value itself is not touched.<br/>
    * This is because you might be controlling the transparency alpha value elsewhere.
    * To apply the transparency, you can simply do this:
    ```cpp
    #include "ldtk_gen_project.h"
    #include <bn_blending.h>
    ...
    bn::blending::set_transparency_alpha(ldtk::gen::gen_project.opacity());
    ```

### Others

* **DO NOT** access anything named with `priv` directly.
    * Those are private headers that might be changed in the future.<br/>
      So, always access members starting from `ldtk::gen::gen_project` member functions.

## Licenses

### Source codes

butano-ldtk is licensed under the [zlib license](LICENSE).

* [Butano](https://github.com/GValiente/butano) is licensed under the [zlib license](licenses/butano.txt).
* [LDtk](https://github.com/deepnight/ldtk) is licensed under the [MIT license](licenses/ldtk.txt).

### Assets

License files of the graphics used for examples can be found in each example folders. (`examples/*/licenses/`)
