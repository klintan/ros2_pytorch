# ROS2 PyTorch node

Run a PyTorch model in C++ in ROS2. A template node to be able to adapt to whatever model you want to run, for instance object detection of people and bikes.


Features
- 

There might be major changes to the code as it is a WIP.


### Topics
`/` - topic for 

## Build and installation
Make sure to run setup.bash and local_setup.bash for your ROS2 environment for all dependencies or symlink them into the repo.

Add Libtorch to your `CMAKE_PREFIX_PATH` env variable, like so:

```
export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/path/to/libtorch
```

Add CV_Bridge as well if its not already on your `CMAKE_PREFIX_PATH`


`export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:<path>` 

(If its built using `colcon build` this is the path to `cv_bridge` folder in the `install` folder.)

Run

`colcon build` 


Unfortunately you also have to copy two libs from https://github.com/intel/mkl-dnn/releases into the libtorch lib folder.

`libmklml.dylib`
`libiomp5.dylib`


## Usage

`ros2 run ros2_pytorch ros2_pytorch_node`



## References
