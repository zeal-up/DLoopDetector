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
static const char *BRIEF_PATTERN_FILE = "./resources/brief_pattern.yml";

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

/// This functor extracts BRIEF descriptors in the required format
class BriefExtractor: public FeatureExtractor<BRIEF256::bitset>
{
public:
  /** 
   * Extracts features from an image
   * @param im image
   * @param keys keypoints extracted
   * @param descriptors descriptors extracted
   */
  void operator()(const cv::Mat &im, 
    vector<cv::KeyPoint> &keys, vector<BRIEF256::bitset> &descriptors) const override;

  /**
   * Creates the brief extractor with the given pattern file
   * @param pattern_file
   */
  BriefExtractor(const std::string &pattern_file);

private:

  /// BRIEF descriptor extractor
  BRIEF256 m_brief;
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
  demoDetector<BriefVocabulary, BriefLoopDetector, FBrief::TDescriptor>
    demo(voc_file, IMAGE_DIR, POSE_FILE, IMAGE_W, IMAGE_H, show);
  
  try 
  {
    // run the demo with the given functor to extract features
    BriefExtractor extractor(BRIEF_PATTERN_FILE);
    demo.run("BRIEF", extractor);
  }
  catch(const std::string &ex)
  {
    cout << "Error: " << ex << endl;
  }

  return 0;
}

// ----------------------------------------------------------------------------

BriefExtractor::BriefExtractor(const std::string &pattern_file)
{
  // The DVision::BRIEF extractor computes a random pattern by default when
  // the object is created.
  // We load the pattern that we used to build the vocabulary, to make
  // the descriptors compatible with the predefined vocabulary
  
  // loads the pattern
  cv::FileStorage fs(pattern_file.c_str(), cv::FileStorage::READ);
  if(!fs.isOpened()) throw string("Could not open file ") + pattern_file;
  
  vector<int> x1, y1, x2, y2;
  fs["x1"] >> x1;
  fs["x2"] >> x2;
  fs["y1"] >> y1;
  fs["y2"] >> y2;
  
  m_brief.importPairs(x1, y1, x2, y2);
}

// ----------------------------------------------------------------------------

void BriefExtractor::operator() (const cv::Mat &im, 
  vector<cv::KeyPoint> &keys, vector<BRIEF256::bitset> &descriptors) const
{
  // extract FAST keypoints with opencv
  const int fast_th = 20; // corner detector response threshold
  cv::FAST(im, keys, fast_th, true);
  
  // compute their BRIEF descriptor
  m_brief.compute(im, keys, descriptors);
}

// ----------------------------------------------------------------------------

