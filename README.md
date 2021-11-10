# Person-Car-and-Bicycle-Detection-with-OpenCV

This project is done to detect an object chosen by the user (Person, Bicycle, or Car) from the chosen source(camera_id or video path).

Qt is used to create the GUI, and for image processing, the OpenCV library is used. YOLO is chosen as the object detection algorithm.
YOLO’s COCO pre-trained weights is used for detection as it contains the specified object classes. YOLOv3-608 is selected as the model to have 
optimum accuracy values. Detection is done on GPU to accelerate the process.

![image](https://user-images.githubusercontent.com/64530582/141162435-54ad9e1a-5b0f-4d40-b9f9-eaa9eb95537f.png)
