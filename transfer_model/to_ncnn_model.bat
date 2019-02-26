REM transform mtcnn model, WH to HW
python .\mtcnn_hw.py .\caffe_models\det1.caffemodel .\caffe_models\det1.prototxt
python .\mtcnn_hw.py .\caffe_models\det2.caffemodel .\caffe_models\det2.prototxt
python .\mtcnn_hw.py .\caffe_models\det3.caffemodel .\caffe_models\det3.prototxt

REM upgrade net proto binary
.\caffe_bin\upgrade_net_proto_binary.exe .\caffe_models\hw_det1.caffemodel .\caffe_models_upgrade\det1.caffemodel
.\caffe_bin\upgrade_net_proto_binary.exe .\caffe_models\hw_det2.caffemodel .\caffe_models_upgrade\det2.caffemodel
.\caffe_bin\upgrade_net_proto_binary.exe .\caffe_models\hw_det3.caffemodel .\caffe_models_upgrade\det3.caffemodel
.\caffe_bin\upgrade_net_proto_binary.exe .\caffe_models\VanFace.caffemodel .\caffe_models_upgrade\VanFace.caffemodel

REM copy prototxt
copy .\caffe_models\det1.prototxt .\caffe_models_upgrade\det1.prototxt
copy .\caffe_models\det2.prototxt .\caffe_models_upgrade\det2.prototxt
copy .\caffe_models\det3.prototxt .\caffe_models_upgrade\det3.prototxt
copy .\caffe_models\landmark_deploy.prototxt .\caffe_models_upgrade\landmark_deploy.prototxt

REM transform caffe model to ncnn
.\caffe2ncnn.exe .\caffe_models_upgrade\det1.prototxt .\caffe_models_upgrade\det1.caffemodel .\ncnn_models\det1.param .\ncnn_models\det1.bin
.\caffe2ncnn.exe .\caffe_models_upgrade\det2.prototxt .\caffe_models_upgrade\det2.caffemodel .\ncnn_models\det2.param .\ncnn_models\det2.bin
.\caffe2ncnn.exe .\caffe_models_upgrade\det3.prototxt .\caffe_models_upgrade\det3.caffemodel .\ncnn_models\det3.param .\ncnn_models\det3.bin
.\caffe2ncnn.exe .\caffe_models_upgrade\landmark_deploy.prototxt .\caffe_models_upgrade\VanFace.caffemodel .\ncnn_models\VanFace.param .\ncnn_models\VanFace.bin

pause