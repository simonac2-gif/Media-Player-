//==============================================================================

#include "MainComponent.h"
#include "MainApplication.h"

//==============================================================================
// Component overrides

MainComponent::MainComponent() : midiOutputMenu(*this) {
//    Set our managerData member variable to the MediaManager's managerData and add this component as a listener of our local copy.
//    Add the openButton and make it visible, add this component as its listener.
//    Add the midiOutputMenu and make visible. When nothing is selected is should display "MIDI Outputs". it and add this component as a listener.
//    Add and make visible the infoButton. Call setEnabled(false) to disabled it and add this component as a listener.
//    Create the Transport giving it our local managerData and make it visible. The transport's initial state should disabled and our component should be its listener.
    managerData = MainApplication::getApp().getMediaManager()->getManagerData();
    managerData.addListener(this);
    
    
   
    
    
    addAndMakeVisible(openButton);
    openButton.addListener(this);
    addAndMakeVisible(midiOutputMenu);
    midiOutputMenu.setText("MIDI Outputs");
    midiOutputMenu.setColour(ComboBox::textColourId, Colours::white);
    midiOutputMenu.addListener(this);
    addAndMakeVisible(infoButton);
    infoButton.setEnabled(true);
    infoButton.addListener(this);
    
    transport = std::make_unique<Transport>(managerData);
    transport->setEnabled(true);
    addAndMakeVisible(transport.get());
   
    
 //   managerData = MediaManager::getManagerData();
  

    
    
    setVisible(true);
   
   
  
   
   
    
  
    
    
}

MainComponent::~MainComponent() {
    managerData.removeListener(this);
    
}

void MainComponent::paint (Graphics& gr) {
    auto bgc = LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    gr.setColour(bgc);
    gr.fillAll(bgc);
//    gr.setColour(Colours::red);
//    gr.drawRect(openButton.getBounds());
//    gr.drawRect(midiOutputMenu.getBounds());
//    gr.drawRect(infoButton.getBounds());
//    gr.drawRect(transport->getBounds());
    
}

void MainComponent::resized() {
    auto bounds = getLocalBounds().reduced(8);
    auto line = bounds.removeFromTop(24);
    openButton.setBounds(line.removeFromLeft(90));
    infoButton.setBounds(line.removeFromRight(90));
    midiOutputMenu.setSize(180, line.getHeight());
    midiOutputMenu.setCentrePosition(line.getCentreX(), line.getCentreY());
    bounds.removeFromTop(8);
    transport->setCentrePosition(bounds.getCentreX(), bounds.getCentreY());
    
    
}

//==============================================================================
// JUCE Listener overrides

void MainComponent::buttonClicked(Button* button) {
    
    if (button == &openButton) {
        managerData.launchOpenMediaDialog();
        
    } else if (button == &infoButton) {
    managerData.launchMediaInfoDialog();
        
    }
}

void MainComponent::comboBoxChanged(ComboBox* menu) {
    if (menu == &midiOutputMenu) {
        auto id = midiOutputMenu.getSelectedId();
        managerData.setMidiOutputOpenID(id);
    }
}

//==============================================================================
// ValueTree::Listener overrides

void MainComponent::valueTreePropertyChanged(juce::ValueTree& tree,
                                             const juce::Identifier& ident) {
    auto mediaType = managerData.getLoadedMediaType();
    if (ident == MediaManagerData::TRANSPORT_PLAYING) {
        
        
    
    if (managerData.getTransportPlaying() == true) {
        openButton.setEnabled(true);
        
    }
    if (mediaType == MediaManagerData::MEDIA_MIDI) {
        if (managerData.getTransportPlaying() == false) {
            midiOutputMenu.setEnabled(true);
            
        }
        midiOutputMenu.setEnabled(false);
        
        
    }
}
    
    
    else if (ident == MediaManagerData::LOADED_MEDIA_TYPE) {
        if (mediaType == MediaManagerData::MEDIA_AUDIO) {
            infoButton.setEnabled(true);
            midiOutputMenu.setEnabled(false);
            managerData.setTransportEnabled(true);
            managerData.setTransportTempoEnabled(false);

        } else if (mediaType == MediaManagerData::MEDIA_MIDI) {
            infoButton.setEnabled(true);
            midiOutputMenu.setEnabled(true);
            managerData.setTransportEnabled(true);
            managerData.setTransportTempoEnabled(false);
            
        } else if (mediaType == MediaManagerData::MEDIA_NONE) {
            infoButton.setEnabled(false);
            midiOutputMenu.setEnabled(false);
            managerData.setTransportEnabled(false);
            managerData.setTransportTempoEnabled(false);


            
        }
    } else if (ident == MediaManagerData::MIDI_OUTPUT_OPEN_ID) {
        auto midiOpen = managerData.getMidiOutputOpenID();
        if (mediaType == MediaManagerData::MEDIA_MIDI) {
            managerData.setTransportEnabled(midiOpen);
            managerData.setTransportTempoEnabled(midiOpen);

        }
    }
    
}
