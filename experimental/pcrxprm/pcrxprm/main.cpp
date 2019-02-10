#include "tau_master.h"

int main(int argc, const char * argv[])
{
    TauDrawer tau("Tau", FRAME_WIDTH, FRAME_HEIGHT
#ifndef HAS_CAMERA
                  , FRAME_IMAGE_IMAGE_SOURCE_PATH, FRAME_IMAGE_SOURCE_NUM_FRAMES
#endif
                  );
    Environment env(&tau, TAU_FPS);

    env.pause();
    sleep(0.1);
    env.resume();
    Mat local_frame;
    while(1)
    {
//        local_frame = tau.frame;//tau.DrawRhoFrame();
        imshow(TITLE_STRING, tau.GetDensitiesFrame(tau.frame));
        imshow("Detection Map", tau.rho_drawer.GetDetectionMapFrame());
//        imshow("Rho Frame", local_frame);
        imshow("X Detection", tau.DrawRhoDetection(X_DIMENSION));
        imshow("Y Detection", tau.DrawRhoDetection(Y_DIMENSION));

        char c = waitKey(KEY_DELAY);
        switch(c)
        {
            case ' ':
                if(env.status != LIVE) env.resume();
                else env.pause();
                break;
            default:
                if(tau.utility.loop(c))
                {
                    tau.utility.trigger();
                    tau.trigger();
                }
                break;
            case 's':
                env.pause();
                sleep(0.01);
                tau.avg = 0;
                tau.count = 0;
                env.start();
                sleep(10);
                env.pause();
                //                printf("%d\n", tau.count);
                printf("Tau averaged %fms and error %.3fpx for %d iterations\n", tau.avg*1000, tau.accuracy, tau.count);
                break;
        }
    }
}
