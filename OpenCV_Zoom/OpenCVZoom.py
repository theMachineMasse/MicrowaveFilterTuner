##############################################
# Project: Microwave Filter Tuner
# File: OpenCV.py
# Date: January 18, 2020
# Programmer(s): Braden Massé and Matthew Rostad
# Sub-Systems: Visual Identification Sub-System
# Version: 1.2
##############################################

# Import Libraries #
import numpy as np
import cv2 as cv2
import math
import statistics
from numpy import array

# Globals #
g_zoomCamPort = 0  # COM port for the zoom camera
g_height1 = 260  # height from the bed to the zoom camera
g_pixelsPerMM = 16.2    # pixels per milimeter at the bed height


##############################################
# Function: zoom_Camera()
# Programmer(s): Matthew Rostad
# Date: January 26,2020
# Purpose: To identify all tuning screws and determine X, Y, and Z position and determine depth and screw angle
# Arguments: sensitivityVal
# Outputs: N/A
##############################################

def zoom_Camera(sensitivityVal):
    # values to be outputed
    screwDepth = 0

    screwLocations = []

    minDist = 80       # minimum distance between scerws
    upCannyThreshold = 100  # sensitivity for detecting circles
    centerThreshold = 40    # sensitivity for detecting circle centers
    maxR = 180          # maximum screw radius
    screwCount = 0
    dp = 1
    minDistCenter = 9999 # finds the screw the closest to the center
    closestCenter = 0   # the screw that is the closest to the center
    measuredDepth = 260  # units are mm
    screwDiameter = 10  # units are mm
    referenceRadius = 102  # units are pixels


    cap = cv2.VideoCapture(g_zoomCamPort)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 2592)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1944)
    for i in range(0,20):
        ret, img = cap.read()
        print(ret)
    cap.release()
    img = cv2.flip(img, -1)

    '''
    img = cv2.imread('NewPhotos6/opencv_frame_1.png')  # testing, comment out if taking picture
    img = cv2.flip(img, -1)
    '''


    output = img.copy()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # change to greyscale image
    gray = cv2.medianBlur(gray, 5)                 # apply blur to reduce false positives

    circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, dp, minDist, param1=upCannyThreshold, param2=centerThreshold,
                               minRadius=sensitivityVal, maxRadius=maxR)
    if circles is None:
        print("No screws Detected")
    else:
        print(circles.size, "screws detected")
    detected_circles = np.uint16(np.around(circles))

    for (x, y, r) in detected_circles[0, :]:
        # use only the screw in the center of the image
        # find the center of the image
        height, width = img.shape[:2]       # get the height and width of the image
        verticalCenter = height/2
        horizontalCenter = width/2

        #################################
        # determine which screw is closest to the center
        distCenter = abs(x - horizontalCenter) + abs(y - verticalCenter)

        if distCenter < minDistCenter:
            minDistCenter = distCenter
            closestCenter = screwCount

        screwCount = screwCount + 1




    x, y, r = detected_circles[0][closestCenter]
    '''
    xScrewOffset = 37.82
    yScrewOffset = 3.4
    calculatedDepth = 185
    new_pixelsPerMM = g_pixelsPerMM * (g_height1 / calculatedDepth)
    print(x)
    print(y)
    print(verticalCenter)
    print(horizontalCenter)

    newY = y - verticalCenter
    newX = x - horizontalCenter

    xOffset = (newX / new_pixelsPerMM) + xScrewOffset
    yOffset = (newY / new_pixelsPerMM) + yScrewOffset
'''

    #################################
    # determine the depth of the screw
    focalLength = ((referenceRadius * 2) * measuredDepth) / screwDiameter  # determine focal length
    calculatedDepth = (screwDiameter * focalLength) / (r * 2)

    ################################
    calculatedDepth = 153
    print('depth', calculatedDepth)

    offsetImage = img.copy()

    xPixelError = x - horizontalCenter
    yPixelError = y - verticalCenter
    print('x error', xPixelError)
    print('y error', yPixelError)

    #new_pixelsPerMM = g_pixelsPerMM * (g_height1 / calculatedDepth)
    calculatedDepth = 205
    new_pixelsPerMM = -0.1283 * calculatedDepth + 47.762
    # new_pixelsPerMM = 21.7
    print('px per mm', new_pixelsPerMM)
    xMMError = xPixelError / new_pixelsPerMM + 137.4
    yMMError = yPixelError / new_pixelsPerMM + 109.6
    print('x error mm', xMMError)
    print('y error mm', yMMError)

    offsetImage = cv2.line(offsetImage, (0, int(y)), (2592, int(y)), (0, 0, 255), 2)
    offsetImage = cv2.line(offsetImage, (int(x), 0), (int(x), 1944), (0, 0, 255), 2)

    offsetImage = cv2.line(offsetImage, (0,int(verticalCenter)), (2592, int(verticalCenter)), (0,0,0), 2)
    offsetImage = cv2.line(offsetImage, (int(horizontalCenter), 0), (int(horizontalCenter), 1944), (0, 0, 0), 2)
    cv2.imshow("offset", offsetImage)
























    print(closestCenter)
    print(screwLocations)

    # output a circle on only the screw closest to the center
    x,y,r = detected_circles[0, closestCenter]
    cv2.circle(output, (x, y), r, (0, 255, 0), 3)  # draw circles on detected screws
    print("Circle ", screwCount, "at", x, y, " with radius of", r, )  # testing
    cv2.circle(output, (x, y), 2, (0, 255, 0), 3)  # draw dot on center of detected screws
    cv2.imshow('output', output)  # display output with screws identified, needs to be integrated into GUI

    #########################################
    # find screw angle

    cropImg = img[(y-r):(y+r), (x-r):(x+r)]
    imageSize = r*2
    cropCopy = cropImg.copy()
    cv2.imshow('crop', cropImg)

    crop_gray = cv2.cvtColor(cropImg, cv2.COLOR_BGR2GRAY)  # change to greyscale image
    cannyEdges = cv2.Canny(crop_gray, 40, 120, None, 3)

    linesP = cv2.HoughLinesP(cannyEdges, 1, np.pi / 180, 20, None, 20, 10)
    imageCutOff = 20
    angleAllowance = 10
    similarAngles = []
    similarAngle = []
    angles = []
    if linesP is not None:
        for i in range(0, len(linesP)):
            if not ((linesP[i][0][0] < imageCutOff or linesP[i][0][2] < imageCutOff) or
                    (linesP[i][0][0] > imageSize - imageCutOff or linesP[i][0][2] > imageSize - imageCutOff) or
                    (linesP[i][0][1] > imageSize - imageCutOff or linesP[i][0][3] > imageSize - imageCutOff) or
                    (linesP[i][0][1] < imageCutOff or linesP[i][0][3] < imageCutOff)):
                l = linesP[i][0]
                cv2.line(cropCopy, (l[0], l[1]), (l[2], l[3]), (0, 0, 255), 3, cv2.LINE_AA)

                angle = math.atan2(linesP[i][0][2] - linesP[i][0][0], linesP[i][0][3] - linesP[i][0][1])
                angles.append(math.degrees(angle))

        cv2.imshow("Detected Lines (in red) - Probabilistic Line Transform", cropCopy)

    # try to average out all angels and find angle of screw
    for i in range(len(angles)):
        if angles[i] > 90:
            angles[i] = angles[i] - 90

    for i in range(len(angles)):
        for j in range(len(angles)):
            if abs(angles[i] - angles[j]) < angleAllowance:
                similarAngle.append(angles[j])
        similarAngles.append(similarAngle)
        similarAngle = []

    mostSimilarAngles = max(similarAngles,key=len)
    screwAngle = statistics.mean(mostSimilarAngles)

    #screwOutputs = [offsetmm, screwAngle, calculatedDepth]

    # End of Function Clean-Up *
    cv2.waitKey(0)  # close image on pressing any key
    cv2.destroyAllWindows()

    return

# global list
# screw number, X pos, Y pos, Z pos, flag

#screwLocationsGList = []


zoomScrewOutputs = zoom_Camera(30)

'''
screwOffset = zoomScrewOutputs[0]
screwAngle = zoomScrewOutputs[1]
zoomScrewDepth = zoomScrewOutputs[2]

print(screwOffset)
print(screwAngle)
print(zoomScrewDepth)
'''

#wide_Angle_Camera(30)  # testing
#print(screwLocationsGList)

