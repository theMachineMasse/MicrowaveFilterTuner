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

def zoomCamera(tuningSettings):
    screwType = tuningSettings[0]
    wideSens = tuningSettings[1]
    sensitivityVal = tuningSettings[2]
    widePort = tuningSettings[3]
    g_zoomCamPort = tuningSettings[4]
    comPort = tuningSettings[5]

    # values to be outputed
    screwDepth = 0

    screwLocations = []
    global upCannyThreshold
    global centerThreshold
    minDist = 80       # minimum distance between scerws
    upCannyThreshold = sensitivityVal * 2  # sensitivity for detecting circles
    centerThreshold = sensitivityVal    # sensitivity for detecting circle centers
    maxR = 145  # max radius of screw, update based on GUI selection, needs to depend on screw selected
    minR = 70  # min radius of screw, update based on GUI selection, needs to depend on screw selected
    screwCount = 0
    dp = 1
    minDistCenter = 9999 # finds the screw the closest to the center
    closestCenter = 0   # the screw that is the closest to the center
    measuredDepth = 195  # units are mm
    screwDiameter = 10  # units are mm
    referenceRadius = 102  # units are pixels
    screwZOffset = 100 # distance between camera and screw driver bit in mm
    screwXoffset = 40.2
    screwYoffset = 9.8
    screwAngle = 0

    circles = None

    while circles is None:
        print('taking photo')
        cap = cv2.VideoCapture(g_zoomCamPort)
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, 2592)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1944)
        for i in range(0,10):
            ret, img = cap.read()
            print(ret)
        cap.release()
        img = cv2.flip(img, -1)

        '''
        img = cv2.imread('NewPhotos6/opencv_frame_2.png')  # testing, comment out if taking picture
        img = cv2.flip(img, -1)
        '''


        output = img.copy()
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # change to greyscale image
        gray = cv2.medianBlur(gray, 5)                 # apply blur to reduce false positives

        circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, dp, minDist, param1=upCannyThreshold, param2=centerThreshold,
                                   minRadius=minR, maxRadius=maxR)

        # if no circles are detected increase sensitivity
        if circles is None:
            upCannyThreshold = upCannyThreshold - 20
            centerThreshold = centerThreshold - 10
            print(upCannyThreshold)


    # reset canny thresholds
    upCannyThreshold = 85  # sensitivity for detecting circles
    centerThreshold = 45    # sensitivity for detecting circle centers

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

    #################################
    # determine the depth of the screw
    focalLength = ((referenceRadius * 2) * measuredDepth) / screwDiameter  # determine focal length
    calculatedDepth = (screwDiameter * focalLength) / (r * 2)

    ################################
    print('depth', calculatedDepth)

    offsetImage = img.copy()

    xPixelError = x - horizontalCenter
    yPixelError = y - verticalCenter
    print('x error', xPixelError)
    print('y error', yPixelError)

    #new_pixelsPerMM = g_pixelsPerMM * (g_height1 / calculatedDepth)
    # calculatedDepth = 205
    # bedHeight = 260
    # filterHeight = 65
    # calculatedDepth = bedHeight - filterHeight
    new_pixelsPerMM = -0.1283 * calculatedDepth + 47.762
    # new_pixelsPerMM = 21.7
    print('px per mm', new_pixelsPerMM)
    xMMError = xPixelError / new_pixelsPerMM # + 137.4
    yMMError = yPixelError / new_pixelsPerMM # + 109.6
    print('x error mm', xMMError)
    print('y error mm', yMMError)

    cv2.circle(offsetImage, (x, y), r, (0, 255, 0), 2)  # draw circles on detected screws

    offsetImage = cv2.line(offsetImage, (0, int(y)), (2592, int(y)), (0, 0, 255), 4)
    offsetImage = cv2.line(offsetImage, (int(x), 0), (int(x), 1944), (0, 0, 255), 4)

    offsetImage = cv2.line(offsetImage, (0,int(verticalCenter)), (2592, int(verticalCenter)), (0,0,0), 4)
    offsetImage = cv2.line(offsetImage, (int(horizontalCenter), 0), (int(horizontalCenter), 1944), (0, 0, 0), 4)



    cv2.imshow("offset", offsetImage)
    cv2.imwrite('Images/zoomOutput.png', offsetImage)


    print(closestCenter)
    print(screwLocations)

    # output a circle on only the screw closest to the center
    x,y,r = detected_circles[0, closestCenter]
    cv2.circle(output, (x, y), r, (0, 255, 0), 3)  # draw circles on detected screws
    print("Circle ", screwCount, "at", x, y, " with radius of", r, )  # testing
    cv2.circle(output, (x, y), 2, (0, 255, 0), 3)  # draw dot on center of detected screws
    #cv2.imshow('output', output)  # display output with screws identified, needs to be integrated into GUI

    #########################################
    # find screw angle

    cropImg = img[(y-r+30):(y+r-30), (x-r+30):(x+r-30 )]
    imageSize = r*2
    cropCopy = cropImg.copy()
    #cv2.imshow('crop', cropImg)

    crop_gray = cv2.cvtColor(cropImg, cv2.COLOR_BGR2GRAY)  # change to greyscale image
    cannyEdges = cv2.Canny(crop_gray, 80, 200, None, 3)

    linesP = cv2.HoughLinesP(cannyEdges, 3, np.pi / 180, 30, None, 20, 5)
    imageCutOff = 20
    angleAllowance = 5
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

        #cv2.imshow("Detected Lines (in red) - Probabilistic Line Transform", cropCopy)

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

    else:
        screwAngle = -1
    #screwOutputs = [offsetmm, screwAngle, calculatedDepth]

    # End of Function Clean-Up *
    cv2.waitKey(0)  # close image on pressing any key
    cv2.destroyAllWindows()
    print(screwAngle)

    ZoomOffsetsAngles = [xMMError + screwXoffset, yMMError + screwYoffset, calculatedDepth - screwZOffset, screwAngle]
    return ZoomOffsetsAngles



