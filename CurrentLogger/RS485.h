

#ifndef RS485_h
#define RS485_h

typedef unsigned char UINT8;


class RS485
{
  public:

  RS485(const UINT8 txPin ,const UINT8 rxPin,const UINT8 switchPin);
  void  Init();
  void Send(unsigned char *);
  uint32_t ReceiveResponse(uint32_t numOfBytes , unsigned char*);
  bool checkIfNumOfBytesAvailable(int32_t);
  private:

  const UINT8 txPin;
  const UINT8 rxPin;
  const UINT8 switchPin;
  UINT8 isInit;

};


#endif
