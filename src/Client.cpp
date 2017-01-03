

#include <fstream>
#include "Client.h"
#include "Driver.h"


int main(){
    int id,age,exp,vid;
    char status,temp;
    cin >> id >> temp >> age >> temp >> status >> temp >> exp >> vid;
    ClientDriver clientDriver = ClientDriver(5555);
    clientDriver.createAndSendDriver(id,age,status,exp,vid);
}


ClientDriver::ClientDriver(int portNumber) : udp(Udp(0, portNumber)) {
    udp.initialize();
    cout << "hello from client";
}

int ClientDriver::createAndSendDriver(int id, int age, char status, int experience,
                                      int vehicle_id) {
    Driver *driver = new Driver(id, age, status, experience, vehicle_id);
    //serializing the driver and sending it.
    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive oa(s);
    oa << driver;
    s.flush();
    udp.sendData(serial_str, serial_str.length());

    //receiving the taxi from server and adding it to the driver
    Taxi *taxi;
    char buffer[1024];
    udp.reciveData(buffer, sizeof(buffer));
    //de serializing the taxi we have received.
    string stringedBuffer(buffer, sizeof(buffer));
    boost::iostreams::basic_array_source<char> device((char *) stringedBuffer.c_str(),
                                                      stringedBuffer.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    boost::archive::binary_iarchive ia(s2);
    ia >> taxi;
    driver->assignTaxi(taxi);
    cout << driver->getTaxi()->getId() << "," << driver->getTaxi()->getCarManufacturer();

    while (true) {
        char buffer2[1024];
        if (!driver->isOccupied()) {
            string stringedId;
            ostringstream convert;
            convert << driver->getId();
            stringedId = convert.str();
            udp.sendData("id", 3);
            //sendin id
            udp.sendData(stringedId, stringedId.size());
            //or trip info or "no trip info"
            udp.reciveData(buffer2, sizeof(buffer2));
            string stringedBuffer2(buffer2, sizeof(buffer2));
            /*if  (stringedBuffer2.compare("no trip")) {
                continue;
            } else {*/
                //receiving the trip info from the server and adding it to the driver
                bool occupied;
                boost::iostreams::basic_array_source<char> device2((char *) stringedBuffer2.c_str(),
                                                                   stringedBuffer2.size());
                boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s3(device);
                boost::archive::binary_iarchive ia2(s3);
                ia2 >> occupied;
                driver->setOccupied(occupied);
            //}
        }
        else {
            udp.sendData("ready to go", 12);
            string stringedId;
            ostringstream convert;
            convert << driver->getId();
            stringedId = convert.str();
            udp.reciveData(buffer2, sizeof(buffer2));
            string stringedBuffer2(buffer2, sizeof(buffer2));
            if (stringedBuffer2.compare("finish")) {
                break;
            }
                //if the server wants to know our location
            else if (stringedBuffer2.compare("location")) {
                //udp.sendData(driver->getLocation()->printValue());
                boost::iostreams::back_insert_device<std::string> inserter1(serial_str);
                boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> >
                        s3(inserter);
                boost::archive::binary_oarchive oa3(s);
                //sending the location
                oa << *driver->getLocation();
                s.flush();
                udp.sendData(serial_str, serial_str.length());
            } else {
                //check if it is ok to use buffer 2 again
                //drives as long as it receives go
                if (stringedBuffer2.compare("go")) {
                    driver->drive();
                }
                //if i have finished the trip
                if (driver->getTripInfo()->getRoute()->empty()) {
                    udp.sendData("finished trip", 14);
                    break;
                }
            }
        }
    }
    udp.~Udp();
}