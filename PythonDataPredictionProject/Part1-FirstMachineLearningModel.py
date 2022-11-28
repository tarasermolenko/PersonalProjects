# Final Project Part 1 - First Machine Learning Model
# Taras Ermolenko
# 2021-03-20
# Machine Learning Model Training Predictions
# Given test data guess outcome based on pattern.

import random
from sklearn.linear_model import LinearRegression


#-----------------------------------------------------------------------------#

# Function of polynomail relation taken from instructions
def alg_relation(x1, x2, x3):
    y = (x1 * 1) + (x2 * 2) + (x3 * 3)
    return y
    
#-----------------------------------------------------------------------------#

# Input list
train_input = []


# Creates nested list of random integers for training input
for i in range(101):
    train_input.append([random.randint(0,1000), random.randint(0,1000),
                        random.randint(0,1000)])
#-----------------------------------------------------------------------------#

# Output list
train_output = []

# Creates nested output list of input list that has been altered
# by the relation function

for i in range(101):
    train_output.append(alg_relation(train_input[i][0], train_input[i][1],
                                     train_input[i][2]))


#-----------------------------------------------------------------------------#

# Prediciotn funciton that guesses the relation taken from instrucitons    
predictor = LinearRegression(n_jobs=-1)
predictor.fit(X=train_input, y=train_output)

X_test = [[10,20,30]]
outcome = predictor.predict(X=X_test)
coefficients = predictor.coef_

#-----------------------------------------------------------------------------#

# Display Results of X_test
print('Prediction: ' + str(outcome))

# Display Results of Relation Predictions
print('Coefficients: ' + str(coefficients))

#-----------------------------------------------------------------------------#
