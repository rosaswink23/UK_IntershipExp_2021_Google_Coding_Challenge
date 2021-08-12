#include "videoplaylist.h"

#include <iostream>
#include <utility>
#include <vector>

VideoPlaylist::VideoPlaylist(std::string name, std::vector<Video> video) :
  mName(std::move(name)),
  mVideo(std::move(video)) {
}

const std::string& VideoPlaylist::getName() const { return mName; }

const std::vector<Video>& VideoPlaylist::getVideos() const { return mVideo; }

void VideoPlaylist::addVideo(Video video) {
  mVideo.push_back(video);
}

void VideoPlaylist::removeVideo(std::string videoId) {
  int i = 0;
  for (auto& videoInList : mVideo) {
    if (videoId == videoInList.getVideoId()) {
      mVideo.erase(mVideo.begin()+i);
      break;
    }
    i++;
  }
}

void VideoPlaylist::clearVideo() {
  mVideo.clear();
}