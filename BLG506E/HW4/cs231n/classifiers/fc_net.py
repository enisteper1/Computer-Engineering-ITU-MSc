from builtins import range
from builtins import object
import numpy as np

from cs231n.layers import *
from cs231n.layer_utils import *


class TwoLayerNet(object):
    """
    A two-layer fully-connected neural network with ReLU nonlinearity and
    softmax loss that uses a modular layer design. We assume an input dimension
    of D, a hidden dimension of H, and perform classification over C classes.

    The architecure should be affine - relu - affine - softmax.

    Note that this class does not implement gradient descent; instead, it
    will interact with a separate Solver object that is responsible for running
    optimization.

    The learnable parameters of the model are stored in the dictionary
    self.params that maps parameter names to numpy arrays.
    """

    def __init__(self, input_dim=3*32*32, hidden_dim=100, num_classes=10,
                 weight_scale=1e-3, reg=0.0):
        """
        Initialize a new network.

        Inputs:
        - input_dim: An integer giving the size of the input
        - hidden_dim: An integer giving the size of the hidden layer
        - num_classes: An integer giving the number of classes to classify
        - weight_scale: Scalar giving the standard deviation for random
          initialization of the weights.
        - reg: Scalar giving L2 regularization strength.
        """
        self.params = {}
        self.reg = reg

        ############################################################################
        # TODO: Initialize the weights and biases of the two-layer net. Weights    #
        # should be initialized from a Gaussian centered at 0.0 with               #
        # standard deviation equal to weight_scale, and biases should be           #
        # initialized to zero. All weights and biases should be stored in the      #
        # dictionary self.params, with first layer weights                         #
        # and biases using the keys 'W1' and 'b1' and second layer                 #
        # weights and biases using the keys 'W2' and 'b2'.                         #
        ############################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        # First layer weights NxD
        self.params['W1'] = np.random.randn(input_dim, hidden_dim) * weight_scale
        # First layer biases Nx1
        self.params['b1'] = np.zeros(hidden_dim)
        # Second layer weights DxOut
        self.params['W2'] = np.random.randn(hidden_dim, num_classes) * weight_scale
        # Second layer biases Dx1
        self.params['b2'] = np.zeros(num_classes)

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        ############################################################################
        #                             END OF YOUR CODE                             #
        ############################################################################


    def loss(self, X, y=None):
        """
        Compute loss and gradient for a minibatch of data.

        Inputs:
        - X: Array of input data of shape (N, d_1, ..., d_k)
        - y: Array of labels, of shape (N,). y[i] gives the label for X[i].

        Returns:
        If y is None, then run a test-time forward pass of the model and return:
        - scores: Array of shape (N, C) giving classification scores, where
          scores[i, c] is the classification score for X[i] and class c.

        If y is not None, then run a training-time forward and backward pass and
        return a tuple of:
        - loss: Scalar value giving the loss
        - grads: Dictionary with the same keys as self.params, mapping parameter
          names to gradients of the loss with respect to those parameters.
        """
        scores = None
        ############################################################################
        # TODO: Implement the forward pass for the two-layer net, computing the    #
        # class scores for X and storing them in the scores variable.              #
        ############################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****


        # ReLU affine forward pass
        first_out, first_cache = affine_relu_forward(X, self.params['W1'],
                                                        self.params['b1'])
        # Affine forward pass
        scores, second_cache = affine_forward(first_out, self.params['W2'],
                                                         self.params['b2'])

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        ############################################################################
        #                             END OF YOUR CODE                             #
        ############################################################################

        # If y is None then we are in test mode so just return scores
        if y is None:
            return scores

        loss, grads = 0, {}
        ############################################################################
        # TODO: Implement the backward pass for the two-layer net. Store the loss  #
        # in the loss variable and gradients in the grads dictionary. Compute data #
        # loss using softmax, and make sure that grads[k] holds the gradients for  #
        # self.params[k]. Don't forget to add L2 regularization!                   #
        #                                                                          #
        # NOTE: To ensure that your implementation matches ours and you pass the   #
        # automated tests, make sure that your L2 regularization includes a factor #
        # of 0.5 to simplify the expression for the gradient.                      #
        ############################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        # Loss and Gradient of the loss with respect to x
        loss, dx = softmax_loss(scores, y)

        # Adding L2 regularization to total loss
        loss += 0.5 * self.reg * (np.sum(self.params['W1']**2) + np.sum(self.params['W2']**2))
        
        # To calculate gradients backward pass is implemented
        d_first_out, dW2, db2 = affine_backward(dx, second_cache)
        d_x, dW1, db1 = affine_relu_backward(d_first_out, first_cache)

        # Updating first layer weights
        grads['W1'] = dW1 + self.reg * self.params['W1']
        # Updating first layer biases
        grads['b1'] = db1
        # Updating second layer weights
        grads['W2'] = dW2 + self.reg * self.params['W2']
        # Updating second layer biases
        grads['b2'] = db2
        

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        ############################################################################
        #                             END OF YOUR CODE                             #
        ############################################################################

        return loss, grads


class FullyConnectedNet(object):
    """
    A fully-connected neural network with an arbitrary number of hidden layers,
    ReLU nonlinearities, and a softmax loss function. This will also implement
    dropout and batch/layer normalization as options. For a network with L layers,
    the architecture will be

    {affine - [batch/layer norm] - relu - [dropout]} x (L - 1) - affine - softmax

    where batch/layer normalization and dropout are optional, and the {...} block is
    repeated L - 1 times.

    Similar to the TwoLayerNet above, learnable parameters are stored in the
    self.params dictionary and will be learned using the Solver class.
    """

    def __init__(self, hidden_dims, input_dim=3*32*32, num_classes=10,
                 dropout=1, normalization=None, reg=0.0,
                 weight_scale=1e-2, dtype=np.float32, seed=None):
        """
        Initialize a new FullyConnectedNet.

        Inputs:
        - hidden_dims: A list of integers giving the size of each hidden layer.
        - input_dim: An integer giving the size of the input.
        - num_classes: An integer giving the number of classes to classify.
        - dropout: Scalar between 0 and 1 giving dropout strength. If dropout=1 then
          the network should not use dropout at all.
        - normalization: What type of normalization the network should use. Valid values
          are "batchnorm", "layernorm", or None for no normalization (the default).
        - reg: Scalar giving L2 regularization strength.
        - weight_scale: Scalar giving the standard deviation for random
          initialization of the weights.
        - dtype: A numpy datatype object; all computations will be performed using
          this datatype. float32 is faster but less accurate, so you should use
          float64 for numeric gradient checking.
        - seed: If not None, then pass this random seed to the dropout layers. This
          will make the dropout layers deteriminstic so we can gradient check the
          model.
        """
        self.normalization = normalization
        self.use_dropout = dropout != 1
        self.reg = reg
        self.num_layers = 1 + len(hidden_dims)
        self.dtype = dtype
        self.params = {}

        ############################################################################
        # TODO: Initialize the parameters of the network, storing all values in    #
        # the self.params dictionary. Store weights and biases for the first layer #
        # in W1 and b1; for the second layer use W2 and b2, etc. Weights should be #
        # initialized from a normal distribution centered at 0 with standard       #
        # deviation equal to weight_scale. Biases should be initialized to zero.   #
        #                                                                          #
        # When using batch normalization, store scale and shift parameters for the #
        # first layer in gamma1 and beta1; for the second layer use gamma2 and     #
        # beta2, etc. Scale parameters should be initialized to ones and shift     #
        # parameters should be initialized to zeros.                               #
        ############################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        # In order to reach dimensions at ease hstack method is used.
        total_layers = np.hstack([input_dim,
                                  hidden_dims, num_classes])

        for i in range(self.num_layers):
            # 
            rand_weight = np.random.randn(total_layers[i], \
                                           total_layers[i + 1]) * weight_scale
            weight_key = "W{}".format(i+1)
            self.params[weight_key] = rand_weight
            # Bias is initalized with respective to layer size
            rand_bias = np.zeros(total_layers[i + 1])
            bias_key = "b{}".format(i+1)
            self.params[bias_key] = rand_bias

        # If normalization is given as True
        if self.normalization:
            # batch/layer norm doesn't apply to the output, hence the -1
            for i in range(self.num_layers - 1):
                # Assign gamma key
                gamma_key = 'gamma{}'.format(i + 1)
                self.params[gamma_key] = np.ones((total_layers[i + 1]))
                # Assign key
                beta_key = 'beta{}'.format(i+1)
                # Assign beta params to relevant beta key
                self.params[beta_key] = np.zeros((total_layers[i + 1]))

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        ############################################################################
        #                             END OF YOUR CODE                             #
        ############################################################################

        # When using dropout we need to pass a dropout_param dictionary to each
        # dropout layer so that the layer knows the dropout probability and the mode
        # (train / test). You can pass the same dropout_param to each dropout layer.
        self.dropout_param = {}
        if self.use_dropout:
            self.dropout_param = {'mode': 'train', 'p': dropout}
            if seed is not None:
                self.dropout_param['seed'] = seed

        # With batch normalization we need to keep track of running means and
        # variances, so we need to pass a special bn_param object to each batch
        # normalization layer. You should pass self.bn_params[0] to the forward pass
        # of the first batch normalization layer, self.bn_params[1] to the forward
        # pass of the second batch normalization layer, etc.
        self.bn_params = []
        if self.normalization=='batchnorm':
            self.bn_params = [{'mode': 'train'} for i in range(self.num_layers - 1)]
        if self.normalization=='layernorm':
            self.bn_params = [{} for i in range(self.num_layers - 1)]

        # Cast all parameters to the correct datatype
        for k, v in self.params.items():
            self.params[k] = v.astype(dtype)


    def loss(self, X, y=None):
        """
        Compute loss and gradient for the fully-connected net.

        Input / output: Same as TwoLayerNet above.
        """
        X = X.astype(self.dtype)
        mode = 'test' if y is None else 'train'

        # Set train/test mode for batchnorm params and dropout param since they
        # behave differently during training and testing.
        if self.use_dropout:
            self.dropout_param['mode'] = mode
        if self.normalization=='batchnorm':
            for bn_param in self.bn_params:
                bn_param['mode'] = mode
        scores = None
        ############################################################################
        # TODO: Implement the forward pass for the fully-connected net, computing  #
        # the class scores for X and storing them in the scores variable.          #
        #                                                                          #
        # When using dropout, you'll need to pass self.dropout_param to each       #
        # dropout forward pass.                                                    #
        #                                                                          #
        # When using batch normalization, you'll need to pass self.bn_params[0] to #
        # the forward pass for the first batch normalization layer, pass           #
        # self.bn_params[1] to the forward pass for the second batch normalization #
        # layer, etc.                                                              #
        ############################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        # In order to store parameters cache dict is defined
        cache_dict = {} 

        # Assigning weights, biases with normalization and/or dropout
        # or not
        for i in range(self.num_layers - 1):
            # Define weight key
            weight_key = "W{}".format(i + 1)
            # Assign weight with relative key
            W = self.params[weight_key]
            # Define bias key
            bias_key = "b{}".format(i + 1)
            # Assign bias with relative key
            b = self.params[bias_key]
            
            # If normalization is set to True
            if self.normalization:
                # Define gamma key
                gamma_key = "gamma{}".format(i + 1)
                # Assign gamma with relative key
                gamma = self.params[gamma_key]
                # Define beta key
                beta_key = 'beta{}'.format(i + 1)
                # Assign beta with relative key
                beta  = self.params[beta_key]
                bn_params = self.bn_params[i]   

                # Run norm relu forward with normalization
                X, cache = affine_norm_relu_forward(X, W, b, gamma, beta, bn_params, \
                                                    self.normalization)
            else:
                # Apply relu forward without normalization process
                X, cache = affine_relu_forward(X, W, b)
            # Assign cache_dict to relevant cache
            cache_dict[i + 1] = cache

            # If dropout is set to True
            if self.use_dropout:
                # Use dropout at relative parameteres
                X, cache = dropout_forward(X, self.dropout_param)
                # Assign key 
                cache_key = "dropout{}".format(i + 1)
                # Assign value to relative key
                cache_dict[cache_key] = cache
        # Define weight key
        weight_key = "W{}".format(self.num_layers)
        # Assign weight
        W = self.params[weight_key]
        # Define bias key
        bias_key = "b{}".format(self.num_layers)
        # Assign bias
        b = self.params[bias_key]
        # Run forward pass
        scores, cache = affine_forward(X, W, b)
        # Assign cache to relevant key
        cache_dict[self.num_layers] = cache 

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        ############################################################################
        #                             END OF YOUR CODE                             #
        ############################################################################

        # If test mode return early
        if mode == 'test':
            return scores

        loss, grads = 0.0, {}
        ############################################################################
        # TODO: Implement the backward pass for the fully-connected net. Store the #
        # loss in the loss variable and gradients in the grads dictionary. Compute #
        # data loss using softmax, and make sure that grads[k] holds the gradients #
        # for self.params[k]. Don't forget to add L2 regularization!               #
        #                                                                          #
        # When using batch/layer normalization, you don't need to regularize the scale   #
        # and shift parameters.                                                    #
        #                                                                          #
        # NOTE: To ensure that your implementation matches ours and you pass the   #
        # automated tests, make sure that your L2 regularization includes a factor #
        # of 0.5 to simplify the expression for the gradient.                      #
        ############################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        loss, dx = softmax_loss(scores, y)

        # L2 Regularization calculation
        for i in range(self.num_layers):
            # Define key of relevant weight
            weight_key = "W{}".format(i + 1)
            # Get relevant weights
            W = self.params[weight_key]
            # Add regularization to total loss
            loss += 0.5 * self.reg * np.sum(W ** 2)   
        # Calculation of 
        dx, dw, db = affine_backward(dx, cache_dict[self.num_layers])
        # Define relevant key weight
        grad_wkey = "W{}".format(self.num_layers)
        # Calculate gradient of relevant weight
        grads[grad_wkey] = dw + self.reg * self.params['W' + str(self.num_layers)]
        # Define relevant bias key
        grad_bkey = "b{}".format(self.num_layers)
        # Assign gradient of bias
        grads[grad_bkey] = db

        # 
        for i in range(self.num_layers - 2, -1, -1):
            
            # If dropout is set to True
            if self.use_dropout:
                # Dropout is used except last layer
                cache_key = "dropout{}".format(i + 1)
                dx = dropout_backward(dx, cache_dict[cache_key])

            # If normalization is set to True
            if self.normalization:
                # Run norm backward procss to determine gradients            
                dx, dw, db, dgamma, dbeta = affine_norm_relu_backward(dx, cache_dict[i + 1], \
                                                                    self.normalization)
                # Definne grad gamma key
                gamma_key = 'gamma{}'.format(i + 1)
                # Assign gradient of gamma to relevant key
                grads[gamma_key] = dgamma
                # Define grad beta key
                beta_key = "beta{}".format(i + 1)
                # Assign gradient of beta to relevant key
                grads[beta_key] = dbeta
            else:
                # If not normalization done then use normal backward process
                dx, dw, db = affine_relu_backward(dx, cache_dict[i + 1])

            # Define key
            grad_wkey = "W{}".format(i + 1)
            # Assign relative gradient of weight
            grads[grad_wkey] = dw + self.reg * self.params[grad_wkey]
            # Define bias key
            grad_bkey = "b{}".format(i+1)
            # Assign relative gradient of bias
            grads[grad_bkey] = db

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        ############################################################################
        #                             END OF YOUR CODE                             #
        ############################################################################

        return loss, grads
