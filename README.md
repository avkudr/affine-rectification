# Affine rectification
Originally, the algorithm was developed for images coming from Scanning Electron Microscope (SEM). However, it is suitable for all images taken with affine cameras.

The main difference between images coming from a standard camera and images from SEM is that in case of SEM the magnification may be really high (from x1000 to x100,000). It allows to use an affine camera model instead of a perspective one. 

Rectification consists in warping two images in the common plane (making them coplanar) to reduce the search of correspondence to one dimension, i.e., to a horizontal line. Here, this technique is based on epipolar geometry which gives a number of geometric constraints between the 3D points and their projections onto the 2D images that can be rewritten mathematically in the form of 3 × 3 fundamental matrix which has a special form in affine case:

        [ 0 0 a ]
    F = [ 0 0 b ]
        [ c d e ]
        
The algorithm compises the following steps:
- keypoints detection and matching (using OpenCV functions)
- estimation of the affine fundamental matrix (new)
- estimation of rectifying transformations for both images (new)
- transform images     

## Dependencies

- OpenCV. Follow [this tutorial](https://docs.opencv.org/trunk/d7/d9f/tutorial_linux_install.html) to install it.

## Installation

After you've installed all dependencies, just run this code:
```
git clone --recursive https://github.com/avkudr/affine-rectification.git
cd affine-rectification
mkdir build
cd build 
cmake ..
make
```

## Testing

Launch ```./main_rectification```.

If you want to test it on your data, all you need to do is to change these lines in ```src/main_rectification.cpp```:
```
  double detectionThres = 0.004;
  double nnMatchRatio = 0.5;
  cv::Mat img1 = imread("../data/potamogeton1.jpg", cv::IMREAD_GRAYSCALE);
  cv::Mat img2 = imread("../data/potamogeton2.jpg", cv::IMREAD_GRAYSCALE);
```
Explanation:
- ```detectionThres```: threshold for feature detection. If the number of features is not enough, choose a smaller value, like 0.001.
- ```nnMatchRatio```: determines the ratio of matches that are considered as good. Lower value results in bigger number of matches, but, pay attention to outliers.
- ```img1``` and ```img2```: specify the paths to your images.

## Citation

If you use this code in your project, please cite the following paper [[link]](http://ieeexplore.ieee.org/abstract/document/8001905/)
```
@inproceedings{kudryavtsev2017stereo,
  title={Stereo-image rectification for dense 3D reconstruction in scanning electron microscope},
  author={Kudryavtsev, Andrey V and Demb{\'e}l{\'e}, Sounkalo and Piat, Nadine},
  booktitle={Int. Conf. on Manipulation, Automation and Robotics at Small Scales (MARSS'2017)},
  pages={1--6},
  year={2017},
}
```
