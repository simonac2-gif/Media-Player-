#include "MediaManager.h"
#include "MainApplication.h"

MediaManager::MediaManager() {
    
   auto str =  audioDeviceManager.initialise(0, 2, nullptr, true);
   // playbackThread.get()(playbackThread::client, 100, 60, 0);
    jassert(str.isEmpty());
    
//    Create the MIDI playbackThread, passing it this component, 100 ticks-per-beat, tempo of 60.
    playbackThread = std::make_unique<MidiPlaybackThread>(this, 100, 60.0);
//    Start the playbackThread.

    
    playbackThread->startThread();
    
//    Add this MediaManager as listener on the managerData value tree.
    managerData.addListener(this);
    
//    Register the default formats for formatManager (See: AudioFormatManager)
    formatManager.registerBasicFormats();
    
//    Add the audioSourcePlayer to the AudioDeviceManager so it will continuously stream data.
    audioDeviceManager.addAudioCallback(&audioSourcePlayer);
//    Make the transportSource the source for the audioSourcePlayer.
    audioSourcePlayer.setSource(&transportSource);
    
//    The component is a timer, start it running every 50ms.
    startTimer(50);
    
//
}

MediaManager::~MediaManager() {
    stopTimer();
    
    managerData.removeListener(this);
    playbackThread->stopThread(100);
    playbackThread = nullptr;
    

    sendAllSoundsOff();
    closeMidiOutput();
    

    
    transportSource.setSource(nullptr);
    audioSourcePlayer.setSource(nullptr);
    
//    audioSourcePlayer.setSource(nullptr);
    
    audioDeviceManager.removeAudioCallback(&audioSourcePlayer);
    
    
    
    
    
}

///=============================================================================
/// The ManagerData callbacks

void MediaManager::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& ident) {
  auto mediaType = managerData.getLoadedMediaType();
    if (ident == MediaManagerData::LAUNCH_OPEN_MEDIA_DIALOG) {
        openMediaFile();
        
    } else if (ident == MediaManagerData::LAUNCH_MEDIA_INFO_DIALOG) {
        openMediaInfoDialog();
    } else if (ident == MediaManagerData::TRANSPORT_PLAYING) {
        if (managerData.getTransportPlaying()) {
            switch (mediaType) {
                case MediaManagerData::MEDIA_MIDI: playMidi(); break;
                case MediaManagerData::MEDIA_AUDIO: playAudio(); break;
                    
            }
            
        } else {
            switch (mediaType) {
                case MediaManagerData::MEDIA_MIDI: playMidi(); break;
                case MediaManagerData::MEDIA_AUDIO: playAudio(); break;
                    
            }
        }
    } else if (ident == MediaManagerData::TRANSPORT_GAIN) {
        auto val = managerData.getTransportGain();
        switch (mediaType) {
            case MediaManagerData::MEDIA_MIDI: setMidiGain(val); break;
            case MediaManagerData::MEDIA_AUDIO:setAudioGain(val); break;
                
        }
        
        
    } else if (ident == MediaManagerData::MIDI_OUTPUT_OPEN_ID) {
        auto devId = managerData.getMidiOutputOpenID();
        if (devId == 0) {
            closeMidiOutput();
            
        } else {
            openMidiOutput(devId - 1);
        }
    }
    else if (ident == MediaManagerData::TRANSPORT_TEMPO) {
        auto t = managerData.getTransportTempo();
        switch (mediaType) {
            case MediaManagerData::MEDIA_MIDI: setMidiTempo(t); break;
            case MediaManagerData::MEDIA_AUDIO: setAudioTempo(t); break;
                
        }
    } else if (ident == MediaManagerData::TRANSPORT_REWIND) {
        if (managerData.getTransportPlaying()) {
            managerData.clickPlayPause();
            
        }
        switch (mediaType) {
            case MediaManagerData::MEDIA_MIDI:rewindMidi(); break;
            case MediaManagerData::MEDIA_AUDIO:rewindAudio(); break;
                
        }
    } else if (ident == MediaManagerData::TRANSPORT_POSITION) {
        auto pos = managerData.getTransportPosition();
        switch (mediaType) {
            case MediaManagerData::MEDIA_MIDI: setMidiPlaybackPosition(pos); break;
            case MediaManagerData::MEDIA_AUDIO: setAudioPlaybackPosition(pos); break;
                
        }
    }
       
        
    }


///=============================================================================
// PlaybackPositionTimer callback

void MediaManager::timerCallback() {
    
    
    if (managerData.getTransportPlaying()) {
        auto mediaType = managerData.getLoadedMediaType();
        if (mediaType == MediaManagerData::MEDIA_MIDI) {
            scrollMidiPlaybackPosition();
        } else if (mediaType == MediaManagerData::MEDIA_AUDIO) {
            scrollAudioPlaybackPosition();
        }
        
    }
//    Timer callback for scrolling the transport's position.
//
//    Call either scrollMidiPlaybackPosition() or scrollAudioPlaybackPosition() depending on the value of getLoadedMediaType().
//
   
    
   
}

//==============================================================================
// Generic Media Support
//==============================================================================

void MediaManager::openMediaFile() {
    String midiFileTypes {"*.mid;*.midi"};
    String audioFileType (formatManager.getWildcardForAllFormats());
    String allowableTypes = midiFileTypes + ";" + audioFileType;
    FileChooser fc ("Open Media File");
    
    fc.browseForFileToOpen();
    if (fc.browseForFileToOpen()) {
        File file = fc.getResult();
        if (midiFileTypes.contains(file.getFileExtension())) {
            loadMidiFile(file);
            
        } else if (audioFileType.contains(file.getFileExtension())) {
            loadAudioFile(file);
            
        }


    }
}

void MediaManager::openMediaInfoDialog() {
//    Opens a dialog showing information about the loaded midi or audio files.
//
//    Call getMediaInfo() to get information about the loaded media file and if the string is empty return. Otherwise do the following actions:
//
//    Create a TextEditor to hold the info. Configure the editor to be multiline, readonly, sized 400x200. Then give it the text string.
//    Allocate a juce::DialogWindow::LaunchOptions struct and fill it. The dialog's titlebar should be native, it should be resizable, the title should be "Media Info" and the background color our application's backgroundColor.
//
    
    
    auto s = getMediaInfo();
    
    if (s.isEmpty()) {
        return;
    } else {
        const auto textEditor = new TextEditor;
        Font textFont("Times", 15.0f, Font::FontStyleFlags::plain);
        textEditor->setFont(textFont);
        textEditor->setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::green);
        
           textEditor->setMultiLine(true);
           textEditor->setReadOnly(true);
           textEditor->setSize(400, 200);
//           textEditor->setText(s);
           DialogWindow::LaunchOptions launchOptions;
        launchOptions.useNativeTitleBar = true;
        launchOptions.resizable = true;
           launchOptions.dialogTitle = "Media Info";
           launchOptions.dialogBackgroundColour = LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId);
           launchOptions.content.setOwned(textEditor);
           launchOptions.launchAsync();
    }

    
    
}

const String MediaManager::getMediaInfo() {
//    Returns information about the current media file.
//
//    If the managerData's loaded media type is MEDIA_MIDI then return getMidiInfo() else if its MEDIA_AUDIO return getAudioInfo() else return a null string.
    
    auto mediaType = managerData.getLoadedMediaType();
    if (mediaType == MediaManagerData::MEDIA_MIDI) {
       return getMidiInfo();
        
    } else if (mediaType == MediaManagerData::MEDIA_AUDIO) {
        return getAudioInfo();
    }
  return "";
}

//==============================================================================
// Audio playback support

void MediaManager::openAudioSettings() {
    auto comp = std::make_unique<AudioDeviceSelectorComponent>(audioDeviceManager,0,0,2,2,false,true,true,false);
    comp->setSize(500, 270);
    DialogWindow::LaunchOptions options;
    options.dialogTitle = "Audio Settings";
    options.useNativeTitleBar = true;

    options.dialogBackgroundColour= Colours::black;
    options.content.setOwned(comp.release());
    options.launchAsync();
    
    
}

const String MediaManager::getAudioInfo() {
    auto audioFile = managerData.getLoadedMediaFile();
    if (audioFile == File()) return "";
    if (audioFileReaderSource == nullptr) return "";
    AudioFormatReader* reader = audioFileReaderSource->getAudioFormatReader();
    if (reader == nullptr) return "";
    
    
//    Audio file:" The pathname of the audio file
//    "File size:" The size of file in bytes.
//    "Audio format:" The audio format of the file.
//    "Channels:" The number of channels in the audio file.
//    "Sample rate:" The sample rate of the file.
//    "Sample frames:" The number of audio frames in the file.
//    "Bits per sample:" The bits per sample.
//    "Floating point data:" Either "yes" or "no".
//    Access the reader's metadata values (getAllKeys() and getAllValues()) and print the list of 'key' and 'value' pairs each on its own line.
    String info;
    info << "Audio file: " << audioFile.getFullPathName() << "\n" << "File size: " << "\n" << "Audio format: " << "(audio format name)" << "\n" << "Channels: " << "(number of channels)" << "\n" << "Sample rate: " << "(sampling rate)" << "\n" << "Sample frames: " << "(length in samples)" << "\n" << "Bits per sample: " << "(number of bits in a sample)" << "\n" << "Floating point data: " << "(yes or no)" << "\n";
    return info;
 
}

void MediaManager::loadAudioFile(File audioFile) {
//    Loads an audio file into the audio transport.
//
//    Your method should take the following actions:
//
//    Try to create an AudioFormatReader for the given audio file, if you can then call loadIntoTrasport() to load it and update managerData with the the new midi file and media type (MEDIA_AUDIO). If a reader is not created open an AlertWindow telling the user that the audio file was an unsupported format, then zero out the mediaFile and mediaType in the managerData (use File() and MEDIA_NONE.
//    Call managerData.setLoadedMediaFile() and pass it the file, this will be a valid file if loaded otherwise an empty file. e.g. File().
//    Call managerData.setLoadedMediaType() and pass it MEDIA_AUDIO if the file was loaded into the transport, otherwise MEDIA_NONE.
    
    if (AudioFormatReader* reader = formatManager.createReaderFor(audioFile)) {
        loadIntoTransport(reader);
        
    } else {
        String msg("The File ");
        msg << audioFile.getFileName() << " is an unsupported audio format.";
        AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon, "Open Media File", msg);
        audioFile = File();
        
    }
    auto mediaType = (audioFile == File()) ? MediaManagerData::MEDIA_NONE : MediaManagerData::MEDIA_AUDIO;
    
    managerData.setLoadedMediaFile(audioFile);
    managerData.setLoadedMediaType(mediaType);
    
}

void MediaManager::loadIntoTransport(AudioFormatReader* reader) {
    managerData.setTransportPlaying(false);
    managerData.setTransportPosition(0);
    clearAudioPlaybackState();
    audioFileReaderSource.reset(new AudioFormatReaderSource(reader, true));
    transportSource.setSource(audioFileReaderSource.get(), 0, 0, reader->sampleRate);
    managerData.setPlaybackDuration(transportSource.getLengthInSeconds());
    
    
}

void MediaManager::clearAudioPlaybackState() {
    transportSource.setSource(nullptr);
    audioFileReaderSource = nullptr;
    
}

//==============================================================================
// MIDI transport callbacks

void MediaManager::playMidi() {
    
  std::cout << "MediaManager::playMidi()\n";
//    Plays the current midi sequence.
//
//    Your method should take the following actions:
//
//    If the playback thread's position is at the end then set it back to position 0.0.
//    Set the playback thread's tempo to managerData's tempo.
//    Set the playback thread's gain to managerData's gain.
//    Start the playback thread playing (call setPaused(false)).
    if (playbackThread.get()->isPlaybackAtEnd()) {
        playbackThread.get()->setPlaybackPosition(0.0, 0);
        
        
    }
    playbackThread.get()->setTempo(managerData.getTransportTempo());
    playbackThread.get()->setGain(managerData.getTransportGain());
    playbackThread.get()->setPaused(false);
}

void MediaManager::pauseMidi() {
  std::cout << "MediaManager::pauseMidi()\n";
    playbackThread->setPaused(true);
}

void MediaManager::setMidiGain(double gain) {
  std::cout << "MediaManager::setMidiGain(" << gain << ")\n";
    playbackThread->setGain(gain);
    

}

void MediaManager::setMidiTempo(double tempo) {
  std::cout << "MediaManager::setMidiTempo(" << tempo << ")\n";
    playbackThread->setTempo(tempo);
    
}

void MediaManager::rewindMidi() {
  std::cout << "MediaManager::rewindMidi()\n";
    playbackThread->setPlaybackPosition(0.0);
    
}

void MediaManager::setMidiPlaybackPosition(double position) {
    double time = position * midiFileDuration;
    int index = sequence.getNextIndexAtTime(time);
    if (position >= midiFileDuration) {
        index = midiFileLength;
        auto playing = managerData.getTransportPlaying();
        if (playing) {
            
            playbackThread->pause();
        }
        playbackThread->clear();
        playbackThread->setPlaybackPosition(time, index);
        if (playing) {
            playbackThread->play();
            
        }
        
    }
  std::cout << "MediaManager::setMidiPlaybackPosition("<< position <<")\n";
}

void MediaManager::scrollMidiPlaybackPosition() {
  std::cout << "MediaManager::scrollMidiPlaybackPosition()\n";
//    Scrolls the transport's position slider during playback without triggering a TRANSPORT_POSITION callback back to this manager.
//
//    Your method should take the following actions:
//
//    If the transport is currently paused, return.
//    Call MidiPlaybackThread::getPlaybackBeat() to get the current playback beat of the midi thread.
//    Get the total playback duration of the midi file.
//    Divide the beat by the total duration to convert it to a value between 0.0 and 1.0 and set the Tranpost's playback position to that value.
//    If that value is 1.0 or greater then call managerData.clickPlayPause() to stop the transport running.
    
  double sec =  playbackThread->getPlaybackBeat();
    double dur = managerData.getPlaybackDuration();
    double pos =sec/dur;
    managerData.setTransportPosition(pos, this);
    if (pos >= 1.0) {
        managerData.clickPlayPause();
        
    }
    if (!(transportSource.isPlaying())) {
        return;
    } else {
        auto cur =playbackThread.get()->getPlaybackBeat();
        auto dur = midiFileDuration;
        double conv = (double)cur / (double)dur;
        transportSource.setPosition(conv);
    }
}

/// Audio transport callbacks

void MediaManager::playAudio() {
  std::cout << "MediaManager::playAudio()\n";
  // If the transportSource has finished playing set its position to 0.0
  if (transportSource.hasStreamFinished()) {
    transportSource.setPosition(0.0);
  }
  // Set the transportSource's gain to the managerData's gain.
  transportSource.setGain(managerData.getTransportGain());
  // Start the transport source.
  transportSource.start();
}

void MediaManager::pauseAudio() {
  std::cout << "MediaManager::pauseAudio()\n";
  // Stop the transportSource.
  transportSource.stop();
}

void MediaManager::setAudioGain(double gain) {
  std::cout << "MediaManager::setAudioGain(" << gain << ")\n";
  // Set the transportSource's gain to the managerData's gain.
  transportSource.setGain(gain);
}

void MediaManager::setAudioTempo(double tempo) {
  std::cout << "MediaManager::setAudioTempo(" << tempo << ")\n";
  // nothing to do!
}

void MediaManager::rewindAudio() {
  std::cout << "MediaManager::rewindAudio()\n";
  // set the transportSource's position back to 0.0.
  transportSource.setPosition(0.0);
}

/// Sets the audio transport player's position.
void MediaManager::setAudioPlaybackPosition(double pos) {
  auto playing = managerData.getTransportPlaying();
  std::cout << "media manager: receiving position:" << pos
  << ", playing:" << managerData.getTransportPlaying() << "\n" ;
  if (pos == 0) {
    // setSource(0) stops popping on rewind and play
    transportSource.setSource(0);
    transportSource.setSource(audioFileReaderSource.get());
    if (playing) transportSource.start();
  }
  else {
    // std::cout << "transport position=" << position << "\n";
    if (playing) transportSource.stop(); // not sure why this is necessary!
    transportSource.setPosition(pos * transportSource.getLengthInSeconds());
    if (playing) transportSource.start();
  }
}

void MediaManager::scrollAudioPlaybackPosition() {
  double sec = transportSource.getCurrentPosition();
  double dur = transportSource.getLengthInSeconds();
  double pos = sec/dur;
  std::cout << "pbPos="<<sec<<", pbDur="<<dur<<", pbrat="<<pos<<"\n";
  managerData.setTransportPosition(pos, this);
  // auto-pause if at end-of-file
  if (pos >= 1.0)
    managerData.clickPlayPause();
}

///==============================================================================

void MediaManager::openMidiOutput(int dev) {
  // Call MidiOutput::openDevice and reset the midiOutputDevice to it.
    
    
//    Opens a midi output port for the given device id.
//
//    Your method should call the managerData's getMidiOutputOpenID(), if its zero call closeMidiOutput() else call openMidiOutput(), passing it the id. Be sure to subtract 1 from the id so it becomes a valid device index!
    
    midiOutputDevice.reset();
    midiOutputDevice = midiOutputDevice.get()->openDevice(std::to_string(dev));
    auto oID = managerData.getMidiOutputOpenID();
    if (oID == 0) {
        closeMidiOutput();
        
    } else {
        openMidiOutput(dev - 1);
    }
  jassert(midiOutputDevice != nullptr);
}

void MediaManager::closeMidiOutput() {
  // Set the midiOutputDevice to nullptr.
  midiOutputDevice.reset(nullptr);
}

bool MediaManager::isInternalSynthAvailable() {
  return false;
}


///==============================================================================
/// MidiFile Functions

const String MediaManager::getMidiInfo() {
//    Returns information about the current midi file.
//
//    Your method should collect an eol-delimited string containing the following information from either the file or the various midi data members, each on its own line terminated by a return character ("\n"):
    
    auto midiFile = managerData.getLoadedMediaFile();
    
//    "Midi file:" the pathname of the midi file
//    "File size:" the size of file in bytes.
//    "MIDI file format: level" either 0 or 1 depending on if it has more than one track.
//    "Number of Tracks:" the number of tracks in the midi file.
//    "Duration:" the duration in seconds of the midi file.
//    "Number of Messages:" the number of messages in the midi file.
    
//    int midiFileNumTracks {0};
//
//    int midiFileTimeFormat {0};
//
//    int midiFileLength {0};
//
//    double midiFileDuration {0.0};
//
//    MidiMessageSequence sequence;
   
    String info;
    info << "MIDI file: " << midiFile.getFullPathName() << "\n" << "File size: " << midiFile.descriptionOfSizeInBytes(midiFile.getSize()) << "\n" << "MIDI file format: level " << ((midiFileNumTracks > 0) ? 1 : 0) << "\n" << "Number of Tracks: " << midiFileNumTracks << "\n" << "Duration: "<< midiFileDuration << "\n" << "Numebr of Messages: " << midiFileLength << "\n";
    
 
  return info;
}

void MediaManager::clearMidiPlaybackState() {
//    Set midiFileNumTracks to 0.
//    Set midiFileTimeFormat to 0.
//    Clear all the midi messags in sequence.
//    Set the playbackThread's playback position to (0.0, 0).
    midiFileDuration = 0;
    midiFileLength = 0;
    midiFileNumTracks = 0;
    midiFileTimeFormat = 0;
    sequence.clear();
    playbackThread.get()->setPlaybackPosition(0.0, 0);
    
    
}

void MediaManager::loadMidiFile(File midiFile) {
//    Loads a midi file into the midi playback sequence.
//
//    Your method should take the following actions:
//
//    Create a local variable holding a FileInputStream for the file.
//    If the FileInputStream opened ok:
//    Create a local variable holding a juce MidiFile.
//    use MidiFile::readFrom() to see if the input is a valid MidiFile, if it was, and if the MidiFile's time format is greater than 0 then:
//    pass the midi file to loadIntoPlayer().
//    pass the file (not the MidiFile) to managerData.setLoadMediaFile().
//    pass MEDIA_MIDI to managerData.setLoadedMediaType().
//    Otherwise (the previous step failed):
//    open an alert window to tell the user that the the midi file could not be loaded.
//    pass a null File to managerData.setLoadMediaFile().
//    pass MEDIA_NONE to managerData.setLoadedMediaType().
    
    FileInputStream f(midiFile);
    MidiFile midifile;
    if (f.openedOk() && midifile.readFrom(f) && midifile.getTimeFormat()>0) {
        loadIntoPlayer(midifile);
    } else {
        return;
    }
    managerData.setLoadedMediaFile(midiFile);
    
    
    auto mediatype = (midiFile == File()) ? MediaManagerData::MEDIA_NONE : MediaManagerData::MEDIA_MIDI;
    managerData.setLoadedMediaType(mediatype);
    
  
    
}

void MediaManager::loadIntoPlayer(MidiFile& midifile) {
    managerData.setTransportPlaying(false);
    managerData.setTransportPosition(0);
    clearMidiPlaybackState();
    midiFileNumTracks= midifile.getNumTracks();
    midiFileTimeFormat = midifile.getTimeFormat();
    midifile.convertTimestampTicksToSeconds();
    for (int i = 0; i < midiFileNumTracks; i++) {
        const juce::MidiMessageSequence* seq = midifile.getTrack(i);
       
        sequence.addSequence(*seq, 0.0, 0.0,  sequence.getEndTime() + 1);
        sequence.updateMatchedPairs();
        
        
    }
    midiFileLength = sequence.getNumEvents();
    midiFileDuration = sequence.getEndTime();
    playbackThread->setPlaybackLimit(midiFileDuration, midiFileLength);
    managerData.setPlaybackDuration(midiFileDuration);
    
//    Loads a valid MidiFile into the playback sequence.
//
//    Your method should take the following actions:
//
//    Push the stop and rewind buttons to halt playback and clear any pending messages
//    Set midiFileNumTracks and midiFileTimeFormat the values from the midifile.
//    Convert the beat-based midi file into time in seconds.
//    Iterate track index from 0 below MidiFile::getNumTracks() and merge them into our empty playback sequence:
//    get the current track using MidiFile::getTrack()
//    call Sequence::addSequence() to merge the current track into our single playback sequence. For the end time add 1 to the value of getEndTime() to ensure that everything is added!
//    all updateMatchedPairs() on the sequence to attach noteOns to noteOffs.
//    Set midiFileLenfth to the number of events in the sequence
//    Set file playback duration to the very last note event
//    Set the playbackThread's playback limit to the file duration and file length
//    Set the transport's playback range to the file duration

//    playbackThread.get()->pause();
//    playbackThread.get()->setPlaybackPosition(0.0);
//    midiFileNumTracks = midifile.getNumTracks();
//    midiFileTimeFormat = midifile.getTimeFormat();
//    midifile.convertTimestampTicksToSeconds();
//    for (int i = 0; i < midifile.getNumTracks(); i++) {
//
//        sequence.addSequence(*midifile.getTrack(i), sequence.getEndTime() + 1);
//
//
//    }
//    sequence.updateMatchedPairs();
//
//    midiFileDuration = sequence.getEventTime(sequence.getEndTime());
//    playbackThread.get()->setPlaybackLimit(midifile.getLastTimestamp());
//
//        transportSource.setPosition(midiFileDuration);
//
    
}

///==============================================================================
/// MidiMessage Functions

void MediaManager::sendMessage(const MidiMessage& message) {
  ScopedLock sl (sendLock);
  if (midiOutputDevice) {
    midiOutputDevice->sendMessageNow(message);
  }
  else if (isInternalSynthAvailable()) {
    playInternalSynth(message);
  }
}

void MediaManager::playInternalSynth(const MidiMessage& message) {
}

void MediaManager::sendAllSoundsOff() {
  std::cout << "Sending allSoundsOff, output port is "
  << (midiOutputDevice.get() ? "OPEN" : "CLOSED") << "\n";
  for (int i = 1; i <= 16; i++) {
    juce::MidiMessage msg = juce::MidiMessage::allSoundOff(i);
    sendMessage(msg);
  }
}

//==============================================================================
// MidiPlaybackClient callbacks

void MediaManager::handleMessage (const MidiMessage &message) {
  sendMessage(message);
}

void MediaManager::addMidiPlaybackMessages(MidiPlaybackThread::MidiMessageQueue& queue,
                                           MidiPlaybackThread::PlaybackPosition& position) {
  int index = position.index;
  for (; index < position.length; index++) {
    juce::MidiMessageSequence::MidiEventHolder* ev = sequence.getEventPointer(index);
    // skip over non-channel messages
    if (ev->message.getChannel() < 1)
      continue;
    // skip over noteOffs because we add by pairs with noteOns
    if (ev->message.isNoteOff())
      continue;
    // add every message that is at or earlier (e.g. late) than the current time
    if (ev->message.getTimeStamp() <= position.beat) {
      queue.addMessage(new juce::MidiMessage(ev->message));
      if (ev->noteOffObject) {
        queue.addMessage(new juce::MidiMessage(ev->noteOffObject->message));
      }
    }
    else
      break;
  }
  // index is now the index of the next (future) event or length
  position.index = index;
  if (position.isAtEnd())
    std::cout << "Midi playback at end!\n";
}




