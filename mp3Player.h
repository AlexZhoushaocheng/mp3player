//
// Created by sczhou on 2021/2/22.
//

#ifndef LIBMEDIAPLAYER_MP3PLAYER_H
#define LIBMEDIAPLAYER_MP3PLAYER_H


#include <string>
#include <thread>
#include <atomic>
#include <mutex>

struct mp3dec_ex_t;

class Mp3player final{
public:
    Mp3player();
    ~Mp3player();
    void setMp3File(const std::string& mp3file);
    bool loopPlay();
    void stop();

private:
    std::string filename;
    mp3dec_ex_t *dec{nullptr};
    void *stream{nullptr};
    std::thread loopTh;
    std::atomic_bool loop{false};

    std::mutex stream_lock;
};
#endif //LIBMEDIAPLAYER_MP3PLAYER_H
