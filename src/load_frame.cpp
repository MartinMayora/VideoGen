extern "C"{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <inttypes.h>
    #include <libswscale/swscale.h>
}
//SOLO VIDEO
bool load_frame(const char* filename, int* width_out, int* height_out, unsigned char** data_out){
    //Abre el archivo usado avformat 
    AVFormatContext* av_format_ctx = avformat_alloc_context();
    if(!av_format_ctx){
        printf("no se pudo crear el av_format_ctx\n");
        return false;
    }
      if(avformat_open_input(&av_format_ctx , filename ,NULL,NULL) != 0){
        printf("no se pudo abrir el video\n");
        return false;
    }

    //encuentra el primer stream de video valido dentro del archivo
    int video_stream_index = -1;
    AVCodecParameters* av_codec_params;
    AVCodec* av_codec;
    for(int i=0; i<av_format_ctx->nb_streams; i++){
        av_codec_params = av_format_ctx->streams[i]->codecpar;
        av_codec = avcodec_find_decoder(av_codec_params->codec_id);
        if(!(av_codec)){
            continue;   
        }
        if(av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO){
            video_stream_index = i;
            break;
        }
    }
    if(video_stream_index == -1){
        printf("no se pudo encontrar video stream valido");
        return false;
    }
    // setear contexto de codec para el decoder
    AVCodecContext* av_codec_ctx = avcodec_alloc_context3(av_codec);
    if(!(av_codec_ctx)){
        printf("no se pudo crear av_codec_ctx");
        return false;
    }
    if(avcodec_parameters_to_context(av_codec_ctx, av_codec_params)<0){
        printf("llenar el context fallo");
        return false;
    }
    if(avcodec_open2(av_codec_ctx, av_codec, NULL)<0){
        printf("no se pudo abrir codec");
        return false;
    }

    AVFrame* av_frame = av_frame_alloc();
    if(!av_frame){
        printf("no se pudo conseguir frame");
        return false;
    }
    AVPacket* av_packet = av_packet_alloc();
    if(!av_packet){
        printf("no se pudo encontrar packet");
        return false;
    }
    int res;
    while(av_read_frame(av_format_ctx, av_packet)>=0){
        if(av_packet->stream_index != video_stream_index){
            continue;
        }
        res = avcodec_send_packet(av_codec_ctx, av_packet);
        if(res < 0){
            printf("error al decodear packet:");
            return false;
        }
        res = avcodec_receive_frame(av_codec_ctx, av_frame);
        if(res == AVERROR(EAGAIN) || res == AVERROR_EOF){
            continue;
        }else if(res<0){
            printf("error al decodear packet: ");
            return false;
        }
        //liberar packet
        av_packet_unref(av_packet);
        break;
    }

    uint8_t* data = new uint8_t[av_frame->width * av_frame->height * 4];
    
    SwsContext* sws_scaler_ctx = sws_getContext(av_frame->width, av_frame->height, av_codec_ctx->pix_fmt,  
                        av_frame->width, av_frame->height, AV_PIX_FMT_RGB0, SWS_BILINEAR, NULL,NULL,NULL);
    if(!sws_scaler_ctx){
        printf("no se pudo inicializar sws scales ctx\n");
        return false;
    }
    uint8_t* 
    sws_scale(sws_scaler_ctx, av_frame->data, av_frame->linesize, 0, av_frame->height,)
    //liberar
    avformat_close_input(&av_format_ctx);
    avformat_free_context(av_format_ctx);
    av_packet_free(&av_packet);
    av_frame_free(&av_frame);
    avcodec_free_context(&av_codec_ctx);
    return true;
}