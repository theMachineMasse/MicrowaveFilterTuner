##############################################
# Project: Microwave Filter Tuner
# File: OpenCV.py
# Date: January 18, 2020
# Programmer(s): Braden Massé
# Sub-Systems: Visual Identification Sub-System
# Version: 1.8
##############################################

# Import Libraries #
import numpy as np
import cv2 as cv2

# Globals #
screwLocationsGList = []  # (screwNum, assignedNum, x, y, z, r, locatedFlag)
g_minDepth = 260  # the distance from the tallest tuning screw (mm)
wideCamPort = 1  # COM port for the wide angle camera
g_screwNum = 0  # counter for screw assignment
g_screwCount = 0  # number of screws detected

##############################################
# Function: wide_Angle_Camera()
# Programmer(s): Braden Massé
# Date: January 18,2020
# Purpose: To identify all tuning screws and determine X, Y, and Z position
# Arguments: sensitivityVal
# Outputs: N/A
##############################################
def wide_Angle_Camera(sensitivityVal):
    # Variable Initializations #
    global g_minDepth
    global g_screwCount
    screwLocations = []  # will be appended to global screw positions list
    convertFactor = 38  # number of pixels per cm, needs tuning
    dp = 1
    minDist = 100
    upCannyThres = sensitivityVal * 2
    centerThres = sensitivityVal
    maxR = 40
    minR = 30
    screwCount = 0
    measuredDepth = 210  # units are mm
    screwDiameter = 10  # units are mm
    referenceRadius = 38  # units are pixels
    xUpperFilterMargin = 500  # needs to change depending on GUI selection
    yUpperFilterMargin = 500  # needs to change depending on GUI selection
    xLowerFilterMargin = 500  # needs to change depending on GUI selection
    yLowerFilterMargin = 300  # needs to change depending on GUI selection
    assignedNum = -1  # number assigned to each screw based on user input later

    # Take Picture With Wide Angle Camera #
    # cap = cv2.VideoCapture(wideCamPort);
    # ret, img = cap.read()
    # cv2.imshow('Wide Angle Camera Capture', img)  # testing
    # cap.release()

    # Open Image #
    #img = cv2.imread('negative_test.jpg')  # no screw testing, comment out if taking picture
    img = cv2.imread('opencv_frame_0.png')  # no screw testing, comment out if taking picture

    # Locating of Screws (x, y) #
    output = img.copy()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # change to greyscale image
    gray = cv2.medianBlur(gray, 5)
    circles = []
    circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, dp, minDist, param1=upCannyThres, param2=centerThres, minRadius=minR, maxRadius=maxR)

    # No Screws Located #
    if circles is None:
        screwLocations = [-1, -1, -1, -1, -1, -1, -1]
        screwLocationsGList.append(screwLocations)
        print("No screws detected")  # testing, will need message to appear on GUI

    # Screws Located *
    else:
        # Get Image Dimensions #
        xWidth = img.shape[1]
        yWidth = img.shape[0]

        detected_circles = np.uint16(np.around(circles))

        # Update Tuning Screw List #
        focalLength = ((referenceRadius * 2) * measuredDepth) / screwDiameter  # determine focal length
        for (x, y, r) in detected_circles[0, :]:
            # Find Depth of Current Screw #
            calculatedDepth = (screwDiameter * focalLength) / (r * 2)

            # Find Tallest Tuning Screw's Depth #
            if g_minDepth > calculatedDepth:
                g_minDepth = calculatedDepth

            # Add Screw to Global Screw Locations List #
            # Filtering Based On Filter Size #
            if xLowerFilterMargin < x < (xWidth - xUpperFilterMargin) and yLowerFilterMargin < y < (yWidth - yUpperFilterMargin):  # currently just filtering other screws based size of filter, potential adjustment from user?
                screwLocations = [screwCount, assignedNum, (x / convertFactor), (y / convertFactor), calculatedDepth, r, 0]
                screwLocationsGList.append(screwLocations)

            # Indicate Detected Screws on Output Image #
                cv2.circle(output, (x, y), r, (0, 255, 0), 3)  # draw circles on detected screws
                print("Circle ", screwCount, "at", (x / convertFactor), "cm,", (y / convertFactor), "cm with radius of", r, "pixels and a depth of", calculatedDepth, "mm")  # testing
                cv2.circle(output, (x, y), 2, (0, 255, 0), 3)  # draw dot on center of detected screws
                screwCount = screwCount + 1

        g_screwCount = screwCount
        # Testing Outputs #
        print("Minimum Depth is", g_minDepth, "mm")  # testing
        print("There are", screwCount, "screws")  # testing
        print("Focal length is", focalLength, "mm")  # testing
        print(screwLocationsGList)
        cv2.imwrite('output.png', output)  # use for displaying image on GUI
        cv2.imshow('output', output)  # display output with screws identified, needs to be integrated into GUI
    # End of Function Clean-Up *
    cv2.waitKey(0)  # close image on pressing any key
    cv2.destroyAllWindows()

##############################################
# Function: click_event()
# Programmer(s): Braden Massé
# Date: February 5,2020
# Purpose: Register user clicks and map them to the corresponding screws
# Arguments: N/A
# Outputs: N/A
##############################################
def click_event(event, x, y, flags, param):
    global g_screwNum
    font = cv2.FONT_HERSHEY_COMPLEX
    validFlag = 0
    convertFactor = 38  # number of pixels per cm, needs tuning
    fontScale = 1
    fontThickness = 2

    # Open Image #
    img2 = cv2.imread('output.png')

    # Left Click Event #
    if event == cv2.EVENT_LBUTTONDOWN:
        for i in range(len(screwLocationsGList)):

            # Find Needed Values from List #
            x1 = screwLocationsGList[i][2]*convertFactor
            r1 = screwLocationsGList[i][5]
            y1 = screwLocationsGList[i][3]*convertFactor
            assignedNum = screwLocationsGList[i][1]

            # Check Click Location Is Within A Screw #
            if (x1 - r1) < x < (x1 + r1) and (y1 - r1) < y < (y1 + r1) and assignedNum == -1:
                screwLocationsGList[i][1] = g_screwNum
                # print("Screw identified, screw = ", i)  # testing
                validFlag = 1
                break

        # If Valid Screw Clicked, Add Screw Number To Image #
        if validFlag == 1:
            strXY = str(g_screwNum)
            # strXY = str(x) + ", " + str(y) # testing
            g_screwNum = g_screwNum + 1
            cv2.putText(img2, strXY, (x, y), font, fontScale, (255, 255, 0), fontThickness)
            cv2.imshow('Screw Assignment', img2)
            cv2.imwrite('output.png', img2)  # use for displaying image on GUI
            # print(screwLocationsGList)  # testing

##############################################
# Function: screw_Assignment()
# Programmer(s): Braden Massé
# Date: February 5,2020
# Purpose: Get user to assign appropriate numbers to each detected screw
# Arguments: N/A
# Outputs: N/A
##############################################
def screw_Assignment():
    # Variable Initializations #
    global screwLocationsGList
    tempScrewLocationsList = []

    # Open & Display Image #
    img = cv2.imread('output.png')
    cv2.imshow('Screw Assignment', img)

    # Mouse Event Function Call #
    cv2.setMouseCallback('Screw Assignment', click_event)

    # Close Window #
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    # Check If Any Screws Not Selected; Copy Any With Assigned Numbers #
    for i in range(len(screwLocationsGList)):
        assignedNum = screwLocationsGList[i][1]
        if assignedNum != -1:
            tempScrewLocationsList.append(screwLocationsGList[i])

    # Make Global List Only Contain Assigned Screws #
    screwLocationsGList = tempScrewLocationsList.copy()
    print(screwLocationsGList)  # testing
    print("New List Length is", len(screwLocationsGList))  # testing


wide_Angle_Camera(25)  # testing
screw_Assignment()  # testing
