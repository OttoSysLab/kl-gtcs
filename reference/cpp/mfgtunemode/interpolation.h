#pragma once

#include <qobject.h>
#include <qtopia/u3log.h>

#include "mfgtunedefine.h"

class TInterpolation : public QObject
{
    Q_OBJECT
    U3_LOG(TMFGTuneMode)
public:
    TInterpolation(QObject *parent = 0);
    ~TInterpolation();

    // is get operator AntennaTuneInfoList &info list 
    bool StructInterpolation(AntennaTuneInfoList &info);

private:
    // linearly operator
    void PartIterpolation(QValueList<int*> plist);

};
