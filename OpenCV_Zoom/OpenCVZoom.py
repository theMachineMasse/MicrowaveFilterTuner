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
import math
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
    screwLocations = []     # will be appended to global screw positions list
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
    img = cv2.imread('NewPhotos/opencv_frame_9.png')  # testing, comment out if taking picture
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

        # add variable to global list
        screwLocations = [screwCount, x, y, calculatedDepth, 1]
        screwLocationsGList.append(screwLocations)
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
    ###values to be outputed###
    screwDepth = 0
    screwAngle = 0
    offset = [0, 0]
    screwLocations = []

    minDist = 100       #minimum distance between scerws
    upCannyThres = 130  #sensitivity for detecting circles
    centerThres = 35    #sensitivity for detecting circle centers
    maxR = 120          #maximum screw radius
    screwCount = 0
    dp = 1
    minDistCenter = 9999 #finds the screw the closest to the center
    closestCenter = 0   #the screw that is the closest to the center
    measuredDepth = 210  # units are mm
    screwDiameter = 10  # units are mm
    referenceDiameter = 38  # units are pixels

    focalLength = ((referenceDiameter * 2) * measuredDepth) / screwDiameter  # determine focal length

    img = cv2.imread('NewPhotos/opencv_frame_8.png')  # testing, comment out if taking picture
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
        offset = [x - horizontalCenter, y - verticalCenter]

        #determine the depth of the screw
        calculatedDepth = (screwDiameter * focalLength) / (r * 2)

        distCenter = abs(x - horizontalCenter) + abs(y - verticalCenter)
        print(distCenter)

        if distCenter < minDistCenter:
            minDistCenter = distCenter
            closestCenter = screwCount

        screwLocations.append([screwCount, x, y, calculatedDepth, 1])
        screwCount = screwCount + 1

    print(closestCenter)
    print(screwLocations)
    print('offset', offset)

    #output a circle on only the screw closest to the center
    x,y,r = detected_circles[0, closestCenter]
    cv2.circle(output, (x, y), r, (0, 255, 0), 3)  # draw circles on detected screws
    print("Circle ", screwCount, "at", x, y, " with radius of", r, )  # testing
    cv2.circle(output, (x, y), 2, (0, 255, 0), 3)  # draw dot on center of detected screws
    cv2.imshow('output', output)  # display output with screws identified, needs to be integrated into GUI



    #########################################
    crop_img = img[(y-r):(y+r),(x-r):(x+r)]
    crop_copy = crop_img.copy()
    cv2.imshow('crop',crop_img)

    crop_gray = cv2.cvtColor(crop_img, cv2.COLOR_BGR2GRAY)  # change to greyscale image
    dst = cv2.Canny(crop_gray, 20, 80, None, 3)
    cv2.imshow("dst",dst)
    lines = cv2.HoughLines(dst, 1, np.pi / 180, 40, None, 0, 0)

    if lines is not None:
        for i in range(0, len(lines)):
            rho = lines[i][0][0]
            theta = lines[i][0][1]
            a = math.cos(theta)
            b = math.sin(theta)
            x0 = a * rho
            y0 = b * rho
            pt1 = (int(x0 + 1000 * (-b)), int(y0 + 1000 * (a)))
            pt2 = (int(x0 - 1000 * (-b)), int(y0 - 1000 * (a)))
            cv2.line(crop_copy, pt1, pt2, (0, 0, 255), 3, cv2.LINE_AA)
    cv2.imshow('lines', crop_copy)
    print(len(lines))

    ########################################



    # End of Function Clean-Up *
    cv2.waitKey(0)  # close image on pressing any key
    cv2.destroyAllWindows()

#global list
#screw number, X pos, Y pos, Z pos, flag
screwLocationsGList = []

zoom_Camera(30)
#wide_Angle_Camera(30)  # testing
#print(screwLocationsGList)

