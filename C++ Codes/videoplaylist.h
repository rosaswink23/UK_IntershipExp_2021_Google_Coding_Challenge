#pragma once

#include "video.h"
#include <string>
#include <vector>
#include <bits/stdc++.h> 

/** A class used to represent a Playlist */

class VideoPlaylist {
    private:
        std::string mName;
        std::vector<Video> mVideo;

    public:
        VideoPlaylist(std::string name, std::vector<Video> video);

        // Returns the name of the playlist.
        const std::string& getName() const;
        // Returns the videos put in the playlist.
        const std::vector<Video>& getVideos() const;

        void addVideo(Video video);
        void removeVideo(std::string videoId);
        void clearVideo();
};
