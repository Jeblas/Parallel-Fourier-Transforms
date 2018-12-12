//
// Created by brian on 11/20/18.
//

#include "input_image.h"
#include "complex.h"

#include <fstream>
#include <sstream>
#include <iostream>


InputImage::InputImage(): w(0), h(0), data(nullptr) {}

InputImage::InputImage(const char* filename) {
    std::ifstream ifs(filename);
    if(!ifs) {
        std::cout << "Can't open image file " << filename << std::endl;
        exit(1);
    }

    ifs >> w >> h;
    data = new Complex[w * h];
    for(int r = 0; r < h; ++r) {
        for(int c = 0; c < w; ++c) {
            float real;
            ifs >> real;
            data[r * w + c] = Complex(real);
        }
    }
}

void InputImage::read_image_data(const char* filename) {
    std::ifstream ifs(filename);
    if(!ifs) {
        std::cout << "Can't open image file " << filename << std::endl;
        exit(1);
    }

    ifs >> w >> h;
    data = new Complex[w * h];
    std::string line_in;

    getline(ifs, line_in);
    getline(ifs, line_in);
    if(line_in[0] == '(')
    {
           int i = 0;
           bool in_file = true;
           while(i < h) {
               int j = 0;
               std::string sub_string;
               std::istringstream line_in_ss(line_in);
               getline(line_in_ss, sub_string, '(');
               while (getline(line_in_ss, sub_string, '(')) {
                    std::string in_num_string;
                    std::istringstream sub_string_in_ss(sub_string);
                    getline(sub_string_in_ss,in_num_string, ',');
                    data[i*w + j].real = stof(in_num_string);
                    getline(sub_string_in_ss, in_num_string, ',');
                    data[i*w + j].imag = stof(in_num_string);
                    j++;
               }
               getline(ifs, line_in);
               i++;
           }
    }
    else {
        for (int r = 0; r < h; ++r) {
            for (int c = 0; c < w; ++c) {
                float real;
                ifs >> real;
                data[r * w + c] = Complex(real);
            }
        }
    }
}

int InputImage::get_width() const {
    return w;
}

int InputImage::get_height() const {
    return h;
}

Complex* InputImage::get_image_data() const {
    return data;
}

void InputImage::save_image_data_real(const char *filename, Complex *d, int w, int h) {
    std::ofstream ofs(filename);
    if(!ofs) {
        std::cout << "Can't create output image " << filename << std::endl;
        return;
    }

    ofs << w << " " << h << std::endl;

    for(int r = 0; r < h; r++) {
        for(int c = 0; c < w; c++) {
            ofs << d[r * w + c].real << " ";
        }
        ofs << std::endl;
    }
}

void InputImage::save_image_data(const char* filename, Complex* d, int w, int h) {
    std::ofstream ofs(filename);
    if(!ofs) {
        std::cout << "Can't create output image " << filename << std::endl;
        return;
    }

    ofs << w << " " << h << std::endl;

    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            ofs << d[r * w + c] << " ";
        }
        ofs << std::endl;
    }
}
