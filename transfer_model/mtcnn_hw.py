# coding=utf-8
import os

os.environ['GLOG_minloglevel'] = '2'
import argparse
import math
import numpy as np
import caffe
from caffe.proto import caffe_pb2


def model_to_protxt(model_name):
    model = caffe_pb2.NetParameter()

    with open(model_name, 'rb') as f:
        model.ParseFromString(f.read())

    layers = model.layer
    for layer in layers:
        layer_type = layer.type
        if layer_type in ['Convolution', 'PReLU', 'InnerProduct']:
            print('layer {\n  name: %s\n  type: %s\n}' % (layer.name, layer_type))
            print(type(layer.blobs))

    model_file_name = os.path.basename(model_name)
    out_deploy_file = 'deploy_of_' + os.path.splitext(model_file_name)[0] + '.prototxt'
    with open(out_deploy_file, 'w+') as f:
        f.write(str(model))


def to_mtcnn_hw(deploy_name, model_name):
    out_dir = 'caffe_models'
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)
        
    net = caffe.Net(deploy_name, model_name, caffe.TEST)
    last_conv_layer_name = None
    for layer_name, layer in net.layer_dict.items():
        layer_type = layer.type
        
        if layer_type in ['Convolution', 'InnerProduct']:
            weight_data = net.params[layer_name][0].data
            bias_data = net.params[layer_name][1].data
            need_reshape = False
            ori_shape = None
            # print('%s:\nshape=%s' % (layer_name, weight_data.shape))

            # reshape the conv before the output
            if len(weight_data.shape) == 2 and weight_data.shape[0] > 10:
                # print('reshape the conv before the output')
                need_reshape = True
                ori_shape = weight_data.shape
                if not last_conv_layer_name:
                    raise KeyError('Model is wrong!')
                last_num_out = net.params[last_conv_layer_name][0].data.shape[0]
                
                new_kernal_size = math.sqrt(ori_shape[1] / last_num_out)
                new_shape = np.array([ori_shape[0], last_num_out, new_kernal_size, new_kernal_size], dtype=np.long)
                weight_data = np.reshape(weight_data, new_shape)

            # transform the conv
            if len(weight_data.shape) == 4:
                # print('transpose the conv')
                weight_data = np.transpose(weight_data, (0, 1, 3, 2))

            # transform the boundingboxes
            # if weight_data.shape[0] == 4:
                # print('transform the boundingboxes')
                # for i in range(0, weight_data.shape[0], 2):
                    # wdata_x = weight_data[i].copy()
                    # wdata_y = weight_data[i + 1].copy()
                    # weight_data[i] = wdata_y
                    # weight_data[i + 1] = wdata_x

                    # bdata_x = bias_data[i].copy()
                    # bdata_y = bias_data[i + 1].copy()
                    # bias_data[i] = bdata_y
                    # bias_data[i + 1] = bdata_x

            # transform the landmarks
            # elif weight_data.shape[0] == 10 and len(weight_data.shape) == 2:
                # print('transform the landmarks')
                # wdata_x = weight_data[:5].copy()
                # wdata_y = weight_data[5:].copy()
                # weight_data[:5] = wdata_y
                # weight_data[5:] = wdata_x

                # bdata_x = bias_data[:5].copy()
                # bdata_y = bias_data[5:].copy()
                # bias_data[:5] = bdata_y
                # bias_data[5:] = bdata_x

            if need_reshape:
                weight_data = np.reshape(weight_data, ori_shape)
            net.params[layer_name][0].data.flat = weight_data.flat
            net.params[layer_name][1].data.flat = bias_data.flat
            # print('')
            
            last_conv_layer_name = layer_name

    model_file_name = os.path.basename(model_name)
    out_net_file = os.path.join(out_dir, 'hw_%s' % model_file_name)
    # print('save the result of transform as %s' % out_net_file)
    net.save(out_net_file)


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('model_name', type=str, help='video path or folder', default='')
    parser.add_argument('prototxt', type=str, help='directory to save images', default='')

    args = parser.parse_args()
    return args


if __name__ == '__main__':
    args = parse_args()
    model_name = args.model_name
    deploy_name = args.prototxt
    # model_to_protxt(model_name)
    to_mtcnn_hw(deploy_name, model_name)
