#include <omnetpp.h>

using namespace omnetpp;

class Receiver: public cSimpleModule {
private:
    double TIMEOUT;
    double TIME_INTERVAL;
    int *receivedMsgCount;
    int arrayLength;
    int intervalCount = 0;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

Define_Module(Receiver);

/*
 * Khởi tạo giá trị biến từ omnetpp.ini
 */
void Receiver::initialize() {
    TIME_INTERVAL = par("TIME_INTERVAL").doubleValue();
    TIMEOUT = par("TIMEOUT").doubleValue();

    arrayLength = TIMEOUT / TIME_INTERVAL;
    receivedMsgCount = new int[arrayLength];
    memset(receivedMsgCount, 0, arrayLength * sizeof(int));

    scheduleAt(0, new cMessage("nextInterval"));
}

/*
 * xử lý message nhận được từ sender
 * đếm số message nhận được trong từng interval
 */
void Receiver::handleMessage(cMessage *msg) {
    if (simTime() >= TIMEOUT){
        return;
    }

    if (strcmp(msg->getName(), "sender to receiver msg") == 0) {
        EV << "Received msg" << endl;
        receivedMsgCount[intervalCount]++;
        delete msg;
    }

    if (strcmp(msg->getName(), "nextInterval") == 0) {
        intervalCount++;
        scheduleAt(simTime() + TIME_INTERVAL, msg);
    }
}


/*
 * in ra thống kê số gói tin nhận được theo từng interval
 */
void Receiver::finish(){
    for (int i = 0; i < arrayLength; i++) {
        EV << "interval " << i << ", received " << receivedMsgCount[i] << " messages" << endl;
    }
}
