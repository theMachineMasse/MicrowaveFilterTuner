##############################################
# Project: Microwave Filter Tuner
# File: OpenCV.py
# Date: January 18, 2020
# Programmer(s): Braden Massé
# Sub-Systems: Visual Identification Sub-System
# Version: 1.10
##############################################

# Import Libraries #
import numpy as np
import cv2 as cv2

# Globals #
screwLocationsGList = []  # (screwNum, assignedNum, x, y, z, r, locatedFlag)
g_minDepth = 260  # the distance from the tallest tuning screw (mm)
g_wideCamPort = 1  # COM port for the wide angle camera
g_screwNum = 0  # counter for screw assignment
g_screwsDetected = 0  # flag that is set if any screws detected

##############################################
# Function: wide_Angle_Camera()
# Programmer(s): Braden Massé
# Date: January 18,2020
# Purpose: To identify all tuning screws and determine X, Y, and Z position
# Arguments: sensitivityVal, cropArea
# Outputs: N/A
##############################################
def wide_Angle_Camera(sensitivityVal, cropArea):
    # Globals #
    global g_minDepth
    global g_screwsDetected

    # Variable Initializations #
    convertFactor = 38  # number of pixels per cm, needs tuning
    dp = 1
    minDist = 100  # allowable distance between screws
    upCannyThres = sensitivityVal * 2  # updated based on passed parameter
    centerThres = sensitivityVal  # updated based on passed parameter
    maxR = 40  # max radius of screw, update based on GUI selection
    minR = 30  # min radius of screw, update based on GUI selection
    screwCount = 0  # counter for number of screws detected
    measuredDepth = 210  # units are mm, used for calibrating depth
    screwDiameter = 10  # units are mm, used for calibrating depth
    referenceRadius = 38  # units are pixels, used for calibrating depth
    assignedNum = -1  # number assigned to each screw based on user input later

    # Crop Area Functionality, May Not Be Necessary #
    # If Area Is Roughly 10" x 10" #
    if cropArea == 0:
        xUpperFilterMargin = 0  # needs to change depending on GUI selection
        yUpperFilterMargin = 0  # needs to change depending on GUI selection
        xLowerFilterMargin = 0  # needs to change depending on GUI selection
        yLowerFilterMargin = 0  # needs to change depending on GUI selection

    # If Area Is Roughly 6" x 6" #
    if cropArea == 1:
        xUpperFilterMargin = 500  # needs to change depending on GUI selection
        yUpperFilterMargin = 500  # needs to change depending on GUI selection
        xLowerFilterMargin = 500  # needs to change depending on GUI selection
        yLowerFilterMargin = 300  # needs to change depending on GUI selection

    # If Area Is Roughly 3" x 3" #
    if cropArea == 2:
        xUpperFilterMargin = 500  # needs to change depending on GUI selection
        yUpperFilterMargin = 500  # needs to change depending on GUI selection
        xLowerFilterMargin = 500  # needs to change depending on GUI selection
        yLowerFilterMargin = 300  # needs to change depending on GUI selection

    # Take Picture With Wide Angle Camera #
    # cap = cv2.VideoCapture(g_wideCamPort);
    # ret, img = cap.read()
    # cv2.imshow('Wide Angle Camera Capture', img)  # testing
    # cap.release()

    # Open Image #
    # img = cv2.imread('negative_test.jpg')  # no screw testing, comment out if taking picture
    img = cv2.imread('opencv_frame_0.png')  # no screw testing, comment out if taking picture

    # Locating of Screws (x, y) #
    output = img.copy()
    temp = img.copy()
    cv2.imwrite('output.png', output)  # use for displaying image on GUI

    # Get Image Dimensions #
    xWidth = img.shape[1]
    yWidth = img.shape[0]

    # Resize and Save Temporary Image #
    x3 = int(xWidth / 2)
    y3 = int(yWidth / 2)
    temp = cv2.resize(temp, (x3, y3))
    cv2.imwrite('temp.png', temp)  # use for displaying image on GUI

    # Search Image For Screws #
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # change to greyscale image
    gray = cv2.medianBlur(gray, 5)
    circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, dp, minDist, param1=upCannyThres, param2=centerThres, minRadius=minR, maxRadius=maxR)

    # No Screws Located #
    if circles is None:
        g_screwsDetected = 0  # set flag to false if no screws detected
        screwLocations = [-1, -1, -1, -1, -1, -1, -1]
        screwLocationsGList.append(screwLocations)
        print("No screws detected")  # testing, will need message to appear on GUI

    # Screws Located *
    else:
        detected_circles = np.uint16(np.around(circles))
        g_screwsDetected = 1  # set flag to true if screws detected

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

        # Testing Outputs #
        print("Minimum Depth is", g_minDepth, "mm")  # testing
        print("There are", screwCount, "screws")  # testing
        print("Focal length is", focalLength, "mm")  # testing
        print(screwLocationsGList)

        # Resize Output #
        output = cv2.resize(output, (x3, y3))

        # Save and Show Output Image #
        cv2.imwrite('output.png', output)  # use for displaying image on GUI
        # cv2.imshow('output', output)  # display output with screws identified, needs to be integrated into GUI

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
    convertFactor = 38  # number of pixels per cm, needs tuning
    fontScale = 0.9
    fontThickness = 2

    # Open Image #
    img2 = cv2.imread('output.png')
    temp = cv2.imread('temp.png')

    # Left Click Event #
    if event == cv2.EVENT_LBUTTONDOWN:
        for i in range(len(screwLocationsGList)):

            # Find Needed Values from List #
            x1 = int((screwLocationsGList[i][2]*convertFactor) / 2)
            r1 = int(screwLocationsGList[i][5] / 2)
            y1 = int((screwLocationsGList[i][3]*convertFactor) / 2)
            assignedNum = screwLocationsGList[i][1]

            # Check Click Location Is Within A Screw #
            if (x1 - r1) < x < (x1 + r1) and (y1 - r1) < y < (y1 + r1) and assignedNum == -1:
                screwLocationsGList[i][1] = g_screwNum
                strXY = str(g_screwNum)  # convert number to a string
                g_screwNum = g_screwNum + 1
                # print("Screw identified, screw = ", i)  # testing

                # If Valid Screw Clicked, Add Screw Number To Image #
                cv2.circle(temp, (x1, y1), r1, (0, 255, 0), 2)  # draw circles on detected screws
                cv2.putText(temp, strXY, (int(x1-r1/2), int(y1+r1/2)), font, fontScale, (255, 255, 0), fontThickness)  # add screw number to image
                cv2.putText(img2, strXY, (x, y), font, fontScale, (255, 255, 0), fontThickness)

                # Display and Save New Output #
                cv2.imshow('Screw Assignment', img2)
                cv2.imwrite('output.png', img2)  # use for displaying image on GUI
                cv2.imwrite('temp.png', temp)  # use for displaying image on GUI

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
    # Globals #
    global screwLocationsGList
    global g_screwNum

    # Variable Initializations #
    tempScrewLocationsList = []

    # Open & Display Image #
    img = cv2.imread('output.png')
    cv2.imshow('Screw Assignment', img)

    # Mouse Event Function Call #
    cv2.setMouseCallback('Screw Assignment', click_event)

    # Close Window If 'Enter' Pressed #
    while 1:
        if cv2.waitKey(33) == 13:
            cv2.destroyAllWindows()
            break

    # Check If Any Screws Not Selected; Copy Any With Assigned Numbers #
    for i in range(len(screwLocationsGList)):
        assignedNum = screwLocationsGList[i][1]

        if assignedNum != -1:
            tempScrewLocationsList.append(screwLocationsGList[i])

    # Make Global List Only Contain Assigned Screws #
    screwLocationsGList = tempScrewLocationsList.copy()
    print(screwLocationsGList)  # testing
    print("New List Length is", len(screwLocationsGList))  # testing

    temp = cv2.imread('temp.png')  # testing
    cv2.imwrite('output.png', temp)  # use for displaying image on GUI
    cv2.imshow('Screw Assignment', temp)  # testing

    # Close Window #
    cv2.waitKey(0)
    cv2.destroyAllWindows()


wide_Angle_Camera(25, 0)  # testing

# To Be Incorporated Into The GUI #
if g_screwsDetected != 0:
    screw_Assignment()  # testing