# Final Project Part 4 - Interactive Model Builder
# Taras Ermolenko, taras_ermolenko@sfu.ca
# 2021-03-28
# Input: CSV file from user, header lines, output column, input column.
# Output: Using 80 percent of data tries to predict reulsting 20 percent,
# them compares to actualy 20 percent and creates errorRate graph showing
# how accurate the prediciton was.


from sklearn.linear_model import LinearRegression
import turtle
import os.path
from  matplotlib import pyplot as plt

#-----------------------------------------------------------------------------# 
''' NOTE TO TA/TESTER:
Need to use .csv file and datapoints used for calculations
(outputs and inputs) must be numbers not words.
Also proper number of headers must be removed (no word data)'''

print('Note: Only works for .csv files and numerical data points)')

filenotfound = True

# Get csv file from user file
while filenotfound:
    user_file_name = str(input('please input full file name including the .csv' 
                               + ' example: (file_name.csv)')).strip()

    # Check if file exists
    if os.path.isfile(user_file_name):
        print ("File found, proceding with computation..")
        filenotfound = False
    else:
        print ("File does not not exist, Try again")

graph_Title = user_file_name

# Control Variable for header removal loop
correct_input_needed = True

# Gets the number of header lines needed to be removed 
while correct_input_needed:
    header_lines = int(input('How many header lines does your file have?' +
        '(Enter 0 if none)').strip())

    if header_lines >= 0 and header_lines <= 10:
        correct_input_needed = False
    else:
        print('Input must be a number from 0 - 10')


file = open(user_file_name, 'r')

# Remove all header elements
if header_lines == 0:
    pass
else:
    for num in range(header_lines):
       junk_header = file.readline()


#-----------------------------------------------------------------------------#  
# Input data list
input_data = []
# Output data list
output_data = []

# Asks user to input column nunber for output data set
# Subtracts one from input as user is counting from one instead of zero
# for ease of use for user.
output_column = int(input('Please enter column number (index) of ' +
    'output variable (countring from 1)')) - 1

# Asks user to input column nunbers for input data set.
# Splits entry into string list.
input_variables = input('Please enter desired input variable ' +
    'columns nunbers (indexes)' +
    'seperated by single space (countring from 1)').split()

# Turn string list into integer list
input_variables_int = [int(i) for i in input_variables]

# Subtracts one from each list item for proper index reference.
# (Converts list user counted from one to counting from zero.)
for i in range(len(input_variables_int)):
    input_variables_int[i] = input_variables_int[i] - 1
    

# Nested List Position Counter Variable of Input Data list
input_data_counter = 0

# Loop through each line in file and append apporoporiate data
# to input and output lists.
for i in file:
    dataline = (i.strip('\n').split(','))
    # Creates Output data list
    output_data.append(float(dataline[output_column]))

    # Initiates nested list item
    input_data.append([])

    # This nested loop creates the nested data list items
    # Using the entered index columns from user
    for j in range(len(input_variables_int)):
        # Takes index from list user gave (subtracted by one)
        column_num = input_variables_int[j]
        
        # Assigns data point for that colum index
        datapoint = float(dataline[column_num])

        # Appends datapoint to appropariate nested list item
        input_data[input_data_counter].append(datapoint)

    # Increments input data nested list index counter    
    input_data_counter += 1
    
#------Copy Original List into 80/20 split-----------------------------------#

input_data_80percentpoint = int(len(input_data) *0.8)
output_data_80percentpoint = int(len(output_data) *0.8)

training_input = input_data[:input_data_80percentpoint]
testing_input = input_data[input_data_80percentpoint:]

training_output = output_data[:output_data_80percentpoint]
testing_output = output_data[output_data_80percentpoint:]

#-----------Prediciton Algorithm----------------------------------------------#
predictor = LinearRegression(n_jobs=-1)
predictor.fit(X=training_input, y=training_output)

outcome = predictor.predict(X=testing_input)

#-----------------------------------------------------------------------------#  

errorRate = {'10%~20%': 0, '20%~30%': 0, '30%~40%': 0,
             '40%~50%': 0, '50%~60%': 0, '60%~70%': 0,
             '70%~80%': 0, '80%~90%': 0, '90%~100%': 0,
             'Over 100%': 0}

# Sorts Resulting data into dictionary
for i in range(len(outcome)):
    if testing_output[i] != 0:
        pError = (abs(testing_output[i] - outcome[i])/testing_output[i])*100
        if pError <= 20:
            errorRate['10%~20%'] += 1
        if pError <= 30 and pError > 20:
            errorRate['20%~30%'] += 1
        if pError <= 40 and pError > 30:
            errorRate['30%~40%'] += 1
        if pError <= 50 and pError > 40:
            errorRate['40%~50%'] += 1
        if pError <= 60 and pError > 50:
            errorRate['50%~60%'] += 1
        if pError <= 70 and pError > 60:
            errorRate['60%~70%'] += 1
        if pError <= 80 and pError > 70:
            errorRate['70%~80%'] += 1
        if pError <= 90 and pError > 80:
            errorRate['80%~90%'] += 1
        if pError <= 100 and pError > 90:
            errorRate['90%~100%'] += 1
        if pError > 100:
            errorRate['Over 100%'] += 1


#-----------------------------------------------------------------------------#  

# Had to switch from Turtle drawing to using matlib library
# to plot random data enter by user.
# Libarary allows for manipulation of window (expand large graphs)
# to fit Data which was too hard to do with turtle by hand.
# Source Reference: https://matplotlib.org/stable/gallery/index.html
plt.bar(range(len(errorRate)), list(errorRate.values()), align='center',
        color = (0.5,0.1,0.5,0.6))
plt.xticks(range(len(errorRate)), list(errorRate.keys()))

plt.title(graph_Title)
plt.xlabel('Error Rate (from most accurate to least)')
plt.ylabel('Number of Calculations')

plt.show()

#------------The-End----------Thank-You---------------------------------------# 
