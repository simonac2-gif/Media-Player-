#include "Transport.h"

/// Constructor.
Transport::Transport(const MediaManagerData& mmd) {
    
    
    
//    Set the local managerData member to the copy passed in and add this transport as a listener.
//    Make playPauseButton behave as a toggle button (See: setClickingTogglesState()).
//    Draw the playPauseButton, add and make it visible, then add this transport as its button listener.
//    Draw the goToStartButton, add and make it visible, then add this transport as its button listener.
//    Get the gain setting from the manager data, draw the gainButton using that gain setting, then add and make it visible, and add this transport as its button listener.
//    Set the gainSlider range to 0.0-1.0, initialize it with the manager's gain setting, add and make it visible and add this transport as a slider listener.
//    Add the tempo slider, set its style to Slider::LinearBar and its text value suffix to " bpm". Its range should be set to 40 to 208 and its initial value to the manager's tempo setting.
//    Configure the start time label to use Justification::centeredRight, and a font size of 12.0. Add and make visible.
//    Configure the end time label to use Justification::centeredLeft and a font size of 12.0. Add and make visible.
//    Add the position slider, set its range to 1.0 and add this component as its listener.
//    Set the size of the transport to 250 width and 78 height.

    managerData = mmd;
    managerData.addListener(this);
    playPauseButton.setClickingTogglesState(true);
    drawPlayButton(playPauseButton);
    addAndMakeVisible(playPauseButton);
    drawGoToStartButton(goToStartButton);
    addAndMakeVisible(goToStartButton);
    drawGainButton(gainButton, managerData.getTransportGain());
    addAndMakeVisible(gainButton);
    gainButton.addListener(this);
    gainSlider.setRange(0.0, 1.0);
    gainSlider.setValue(managerData.getTransportGain());
    addAndMakeVisible(gainSlider);
    gainSlider.addListener(this);
    addAndMakeVisible(tempoSlider);
    tempoSlider.setSliderStyle(Slider::LinearBar);
    tempoSlider.setRange(40, 208);
    tempoSlider.setNumDecimalPlacesToDisplay(0);
    tempoSlider.setValue(0);
    tempoSlider.setTextValueSuffix(" bpm");


    currentTimeLabel.setJustificationType(Justification::centredRight);
    currentTimeLabel.setFont(12.0);
    drawCurrentTimeLabel();
    //currentTimeLabel.setText((String)positionSlider.getValue(), sendNotification);
    addAndMakeVisible(currentTimeLabel);
    
    endTimeLabel.setJustificationType(Justification::centredLeft);
    endTimeLabel.setFont(12.0);
    drawEndTimeLabel();
    //endTimeLabel.setText((String)positionSlider.getMaximum(), dontSendNotification);

    addAndMakeVisible(endTimeLabel);
    
    addAndMakeVisible(positionSlider);
    positionSlider.setRange(0.0, 1.0);
    positionSlider.addListener(this);
    setSize(250, 78);
   // playPauseButton.addListener();
    
//    tempoSlider.setRange({20,180}, 1);
//    tempoSlider.setTextBoxIsEditable(true);
//    tempoSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
//
//
//
    setVisible(true);
}

/// Destructor.
Transport::~Transport() {
}

void Transport::paint(Graphics& gr) {
    gr.drawRoundedRectangle(Transport::getLocalBounds().getX(), Transport::getLocalBounds().getY(), Transport::getLocalBounds().getWidth(), Transport::getLocalBounds().getHeight(), 8.0, 2.0);
   // gr.drawRoundedRectangle(Transport::getLocalBounds(), 8.0, 2.0);
//  // handy for debugging!
//  gr.setColour(Colours::red);
//  gr.drawRect(playPauseButton.getBounds());
//  gr.drawRect(goToStartButton.getBounds());
//  gr.drawRect(gainButton.getBounds());
//  gr.drawRect(tempoSlider.getBounds());
//  gr.drawRect(gainSlider.getBounds());
//    gr.drawRect(currentTimeLabel.getBounds());
//    gr.drawRect(endTimeLabel.getBounds());
}

void Transport::resized() {
//    All subcomponents should be indented 6 pixels from the bounds of of the transport component.
//    The playPause button should be centered in the transport with size 36.
//    The goToStartButton and gainButton are size 24, and positioned on opposite sides of the playPauseButton vertically centered within the playPause button's height.
//    The tempoSlider is centered in height the same as the goToStartButton and occupies the remainer of the left-side space but keeping a 6 pixel indent.
//    The gainSlider is centered in height the same as the gainButton but starting 6 pixels to the right of the gainButton and taking the rest of the width keeping a 6 pixel indent from the right edge.
//    Line 2: the currentTimeLabel and endTimeLabel are left/right justified in the component with width of 44 and height of 24.
//    The positionSlider should occupy the entire width between them.
    auto bound = Transport::getLocalBounds().reduced(6);
    playPauseButton.setSize(36, 36);
    auto bisect = bound.getCentreY() - 12;
    playPauseButton.setCentrePosition(bound.getCentreX(), bisect);
    

    goToStartButton.setSize(24, 24);
    gainButton.setSize(24, 24);
    goToStartButton.setCentrePosition(bound.getCentreX() - 30, bisect);
    gainButton.setCentrePosition(bound.getCentreX() + 30, bisect);
    auto l_rem = goToStartButton.getX() - bound.getX() - 6;
    tempoSlider.setSize(l_rem, 24);
    
    tempoSlider.setTopRightPosition(goToStartButton.getX() - 6, goToStartButton.getY());


    auto r_rem = bound.getY() + bound.getWidth() - (gainButton.getX()+ gainButton.getWidth()) - 6;
    
  
    gainSlider.setSize(r_rem, 24);
    gainSlider.setTopLeftPosition(gainButton.getX() + gainButton.getWidth() + 6, gainButton.getY());

   
    currentTimeLabel.setSize(44, 24);
    endTimeLabel.setSize(44, 24);
    positionSlider.setSize(bound.getWidth()-88, 24);
    positionSlider.setCentrePosition(bound.getCentreX(), bound.getHeight() - 12);
    
    
    currentTimeLabel.setTopRightPosition(positionSlider.getX(), positionSlider.getY());
    endTimeLabel.setTopLeftPosition(positionSlider.getX() + positionSlider.getWidth(), positionSlider.getY());
    
  
    
    
}

//============================================================================
// JUCE Component Callbacks

void Transport::buttonClicked(juce::Button* button) {
//    If the button clicked is the playPauseButton:
//    Flip managerData's play/pause state to its opposite.
//    Call managerData.setTransportPlaying() and pass it the new state value of the button. Pass this component as the second value so we don't get retriggered by the callback.
//    If the button clicked is the goToStartButton:
//    Set the position slider value to to zero, used dontSendNotification as the second argument so it doesnt trigger listeners.
//    Call managerData.setTransportRewind().
//    Clicking the gain button either mutes or sets the gainSlider to .5 gain. Pass juce::sendNotificationAsync as the second value.
    
    if (button == &playPauseButton) {
        button->setToggleState(!button->getToggleState(), NotificationType::dontSendNotification);
        managerData.setTransportPlaying(button->getToggleState());
        
        
        
    } if ( button == &goToStartButton) {
        positionSlider.setValue(0.0, NotificationType::dontSendNotification);
        managerData.setTransportRewind();
        gainSlider.setValue(.5, NotificationType::dontSendNotification);
        
        
    }
}

void Transport::sliderValueChanged(juce::Slider* slider) {
//    Called when the component's sliders are dragged.
//
//    Your method should take the following actions:
//
//    If the slider moved is the positionSlider:
//    Pass the current slider position to managerData.setTransportPosition() passing the transport component ('this') as the second argument to exclude the transport from being called again.
//    Draw the current time label in the Tranport.
//    If the slider moved is the gainSlider:
//    Pass the current slider position to managerData.setTransportGain() passing the transport component ('this') as the second argument to exclude the transport from being called again.
//    Redraw gainButton so it shows the new state.
//    If the slider moved is the tempoSlider:
//    Pass the current slider position to managerData.setTransportTempo() passing the transport component ('this') as the second argument to exclude the transport from being called again.
    if (slider == &positionSlider) {
        managerData.setTransportPosition(slider->getValue(), this);
        drawCurrentTimeLabel();
    } else if (slider == &gainSlider) {
        auto val = slider->getValue();
        
        managerData.setTransportGain(val, this);
        drawGainButton(gainButton, val);
        
        
        
    }else if (slider == &tempoSlider) {
        auto v = slider->getValue();
        managerData.setTransportTempo(v, this);
    }
}

//============================================================================
// JUCE ValueTree callbacks (listening to the managerData changes)

void Transport::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& ident) {
//    If the property is MediaManagerData::TRANSPORT_ENABLED then enable or disable the transport component based on the current value of mediaManager.getTransportEnabled()
//    If the property is MediaManagerData::TRANSPORT_TEMPO_ENABLED then enable or disable the tempoSlider based on the current value of managerData.getTransportTempoEnabled().
//    If the property is MediaManagerData::TRANSPORT_GAIN then set the gainSlider to the current value of managerData.getTransportGain() using dontSendNotification as the second value so the slider does not trigger another callback.
//    If the property is MediaManagerData::TRANSPORT_TEMPO then set tempoSlider to current value of managerData.getTransportTempo() using dontSendNotification as the second value so the slider does not trigger another callback.
//    If the property is MediaManagerData::TRANSPORT_ENABLED then pass that value to the transport's setEnabled() function.
//    If the property is MediaManagerData::TRANSPORT_CLICK_PLAYPAUSE then trigger a click on the playPauseButton.
//    If the property is MediaManagerData::TRANSPORT_POSITION then:
//    Set positionSlider to the current value of managerData.getTransportPosition() using dontSendNotification as the second value so the slider does not trigger another callback.
//    Call drawCurrentTimeLabel() to display the current time.
//    If the property is MediaManagerData::TRANSPORT_PLAYBACK_DURATION property changed the call drawEndTimeLabel() to display the current ending time.
    if (ident == MediaManagerData::TRANSPORT_ENABLED) {
      setEnabled(managerData.getTransportEnabled());
    } else if (ident == MediaManagerData::TRANSPORT_TEMPO_ENABLED) {
        tempoSlider.setEnabled(managerData.getTransportTempoEnabled());
        
    } else if (ident == MediaManagerData::TRANSPORT_GAIN) {
        gainSlider.setValue(managerData.getTransportGain(), dontSendNotification);
        
    } else if (ident == MediaManagerData::TRANSPORT_TEMPO) {
        tempoSlider.setValue(managerData.getTransportTempo(), dontSendNotification);
        
    } else if (ident == MediaManagerData::TRANSPORT_CLICK_PLAYPAUSE) {
        playPauseButton.triggerClick();
        playPauseButton.setClickingTogglesState(true);
        
        
    } else if (ident == MediaManagerData::TRANSPORT_POSITION) {
        positionSlider.setValue(managerData.getTransportPosition(), dontSendNotification);
        drawEndTimeLabel();
        
    } else if (ident == MediaManagerData::TRANSPORT_PLAYBACK_DURATION) {
        drawEndTimeLabel();
        
    }
}

//============================================================================
// Icon Button Drawing

void Transport::drawPlayButton(juce::DrawableButton& button) {
  juce::DrawablePath triangleDrawable, rectangleDrawable;
  juce::Path trianglePath, rectanglePath;
    
    trianglePath.lineTo(0, 0);
    trianglePath.lineTo(48, 24);
    trianglePath.lineTo(0, 48);
    rectanglePath.addRectangle(5, 5, 15, 38);
    rectanglePath.addRectangle(28, 5, 15, 38);
    triangleDrawable.setPath(trianglePath);
    triangleDrawable.setFill(iconColor);
    rectangleDrawable.setPath(rectanglePath);
    rectangleDrawable.setFill(iconColor);
    

  button.setImages(&triangleDrawable, nullptr, nullptr, nullptr, &rectangleDrawable);
}

void Transport::drawGoToStartButton(juce::DrawableButton& b) {
//    Draws the goToStartButton as a left-side rectangle and a left-pointing triangle.
//
//    The rectangle coordinates are: 100, 0, 100, 100, 13.4, 50 and the triangle coordinates are: 13.4, 0, 13.4, 100.
  // Juce path drawing done in percentage (100x100)
  juce::DrawablePath image;
  juce::Path path;
    path.addRectangle(13.4, 0, 13.4, 100);
    path.addTriangle(100, 0, 100, 100, 13.4, 50);
    image.setPath(path);
    image.setFill(iconColor);
  b.setImages(&image);
}

void Transport::drawGainButton(juce::DrawableButton& button, double gain) {
  // Juce path drawing done in percentage (100x100)
  juce::DrawablePath drawable;
  juce::Path p;
  
  // speaker rect from 0 to 30
  p.addRectangle(0, 30, 30, 35);
  // speaker cone from 0 to 45
  p.addTriangle(0, 50, 40, 0, 40, 100);
  // waves start at x=55 spaced 15 apart
  if (gain > 0.1)
    p.addCentredArc(55, 50, 6, 20,  0, 0, 3.14159f, true);
  if (gain > 0.4)
    p.addCentredArc(70, 50, 5, 35,  0, 0, 3.14159f, true);
  if (gain > 0.7)
    p.addCentredArc(85, 50, 5, 50,  0, 0, 3.14159f, true);
  // this makes button image width 100 no matter how many arcs added
  p.startNewSubPath(100, 0);
  drawable.setPath(p);
  drawable.setFill(iconColor);
  button.setImages(&drawable);
}

void Transport::drawCurrentTimeLabel() {
    auto scale = managerData.getPlaybackDuration() * positionSlider.getValue();
    auto s = toFormattedTimeString(scale);
    currentTimeLabel.setText(s, NotificationType::dontSendNotification);
}

void Transport::drawEndTimeLabel() {
    
    auto scale = managerData.getPlaybackDuration() * positionSlider.getValue();
    auto s = toFormattedTimeString(scale);
    endTimeLabel.setText(s, NotificationType::dontSendNotification);
}

const juce::String Transport::toFormattedTimeString(const double seconds) {
    auto s = juce::roundToIntAccurate(seconds);
    auto time = div(s, 60);
    
    auto min = time.quot;
    auto second = time.rem;
    
    
    
    String foo;
    if (min < 10) {
        foo << 0;
        
    }
    
    foo << min;
    
    foo << ":";
    if (second < 10) {
        foo << 0;
    }
    foo << second;
    
    
    
    return foo;
  
}
