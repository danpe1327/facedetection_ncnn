
---
# 前言
  ncnn 是一个为手机端极致优化的高性能神经网络前向计算框架。ncnn 从设计之初深刻考虑手机端的部署和使用。无第三方依赖，跨平台，手机端 cpu 的速度快于目前所有已知的开源框架。基于 ncnn，开发者能够将深度学习算法轻松移植到手机端高效执行，开发出人工智能 APP，将 AI 带到你的指尖。ncnn 目前已在腾讯多款应用中使用，如 QQ，Qzone，微信，天天P图等。
  
  ncnn 发展至今，已经支持了许多优秀的开源算法，例如MTCNN、YOLO v3、SSD等。本文主要介绍如何将68点的人脸关键点算法（vanface， https://github.com/lsy17096535/face-landmark ）移植到 ncnn 上， 并配合 MTCNN 检测得到的人脸进行关键点的检测。
  

---
## 主要环境：
- git
- cmake
- vs2017

---

## 依赖库的准备

本例中使用的模型皆为基于 caffe 框架的模型，因此将其转换为 ncnn 框架模型需要依赖 caffe 和 ncnn 库。此外，图片的处理操作主要依赖 opencv 库。
1. caffe 库的编译

    参考 https://github.com/BVLC/caffe/tree/windows

2. ncnn 库的编译

    参考 https://github.com/Tencent/ncnn/wiki/how-to-build#build-for-windows-x64-using-visual-studio-community-2017

3. opencv 库下载

    本例使用的是预编译的 opencv 库，版本号为 3.4.2，其他高于 opencv 3.0 的版本均可。

## 模型的转换

本例中使用的 mtcnn 和 vanface 模型均为作者开源的模型，mtcnn 模型的输入为 col-major， 而 vanface 模型则为 row-major。因此，在将模型从 caffe 存储方式转换至 ncnn 存储方式前，需要手动将 mtcnn 模型的权重进行转置，具体可以使用 transfer_model 目录下的转换脚本。

```
python .\mtcnn_hw.py .\caffe_models\det1.caffemodel .\caffe_models\det1.prototxt
python .\mtcnn_hw.py .\caffe_models\det2.caffemodel .\caffe_models\det2.prototxt
python .\mtcnn_hw.py .\caffe_models\det3.caffemodel .\caffe_models\det3.prototxt
```

另外，由于 ncnn 只支持新版的 caffe 框架，因此还需要再进一步转换，caffe_bin 目录为 caffe 框架编译成功后的 bin 目录，

```
.\caffe_bin\upgrade_net_proto_binary.exe .\caffe_models\hw_det1.caffemodel .\caffe_models_upgrade\det1.caffemodel
.\caffe_bin\upgrade_net_proto_binary.exe .\caffe_models\hw_det2.caffemodel .\caffe_models_upgrade\det2.caffemodel
.\caffe_bin\upgrade_net_proto_binary.exe .\caffe_models\hw_det3.caffemodel .\caffe_models_upgrade\det3.caffemodel
.\caffe_bin\upgrade_net_proto_binary.exe .\caffe_models\VanFace.caffemodel .\caffe_models_upgrade\VanFace.caffemodel    
```

最后，将模型转为 ncnn 模型存储方式，
```
.\caffe2ncnn.exe .\caffe_models_upgrade\det1.prototxt .\caffe_models_upgrade\det1.caffemodel .\ncnn_models\det1.param .\ncnn_models\det1.bin
.\caffe2ncnn.exe .\caffe_models_upgrade\det2.prototxt .\caffe_models_upgrade\det2.caffemodel .\ncnn_models\det2.param .\ncnn_models\det2.bin
.\caffe2ncnn.exe .\caffe_models_upgrade\det3.prototxt .\caffe_models_upgrade\det3.caffemodel .\ncnn_models\det3.param .\ncnn_models\det3.bin
.\caffe2ncnn.exe .\caffe_models_upgrade\landmark_deploy.prototxt .\caffe_models_upgrade\VanFace.caffemodel .\ncnn_models\VanFace.param .\ncnn_models\VanFace.bin

```

## 模型测试

生成正确的 ncnn 模型后，建立vs工程进行调试，可以vs新建工程。然后，添加包含目录（ncnn 和 opencv 的头文件）和库目录，接着在链接器里添加两者的lib库引用。MTCNN 的检测代码来自于 https://github.com/moli232777144/mtcnn_ncnn ，VanFace 的代码主要参考作者的检测代码。
附上最终的检测效果：
![](https://raw.githubusercontent.com/Tencent/ncnn/master/images/result_sample.jpg)
