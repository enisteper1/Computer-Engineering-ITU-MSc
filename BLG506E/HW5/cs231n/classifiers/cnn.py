from builtins import object
import numpy as np

from cs231n.layers import *
from cs231n.fast_layers import *
from cs231n.layer_utils import *


class ThreeLayerConvNet(object):
    """
    A three-layer convolutional network with the following architecture:

    conv - relu - 2x2 max pool - affine - relu - affine - softmax

    The network operates on minibatches of data that have shape (N, C, H, W)
    consisting of N images, each with height H and width W and with C input
    channels.
    """

    def __init__(self, input_dim=(3, 32, 32), num_filters=32, filter_size=7,
                 hidden_dim=100, num_classes=10, weight_scale=1e-3, reg=0.0,
                 dtype=np.float32):
        """
        Initialize a new network.

        Inputs:
        - input_dim: Tuple (C, H, W) giving size of input data
        - num_filters: Number of filters to use in the convolutional layer
        - filter_size: Width/height of filters to use in the convolutional layer
        - hidden_dim: Number of units to use in the fully-connected hidden layer
        - num_classes: Number of scores to produce from the final affine layer.
        - weight_scale: Scalar giving standard deviation for random initialization
          of weights.
        - reg: Scalar giving L2 regularization strength
        - dtype: numpy datatype to use for computation.
        """
        self.params = {}
        self.reg = reg
        self.dtype = dtype

        ############################################################################
        # TODO: Initialize weights and biases for the three-layer convolutional    #
        # network. Weights should be initialized from a Gaussian centered at 0.0   #
        # with standard deviation equal to weight_scale; biases should be          #
        # initialized to zero. All weights and biases should be stored in the      #
        #  dictionary self.params. Store weights and biases for the convolutional  #
        # layer using the keys 'W1' and 'b1'; use keys 'W2' and 'b2' for the       #
        # weights and biases of the hidden affine layer, and keys 'W3' and 'b3'    #
        # for the weights and biases of the output affine layer.                   #
        #                                                                          #
        # IMPORTANT: For this assignment, you can assume that the padding          #
        # and stride of the first convolutional layer are chosen so that           #
        # **the width and height of the input are preserved**. Take a look at      #
        # the start of the loss() function to see how that happens.                #                           
        ############################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        # extracting depth, height, width of input
        depth, input_height, input_width = input_dim

        # Generating weights with respect to input sizes and weight scale parameter
        self.params['W1'] = np.random.randn(num_filters, depth, filter_size, filter_size) * weight_scale        

        # Genrating bias with respecto input num_filters parameter
        self.params['b1'] = np.zeros(num_filters)

        # Generating pooled height and pooled width size which will be output of input layer
        height_pooled, width_pooled = (input_height - 2)//2 + 1, (input_width - 2)//2 + 1  # 2x2 max pooling assuming typical S = 2
        
        # Defining new weights with respect to pooling layer
        self.params['W2'] = np.random.randn(num_filters * height_pooled * width_pooled, hidden_dim) * weight_scale
        self.params['b2'] = np.zeros(hidden_dim)

        # Generating final layers with respect to hidden_dim, num_classes and weight scale parameters
        self.params['W3'] = np.random.randn(hidden_dim, num_classes) * weight_scale
        self.params['b3'] = np.zeros(num_classes)

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        ############################################################################
        #                             END OF YOUR CODE                             #
        ############################################################################

        for k, v in self.params.items():
            self.params[k] = v.astype(dtype)


    def loss(self, X, y=None):
        """
        Evaluate loss and gradient for the three-layer convolutional network.

        Input / output: Same API as TwoLayerNet in fc_net.py.
        """
        W1, b1 = self.params['W1'], self.params['b1']
        W2, b2 = self.params['W2'], self.params['b2']
        W3, b3 = self.params['W3'], self.params['b3']

        # pass conv_param to the forward pass for the convolutional layer
        # Padding and stride chosen to preserve the input spatial size
        filter_size = W1.shape[2]
        conv_param = {'stride': 1, 'pad': (filter_size - 1) // 2}

        # pass pool_param to the forward pass for the max-pooling layer
        pool_param = {'pool_height': 2, 'pool_width': 2, 'stride': 2}

        scores = None
        ############################################################################
        # TODO: Implement the forward pass for the three-layer convolutional net,  #
        # computing the class scores for X and storing them in the scores          #
        # variable.                                                                #
        #                                                                          #
        # Remember you can use the functions defined in cs231n/fast_layers.py and  #
        # cs231n/layer_utils.py in your implementation (already imported).         #
        ############################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        # Convolutional ReLU pool forward pass
        pooling_output, conv_cache = conv_relu_pool_forward(X, W1, b1, conv_param, pool_param)

        # Affine ReLU pass
        affine_output, affine_cache = affine_relu_forward(pooling_output, W2, b2)

        # Affine Softmax pass
        scores, affine2_cache = affine_forward(affine_output, W3, b3)


        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        ############################################################################
        #                             END OF YOUR CODE                             #
        ############################################################################

        if y is None:
            return scores

        loss, grads = 0, {}
        ############################################################################
        # TODO: Implement the backward pass for the three-layer convolutional net, #
        # storing the loss and gradients in the loss and grads variables. Compute  #
        # data loss using softmax, and make sure that grads[k] holds the gradients #
        # for self.params[k]. Don't forget to add L2 regularization!               #
        #                                                                          #
        # NOTE: To ensure that your implementation matches ours and you pass the   #
        # automated tests, make sure that your L2 regularization includes a factor #
        # of 0.5 to simplify the expression for the gradient.                      #
        ############################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        # Softmax loss to calculate loss
        loss, softmax_grad = softmax_loss(scores, y)

        # loss from regularization of first weight
        loss_r1 = self.reg * np.sum(W1 ** 2)
        # loss from regularization of second layer
        loss_r2 = self.reg * np.sum(W2 ** 2)
        # loss from regularization of third layer
        loss_r3 = self.reg * np.sum(W3 ** 2)
        loss = loss + loss_r1 + loss_r2 + loss_r3

        # Affine backward from output to input
        # Gradients of output layer are calculated first 
        dout_3, grads['W3'], grads['b3'] = affine_backward(softmax_grad, affine2_cache)

        # Gradients of second layer are calculated then
        dout_2, grads['W2'], grads['b2'] = affine_relu_backward(dout_3, affine_cache)

        # Gradients of first layer are calculated lastly
        dout, grads['W1'], grads['b1'] = conv_relu_pool_backward(dout_2, conv_cache)

        # L2 regularization
        grads['W1'] += 2 * self.reg * W1
        grads['W2'] += 2 * self.reg * W2
        grads['W3'] += 2 * self.reg * W3  

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        ############################################################################
        #                             END OF YOUR CODE                             #
        ############################################################################

        return loss, grads