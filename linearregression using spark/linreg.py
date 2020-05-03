# linreg.py
#
# Standalone Python/Spark program to perform linear regression.
# Performs linear regression by computing the summation form of the
# closed form expression for the ordinary least squares estimate of beta.
# 
# TODO: Write this.
# 
# Takes the yx file as input, where on each line y is the first element 
# and the remaining elements constitute the x.
#
# Usage: spark-submit linreg.py <inputdatafile>
# Example usage: spark-submit linreg.py yxlin.csv
#
#

import sys
import numpy as np

from pyspark import SparkContext
import numpy as np

if __name__ == "__main__":
  if len(sys.argv) !=2:
    print >> sys.stderr, "Usage: linreg <datafile>"
    exit(-1)

sc = SparkContext(appName="LinearRegression")

## Splitting the line to a proper dataset as per our use case...
def func_split(each_line):
    curr_line=each_line.split(',')
    lst=[float(each_word) for each_word in curr_line]
    return lst   

  # Input yx file has y_i as the first element of each line 
  # and the remaining elements constitute x_i
yxinputFile = sc.textFile(sys.argv[1])
rdd = yxinputFile.map(func_split)
#print(rdd.take(5))
sc.version
  #print "yxlength: ", yxlength
rdd_y_data=rdd.map(lambda x:[x[0]])
rdd_x_data=rdd.map(lambda x:[1]+x[1:])
## Adding bias inorder to make multiplication easier....
  # dummy floating point array for beta to illustrate desired output format
rdd_x_data.take(3)
rdd_y_data.take(3)
## Easy way of performing XtransposeX...
def xxtranspose(each_point):
    curr_point=np.array(each_point)[np.newaxis]
    transpose=np.transpose(curr_point)
    final_ans=np.dot(transpose,curr_point)
    #print(final_ans.shape)
    return ('xx',final_ans)
rddxx=rdd_x_data.map(xxtranspose)
###This is the redcue part of collect all the distributed results...
rddfirstcomponent = rddxx.reduceByKey(lambda x,y : x+y)
rddfirstcomponent.take(100)
### Calculating xtransposey.....
def xtranspose_y(each_line):
    y_point=np.array(each_line[0])[np.newaxis]
    x_point=np.array([1]+each_line[1:])[np.newaxis]
    final_ans=np.asmatrix(np.transpose(x_point)*y_point)
    return('xy',final_ans)
rddxy=rdd.map(xtranspose_y)
rddxy.take(5)
## Summing up all the values of xtransposey......
rddsecondcomponent = rddxy.reduceByKey(lambda x,y : x+y)
rddsecondcomponent.take(100)
### TRyinng to take out the values from rdd to perform inverse else you will get dimensionality error....
values=np.array(rddfirstcomponent.values().first())
values
## Computing the inverse of a matrix.... using numpy....
rdd_inverse_first_component=np.linalg.inv(values)
rdd_inverse_first_component
## After calcualting the first part in closed estimate for final result multiply with secind componenet....
rdd_second_component=rddsecondcomponent.values().first()
rdd_second_component
## Final result...
curr_ans=rdd_inverse_first_component*rdd_second_component
### Making in to a 1-d vector for easier visulaization and multiplication...
curr_ans= np.squeeze(np.asarray(curr_ans))
for coeff in curr_ans:
    print(coeff)
sc.stop()
