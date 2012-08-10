/*M///////////////////////////////////////////////////////////////////////////////////////
//
// IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
// By downloading, copying, installing or using the software you agree to this license.
// If you do not agree to this license, do not download, install,
// copy or use the software.
//
//
//                          License Agreement
//               For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2008-2011, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
//  * The name of the copyright holders may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//M*/

#include "perf_precomp.hpp"

#ifdef HAVE_CUDA

GPU_PERF_TEST(ConnectedComponents, cv::gpu::DeviceInfo, cv::Size)
{
    cv::gpu::DeviceInfo devInfo = GET_PARAM(0);
    cv::gpu::setDevice(devInfo.deviceID());

    cv::Mat image = readImage("gpu/labeling/aloe-disp.png", cv::IMREAD_GRAYSCALE);

    // cv::threshold(image, image, 150, 255, CV_THRESH_BINARY);

    cv::gpu::GpuMat mask;
    mask.create(image.rows, image.cols, CV_8UC1);

    cv::gpu::GpuMat components;
    components.create(image.rows, image.cols, CV_32SC1);

    cv::gpu::connectivityMask(cv::gpu::GpuMat(image), mask, cv::Scalar::all(0), cv::Scalar::all(2));

    ASSERT_NO_THROW(cv::gpu::labelComponents(mask, components));

    declare.time(1.0);

    TEST_CYCLE()
    {
        cv::gpu::labelComponents(mask, components);
    }
}

INSTANTIATE_TEST_CASE_P(Labeling, ConnectedComponents, testing::Combine(ALL_DEVICES, testing::Values(cv::Size(261, 262))));

#endif