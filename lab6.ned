simple Sender{
    parameters:
        int EXB_SIZE;
        double TIMEOUT;
        double TIME_INTERVAL;
        double CHANNEL_DELAY;
    gates:
    	output out;
}

simple Receiver{
    parameters:
        double TIMEOUT;
    	double TIME_INTERVAL;
    gates:
        input in;
}

network sender_receiver{
    submodules:
        sender: Sender;
        receiver: Receiver;
    connections:
        sender.out --> { delay = 15ms; } --> receiver.in;
}