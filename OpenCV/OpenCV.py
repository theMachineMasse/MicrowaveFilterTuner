##############################################
# Project: Microwave Filter Tuner
# File: OpenCV.py
# Date: January 18, 2020
# Programmer(s): Braden Massé
# Sub-Systems: Visual Identification Sub-System
# Version: 1.2
##############################################

# Import Libraries #
import numpy as np
import cv2 as cv2

# Globals #
screwLocationsGList = []  # (screwNum, x, y, z, locatedFlag)
g_minDepth = 260  # the distance from the tallest tuning screw (mm)
wideCamPort = 1  # COM port for the wide angle camera

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
    screwLocations = []  # will be appended to global screw positions list
    global g_minDepth
    convertFactor = 38  # number of pixels per cm, needs tuning
    dp = 1
    minDist = 50
    upCannyThres = 40
    centerThres = 20
    maxR = 40
    screwCount = 0
    measuredDepth = 210  # units are mm
    screwDiameter = 10  # units are mm
    referenceRadius = 38  # units are pixels
    frameWidth = 2592
    frameHeight = 1944
    cutoffVal = 2

    # Take Picture With Wide Angle Camera #
    # cap = cv2.VideoCapture(wideCamPort);
    # ret, img = cap.read()
    # cv2.imshow('Wide Angle Camera Capture', img)  # testing
    # cap.release()

    # Locating of Screws (x, y) #
    img = cv2.imread('NewPhotos1/opencv_frame_12.png')  # testing, comment out if taking picture
    output = img.copy()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # change to greyscale image
    gray = cv2.medianBlur(gray, 5)

    circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, dp, minDist, param1=upCannyThres, param2=centerThres, minRadius=sensitivityVal, maxRadius=maxR)
    detected_circles = np.uint16(np.around(circles))

    # Update Tuning Screw List #
    focalLength = ((referenceRadius * 2) * measuredDepth) / screwDiameter  # determine focal length
    for (x, y, r) in detected_circles[0, :]:
        # Find Depth of Current Screw #
        calculatedDepth = (screwDiameter * focalLength) / (r * 2)

        # Find Tallest Tuning Screw's Depth #
        if g_minDepth > calculatedDepth:
            g_minDepth = calculatedDepth

        # Add Screw to Global Screw Locations List

        if x > cutoffVal and y > cutoffVal and x < (frameWidth - cutoffVal) and y < (frameHeight - cutoffVal):  # currently just filtering other screws based size of filter, potential adjustment from user?
            screwLocations = [screwCount, (x / convertFactor), (y / convertFactor), calculatedDepth, 0]
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
    cv2.imshow('output', output)  # display output with screws identified, needs to be integrated into GUI

    # End of Function Clean-Up *
    cv2.waitKey(0)  # close image on pressing any key
    cv2.destroyAllWindows()


wide_Angle_Camera(30)  # testing
