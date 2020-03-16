##############################################
# Project: Microwave Filter Tuner
# File: OpenCV.py
# Date: January 18, 2020
# Programmer(s): Braden Massé and Matthew Rostad
# Sub-Systems: Visual Identification Sub-System
# Version: 1.13
##############################################

# Import Libraries #
import numpy as np
import cv2 as cv2

# Globals #
global screwLocationsGList
screwLocationsGList = []  # (screwNum, assignedNum, x, y, z, r, locatedFlag)
global screwLocationsPixelsGList
screwLocationsPixelsGList = []  # (screwNum, assignedNum, x, y, z, r, locatedFlag)
global g_minDepth
g_minDepth = 260  # the distance from the tallest tuning screw (mm)
global g_wideCamPort
g_wideCamPort = 0  # COM port for the wide angle camera
global g_screwNum
g_screwNum = 0  # counter for screw assignment
global g_screwsDetected
g_screwsDetected = 0  # flag that is set if any screws detected
global g_height1
g_height1 = 246  # height from the bed to the wide angle camera
global g_pixelsPerMM
g_pixelsPerMM = 5.85    # pixels per milimeter at the bed height


##############################################
# Function: wideAngleCamera()
# Programmer(s): Braden Massé and Matthew Rostad
# Date: January 18,2020
# Purpose: To identify all tuning screws and determine X, Y, and Z position
# Arguments: sensitivityVal
# Outputs: N/A
##############################################
def wideAngleCamera(sensitivityVal):

    # Globals #
    global g_minDepth
    global g_screwsDetected

    # Variable Initializations #
    dp = 1
    minDist = 100  # allowable distance between screws
    upCannyThres = sensitivityVal * 2  # updated based on passed parameter
    centerThres = sensitivityVal  # updated based on passed parameter
    maxR = 45  # max radius of screw, update based on GUI selection, needs to depend on screw selected
    minR = 20  # min radius of screw, update based on GUI selection, needs to depend on screw selected
    screwCount = 0  # counter for number of screws detected
    measuredDepth = 180  # units are mm, used for calibrating depth, needs tuning
    screwDiameter = 10  # units are mm, used for calibrating depth, needs to depend on screw selected
    referenceRadius = 36  # units are pixels, used for calibrating depth, needs to depend on screw selected, needs tuning
    assignedNum = -1  # number assigned to each screw based on user input later

    # Take Picture With Wide Angle Camera #
    cap = cv2.VideoCapture(g_wideCamPort)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 2592)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1944)
    for i in range(0, 30):
        ret, img = cap.read()
    cap.release()
    # cv2.imshow("test", img)
    # cv2.waitKey(0)  # close image on pressing any key
    # cv2.destroyAllWindows()

    # No Image Taken #
    if not ret:
        g_screwsDetected = 0  # set flag to false if no screws detected
        screwLocations = [-1, -1, -1, -1, -1, -1, -1]
        screwLocationsGList.append(screwLocations)
        print("No image taken")  # testing, will need message to appear on GUI

    # Open Image (OBSOLETE) #
    # img = cv2.imread('negative_test.jpg')  # no screw testing, comment out if taking picture
    # img = cv2.imread('opencv_frame_0.png')  # no screw testing, comment out if taking picture

    # Locating of Screws (x, y) #
    img = cv2.flip(img, -1)
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
            #calculatedDepth = 220
            # Find Tallest Tuning Screw's Depth #
            if g_minDepth > calculatedDepth:
                g_minDepth = calculatedDepth

            # calculate values based on heights
            # calculatedDepth = 190
            xOffset = 0.8337 * calculatedDepth - 125.73
            yOffset = 0.5941 * calculatedDepth - 87.438
            xZoomOffset = 40
            yZoomOffset = 6
            new_pixelsPerMM = g_pixelsPerMM * (g_height1 / calculatedDepth)

            # Add Screw to Global Screw Locations List #
            # screwLocations = [screwCount, assignedNum, ((x / g_convertFactor) - g_xOffset), ((y / g_convertFactor) - g_yOffset), calculatedDepth, r, 0]
            screwLocations = [screwCount, assignedNum, ((x / new_pixelsPerMM) - xOffset - xZoomOffset),
                              ((y / new_pixelsPerMM) - yOffset - yZoomOffset), calculatedDepth, r, 0]
            screwLocationsGList.append(screwLocations)

            screwLocations2 = [screwCount, assignedNum, x, y, calculatedDepth, r, 0]
            screwLocationsPixelsGList.append(screwLocations2)

            # Indicate Detected Screws on Output Image #
            cv2.circle(output, (x, y), r, (0, 255, 0), 3)  # draw circles on detected screws
            # print("Circle ", screwCount, "at", ((x / g_convertFactor) - g_xOffset), "cm,", ((y / g_convertFactor) - g_yOffset), "cm with radius of", r, "pixels and a depth of", calculatedDepth, "mm")  # testing

            print(screwCount, "at", ((x / new_pixelsPerMM) - xOffset), ((y / new_pixelsPerMM) - yOffset))
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

    return g_screwsDetected
    # End of Function Clean-Up *


##############################################
# Function: clickEvent()
# Programmer(s): Braden Massé
# Date: February 5,2020
# Purpose: Register user clicks and map them to the corresponding screws
# Arguments: N/A
# Outputs: N/A
##############################################
def clickEvent(event, x, y, flags, param):

    # Globals #
    global g_screwNum

    # Variable Initializations #
    font = cv2.FONT_HERSHEY_COMPLEX
    fontScale = 0.9
    fontThickness = 2

    # Open Image #
    img2 = cv2.imread('output.png')
    temp = cv2.imread('temp.png')

    # Left Click Event #
    if event == cv2.EVENT_LBUTTONDOWN:
        for i in range(len(screwLocationsGList)):

            # Find Needed Values from List #
            x1 = int(screwLocationsPixelsGList[i][2] / 2)
            r1 = int(screwLocationsPixelsGList[i][5] / 2)
            y1 = int(screwLocationsPixelsGList[i][3] / 2)
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
# Function: screwAssignment()
# Programmer(s): Braden Massé
# Date: February 5,2020
# Purpose: Get user to assign appropriate numbers to each detected screw
# Arguments: N/A
# Outputs: N/A
##############################################
def screwAssignment():
    # Globals #
    global screwLocationsGList
    global g_screwNum

    # Variable Initializations #
    tempScrewLocationsList = []

    # Open & Display Image #
    img = cv2.imread('output.png')
    cv2.imshow('Screw Assignment', img)

    # Mouse Event Function Call #
    cv2.setMouseCallback('Screw Assignment', clickEvent)

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

    temp = cv2.imread('temp.png')
    cv2.imwrite('output.png', temp)  # use for displaying image on GUI
    cv2.imshow('Screw Assignment', temp)  # testing

    # Close Window #
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    return screwLocationsGList

# To Be Incorporated Into The GUI #
#wideAngleCamera(25)  # testing

#if g_screwsDetected != 0:
#    screwAssignment()  # testing
