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
from numpy import array

# Globals #
# g_tuningScrews;  ## need help getting this list to work  with arguments (screwNum, x, y, z, locatedFlag)
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
    global g_minDepth
    convertFactor = 38  # number of pixels per cm, needs tuning
    dp = 1
    minDist = 100
    upCannyThres = 150
    centerThres = 35
    maxR = 100
    screwCount = 0
    measuredDepth = 210  # units are mm
    screwDiameter = 10  # units are mm
    referenceDiameter = 38  # units are pixels

    # Take Picture With Wide Angle Camera #
    # cap = cv2.VideoCapture(wideCamPort);
    # ret, img = cap.read()
    # cv2.imshow('Wide Angle Camera Capture', img)  # testing
    # cap.release()

    # Locating of Screws (x, y) #
    img = cv2.imread('opencv_frame_5.png')  # testing, comment out if taking picture
    output = img.copy()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # change to greyscale image
    gray = cv2.medianBlur(gray, 5)
    circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, dp, minDist, param1=upCannyThres, param2=centerThres, minRadius=sensitivityVal, maxRadius=maxR)
    detected_circles = np.uint16(np.around(circles))

    # Update Tuning Screw List #
    focalLength = ((referenceDiameter * 2) * measuredDepth) / screwDiameter  # determine focal length
    for (x, y, r) in detected_circles[0, :]:
        # Find Depth of Current Screw #
        calculatedDepth = (screwDiameter * focalLength) / (r * 2)

        # Find Tallest Tuning Screw's Depth #
        if g_minDepth > calculatedDepth:
            g_minDepth = calculatedDepth

        # Indicate Detected Screws on Output Image #
        cv2.circle(output, (x, y), r, (0, 255, 0), 3)  # draw circles on detected screws
        print("Circle ", screwCount, "at", (x / convertFactor), "cm,", (y / convertFactor), "cm with radius of", r, "pixels and a depth of", calculatedDepth, "mm")  # testing
        cv2.circle(output, (x, y), 2, (0, 255, 0), 3)  # draw dot on center of detected screws
        screwCount = screwCount + 1

        # add line(s) for storing screw info (screwNum, x, y, z, locatedFlag) into global list/array
        # use convertFactor to change the screw positions from pixels to cm before storing in global

    # Testing Outputs #
    print("Minimum Depth is", g_minDepth, "mm")  # testing
    print("There are", screwCount, "screws")  # testing
    print("Focal length is", focalLength, "mm")  # testing

    cv2.imshow('output', output)  # display output with screws identified, needs to be integrated into GUI

    # End of Function Clean-Up *
    cv2.waitKey(0)  # close image on pressing any key
    cv2.destroyAllWindows()


def zoom_Camera(sensitivityVal):
    minDist = 100       #minimum distance between scerws
    upCannyThres = 100  #sensitivity for detecting circles
    centerThres = 35    #sensitivity for detecting circle centers
    maxR = 120          #maximum screw radius
    screwCount = 0
    dp = 1
    minDistCenter = 999 #finds the screw the closest to the center
    closestCenter = 0   #the screw that is the closest to the center

    img = cv2.imread('opencv_frame_3.png')  # testing, comment out if taking picture
    output = img.copy()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # change to greyscale image
    gray = cv2.medianBlur(gray, 5)                 #apply blur to reduce false positives

    circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, dp, minDist, param1=upCannyThres, param2=centerThres, minRadius=sensitivityVal, maxRadius=maxR)
    detected_circles = np.uint16(np.around(circles))

    for (x, y, r) in detected_circles[0, :]:
        #use only the screw in the center of the image
        #find the center of the image
        height, width = img.shape[:2]       #get the height and width of the image
        verticalCenter = height/2
        horizontalCenter = width/2

        distCenter = abs(x - horizontalCenter) + abs(y - verticalCenter)
        print(distCenter)
        if distCenter < minDistCenter:
            minDistCenter = distCenter
            closestCenter = screwCount

        screwCount = screwCount + 1

    print(closestCenter)

    #output a circle on only the screw closest to the center
    x,y,r = detected_circles[0, closestCenter]
    cv2.circle(output, (x, y), r, (0, 255, 0), 3)  # draw circles on detected screws
    print("Circle ", screwCount, "at", x, y, " with radius of", r, )  # testing
    cv2.circle(output, (x, y), 2, (0, 255, 0), 3)  # draw dot on center of detected screws
    cv2.imshow('output', output)  # display output with screws identified, needs to be integrated into GUI

    # End of Function Clean-Up *
    cv2.waitKey(0)  # close image on pressing any key
    cv2.destroyAllWindows()

zoom_Camera(30)
#wide_Angle_Camera(30)  # testing

