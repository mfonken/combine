#include "tau_master.h"

int main(int argc, const char * argv[])
{
    TauDrawer tau("Tau", FRAME_WIDTH, FRAME_HEIGHT
#ifndef HAS_CAMERA
      , FRAME_IMAGE_IMAGE_SOURCE_PATH, FRAME_IMAGE_SOURCE_NUM_FRAMES
#endif
      );
    Environment env(&tau, TAU_FPS);
    env.start();
//    env.pause();
//    sleep(0.1);
//    env.resume();
    Mat local_frame;
    while(1)
    {
        local_frame = tau.GetDensitiesFrame(tau.frame);
//        imshow("Thresh Frame", tau.utility.outframe);
        imshow(TITLE_STRING, local_frame);
        imshow("Detection Map", tau.rho_drawer.GetDetectionMapFrame());
//        imshow("Rho Frame", local_frame);
//        imshow("X Detection", tau.DrawRhoDetection(X_DIMENSION));
//        imshow("Y Detection", tau.DrawRhoDetection(Y_DIMENSION));

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
            case 'p':
                struct timeval tp;
                gettimeofday(&tp, NULL);
                long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000; //get current timestamp in milliseconds
                imwrite(FRAME_SAVE_ROOT + to_string(ms) + ".png", local_frame);
                break;
        }
    }
}
