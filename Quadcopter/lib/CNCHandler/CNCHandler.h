#ifndef CNCHANDLER_EXPORTS_H
#define CNCHANDLER_EXPORTS_H

class CNCHandler {
  RF24 myRadio (7, 8);
  byte addresses[][6] = {"1Node", "2Node"};
};

#endif /* CNCHANDLER_EXPORTS_H*/
