from builtins import range
from builtins import object
import numpy as np
import math
from past.builtins import xrange


class KNearestNeighbor(object):
    """ a kNN classifier with L2 distance """

    def __init__(self):
        pass

    def train(self, X, y):
        """
        Train the classifier. For k-nearest neighbors this is just
        memorizing the training data.

        Inputs:
        - X: A numpy array of shape (num_train, D) containing the training data
          consisting of num_train samples each of dimension D.
        - y: A numpy array of shape (N,) containing the training labels, where
             y[i] is the label for X[i].
        """
        self.X_train = X
        self.y_train = y

    def predict(self, X, k=1, num_loops=0):
        """
        Predict labels for test data using this classifier.

        Inputs:
        - X: A numpy array of shape (num_test, D) containing test data consisting
             of num_test samples each of dimension D.
        - k: The number of nearest neighbors that vote for the predicted labels.
        - num_loops: Determines which implementation to use to compute distances
          between training points and testing points.

        Returns:
        - y: A numpy array of shape (num_test,) containing predicted labels for the
          test data, where y[i] is the predicted label for the test point X[i].
        """
        if num_loops == 0:
            dists = self.compute_distances_no_loops(X)
        elif num_loops == 1:
            dists = self.compute_distances_one_loop(X)
        elif num_loops == 2:
            dists = self.compute_distances_two_loops(X)
        else:
            raise ValueError("Invalid value %d for num_loops" % num_loops)

        return self.predict_labels(dists, k=k)
    
    def l2_distance(self, x, y):
        """
        Calculates L2 distance between two array.
        Inputs:
        - x: A numpy array
        - y: A numpy array
        
        Returns:
        - A numerical value which represents distance between given arrays.
        """
        
        return np.sqrt(np.sum((x - y)**2))

    def compute_distances_two_loops(self, X):
        """
        Compute the distance between each test point in X and each training point
        in self.X_train using a nested loop over both the training data and the
        test data.

        Inputs:
        - X: A numpy array of shape (num_test, D) containing test data.

        Returns:
        - dists: A numpy array of shape (num_test, num_train) where dists[i, j]
          is the Euclidean distance between the ith test point and the jth training
          point.
        """
        num_test = X.shape[0]
        num_train = self.X_train.shape[0]
        dists = np.zeros((num_test, num_train))
        for i in range(num_test):
            for j in range(num_train):
                #####################################################################
                # TODO:                                                             #
                # Compute the l2 distance between the ith test point and the jth    #
                # training point, and store the result in dists[i, j]. You should   #
                # not use a loop over dimension, nor use np.linalg.norm().          #
                #####################################################################
                # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
                
                # l2_distance formula is defined as a function in order to increase
                # readibility of the code.
                dists[i][j] = self.l2_distance(X[i], self.X_train[j])

                # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        return dists
   
    
    def compute_distances_one_loop(self, X):
        """
        Compute the distance between each test point in X and each training point
        in self.X_train using a single loop over the test data.

        Input / Output: Same as compute_distances_two_loops
        """
        num_test = X.shape[0]
        num_train = self.X_train.shape[0]
        dists = np.zeros((num_test, num_train))
        for i in range(num_test):
            #######################################################################
            # TODO:                                                               #
            # Compute the l2 distance between the ith test point and all training #
            # points, and store the result in dists[i, :].                        #
            # Do not use np.linalg.norm().                                        #
            #######################################################################
            # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
            
            # Instead of calculating distances between test and train images one by one
            # All train images are compared with the each test image at once.
            # Thus, result distances are assigned to relevant row.
            dists[i, :] = np.sqrt(np.sum((self.X_train - X[i])**2,axis=1))

            # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        return dists

    def compute_distances_no_loops(self, X):
        """
        Compute the distance between each test point in X and each training point
        in self.X_train using no explicit loops.

        Input / Output: Same as compute_distances_two_loops
        """
        num_test = X.shape[0]
        num_train = self.X_train.shape[0]
        dists = np.zeros((num_test, num_train))
        #########################################################################
        # TODO:                                                                 #
        # Compute the l2 distance between all test points and all training      #
        # points without using any explicit loops, and store the result in      #
        # dists.                                                                #
        #                                                                       #
        # You should implement this function using only basic array operations; #
        # in particular you should not use functions from scipy,                #
        # nor use np.linalg.norm().                                             #
        #                                                                       #
        # HINT: Try to formulate the l2 distance using matrix multiplication    #
        #       and two broadcast sums.                                         #
        #########################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        # As it given as hint by formulating l2 distance using matrix multiplication
        # and two broadcast sums it means opening the (x_train - x)**2 as 
        # x_train^2 - 2*x_train*x + x^2. Thus 
        # Multiplication of the two array is done via matrix multiplication method.
        # Since the result shape is required to be (500, 1) X_train is transposed.
        # Thus 500, 5000 shape is obtained.
        multiply = 2 * np.matmul(X, self.X_train.T)
        
        # Since train values does not change with respect to input image
        # all values of train data is squared, summed and transposed.
        x_train2 = np.sum(self.X_train**2, axis=1).reshape(num_train, 1).T
        
        # For each test image values are calculated by square and sum 
        # Then, data is reshaped to (500, 1) for final calculation
        x2 = np.sum(X**2, axis=1).reshape(num_test, 1)
        
        # Finally all distances with respect to test images are calculated.
        dists = np.sqrt(x_train2 - multiply + x2)

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        return dists

    def predict_labels(self, dists, k=1):
        """
        Given a matrix of distances between test points and training points,
        predict a label for each test point.

        Inputs:
        - dists: A numpy array of shape (num_test, num_train) where dists[i, j]
          gives the distance betwen the ith test point and the jth training point.

        Returns:
        - y: A numpy array of shape (num_test,) containing predicted labels for the
          test data, where y[i] is the predicted label for the test point X[i].
        """
        num_test = dists.shape[0]
        y_pred = np.zeros(num_test)        
        for i in range(num_test):
            # A list of length k storing the labels of the k nearest neighbors to
            # the ith test point.
            closest_y = []
            #########################################################################
            # TODO:                                                                 #
            # Use the distance matrix to find the k nearest neighbors of the ith    #
            # testing point, and use self.y_train to find the labels of these       #
            # neighbors. Store these labels in closest_y.                           #
            # Hint: Look up the function numpy.argsort.                             #
            #########################################################################
            # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
            
            # closest train images are obtained and limited with respect to k
            closest_images = np.argsort(dists[i])[:k]
            # from closest image indexes classes of these images are obtained
            # then assigned to closest_y
            closest_y = self.y_train[closest_images]
                        

            # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
            #########################################################################
            # TODO:                                                                 #
            # Now that you have found the labels of the k nearest neighbors, you    #
            # need to find the most common label in the list closest_y of labels.   #
            # Store this label in y_pred[i]. Break ties by choosing the smaller     #
            # label.                                                                #
            #########################################################################
            # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
            
            # Each class has been counted to determine most occured class to assign it
            # as a class of the test image
            
            counts = np.bincount(closest_y)
            
            # However when k > 1 same class numbered closest_y array may occur.
            # like -> closest_y = [6, 6, 4, 4, 5]
            # In order to eliminate this situation max occurance number is obtained from
            # bincount results.
            max_occurance_count = np.max(counts)
            
            # Minimum distance to determine closest class
            min_distance = math.inf
            # Get max_occured_classes and check if its above 1 which means
            # there is more than one class has occured as same and maximum.
            max_occured_classes = np.where(counts == max_occurance_count)[0]
            if len(max_occured_classes) > 1:
                # If condition is satisfied then check distances of these classes
                # to test image.
                for ea_class in max_occured_classes:
                    # Masking indexes to use only relevant indexes of class
                    masked_indexes = np.where(closest_y == ea_class)
                    
                    # from dists array of the test image distances of relevant indexes
                    # are obtained and summed to calculate total distance of the class
                    # to the test image.
                    total_class_distance = sum(dists[i][closest_images[masked_indexes]])
                    
                    # If it is lower than min_distance assign y_pred[i] as that class
                    if total_class_distance < min_distance:
                        y_pred[i] = ea_class
                        min_distance = total_class_distance
            else:
                # If there is a dominant class then assign that class as output
                y_pred[i] = np.argmax(counts)
            # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        return y_pred
