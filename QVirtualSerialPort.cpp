#include "QVirtualSerialPort.h"


QVirtualSerialPort::QVirtualSerialPort(QextSerialPort::QueryMode mode, QObject *parent):QextSerialPort(mode,parent)
{
	setPortName(QLatin1String("virtual"));
	projectorPower="P0\n";
}


QVirtualSerialPort::~QVirtualSerialPort(void)
{
}

QByteArray QVirtualSerialPort::readAll()
{
//	QByteArray bytes("abc");
	QByteArray bytes = m_Bufbytes;
	m_Bufbytes.clear();
	return bytes; 
}
qint64 QVirtualSerialPort::write(const QByteArray &data)
{
	QString strCmd = QString(data);
	if(strCmd =="R\n")
	{
		m_Bufbytes.append("R0\n");
		m_Bufbytes.append("X1\n");
	}else if(strCmd=="P1\n"){
		projectorPower="P1\n";
	}
	else if(strCmd=="S\n"){
		m_Bufbytes.append("S100\n");
	}
	else{
		m_Bufbytes.append(data);
	}
	QTimer::singleShot(100, this, SLOT(emitReadReady()));
	return data.length(); 
}
bool QVirtualSerialPort::open(OpenMode mode)
{
	write("V1 1 2\n");
	write("WB9C1\n");
	startWatchDogTimer();
	return isOpen();
}
void QVirtualSerialPort::emitReadReady()
{
	emit readyRead();
}
void QVirtualSerialPort::startWatchDogTimer()
{
    // We call the watchdog every 10 seconds
	timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(watchDog()));
    timer->start(2000);
   // QTimer::start(2000, this, SLOT(watchDog())); // Check in 10 seconds
}
void QVirtualSerialPort::watchDog()
{
	write(projectorPower.toLatin1());
}