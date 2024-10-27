# FAST-LIO-SAM-SC-QN
+ This repository is a SLAM implementation combining [FAST-LIO2](https://github.com/hku-mars/FAST_LIO) with pose graph optimization and loop closing based on [ScanContext](https://github.com/gisbi-kim/scancontext_tro), [Quatro](https://quatro-plusplus.github.io/), and [Nano-GICP module](https://github.com/engcang/nano_gicp)
    + [ScanContext](https://github.com/gisbi-kim/scancontext_tro) - a global descriptor for LiDAR point cloud, here it is used as loop candidate pair detection
    + [Quatro](https://quatro-plusplus.github.io/) - fast, accurate and robust global registration which provides great initial guess of transform
    + [Quatro module](https://github.com/engcang/quatro) - `Quatro` as a module, can be easily used in other packages
    + [Nano-GICP module](https://github.com/engcang/nano_gicp) - fast ICP combining [FastGICP](https://github.com/SMRT-AIST/fast_gicp) + [NanoFLANN](https://github.com/jlblancoc/nanoflann)
+ Note: similar repositories already exist
    + [FAST_LIO_LC](https://github.com/yanliang-wang/FAST_LIO_LC): FAST-LIO2 + SC-A-LOAM based SLAM
    + [FAST_LIO_SLAM](https://github.com/gisbi-kim/FAST_LIO_SLAM): FAST-LIO2 + ScanContext based SLAM
    + [FAST_LIO_SAM](https://github.com/kahowang/FAST_LIO_SAM): FAST-LIO2 + LIO-SAM (not modularized)
    + [FAST_LIO_SAM](https://github.com/engcang/FAST-LIO-SAM): FAST-LIO2 + LIO-SAM (modularized)
    + [FAST_LIO_SAM_QN](https://github.com/engcang/FAST-LIO-SAM-QN): FAST-LIO2 + LIO-SAM + Quatro + Nano-GICP (modularized)
+ Note2: main code (PGO) is modularized and hence can be combined with any other LIO / LO
    + This repo is to learn GTSAM myself!
    + and as GTSAM tutorial for beginners - [GTSAM 튜토리얼 한글 포스팅](https://engcang.github.io/2023/07/15/gtsam_tutorial.html)
## Video clip - https://youtu.be/MQ8XxRY472Y

<br>

## Main difference between [FAST_LIO_SAM_QN](https://github.com/engcang/FAST-LIO-SAM-QN) and FAST_LIO_SAM_SC_QN
+ FAST_LIO_SAM_QN sets loop candidate pair as (current keyframe, the closest and old enough keyframe)
+ FAST_LIO_SAM_SC_QN gets loop candidate pair from ScanContext

<br>

## Dependencies
+ `C++` >= 17, `OpenMP` >= 4.5, `CMake` >= 3.10.0, `Eigen` >= 3.2, `Boost` >= 1.54
+ `ROS`
+ [`GTSAM`](https://github.com/borglab/gtsam) >= 4.1.1
    ```shell
    wget -O gtsam.zip https://github.com/borglab/gtsam/archive/refs/tags/4.1.1.zip
    unzip gtsam.zip
    cd gtsam-4.1.1/
    mkdir build && cd build
    cmake -DGTSAM_BUILD_WITH_MARCH_NATIVE=OFF -DGTSAM_USE_SYSTEM_EIGEN=ON ..
    sudo make install -j16
    ```
+ [`Teaser++`](https://github.com/MIT-SPARK/TEASER-plusplus)
    ```shell
    git clone https://github.com/MIT-SPARK/TEASER-plusplus.git
    cd TEASER-plusplus && mkdir build && cd build
    cmake .. -DENABLE_DIAGNOSTIC_PRINT=OFF
    sudo make install -j16
    sudo ldconfig
    ```
+ `tbb` (is used for faster `Quatro`)
    ```shell
    sudo apt install libtbb-dev
    ```

## How to build
+ Get the code and then build the main code.
    ```shell
    cd ~/your_workspace/src
    git clone https://github.com/engcang/FAST-LIO-SAM-SC-QN --recursive

    cd ~/your_workspace
    # nano_gicp, quatro first
    catkin build nano_gicp -DCMAKE_BUILD_TYPE=Release
    # Note the option!
    catkin build quatro -DCMAKE_BUILD_TYPE=Release -DQUATRO_TBB=ON -DQUATRO_DEBUG=OFF
    catkin build -DCMAKE_BUILD_TYPE=Release
    . devel/setup.bash
    ```

## How to run
+ Then run (change config files in third_party/`FAST_LIO`)
    ```shell
    roslaunch fast_lio_sam_sc_qn run.launch lidar:=ouster
    roslaunch fast_lio_sam_sc_qn run.launch lidar:=velodyne
    roslaunch fast_lio_sam_sc_qn run.launch lidar:=livox
    ```
* In particular, we provide a preset launch option for specific datasets:
    ```shell
    roslaunch fast_lio_sam_sc_qn run.launch lidar:=kitti
    roslaunch fast_lio_sam_sc_qn run.launch lidar:=mulran
    roslaunch fast_lio_sam_sc_qn run.launch lidar:=newer-college20
    ```

<br>

## Structure
+ odomPcdCallback
    + pub realtime pose in corrected frame
    + keyframe detection -> if keyframe, add to pose graph + save to keyframe queue + generate ScanContext
    + pose graph optimization with iSAM2
+ loopTimerFunc
    + process a saved keyframe
        + detect loop -> if loop, add to pose graph
+ visTimerFunc
    + visualize all **(Note: global map is only visualized once uncheck/check the mapped_pcd in rviz to save comp.)**

<br>

## Memo
+ `Quatro` module fixed for empty matches
+ `Quatro` module is updated with `optimizedMatching` which limits the number of correspondences and increased the speed

<br>

## License
<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a>

This work is licensed under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-nc-sa/4.0/)
