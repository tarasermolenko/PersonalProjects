# Final Project Part 3 - Realestate Evaluation
# Taras Ermolenko, taras_ermolenko@sfu.ca
# 2021-03-26
# Input: CSV data of Realestate. 
# Output: Using 80 percent of data tries to predict reulsting 20 percent,
# them compares to actualy 20 percent and creates errorRate graph showing
# how accurate the prediciton was.


from sklearn.linear_model import LinearRegression
import turtle

#-----------------------------------------------------------------------------#

# Function that draws bar graph with labels for each bar.
def drawBar(t, height, label):
    """ Get turtle t to draw one bar, of height. """
    t.begin_fill() 
    t.left(90)               
    t.forward(height*20)        
    t.write(str(label))
    t.right(90)
    t.forward(80)            # width of bar, along the top
    t.right(90)
    t.forward(height*20)        
    t.left(90)
    t.pen()
    t.forward(60)
    t.pendown()
    t.end_fill() 

#-----------------------------------------------------------------------------#
    
# Open file and remove header line
file = open('RealestateData.csv', 'r')

junk_header = file.readline()

#-----------------------------------------------------------------------------# 

# input data for realestate variables
input_data = []

# Out put data
house_pice_count = []

#-----------------------------------------------------------------------------#

# Goes through file and appends appropariate data to our data lists.
for i in file:
    dataline = (i.strip('\n').split(','))
    house_pice_count.append(float(dataline[7]))
            
    input_data.append([float(dataline[1]), float(dataline[2]),
                       float(dataline[3]), float(dataline[4]),
                       float(dataline[5]), float(dataline[6])])

                      
#-----------------------------------------------------------------------------#
    
# Copies Original List into 80/20 split
training_input = input_data[:110]
testing_input = input_data[110:141]

training_output = house_pice_count[:110]
testing_output = house_pice_count[110:141]

#-----------------------------------------------------------------------------#

#Prediciton Algorithm taken from instructions
predictor = LinearRegression(n_jobs=-1)
predictor.fit(X=training_input, y=training_output)

outcome = predictor.predict(X=testing_input)

#-----------------------------------------------------------------------------#

# Dictionary that holds prediciton accuracy results
errorRate = {'10%~20%': 0, '20%~30%': 0, '30%~40%': 0, '40%~50%': 0,
             '50%~60%': 0, '60%~70%': 0, '70%~80%': 0, '80%~90%': 0,
             '90%~100%': 0, 'Over 100%': 0}

# Calculate Prediction Accuracy
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

# Prints results to console so you can compare it to graph.
print(errorRate)
#------------------------------------------------------------------------------#

# Sets up screen size and colors for graph to be drawn
wn = turtle.Screen()
wn.setworldcoordinates(-500, -500, 1300, 800)

wn.bgcolor("lightgrey")

tay = turtle.Turtle()
tay.penup()
tay.goto(-250, -250)
tay.pendown()

tay.color("darkblue")
tay.fillcolor("lightgreen")
tay.pensize(3)

# Loop through dictionary and uses drawBar funciton to draw the graph
for key, value in errorRate.items():
    drawBar(tay, value, key)

# After main graph is draw underlines it for more aesthetic look. 
tay.right(180)
tay.forward(len(errorRate)*145 + 10)
tay.right(90)

# Y axis label increments
labels = [0, 5, 10, 15, 20, 25]

# Creates Y axis labels 
for i in range(6):
    tay.penup()
    tay.left(90)
    tay.forward(60)
    tay.write(labels[i])
    tay.backward(60)
    tay.right(90)
    tay.pendown()
    tay.forward(95)

# Writes header and X axis description
tay.penup()
tay.goto(-100, 450)
tay.pendown()
tay.pensize(10)
tay.write(''' REAL ESTATE PRICING MACHINE LEARNING PREDICTION RESULTS GRAPH
y-axis: Number of Predicitons
x-axis: Accuracy of Prediction
Variables Machine Used To Make Prediction:
1)Transaction Data 2)House Age 3)Distance to nearset MRT
4)Number of convinience stores near by 5)Latitude 6)Longitude 
''')
tay.penup()
tay.goto(200, -400)
tay.pendown()
tay.write('''Percentage Error comparted to Actual Value
(left to right from most accurate to least)
''')

tay.hideturtle()
    
wn.exitonclick()
#-----------------------------------------------------------------------------#
