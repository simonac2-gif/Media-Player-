#include "MediaManagerData.h"

const juce::Identifier MediaManagerData::MEDIA_MANAGER_DATA_TYPEID {"MediaManagerData"};
const juce::Identifier MediaManagerData::TRANSPORT_PLAYING {"TransportPlaying"};
const juce::Identifier MediaManagerData::TRANSPORT_GAIN {"TransportGain"};
const juce::Identifier MediaManagerData::TRANSPORT_TEMPO {"TransportTempo"};
const juce::Identifier MediaManagerData::TRANSPORT_POSITION {"TransportPosition"};
const juce::Identifier MediaManagerData::TRANSPORT_ENABLED {"TransportEnabled"};
const juce::Identifier MediaManagerData::TRANSPORT_TEMPO_ENABLED {"TransportTempoEnabled"};
const juce::Identifier MediaManagerData::TRANSPORT_CLICK_PLAYPAUSE {"TransportClickPlayPlause"};
const juce::Identifier MediaManagerData::TRANSPORT_REWIND {"TransportRewind"};
const juce::Identifier MediaManagerData::TRANSPORT_PLAYBACK_DURATION {"TransportPlaybackDuration"};
const juce::Identifier MediaManagerData::LOADED_MEDIA_FILE {"LoadedMediaFile"};
const juce::Identifier MediaManagerData::LOADED_MEDIA_TYPE {"LoadedMediaType"};
const juce::Identifier MediaManagerData::MIDI_OUTPUT_OPEN_ID {"MidiOutputOpenID"};
const juce::Identifier MediaManagerData::INTERNAL_SYNTH_AVAILABLE {"InternalSynthAvailable"};
const juce::Identifier MediaManagerData::LAUNCH_OPEN_MEDIA_DIALOG {"LaunchOpenMediaDialog"};
const juce::Identifier MediaManagerData::LAUNCH_MEDIA_INFO_DIALOG {"LaunchMediaInfoDialog"};

bool MediaManagerData::getTransportEnabled() {
  return data.getProperty(TRANSPORT_ENABLED, false);
}

void MediaManagerData::setTransportEnabled(bool shouldBeEnabled) {
  data.setProperty(TRANSPORT_ENABLED, shouldBeEnabled, nullptr);
}

bool MediaManagerData::getTransportTempoEnabled() {
    
    return data.getProperty(TRANSPORT_TEMPO_ENABLED, false);
}

void MediaManagerData::setTransportTempoEnabled(bool shouldBeEnabled) {
    data.setProperty(TRANSPORT_TEMPO_ENABLED, shouldBeEnabled, nullptr);
}

bool MediaManagerData::getTransportPlaying() {
    return data.getProperty(TRANSPORT_PLAYING, false);
}

void MediaManagerData::setTransportPlaying(bool shouldBePlaying, ValueTree::Listener* exclude) {
    data.setPropertyExcludingListener(exclude, TRANSPORT_PLAYING, shouldBePlaying, nullptr);
    
}

double MediaManagerData::getTransportGain() {
    return data.getProperty(TRANSPORT_GAIN, 1.0);
}

void MediaManagerData::setTransportGain(double gain, ValueTree::Listener* exclude) {
    data.setPropertyExcludingListener(exclude, TRANSPORT_GAIN, gain, nullptr);
}

double MediaManagerData::getTransportTempo() {
    return data.getProperty(TRANSPORT_TEMPO, 60.0);
}

void MediaManagerData::setTransportTempo(double tempo, ValueTree::Listener* exclude) {
    data.setPropertyExcludingListener(exclude, TRANSPORT_TEMPO, tempo, nullptr);
    
}

double MediaManagerData::getTransportPosition() {
    
    return data.getProperty(TRANSPORT_POSITION, 0.0);
}

void MediaManagerData::setTransportPosition(double pos, ValueTree::Listener* exclude) {
    data.setPropertyExcludingListener(exclude, TRANSPORT_POSITION, pos, nullptr);
    
    
}

void MediaManagerData::clickPlayPause(ValueTree::Listener* exclude) {
    
//    ValueTree &     setProperty (const Identifier &name, const var &newValue, UndoManager *undoManager)
//         Changes a named property of the tree. More...
    
    
     
}

void MediaManagerData::setTransportRewind(ValueTree::Listener* exclude) {
  int val = data.getProperty(TRANSPORT_REWIND, 0);
  data.setPropertyExcludingListener(exclude, TRANSPORT_REWIND, val + 1, nullptr);
}

double MediaManagerData::getPlaybackDuration() {
    
    
    return data.getProperty(TRANSPORT_PLAYBACK_DURATION, false);
  
}

void MediaManagerData::setPlaybackDuration(double tempo, ValueTree::Listener* exclude) {
    data.setPropertyExcludingListener(exclude, TRANSPORT_PLAYBACK_DURATION, tempo, nullptr);
    
}

File MediaManagerData::getLoadedMediaFile() {
   auto file =  data.getProperty(LOADED_MEDIA_FILE, "");
    
    
  return File(file);
}

void MediaManagerData::setLoadedMediaFile(const File& file) {
    
    data.setProperty(LOADED_MEDIA_FILE, file.getFullPathName(), nullptr);
   
}

int MediaManagerData::getLoadedMediaType() {
    
  return data.getProperty(LOADED_MEDIA_TYPE, MEDIA_NONE);
}

void MediaManagerData::setLoadedMediaType(int mediaType) {
    data.setProperty(LOADED_MEDIA_TYPE, mediaType, nullptr);
    
}

int MediaManagerData::getMidiOutputOpenID() {
    
  return data.getProperty(MIDI_OUTPUT_OPEN_ID, MEDIA_NONE);
}

void MediaManagerData::setMidiOutputOpenID(int ident, ValueTree::Listener* exclude) {
    data.setPropertyExcludingListener(exclude, MIDI_OUTPUT_OPEN_ID, ident, nullptr);
}

bool MediaManagerData::isInternalSynthAvailable() {
  return data.getProperty(INTERNAL_SYNTH_AVAILABLE, false);
}

void MediaManagerData::setInternalSynthAvailable(bool isAvailable) {
    data.setProperty(INTERNAL_SYNTH_AVAILABLE, isAvailable, nullptr);
}

void MediaManagerData::launchOpenMediaDialog() {
    data.sendPropertyChangeMessage(LAUNCH_OPEN_MEDIA_DIALOG);
 }

void MediaManagerData::launchMediaInfoDialog() {
    data.sendPropertyChangeMessage(LAUNCH_MEDIA_INFO_DIALOG);
}
