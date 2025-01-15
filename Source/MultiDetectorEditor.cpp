#include "MultiDetectorEditor.h"
#include "MultiDetector.h"

CustomTextBoxParameterEditor::CustomTextBoxParameterEditor (Parameter* param) : ParameterEditor (param)
{
    jassert (param->getType() == Parameter::FLOAT_PARAM
             || param->getType() == Parameter::INT_PARAM
             || param->getType() == Parameter::STRING_PARAM);

    parameterNameLabel = std::make_unique<Label> ("Parameter name", param->getName());
    Font labelFont = Font ("Silkscreen", "Regular", 10);
    int labelWidth = 120; //labelFont.getStringWidth(param->getName());
    parameterNameLabel->setFont (labelFont);
    parameterNameLabel->setColour (Label::textColourId, Colours::darkgrey);
    addAndMakeVisible (parameterNameLabel.get());

    if (param->getType() == Parameter::FLOAT_PARAM)
        valueTextBox = std::make_unique<Label> ("Parameter value", String (float (param->getValue())));
    else
        valueTextBox = std::make_unique<Label> ("Parameter value", param->getValue().toString());

    valueTextBox->setFont (Font ("CP Mono", "Plain", 15));
    //valueTextBox->setName(param->getProcessor()->getName() + " (" + String(param->getProcessor()->getNodeId()) + ") - " + param->getName());
    valueTextBox->setColour (Label::textColourId, Colours::white);
    valueTextBox->setColour (Label::backgroundColourId, Colours::grey);
    valueTextBox->setEditable (true);
    valueTextBox->addListener (this);
    valueTextBox->setTooltip (param->getDescription());
    addAndMakeVisible (valueTextBox.get());

    finalWidth = std::max (labelWidth, 80);

    setBounds (0, 0, labelWidth, 40);
}

void CustomTextBoxParameterEditor::labelTextChanged (Label* label)
{
    if (param->getType() == Parameter::FLOAT_PARAM)
        param->setNextValue (label->getText().getFloatValue());
    else
        param->setNextValue (label->getText());
}

void CustomTextBoxParameterEditor::updateView()
{
    if (param != nullptr)
    {
        valueTextBox->setEditable (true);

        if (param->getType() == Parameter::FLOAT_PARAM)
            valueTextBox->setText (String (float (param->getValue())), dontSendNotification);
        else
            valueTextBox->setText (param->getValue().toString(), dontSendNotification);
    }
    else
    {
        valueTextBox->setEditable (false);
    }
}

void CustomTextBoxParameterEditor::resized()
{
    parameterNameLabel->setBounds (0, 0, 120, 18);
    valueTextBox->setBounds (5, 20, 60, 18);
}

MultiDetectorEditor::MultiDetectorEditor (GenericProcessor* parentNode)
    : GenericEditor (parentNode)
{
    rippleDetector = (MultiDetector*) parentNode;

    addSelectedChannelsParameterEditor (Parameter::ParameterScope::STREAM_SCOPE, "CNN_Input", 10, 22);
    ParameterEditor* inputLayer = getParameterEditor ("CNN_Input");
    inputLayer->setLayout (ParameterEditor::Layout::nameOnTop);
    inputLayer->setSize (90, 34);

    fileButton = std::make_unique<UtilityButton> ("Load Model");
    fileButton->addListener (this);
    fileButton->setRadius (3.0f);
    fileButton->setTooltip ("Load a model file");
    fileButton->setBounds (130, 32, 80, 20);
    addAndMakeVisible (fileButton.get());

    fileNameLabel = std::make_unique<Label> ("File name", "No file loaded");
    fileNameLabel->setBounds (215, 32, 200, 20);
    addAndMakeVisible (fileNameLabel.get());

    addTextBoxParameterEditor (Parameter::ParameterScope::STREAM_SCOPE, "pulse_duration", 10, 57);
    ParameterEditor* pulseDuration = getParameterEditor ("pulse_duration");
    pulseDuration->setLayout (ParameterEditor::Layout::nameOnTop);
    pulseDuration->setSize (90, 34);

    addTextBoxParameterEditor (Parameter::ParameterScope::STREAM_SCOPE, "timeout", 10, 92);
    ParameterEditor* timeout = getParameterEditor ("timeout");
    timeout->setLayout (ParameterEditor::Layout::nameOnTop);
    timeout->setSize (90, 34);

    addTextBoxParameterEditor (Parameter::ParameterScope::STREAM_SCOPE, "calibration_time", 130, 57);
    ParameterEditor* calibrationTime = getParameterEditor ("calibration_time");
    calibrationTime->setLayout (ParameterEditor::Layout::nameOnTop);
    calibrationTime->setSize (90, 34);

    addTextBoxParameterEditor (Parameter::ParameterScope::STREAM_SCOPE, "threshold", 130, 92);
    ParameterEditor* threshold = getParameterEditor ("threshold");
    threshold->setLayout (ParameterEditor::Layout::nameOnTop);
    threshold->setSize (90, 34);

    addTextBoxParameterEditor (Parameter::ParameterScope::STREAM_SCOPE, "drift", 250, 57);
    ParameterEditor* drift = getParameterEditor ("drift");
    drift->setLayout (ParameterEditor::Layout::nameOnTop);
    drift->setSize (90, 34);

    addComboBoxParameterEditor (Parameter::ParameterScope::STREAM_SCOPE, "output", 250, 92);
    ParameterEditor* output = getParameterEditor ("output");
    output->setLayout (ParameterEditor::Layout::nameOnTop);
    output->setSize (90, 34);

    lastFilePath = CoreServices::getDefaultUserSaveDirectory();
    // More extensions can be added an separated with semi-collons
    // i.e. "*.pb;*.txt;*.exe"
    supportedFileExtensions = "*.pb";

    int fontSize = 15;
    desiredWidth = 360;
}
/**
The listener methods that reacts to the button click. The same method is called for all buttons
on the editor, so the button variable, which cointains a pointer to the button that called the method
has to be checked to know which function to perform.
*/
void MultiDetectorEditor::buttonClicked (Button*)
{
    FileChooser chooseFileReaderFile ("Please select the file you want to load...", lastFilePath, supportedFileExtensions);

    if (chooseFileReaderFile.browseForFileToOpen())
    {
        // Use the selected file
        setFile (chooseFileReaderFile.getResult().getFullPathName());
    }
}

void MultiDetectorEditor::setFile (String file)
{
    File fileToRead (file);
    lastFilePath = fileToRead.getParentDirectory();

    String fileFullName = fileToRead.getFullPathName();
    String filePath = lastFilePath.getFullPathName();
    String fileName = fileToRead.getFileName();

    if (rippleDetector->setFile (fileFullName))
    {
        fileNameLabel->setText (fileName, dontSendNotification);

        //setEnabledState(true);
    }
    else
    {
        //clearEditor();
        fileNameLabel->setText ("No file selected.", dontSendNotification);
    }

    CoreServices::updateSignalChain (this);
    repaint();
}

bool MultiDetectorEditor::updateIntLabel (Label* label, int min, int max, int defaultValue, int* out)
{
    const String& in = label->getText();
    int parsedInt;
    try
    {
        parsedInt = std::atoi (in.toRawUTF8());
    }
    catch (const std::logic_error&)
    {
        label->setText (String (defaultValue), dontSendNotification);
        return false;
    }

    *out = jmax (min, jmin (max, parsedInt));

    label->setText (String (*out), dontSendNotification);
    return true;
}

bool MultiDetectorEditor::updateFloatLabel (Label* label, float min, float max, float defaultValue, float* out)
{
    const String& in = label->getText();
    float parsedFloat;
    try
    {
        parsedFloat = std::atof (in.toRawUTF8());
    }
    catch (const std::logic_error&)
    {
        label->setText (String (defaultValue), dontSendNotification);
        return false;
    }

    *out = jmax (min, jmin (max, parsedFloat));

    label->setText (String (*out), dontSendNotification);
    return true;
}

bool MultiDetectorEditor::updateStringLabel (Label* label, String defaultValue, String& out)
{
    const String& in = label->getText();
    String parsedString;
    try
    {
        parsedString = in.toRawUTF8();
    }
    catch (const std::logic_error&)
    {
        label->setText (defaultValue, dontSendNotification);
        return false;
    }

    out = in;
    label->setText (in, dontSendNotification);
    return true;
}
