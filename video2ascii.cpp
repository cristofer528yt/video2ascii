#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

const string ASCII_CHARS = " .,:;+*?%S#@";
const int TARGET_WIDTH = 150;
const int TARGET_HEIGHT = 75;
const int TARGET_FPS = 30;

string pixelToAscii(int brightness) {
    int index = (brightness * (ASCII_CHARS.length() - 1)) / 255;
    return string(1, ASCII_CHARS[index]);
}

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

int main(int argc, char* argv[]) {
    if (argc != 3 || string(argv[1]) != "--video") {
        cerr << "Usage: " << argv[0] << " --video <video_file>" << endl;
        return 1;
    }

    string videoPath = argv[2];
    VideoCapture cap(videoPath);
    
    if (!cap.isOpened()) {
        cerr << "Error: Could not open video file: " << videoPath << endl;
        return 1;
    }

    double fps = cap.get(CAP_PROP_FPS);
    if (fps <= 0) fps = TARGET_FPS;
    
    int frameDelay = 1000 / TARGET_FPS;
    int totalFrames = cap.get(CAP_PROP_FRAME_COUNT);
    
    cout << "Processing video: " << videoPath << endl;
    cout << "Original FPS: " << fps << ", Target FPS: " << TARGET_FPS << endl;
    cout << "Target resolution: " << TARGET_WIDTH << "x" << TARGET_HEIGHT << endl;
    cout << "Total frames: " << totalFrames << endl;

    Mat frame, gray, resized;
    
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // Convert to grayscale
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        
        // Resize to target dimensions
        resize(gray, resized, Size(TARGET_WIDTH, TARGET_HEIGHT), 0, 0, INTER_LINEAR);
        
        // Clear previous frame from screen
        clearScreen();
        
        // Convert each pixel to ASCII and print immediately
        for (int y = 0; y < resized.rows; y++) {
            string line;
            for (int x = 0; x < resized.cols; x++) {
                uchar brightness = resized.at<uchar>(y, x);
                line += pixelToAscii(brightness);
            }
            cout << line << endl;
        }
        
        // Control frame rate
        this_thread::sleep_for(chrono::milliseconds(frameDelay));
    }

    cap.release();
    return 0;
}
