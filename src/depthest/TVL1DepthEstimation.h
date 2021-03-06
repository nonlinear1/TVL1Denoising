#ifndef _TVL1DepthEstimation_
#define _TVL1DepthEstimation_

#include "../../minimal_imgutilities/src/iucore.h"
#include "../kernels/primal_dual_update.h"
#include "../../minimal_imgutilities/src/iuio.h"

class TVL1DepthEstimation{

public:

    iu::ImageGpu_32f_C1* d_ref_image;
    iu::ImageGpu_32f_C1* d_cur_image;

    iu::ImageGpu_32f_C1* d_px;
    iu::ImageGpu_32f_C1* d_py;

    iu::ImageGpu_32f_C1* d_q;

    iu::ImageGpu_32f_C1* d_u;
    iu::ImageGpu_32f_C1* d_u0;

    iu::VolumeGpu_32f_C1 *d_data_term;
    iu::VolumeGpu_32f_C1 *d_gradient_term;

    std::vector < iu::ImageGpu_32f_C1* > d_dataimages;
    std::vector < iu::ImageGpu_32f_C1* > d_dual_data;

    iu::ImageGpu_32f_C1 *d_sum_dual_times_grad;

    iu::ImageGpu_32f_C1* d_cur2ref_warped;

    iu::VolumeGpu_8u_C1 *d_sortedindices;


    bool allocated;

    iu::ImageGpu_32f_C1 *d_temp_storage;

    int _nimages;

public:

    void InitialiseVariables(float initial_val);
    void InitialiseVariablesAndImageStack(float initial_val);

    unsigned int getImgWidth (){ return d_ref_image->width();}
    unsigned int getImgHeight(){ return d_ref_image->height();}
    unsigned int getNumImages(){ return _nimages; }

    TVL1DepthEstimation():allocated(false),_nimages(2){};
    TVL1DepthEstimation(const std::string& refimgfile, const std::string& curimgfile);
    TVL1DepthEstimation(const std::string& refimgfile, const int nimages);

    void doOneWarp()
    {
        iu::copy(d_u,d_u0);
    }

    void populateImageStack(const std::string& refimgfile);
    void ObtainImageFromTexture(const int which_image);

    void InitialiseWithThisDepthMap(iu::ImageCpu_32f_C1 depth_vals);

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
                                        float dmax,
                                        int which_image);
    void updateWarpedImage(
                            const float2 fl,
                            const float2 pp,
                            TooN::Matrix<3,3> R_lr_,
                            TooN::Matrix<3,1> t_lr_,
                            bool disparity);

    void allocateMemory(const unsigned int width, const unsigned int heightt);

    void sortDataterms();


};


#endif
