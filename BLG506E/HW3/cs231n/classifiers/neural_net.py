from __future__ import print_function

from builtins import range
from builtins import object
import numpy as np
import matplotlib.pyplot as plt

class TwoLayerNet(object):
    """
    A two-layer fully-connected neural network. The net has an input dimension of
    N, a hidden layer dimension of H, and performs classification over C classes.
    We train the network with a softmax loss function and L2 regularization on the
    weight matrices. The network uses a ReLU nonlinearity after the first fully
    connected layer.

    In other words, the network has the following architecture:

    input - fully connected layer - ReLU - fully connected layer - softmax

    The outputs of the second fully-connected layer are the scores for each class.
    """

    def __init__(self, input_size, hidden_size, output_size, std=1e-4):
        """
        Initialize the model. Weights are initialized to small random values and
        biases are initialized to zero. Weights and biases are stored in the
        variable self.params, which is a dictionary with the following keys:

        W1: First layer weights; has shape (D, H)
        b1: First layer biases; has shape (H,)
        W2: Second layer weights; has shape (H, C)
        b2: Second layer biases; has shape (C,)

        Inputs:
        - input_size: The dimension D of the input data.
        - hidden_size: The number of neurons H in the hidden layer.
        - output_size: The number of classes C.
        """
        self.params = {}
        self.params['W1'] = std * np.random.randn(input_size, hidden_size)
        self.params['b1'] = np.zeros(hidden_size)
        self.params['W2'] = std * np.random.randn(hidden_size, output_size)
        self.params['b2'] = np.zeros(output_size)

    def loss(self, X, y=None, reg=0.0):
        """
        Compute the loss and gradients for a two layer fully connected neural
        network.

        Inputs:
        - X: Input data of shape (N, D). Each X[i] is a training sample.
        - y: Vector of training labels. y[i] is the label for X[i], and each y[i] is
          an integer in the range 0 <= y[i] < C. This parameter is optional; if it
          is not passed then we only return scores, and if it is passed then we
          instead return the loss and gradients.
        - reg: Regularization strength.

        Returns:
        If y is None, return a matrix scores of shape (N, C) where scores[i, c] is
        the score for class c on input X[i].

        If y is not None, instead return a tuple of:
        - loss: Loss (data loss and regularization loss) for this batch of training
          samples.
        - grads: Dictionary mapping parameter names to gradients of those parameters
          with respect to the loss function; has the same keys as self.params.
        """
        # Unpack variables from the params dictionary
        W1, b1 = self.params['W1'], self.params['b1']
        W2, b2 = self.params['W2'], self.params['b2']
        N, D = X.shape

        # Compute the forward pass
        scores = None
        #############################################################################
        # TODO: Perform the forward pass, computing the class scores for the input. #
        # Store the result in the scores variable, which should be an array of      #
        # shape (N, C).                                                             #
        #############################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        # First multiplication of X
        # (N,D) x (D,hidden_size) + (hidden_size,) -> (N, hidden_size)
        first_pass = np.matmul(X, W1) + b1
        # ReLU activation function
        first_pass_relu = np.where(first_pass > 0, first_pass , 0)
        
        # final scores
        # (N, hidden_size) x (hidden_size, C) + (C,) -> (N, C)
        scores = np.matmul(first_pass_relu, W2) + b2

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        # If the targets are not given then jump out, we're done
        if y is None:
            return scores

        # Compute the loss
        loss = None
        #############################################################################
        # TODO: Finish the forward pass, and compute the loss. This should include  #
        # both the data loss and L2 regularization for W1 and W2. Store the result  #
        # in the variable loss, which should be a scalar. Use the Softmax           #
        # classifier loss.                                                          #
        #############################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        # num_train to use it at scaling
        num_train = X.shape[0]
        # numerator to obtain s_yi and sum(s_j)
        numerator = np.exp(scores)
        # class probabilities for all values but only correct classes will be considered
        class_probabilities = numerator / np.sum(numerator, axis=1, keepdims=True)
        # Train indexes to index with respect to input number
        train_idxs = np.arange(num_train)
        # As I mentioned above only correct classes are considered
        # Loss is calculated by softmax loss function and scaled by train number
        loss = np.sum(-np.log(class_probabilities[train_idxs, y])) / num_train
        # Regularization is added to prevent overfitting
        loss +=  reg * np.sum(W1 ** 2) + reg * np.sum(W2 ** 2) 

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        # Backward pass: compute gradients
        grads = {}
        #############################################################################
        # TODO: Compute the backward pass, computing the derivatives of the weights #
        # and biases. Store the results in the grads dictionary. For example,       #
        # grads['W1'] should store the gradient on W1, and be a matrix of same size #
        #############################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        # For back propogation -1 implemented
        class_probabilities[train_idxs, y] -= 1
        # Since backpropogation runs from output to the input
        # First calculating gradients of weight 2 and scaling
        dW_2 = np.matmul(first_pass_relu.T, class_probabilities) / num_train
        
        # Calculating gradients for biases of bias 2 and scaling
        db_2 = np.sum(class_probabilities, axis=0) / num_train

        # Adding derivative of regularization results to dW_2
        dW_2 += 2 * reg * W2

        # Storing
        grads['W2'] = dW_2
        grads['b2'] = db_2

        # Since we will use chain rule we have to use weight 2
        d_fp = np.matmul(class_probabilities, W2.T)
        # Preventing the gradients to be lower than 0 for
        # the ones that output score was below 0
        d_fp[first_pass_relu == 0] = 0
        # calculating gradients for weights of weight 1 and scaling
        dW_1 = np.matmul(X.T, d_fp) / num_train

        # Calculating gradients for biases of bias 2 and scaling
        db_1 = np.sum(d_fp, axis=0) / num_train

        # Adding derivative of regularization results to dW_1
        dW_1 += 2 * reg * W1

        # Storing
        grads['W1'] = dW_1
        grads['b1'] = db_1

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        return loss, grads

    def train(self, X, y, X_val, y_val,
              learning_rate=1e-3, learning_rate_decay=0.95,
              reg=5e-6, num_iters=100,
              batch_size=200, verbose=False):
        """
        Train this neural network using stochastic gradient descent.

        Inputs:
        - X: A numpy array of shape (N, D) giving training data.
        - y: A numpy array f shape (N,) giving training labels; y[i] = c means that
          X[i] has label c, where 0 <= c < C.
        - X_val: A numpy array of shape (N_val, D) giving validation data.
        - y_val: A numpy array of shape (N_val,) giving validation labels.
        - learning_rate: Scalar giving learning rate for optimization.
        - learning_rate_decay: Scalar giving factor used to decay the learning rate
          after each epoch.
        - reg: Scalar giving regularization strength.
        - num_iters: Number of steps to take when optimizing.
        - batch_size: Number of training examples to use per step.
        - verbose: boolean; if true print progress during optimization.
        """
        num_train = X.shape[0]
        iterations_per_epoch = max(num_train / batch_size, 1)

        # Use SGD to optimize the parameters in self.model
        loss_history = []
        train_acc_history = []
        val_acc_history = []

        for it in range(num_iters):
            X_batch = None
            y_batch = None

            #########################################################################
            # TODO: Create a random minibatch of training data and labels, storing  #
            # them in X_batch and y_batch respectively.                             #
            #########################################################################
            # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

            train_idxs = np.random.choice(num_train, batch_size)
            X_batch = X[train_idxs]
            y_batch = y[train_idxs]

            # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

            # Compute loss and gradients using the current minibatch
            loss, grads = self.loss(X_batch, y=y_batch, reg=reg)
            loss_history.append(loss)

            #########################################################################
            # TODO: Use the gradients in the grads dictionary to update the         #
            # parameters of the network (stored in the dictionary self.params)      #
            # using stochastic gradient descent. You'll need to use the gradients   #
            # stored in the grads dictionary defined above.                         #
            #########################################################################
            # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
            dW_1 = grads['W1']
            db_1 = grads['b1']

            self.params['W1'] = self.params['W1'] - learning_rate * dW_1
            self.params['b1'] = self.params['b1'] - learning_rate * db_1

            dW_2 = grads['W2']
            db_2 = grads['b2']

            self.params['W2'] = self.params['W2'] - learning_rate * dW_2
            self.params['b2'] = self.params['b2'] - learning_rate * db_2

            

            # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

            if verbose and it % 100 == 0:
                print('iteration %d / %d: loss %f' % (it, num_iters, loss))

            # Every epoch, check train and val accuracy and decay learning rate.
            if it % iterations_per_epoch == 0:
                # Check accuracy
                train_acc = (self.predict(X_batch) == y_batch).mean()
                val_acc = (self.predict(X_val) == y_val).mean()
                train_acc_history.append(train_acc)
                val_acc_history.append(val_acc)

                # Decay learning rate
                learning_rate *= learning_rate_decay

        return {
          'loss_history': loss_history,
          'train_acc_history': train_acc_history,
          'val_acc_history': val_acc_history,
        }

    def predict(self, X):
        """
        Use the trained weights of this two-layer network to predict labels for
        data points. For each data point we predict scores for each of the C
        classes, and assign each data point to the class with the highest score.

        Inputs:
        - X: A numpy array of shape (N, D) giving N D-dimensional data points to
          classify.

        Returns:
        - y_pred: A numpy array of shape (N,) giving predicted labels for each of
          the elements of X. For all i, y_pred[i] = c means that X[i] is predicted
          to have class c, where 0 <= c < C.
        """
        y_pred = None

        ###########################################################################
        # TODO: Implement this function; it should be VERY simple!                #
        ###########################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        # first pass is calulated same as the loss calculation
        first_pass = np.matmul(X, self.params['W1']) + self.params['b1']
        # like on loss calculation relu is implemented as first activation function
        first_pass_relu = np.where(first_pass > 0, first_pass , 0)
        # Scores are obtained by multiplying results of relu with following weights
        # and adding biases
        scores = np.matmul(first_pass_relu, self.params['W2']) + self.params['b2']
        # Final results are obtained
        # Since max value will be still the max after softmax calculation
        # It is not required to implement softmax when calculating prediction.
        y_pred = np.argmax(scores, axis=1)
        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        return y_pred
