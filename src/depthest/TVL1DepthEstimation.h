#ifndef _TVL1DepthEstimation_
#define _TVL1DepthEstimation_

#include "../../imageutilities/src/iucore.h"
#include "../kernels/primal_dual_update.h"
#include "../../imageutilities/src/iuio.h"
//#include "../../imageutilities/src/iumath.h"

class TVL1DepthEstimation{

public:

    iu::ImageGpu_32f_C1* d_ref_image;
    iu::ImageGpu_32f_C1* d_cur_image;

    iu::ImageGpu_32f_C1* d_px;
    iu::ImageGpu_32f_C1* d_py;

    iu::ImageGpu_32f_C1* d_q;

    iu::ImageGpu_32f_C1* d_u;
    iu::ImageGpu_32f_C1* d_u0;

    iu::ImageGpu_32f_C1* d_data_term;
    iu::ImageGpu_32f_C1* d_gradient_term;

//    iu::ImageGpu_32f_C4 *d_data_term_all;

    iu::ImageGpu_32f_C1* d_cur2ref_warped;

    bool allocated;

public:

    void InitialiseVariables(float initial_val);

    unsigned int getImgWidth (){ return d_ref_image->width();}
    unsigned int getImgHeight(){ return d_ref_image->height();}

    TVL1DepthEstimation():allocated(false){};
    TVL1DepthEstimation(const std::string& refimgfile, const std::string& curimgfile);

    void doOneWarp()
    {
        iu::copy(d_u,d_u0);
    }

    void updatePrimalData(const float lambda,
                     const float sigma_primal,
                     const float sigma_dual_data,
                     const float sigma_dual_reg);

    void updatedualData(const float lambda,
                        const float sigma_primal,
                        const float sigma_dual_data,
                        const float sigma_dual_reg);

    void updatedualReg(const float lambda,
                       const float sigma_primal,
                       const float sigma_dual_data,
                       const float sigma_dual_reg);

    void computeImageGradient_wrt_depth(const float2 fl,
                                   const float2 pp,
                                   TooN::Matrix<3,3> R_lr_,
                                   TooN::Matrix<3,1> t_lr_,
                                   bool disparity,
                                   float dmin,
                                   float dmax);
    void updateWarpedImage(
                            const float2 fl,
                            const float2 pp,
                            TooN::Matrix<3,3> R_lr_,
                            TooN::Matrix<3,1> t_lr_,
                            bool disparity);

    void allocateMemory(const unsigned int width, const unsigned int heightt);


};


#endif
