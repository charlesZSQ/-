#ifndef LTC6804_H
#define LTC6804_H

#include <QString>

struct ConReg{
  byte GPIO;
  byte REFON;
  byte SWTRD;
  byte DCTO;
  byte ADCOPT;
  quint16 VUV;
  quint16 VOV;
  quint16 DCC;
}ConReg[2];

#endif // LTC6804_H
