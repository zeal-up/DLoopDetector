/**
 * File: demo_brief.cpp
 * Date: November 2011
 * Author: Dorian Galvez-Lopez
 * Description: demo application of DLoopDetector
 * License: see the LICENSE.txt file
 */

#include <iostream>
#include <vector>
#include <string>

// DLoopDetector and DBoW2
#include <DBoW2/DBoW2.h> // defines BriefVocabulary
#include "DLoopDetector.h" // defines BriefLoopDetector
#include <DVision/DVision.h> // Brief

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "demoDetector.h"

using namespace DLoopDetector;
using namespace DBoW2;
using namespace DVision;
using namespace std;

// ----------------------------------------------------------------------------
static const char *IMAGE_DIR = "./resources/images";
static const char *POSE_FILE = "./resources/pose.txt";
static const int IMAGE_W = 640; // image size
static const int IMAGE_H = 480;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

/// This functor extracts BRIEF descriptors in the required format
class OrbExtractor: public FeatureExtractor<cv::Mat>
{
public:
  /** 
   * Extracts features from an image
   * @param im image
   * @param keys keypoints extracted
   * @param descriptors descriptors extracted
   */
  void operator()(const cv::Mat &im, 
    vector<cv::KeyPoint> &keys, vector<cv::Mat> &descriptors) const override;

  OrbExtractor();

private:
  // opencv orb extractor
  cv::Ptr<cv::ORB> m_orb;
};

// ----------------------------------------------------------------------------
void PrintUsage() {
  std::cout << "Usage: ./demo_brief <path_to_vocfile> [-noshow]" << std::endl;
  std::cout << "  -noshow: do not show the images" << std::endl;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

int main(int argc, char* argv[])
{
  bool show = true;
  if (argc < 2) {
    PrintUsage();
    return 1;
  }
  if (argc > 2 && std::string(argv[2]) == "-noshow")
    show = false;

  std::string voc_file(argv[1]);
  // prepares the demo
  demoDetector<OrbVocabulary, OrbLoopDetector, FORB::TDescriptor>
    demo(voc_file, IMAGE_DIR, POSE_FILE, IMAGE_W, IMAGE_H, show);
  
  try 
  {
    // run the demo with the given functor to extract features
    OrbExtractor extractor = OrbExtractor();
    demo.run("ORB", extractor);
  }
  catch(const std::string &ex)
  {
    cout << "Error: " << ex << endl;
  }

  return 0;
}

// ----------------------------------------------------------------------------

OrbExtractor::OrbExtractor()
{
  m_orb = cv::ORB::create();
}

// ----------------------------------------------------------------------------

void OrbExtractor::operator() (const cv::Mat &im, 
  vector<cv::KeyPoint> &keys, vector<cv::Mat> &descriptors) const
{
  // compute ORB descriptor
  cv::Mat mask;
  cv::Mat descriptorsMat;
  m_orb->detectAndCompute(im, mask, keys, descriptorsMat);

  // change structure of descriptors
  descriptors.resize(descriptorsMat.rows);
  for (int i = 0; i < descriptorsMat.rows; ++i) {
    descriptors[i] = descriptorsMat.row(i);
  }
}

// ----------------------------------------------------------------------------

