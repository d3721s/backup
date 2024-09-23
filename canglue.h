#ifndef CANGLUE_H_INCLUDED
#define CANGLUE_H_INCLUDED
#include <iostream>
#include <cstring> // For memset
#include <sys/socket.h> // For socket functions
#include <linux/can.h> // For CAN definitions
#include <linux/can/raw.h> // For raw CAN socket definitions
#include <net/if.h>
#include <sys/ioctl.h>

class CanBus
{
public:
    CanBus(const char *deviceName) : device(deviceName), sockCanHandle(-1)
    {
        std::cout << "canbus object created" << std::endl;
    }

    ~CanBus()
    {
        if (sockCanHandle != -1)
        {
            close(sockCanHandle);
        }
    }

    void init(void)
    {
        struct sockaddr_can addr;
        struct ifreq ifr;
        sockCanHandle = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        strcpy(ifr.ifr_name, device.c_str());
        ioctl(sockCanHandle, SIOCGIFINDEX, &ifr);

        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;

        bind(sockCanHandle, (struct sockaddr *)&addr, sizeof(addr));
    }

    void sendFrame(const can_frame &frame)
    {
        if (sockCanHandle == -1)
        {
            std::cerr << "Socket not initialized" << std::endl;
        }

        if (send(sockCanHandle, &frame, sizeof(frame), 0) < 0)
        {
            std::cerr << "Failed to send CAN frame" << std::endl;
        }
    }

    void receiveFrame(can_frame &frame)
    {
        if (sockCanHandle == -1)
        {
            std::cerr << "Socket not initialized" << std::endl;
        }
        if (recv(sockCanHandle, &frame, sizeof(frame), 0) < 0)
        {
            std::cerr << "Failed to receive CAN frame" << std::endl;
        }
    }

private:
    std::string device;
    int sockCanHandle;
};

#endif // CANGLUE_H_INCLUDED
