//
// Created by sczhou on 2021/2/22.
//

#include "mp3Player.h"

#include <portaudio.h>
#define MINIMP3_IMPLEMENTATION

#include "minimp3.h"
#include "minimp3_ex.h"

#define FRAME_BUFF_SIZE 256

Mp3player::Mp3player(){
    dec = new mp3dec_ex_t;
    Pa_Initialize();
}
Mp3player::~Mp3player(){
    delete(dec);
    Pa_Terminate();
}

void Mp3player::setMp3File(const std::string& mp3file)
{
    if(mp3file != filename)
    {
        stop();
        filename = mp3file;
    }
}

bool Mp3player::loopPlay(){
    if(1 == Pa_IsStreamActive(stream))
    {
        return true;
    }

    if(mp3dec_ex_open(dec, filename.data(), MP3D_SEEK_TO_SAMPLE)){
        perror("open mp3 file failed");
        return false;
    }
//    printf("open file success \n");
    PaError err = paNoError;
    err = Pa_OpenDefaultStream(&stream, 0, dec->info.channels, paInt16, dec->info.hz, FRAME_BUFF_SIZE, nullptr, nullptr);
    if (err != paNoError) {
        perror("open output stream failed");
        return  false;
    }
//    printf("open stream success \n");
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        perror("start output stream failed");
        return false;
    }
//    printf("start stream success \n");
    mp3d_sample_t *buffer = (mp3d_sample_t*)new char[(dec->samples*sizeof(mp3d_sample_t))];
    size_t readed = mp3dec_ex_read(dec, buffer, dec->samples);
    if (readed != dec->samples) /* normal eof or error condition */
    {
        if (dec->last_error)
        {
            perror("read mp3 file failed");
            return false;
        }
    }

    loop = true;
    loopTh = std::thread([=](){
        while (loop)
        {
            std::lock_guard<std::mutex> locker(stream_lock);
            Pa_WriteStream(stream, buffer, dec->samples);
        }
        delete[](buffer);
    });
}

void Mp3player::stop(){
    loop = false;
    std::lock_guard<std::mutex> locker(stream_lock);
    if(1 == Pa_IsStreamActive(stream))
    {
      Pa_StopStream(stream);
      Pa_CloseStream(stream);
      loopTh.join();
    }
}