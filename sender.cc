#include <omnetpp.h>
#include <iostream>
#include <queue>

using namespace omnetpp;
using namespace std;

class Sender: public cSimpleModule{
private:
    int EXB_SIZE;
    double TIMEOUT;
    double TIME_INTERVAL;
    double CHANNEL_DELAY;

    //source queue
    queue<int> SQ;
    int lastMessageId = -1;
    //exit buffer
    queue<int> EXB;

    void generateMessage();
    void sendToExitBuffer();
    void sendToReceiver();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Sender);

/*
 * Khởi tạo giá trị biến lấy từ omnetpp.ini
 */
void Sender::initialize(){
    EXB_SIZE = par("EXB_SIZE").intValue();
    TIMEOUT = par("TIMEOUT").doubleValue();
    TIME_INTERVAL = par("TIME_INTERVAL").doubleValue();
    CHANNEL_DELAY = par("CHANNEL_DELAY").doubleValue();

    scheduleAt(0, new cMessage("generate"));
    scheduleAt(0, new cMessage("send"));
}

/*
 * xử lý tin nhắn tại nút sender
 * đầu vào con trỏ msg cMessage
 * sinh message theo chu kỳ rồi lưu vào source queue
 * chuyển message từ source queue sang exit buffer nếu exit buffer còn chỗ trống
 * chuyển message từ exitbuffer sang receiver
 */

void Sender::handleMessage(cMessage *msg){
    if(simTime() >= TIMEOUT){
        EV << "Time out" << endl;
        return;
    }

    if(strcmp(msg->getName(), "generate") == 0){
        generateMessage();
        if(EXB.size() < EXB_SIZE)
            sendToExitBuffer();
        scheduleAt(simTime() + TIME_INTERVAL, msg);
    }

    if(strcmp(msg->getName(), "send") == 0){
        sendToReceiver();
        sendToExitBuffer();
        scheduleAt(simTime() + CHANNEL_DELAY, msg);
    }
}

/*sinh message
 * lưu id sinh ra cuối cùng vào source queue
 */
void Sender::generateMessage(){
    SQ.push(++lastMessageId);
    EV << "generated message" << endl;
}

/*
 * chuyển message từ source queue sang exit buffer
 */
void Sender::sendToExitBuffer(){
    if (!SQ.empty()){
           int msgId = SQ.front();
           SQ.pop();
           EXB.push(msgId);
       }
}

/*
 * chuyển message từ exit buffer sang receiver
 */
void Sender::sendToReceiver(){
    int sendMsgId = EXB.front();
    EXB.pop();

    cMessage *sendMsg = new cMessage("sender to receiver msg");

    cMsgPar *msgParam = new cMsgPar("msgId");
    msgParam->setLongValue(sendMsgId);
    sendMsg->addPar(msgParam);

    send(sendMsg, "out");
}





