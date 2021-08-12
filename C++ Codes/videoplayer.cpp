#include "videoplayer.h"
#include "videoplaylist.h"

#define strcasecmp _stricmp
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <bits/stdc++.h> 

bool comp(Video a, Video b) {
  return a.getTitle() < b.getTitle();
}

std::string toLowerCase(const std::string& str) {
  std::string newStr = "";
  for (const auto& c : str) {
    newStr += tolower(c);
  }
  return newStr;
}

void printTags(std::vector<std::string> tags) {
  for (const auto& tag : tags) {
    std::cout << tag;
    if (&tag != &tags.back()) {
      std::cout << " ";
    }
  }
}

bool ifVideoExists(const std::string& videoId, const std::vector<Video>& videos) {
  for (const auto& video : videos) {
    if (videoId == video.getVideoId()) {
      return true;
    }
  }
  return false;
}

bool ifPlaylistExistsAndVideoNotInList(const std::string& playlistName, const std::string& videoId, const std::vector<VideoPlaylist>& playlistList) {
  if (playlistList.empty()) {
    std::cout << "Cannot add video to " << playlistName
                << ": Playlist does not exist" << std::endl;
      return false;
  }
  for (const auto& playlist : playlistList) {
    if (toLowerCase(playlistName) == toLowerCase(playlist.getName())) {
      std::vector<Video> videosInList = playlist.getVideos();
      if (videosInList.empty()){
        return true;
      }
      else {
        for (const auto& videoInList : videosInList) {
          if (videoId == videoInList.getVideoId()) {
            std::cout << "Cannot add video to " << playlistName
                      << ": Video already added" << std::endl;
            return false;
          }
          else if (&videoInList == &videosInList.back() && videoId != videoInList.getVideoId()) {
            return true;
          }
        }
      }
    }
    else {
      std::cout << "Cannot add video to " << playlistName
                << ": Playlist does not exist" << std::endl;
      return false;
    }
  }
}

std::string ifFlagged(std::string videoId, std::map<std::string, std::string> flagMap) {
  std::map<std::string, std::string>::iterator it;
    it = flagMap.find(videoId);
    if (it == flagMap.end()) {
      return "";
    }
    else {
      return it->second;
    }
}

void VideoPlayer::numberOfVideos() {
  std::cout << mVideoLibrary.getVideos().size() << " videos in the library\n"
            << std::endl;
}

void VideoPlayer::showAllVideos() {
  std::cout << "Here's a list of all available videos:" << std::endl;
  std::sort(videos.begin(), videos.end(), comp);
  for (const auto& video : videos) {
    std::cout << "  " << video.getTitle() << " ("
    << video.getVideoId() << ") [";
    printTags(video.getTags());
    std::cout << "]";
    std::string reason = ifFlagged(video.getVideoId(), flagMap);
    if (reason != "") {
      std::cout << " - FLAGGED (reason: " << reason << ")";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void VideoPlayer::playVideo(const std::string& videoId) {

  if (ifVideoExists(videoId, videos) == true) {
    std::string reason = ifFlagged(videoId, flagMap);
    if (reason == "") { // if video not flagged
      if (videoInPlay != "") {
        std::cout << "Stopping video: " << videoInPlay << std::endl;
      }
      videoInPlay = mVideoLibrary.getVideoById(videoId).getTitle();
      isPaused = false;
      std::cout << "Playing video: " << videoInPlay << std::endl;
    }
    else {  // if video is flagged
      std::cout << "Cannot play video: Video is currently flagged (reason: "
                << ifFlagged(videoId, flagMap) << ")" << std::endl;
    }
  }
  else {
    std::cout << "Cannot play video: Video does not exist" << std::endl;
  }
  std::cout << std::endl;
}

void VideoPlayer::stopVideo() {
  if (videoInPlay != "") {
    std::cout << "Stopping video: " << videoInPlay << std::endl;
    videoInPlay = "";
    isPaused = false;
  }
  else {
    std::cout << "Cannot stop video: No video is currently playing" << std::endl;
  }
  std::cout << std::endl;
}

void VideoPlayer::playRandomVideo() {
  if (flagMap.size() == videos.size()) {
    std::cout << "No videos available" << std::endl;
  }
  else {
    while (flagMap.size() <= videos.size()) {
      int random = rand() % videos.size();
      if (ifFlagged(videos[random].getVideoId(), flagMap) == "") {
        if (videoInPlay != "") {
          std::cout << "Stopping video: " << videoInPlay << std::endl;
        }
        videoInPlay = videos[random].getTitle();
        isPaused = false;
        std::cout << "Playing video: " << videos[random].getTitle() << std::endl;
        break;
      }
    }
  }
  std::cout << std::endl;
}

void VideoPlayer::pauseVideo() {
  if (videoInPlay != "") {
    if (isPaused == false) {
      isPaused = true;
      std::cout << "Pausing video: " << videoInPlay << std::endl;
    }
    else {
      std::cout << "Video already paused: " << videoInPlay << std::endl;
    }
  }
  else {
    std::cout << "Cannot pause video: No video is currently playing" << std::endl;
  }
  std::cout << std::endl;
}

void VideoPlayer::continueVideo() {
  if (videoInPlay != "") {
    if (isPaused == true) {
      isPaused = false;
      std::cout << "Continuing video: " << videoInPlay << std::endl;
    }
    else {
      std::cout << "Cannot continue video: Video is not paused" << std::endl;
    }
  }
  else {
    std::cout << "Cannot continue video: No video is currently playing" << std::endl;
  }
  std::cout << std::endl;
}

void VideoPlayer::showPlaying() {
  if (videoInPlay != "") {
    Video video = mVideoLibrary.getVideoByTitle(videoInPlay);
    std::cout << "Currently playing: " << videoInPlay << " ("
              << video.getVideoId() << ") [";
    printTags(video.getTags());
    std::cout << "]";

    if (isPaused == true) {
      std::cout << " - PAUSED" << std::endl;
    }
    else {
      std::cout << std::endl;
    }
  }
  else {
    std::cout << "No video is currently playing" << std::endl;
  }
  std::cout << std::endl;
}

void VideoPlayer::createPlaylist(const std::string& playlistName) {
  if (playlistList.empty()) {
    std::vector<Video> mVideo;
    VideoPlaylist mVideoPlaylist(playlistName, mVideo);
    playlistList.push_back(mVideoPlaylist);
    std::cout << "Successfully created new playlist: "
              << playlistList.back().getName() << std::endl;
  }
  else {
    for (const auto& playlist : playlistList) {
      if (toLowerCase(playlist.getName()) == toLowerCase(playlistName)) {
        std::cout << "Cannot create playlist: A playlist with the same name already exists" << std::endl;
        break;
      }
      else if (&playlist == &playlistList.back()) {
        std::vector<Video> mVideo;
        VideoPlaylist mVideoPlaylist(playlistName, mVideo);
        playlistList.push_back(mVideoPlaylist);
        std::cout << "Successfully created new playlist: "
                  << playlistList.back().getName() << std::endl;
        break;
      }
    }
  }
  std::cout << std::endl;
}

void VideoPlayer::addVideoToPlaylist(const std::string& playlistName,
                                     const std::string& videoId) {
  bool playlistExistsAndVideoNotInList = ifPlaylistExistsAndVideoNotInList(playlistName, videoId, playlistList);
  bool videoExists = ifVideoExists(videoId, videos);
  if (playlistExistsAndVideoNotInList == true) {
    if (videoExists == true) {
      std::string reason = ifFlagged(videoId, flagMap);
      if (reason == "") { // video not flagged
        for (auto& playlist : playlistList) {
          if (toLowerCase(playlistName) == toLowerCase(playlist.getName())) {
            playlist.addVideo(mVideoLibrary.getVideoById(videoId));
            std::cout << "Added video to " << playlistName << ": "
                      << mVideoLibrary.getVideoById(videoId).getTitle() << std::endl;
          }
        }
      }
      else {
        std::cout << "Cannot add video to " << playlistName
                  << ": Video is currently flagged (reason: "
                  << reason << ")" << std::endl;
      }
    }
    else {
      std::cout << "Cannot add video to " << playlistName
                << ": Video does not exist" << std::endl;
    }
  }
  std::cout << std::endl;
}

void VideoPlayer::showAllPlaylists() {
  if (playlistList.empty()) {
    std::cout << "No playlists exist yet" << std::endl;
  }
  else {
    std::cout << "Showing all playlists:" << std::endl;
    for (const auto& playlist : playlistList) {
      std::cout << "  " << playlist.getName() << std::endl;
    }
  }
  std::cout << std::endl;
}

void VideoPlayer::showPlaylist(const std::string& playlistName) {
  if (playlistList.empty()) {
    std::cout << "Cannot show playlist " << playlistName
              << ": Playlist does not exist" << std::endl;
  }
  else {
    for (const auto& playlist : playlistList) {
      if (toLowerCase(playlistName) == toLowerCase(playlist.getName())) {
        std::cout << "Showing playlist: " << playlistName << std::endl;
        if (playlist.getVideos().empty()) {
          std::cout << "  No videos here yet" << std::endl;
          break;
        }
        else {
          for (const auto& video : playlist.getVideos()) {
            std::cout << "  " << video.getTitle() << " ("
                      << video.getVideoId() << ") [";
            printTags(video.getTags());
            std::cout << "]";
            std::string reason = ifFlagged(video.getVideoId(), flagMap);
            if (reason != "") {
              std::cout << " - FLAGGED (reason: " << reason << ")";
            }
            std::cout << std::endl;
          }
        }
      }
      else {
        std::cout << "Cannot show playlist " << playlistName
                  << ": Playlist does not exist" << std::endl;
      }
    }
  }
  std::cout << std::endl;
}

bool ifPlaylistExistsAndVideoInList(const std::string& playlistName, const std::vector<VideoPlaylist>& playlistList, const std::string& videoId, const std::vector<Video> videos) {
  if (playlistList.empty()) {
    std::cout << "Cannot remove video from " << playlistName
              << ": Playlist does not exist" << std::endl;
      return false;
  }
  for (const auto& playlist : playlistList) {
    if (toLowerCase(playlistName) == toLowerCase(playlist.getName())) {
      std::vector<Video> videosInList = playlist.getVideos();
      if (videosInList.empty()){
        return true;
      }
      else {
        for (const auto& videoInList : videosInList) {
          if (videoId == videoInList.getVideoId()) {
            return true;
          }
          else if (&videoInList == &videosInList.back() && videoId != videoInList.getVideoId()) {
            if (ifVideoExists(videoId,videos) == true) {
              std::cout << "Cannot remove video from " << playlistName
                        << ": Video is not in playlist" << std::endl;
            }
            else {
              std::cout << "Cannot remove video from " << playlistName
                        << ": Video does not exist" << std::endl;
            }
            return false;
          }
        }
      }
    }
    else {
      std::cout << "Cannot remove video from " << playlistName
                << ": Playlist does not exist" << std::endl;
      return false;
    }
  }
}

void VideoPlayer::removeFromPlaylist(const std::string& playlistName,
                                     const std::string& videoId) {
  bool playlistExistsAndVideoInList = ifPlaylistExistsAndVideoInList(playlistName, playlistList, videoId, videos);
  if (playlistExistsAndVideoInList == true) {
    for (auto& playlist : playlistList) {
      if (toLowerCase(playlistName) == toLowerCase(playlist.getName())) {
        playlist.removeVideo(videoId);
        std::cout << "Removed video from " << playlistName << ": "
                  << mVideoLibrary.getVideoById(videoId).getTitle() << std::endl;
      }
    }
  }
  std::cout << std::endl;
}

void VideoPlayer::clearPlaylist(const std::string& playlistName) {
  if (playlistList.empty()){
    std::cout << "Cannot clear playlist my_playlist: Playlist does not exist" << std::endl;
  }
  for (auto& playlist : playlistList) {
    if (toLowerCase(playlistName) == toLowerCase(playlist.getName())) {
      playlist.clearVideo();
      std::cout << "Successfully removed all videos from " << playlistName << std::endl;
      break;
    }
    else if (&playlist == &playlistList.back() && toLowerCase(playlistName) != toLowerCase(playlist.getName())) {
      std::cout << "Cannot clear playlist my_playlist: Playlist does not exist" << std::endl;
    }
  }
  std::cout << std::endl;
}

void VideoPlayer::deletePlaylist(const std::string& playlistName) {
  if (playlistList.empty()){
    std::cout << "Cannot delete playlist my_playlist: Playlist does not exist" << std::endl;
  }
  int i = 0;
  for (auto& playlist : playlistList) {
    if (toLowerCase(playlistName) == toLowerCase(playlist.getName())) {
      std::cout << "Deleted playlist: " << playlistName << std::endl;
      playlistList.erase(playlistList.begin()+i);
      break;
    }
    else if (&playlist == &playlistList.back() && toLowerCase(playlistName) != toLowerCase(playlist.getName())) {
      std::cout << "Cannot delete playlist my_playlist: Playlist does not exist" << std::endl;
      break;
    }
    i++;
  }
  std::cout << std::endl;
}

void VideoPlayer::searchVideos(const std::string& searchTerm) {
  std::vector<Video> matchedVideos;
  for (const auto& video : videos) {
    if (toLowerCase(video.getTitle()).find(toLowerCase(searchTerm)) != std::string::npos &&
        ifFlagged(video.getVideoId(), flagMap) == "") {
      matchedVideos.push_back(video);
    }
  }
  if (matchedVideos.empty()) {
    std::cout << "No search results for " << searchTerm << std::endl;
    std::cout << std::endl;
  }
  else {
    std::cout << "Here are the results for " << searchTerm << ":" << std::endl;
    int i = 1;
    std::sort(matchedVideos.begin(), matchedVideos.end(), comp);
    for (const auto& video : matchedVideos) {
      std::cout << "  " << i << ") " << video.getTitle() << " ("
      << video.getVideoId() << ") [";
      printTags(video.getTags());
      std::cout << "]" << std::endl;
      i++;
    }
    std::cout << "Would you like to play any of the above? If yes, specify the number of the video." << 
    "\nIf your answer is not a valid number, we will assume it\'s a no." << std::endl;
    std::string input;
    getline (std::cin, input);
    for (int i=1; i<=matchedVideos.size(); i++) {
      if (input == std::to_string(i)){
        playVideo(matchedVideos[i-1].getVideoId());
        break;
      }
    }
  }
  std::cout << std::endl;
}

void VideoPlayer::searchVideosWithTag(const std::string& videoTag) {
  std::vector<Video> matchedVideos;
  for (const auto& video : videos) {
    for (const auto& tag : video.getTags()) {
      if (toLowerCase(videoTag) == tag  &&
          ifFlagged(video.getVideoId(), flagMap) == "") {
        matchedVideos.push_back(video);
      }
    }
  }
  if (matchedVideos.empty()) {
    std::cout << "No search results for " << videoTag << std::endl;
    std::cout << std::endl;
  }
  else {
    std::cout << "Here are the results for " << videoTag << ":" << std::endl;
    int i = 1;
    std::sort(matchedVideos.begin(), matchedVideos.end(), comp);
    for (const auto& video : matchedVideos) {
      std::cout << "  " << i << ") " << video.getTitle() << " ("
      << video.getVideoId() << ") [";
      printTags(video.getTags());
      std::cout << "]" << std::endl;
      i++;
    }
    std::cout << "Would you like to play any of the above? If yes, specify the number of the video." << 
    "\nIf your answer is not a valid number, we will assume it\'s a no." << std::endl;
    std::string input;
    getline (std::cin, input);
    for (int i=1; i<=matchedVideos.size(); i++) {
      if (input == std::to_string(i)){
        playVideo(matchedVideos[i-1].getVideoId());
        break;
      }
    }
  }
}

void VideoPlayer::flagVideo(const std::string& videoId) {
  bool videoExists = ifVideoExists(videoId, videos);
  if (videoExists == true) {
    std::map<std::string, std::string>::iterator it;
    it = flagMap.find(videoId);
    if (it == flagMap.end()) {  // if video is not flagged
      if (videoInPlay == mVideoLibrary.getVideoById(videoId).getTitle()) {
        std::cout << "Stopping video: " << videoInPlay << std::endl;
        videoInPlay = "";
        isPaused = false;
      }
      flagMap[videoId] = "Not supplied";
      std::cout << "Successfully flagged video: "
                << mVideoLibrary.getVideoById(videoId).getTitle()
                << " (reason: Not supplied)" << std::endl;
    }
    else {  // if video is flagged
      std::cout << "Cannot flag video: Video is already flagged" << std::endl;
    }
  }
  else {
    std::cout << "Cannot flag video: Video does not exist" << std::endl;
  }
  std::cout << std::endl;
}

void VideoPlayer::flagVideo(const std::string& videoId, const std::string& reason) {
  bool videoExists = ifVideoExists(videoId, videos);
  if (videoExists == true) {
    std::map<std::string, std::string>::iterator it;
    it = flagMap.find(videoId);
    if (it == flagMap.end()) {  // if video is not flagged
      if (videoInPlay == mVideoLibrary.getVideoById(videoId).getTitle()) {
        std::cout << "Stopping video: " << videoInPlay << std::endl;
        videoInPlay = "";
        isPaused = false;
      }
      flagMap[videoId] = reason;
      std::cout << "Successfully flagged video: "
                << mVideoLibrary.getVideoById(videoId).getTitle()
                << " (reason: " << reason << ")" << std::endl;
    }
    else {  // if video is flagged
      std::cout << "Cannot flag video: Video is already flagged" << std::endl;
    }
  }
  else {
    std::cout << "Cannot flag video: Video does not exist" << std::endl;
  }
  std::cout << std::endl;
}

void VideoPlayer::allowVideo(const std::string& videoId) {
  bool videoExists = ifVideoExists(videoId, videos);
  if (videoExists == true) {
    std::map<std::string, std::string>::iterator it;
    it = flagMap.find(videoId);
    if (it == flagMap.end()) {  // if video is not flagged
      std::cout << "Cannot remove flag from video: Video is not flagged" << std::endl;
    }
    else {  // if video is flagged
      std::cout << "Successfully remove flag from video: "
                << mVideoLibrary.getVideoById(videoId).getTitle() << std::endl;
      flagMap.erase(videoId);
    }
  }
  else {
    std::cout << "Cannot remove flag from video: Video does not exist" << std::endl;
  }
  std::cout << std::endl;
}