#ifndef _FIND_CLASSES_H_
#define _FIND_CLASSES_H_

#include "asmlibrary.h"

const int N_EXPRESSIONS = 7;

#define EXP_NAME_0 "neutral   "
#define EXP_NAME_1 "surprise  "
#define EXP_NAME_2 "anger     "
#define EXP_NAME_3 "happiness "
#define EXP_NAME_4 "sadness   "
#define EXP_NAME_5 "fear      "
#define EXP_NAME_6 "disgust   "

const int N_POINTS = 35;
int classLabelArray[] = {
	0,0,0,0,0,
	1,1,1,1,1,
	2,2,2,2,2,
	3,3,3,3,3,
	4,4,4,4,4,
	5,5,5,5,5,
	6,6,6,6,6
};
double classPointArray[] = {
	1.02, 0.91, 0.89, 1.02, 0.83, 1.01, 0.97,
	1.05, 1.05, 0.88, 1.05, 0.98, 1.01, 1.07,
	1.00, 1.04, 0.92, 1.00, 0.89, 0.87, 0.98,
	0.96, 0.98, 0.85, 1.02, 1.02, 1.08, 1.03,
	0.98, 1.05, 1.09, 0.94, 1.01, 0.96, 1.12,
	1.39, 2.89, 0.84, 0.87, 0.93, 2.00, 1.03,
	1.39, 2.78, 0.85, 0.95, 0.89, 1.77, 0.95,
	1.42, 2.89, 0.83, 0.96, 0.85, 1.78, 0.98,
	1.28, 2.64, 0.85, 0.97, 0.98, 1.77, 0.89,
	1.45, 3.25, 0.75, 1.24, 1.17, 1.99, 1.00,
	0.90, 1.20, 1.87, 0.92, 0.97, 0.91, 0.92,
	0.83, 1.43, 1.71, 0.96, 0.99, 1.03, 1.07,
	0.89, 1.14, 1.60, 1.31, 0.90, 1.00, 0.93,
	0.92, 1.21, 1.87, 1.00, 0.95, 1.12, 0.90,
	0.91, 1.22, 1.44, 1.00, 1.08, 1.12, 1.06,
	0.96, 1.00, 0.84, 1.94, 1.68, 1.23, 1.30,
	1.04, 1.02, 0.88, 1.75, 1.58, 1.24, 1.33,
	1.04, 0.97, 0.86, 1.70, 1.56, 0.81, 1.20,
	1.04, 0.98, 0.82, 1.89, 1.66, 1.60, 1.25,
	1.00, 1.01, 0.89, 1.76, 1.67, 1.22, 1.24,
	1.10, 2.25, 1.01, 1.24, 1.37, 0.94, 1.10,
	1.17, 2.60, 1.12, 1.22, 1.36, 1.08, 1.02,
	1.13, 2.42, 1.14, 1.28, 1.42, 0.87, 1.10,
	1.18, 2.76, 1.11, 1.40, 1.35, 1.32, 1.10,
	1.11, 2.54, 1.27, 1.33, 1.43, 1.17, 1.12,
	1.11, 1.84, 0.87, 0.97, 0.97, 1.22, 1.14,
	1.21, 2.25, 1.04, 0.94, 1.10, 1.35, 1.08,
	1.19, 2.22, 0.90, 1.00, 0.98, 1.52, 1.08,
	1.19, 2.59, 1.01, 0.95, 0.94, 1.39, 1.13,
	1.13, 2.64, 0.95, 0.99, 0.98, 1.24, 1.14,
	0.96, 1.02, 1.15, 1.30, 1.26, 1.63, 1.21,
	0.86, 1.11, 1.29, 1.33, 1.31, 1.26, 1.30,
	1.02, 1.17, 1.11, 1.38, 1.19, 1.54, 1.40,
	1.05, 1.17, 1.15, 1.49, 1.34, 1.01, 1.30,
	1.00, 1.12, 1.30, 1.23, 1.20, 1.25, 1.35
	};

cv::Mat classLabels(1, N_POINTS, CV_32SC1, &classLabelArray);
cv::Mat classPoints(N_POINTS, N_EXPRESSIONS, CV_64FC1, &classPointArray);
cv::Mat classDistances(N_POINTS, 1, CV_64FC1);
cv::Mat diff(1, N_EXPRESSIONS, CV_64FC1);

double get_distance(cv::Mat& p32, cv::Mat& p64) {

	for(int i=0; i<N_EXPRESSIONS; i++)
		diff.at<double>(0,i) = p32.at<float>(0,i) - p64.at<double>(0,i);
	return cv::norm(diff);
}

cv::Mat get_class_weights(cv::Mat instance) {
	cv::Mat weights = cv::Mat(1,N_EXPRESSIONS, CV_64FC1);
	cvZero(&(CvMat)weights);
	double dist;
	
	for(int i = 0; i<N_POINTS; i++) {
		dist = get_distance(instance, classPoints.row(i));
		// classDistances.at<double>(i,0) = dist;
		// printf("%d ", classLabels.at<int>(0,i));
		weights.at<double>(0, classLabels.at<int>(0,i)) += dist;
	}
	// printf("%lf %lf\n", weights.at<double>(0,0), weights.at<double>(0,1));

	for(int i = 0; i < N_EXPRESSIONS; i++)
		weights.at<double>(0,i) = exp(-weights.at<double>(0,i));

	double sum = cv::sum(weights).val[0];
	for(int i = 0; i < N_EXPRESSIONS; i++)
		weights.at<double>(0,i) /= sum;


	return weights;
}


#endif