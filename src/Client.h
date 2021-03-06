//
// Created by hanani on 26/12/16.
//

#ifndef ADPROG1_1_CLIENTDRIVER_H
#define ADPROG1_1_CLIENTDRIVER_H

#include <vector>
#include "Udp.h"
#include "Tcp.h"

class ClientDriver {
private:
    Tcp tcp;


public:
    /**
     * constructor.
     * @param portNumber is the number of the port we need to connect to.
     * @return a pointer to clientDriver type.
     */
    ClientDriver(int portNumber, string ip);

    /**
     * creates a new driver and sends it to the server.
     * @param id of the driver.
     * @param age of the driver.
     * @param status of the driver.
     * @param experience of the driver.
     * @param vehicle_id of the driver.
     * @return if succeeded ot failed sending.
     */
    int createAndSendDriver(int id, int age, char status, int experience, int vehicle_id);

    /**
     * turning a char array in to a string.
     * @param buffer is the name of the array.
     * @param bufferSize is the size.
     * @return a string.
     */
    string bufferToString(char* buffer, int bufferSize);

    /**
     * static function that checks the validity of the client.
     * @return true or false.
     */
    bool static driverInputValid(vector<string>);

        //virtual ~ClientDriver();


};


#endif //ADPROG1_1_CLIENTDRIVER_H
