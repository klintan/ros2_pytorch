# ROS2 PyTorch node

Run a PyTorch model in C++ in ROS2.


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


Run

`colcon build` 

## Usage

`ros2 run ros2_pytorch ros2_pytorch_node`



## References
