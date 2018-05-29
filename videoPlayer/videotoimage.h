#ifndef VIDEOTOIMAGE_H
#define VIDEOTOIMAGE_H

#include <QObject>
#include <QWidget>
#include <QString>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavformat/version.h>
#include <libavutil/time.h>
#include <libavutil/mathematics.h>
}
class videoToImage : public QObject
{
    Q_OBJECT
public:
    explicit videoToImage(QObject *parent = 0);
    void OpenVideo(QString path);
    void SaveFrame(AVFrame *pFrame, int width, int height,int index);

signals:

public slots:

private:
    AVFormatContext *pFormatCtx;
    AVCodecContext  *pCodecCtx;
    AVCodec  *pCodec;
    int index;
};

#endif // VIDEOTOIMAGE_H
