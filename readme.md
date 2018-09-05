# Unipic

Unipic enables you to get rid of thumbnails or watermarks.

[[Pre-built binaries]()]

## How it works

According to [Looks Like It](http://www.hackerfactor.com/blog/index.php?/archives/432-Looks-Like-It.html) from Dr. Neal Krawetz, a 64 bits data, known as finger-print, can be extracted from an image by a specific algorithm. Images are considered similar if hamming distance between finger-prints of them is less than a given threshold. In fact, hamming distance of an image and its thumbnail is practically zero, which means their finger-prints are the same. In addition, watermarks have less to do with finger-print of an image.

Unipic adopts OpenCV to perform finger-prints extraction from external images and use them to find similar images in unipic storage. Images in unipic storage will be replaced if they have a lower resolution than that of the external similar one. In response of massive amount of images, TBB is imported to accelerate finger-print extraction and comparison by multi-threads.

## Build

Ensure that OpenCV is compiled and installed.

```` sh
git clone https://github.com/yewmint/unipic.git

git submodule init
git submodule update

# run dist build script
bash ./dist.sh
# or debug build script
bash ./rebuild.sh
````

## Directory tree

* `<path-to-workspace>` - work space
* `<path-to-workspace>/storage` - where stored images lie
* `<path-to-workspace>/external` - where new images lie

## Work flow

1. cd to unipic work space
2. put your new images in `external` folder
3. run `unipic`
4. debut images will be appended to storage
5. duplicate images with higher resolution will replace similar ones in storage
