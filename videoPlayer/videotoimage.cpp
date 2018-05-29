#include "videotoimage.h"

videoToImage::videoToImage(QObject *parent) : QObject(parent)
{
    av_register_all();
    pFormatCtx = avformat_alloc_context();
    index =0;
}

void videoToImage::OpenVideo(QString path)
{
    //取得路径，打开视频文件
    char *file_path = path;
    int videoStream =-1;
    AVFrame *pFrame;
    int *got_picture;
    SwsContext *img_convert_ctx;
    avformat_open_input(&pFormatCtx,file_path,NULL,NULL);
    /* 循环查找视频中包含的流的信息，知道知道与视频类型相同的流
     * 记录下来保存在videoStream中
     * 暂时只处理视频流
     */
    for(int i=0;i<pFormatCtx->nb_streams;i++){
        if(pFormatCtx->streams[i]->codec->codec_type == \
           AVMEDIA_TYPE_VIDEO){
            videoStream = i;
        }
    }
    if(videoStream == -1){
        qDebug()<<"Didn't find a video stream";
        return;
    }
    //根据视频流类型查找解码器
    pCodecCtx = pFormatCtx->streams[videoStream]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec == NULL){
        qDebug()<<"Codec not found!";
        return;
    }
    //打开解码器
    if(avcodec_open2(pCodecCtx,pCodec,NULL)<0){
        qDebug()<<"Could not Open codec";
    }

    /*开始读取视频
     *
     */
    int y_size = pCodecCtx->width * pCodecCtx->height;
    //分配一个packet
    AVPacket *packet = (AVPacket *)malloc(sizeof(AVPacket));
    av_new_packet(packet,y_size);//分配packet数据
    //读取一帧数据到packet中
    if(av_read_frame(pFormatCtx,packet)<0){
        break;//这里认为视频读取完了
    }

    if(packet->stream_index == videoStream){
        ret = avcodec_decode_video2(pCodecCtx,pFrame,got_picture,packet);
        if(ret<0){
            qDebug()<<"decode error";
            return;
        }
    }

    if(got_picture){
        sws_scale(img_convert_ctx,
                       (uint8_t const * const *) pFrame->data,
                       pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
                       pFrameRGB->linesize);
    }

    saveFrame(pFrameRGB,pCodecCtx->width,pCodecCtx->height,index++);
    if(index>50){
        return;
    }

}

void videoToImage::SaveFrame(AVFrame *pFrame, int width, int height, int index)
{
      FILE *pFile;
      char szFilename[32];
      int  y;

      // Open file
      sprintf(szFilename, "frame%d.ppm", index);
      pFile=fopen(szFilename, "wb");

      if(pFile==NULL)
        return;

      // Write header
      fprintf(pFile, "P6%d %d255", width, height);

      // Write pixel data
      for(y=0; y<height; y++)
      {
        fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);
      }

      // Close file
      fclose(pFile);
}
