#include "ReverbMachine/PluginProcessor.h"

#include "ReverbMachine/Identifiers.h"
#include "ReverbMachine/PluginEditor.h"
namespace audio_plugin {
ReverbMachineAudioProcessor::ReverbMachineAudioProcessor()
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
              ),
      tree(*this, nullptr, ID::ReverbMachine_state, ID::getParameterLayout()) {
}

ReverbMachineAudioProcessor::~ReverbMachineAudioProcessor() {}

const juce::String ReverbMachineAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool ReverbMachineAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool ReverbMachineAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool ReverbMachineAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;

#endif
}

double ReverbMachineAudioProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int ReverbMachineAudioProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0
  // programs, so this should be at least 1, even if you're not
  // really implementing programs.
}

int ReverbMachineAudioProcessor::getCurrentProgram() {
  return 0;
}

void ReverbMachineAudioProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String ReverbMachineAudioProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);

  return {};
}

void ReverbMachineAudioProcessor::changeProgramName(
    int index,
    const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void ReverbMachineAudioProcessor::prepareToPlay(double sampleRate,
                                                int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  SampleRate::set(sampleRate);
  // this needs to be called once here before the first processBlock
  // such that the core initializes the defalut reverb algo
  verb.updateParams(tree);
  juce::ignoreUnused(samplesPerBlock);
}

void ReverbMachineAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

bool ReverbMachineAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;
  // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif
  return true;
#endif
}

void ReverbMachineAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                               juce::MidiBuffer& midiMessages) {
  juce::ignoreUnused(midiMessages);
  juce::ScopedNoDenormals noDenormals;
  auto inputChannels = getTotalNumInputChannels();
  auto outputChannels = getTotalNumOutputChannels();
  // // In case we have more outputs than inputs, this code clears any output
  // // channels that didn't contain input data, (because these aren't
  // // guaranteed to be empty - they may contain garbage).
  // // This is here to avoid people getting screaming feedback
  // // when they first compile a plugin, but obviously you don't need to keep
  // // this code if your algorithm always overwrites all the output channels.
  //
  // for (auto i = inputChannels; i < outputChannels; ++i)
  //   buffer.clear(i, 0, buffer.getNumSamples());
  //
  //-------------------------------------------------
  verb.updateParams(tree);
  bool inStereo = inputChannels > 1;
  bool outStereo = outputChannels > 1;
  float* lBuf = buffer.getWritePointer(0);
  float* rBuf = outStereo ? buffer.getWritePointer(1) : nullptr;
  verb.processChunk(lBuf, rBuf, buffer.getNumSamples(), inStereo, outStereo);
}

bool ReverbMachineAudioProcessor::hasEditor() const {
  return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReverbMachineAudioProcessor::createEditor() {
  return new ReverbMachineProcessorEditor(*this);
}

void ReverbMachineAudioProcessor::getStateInformation(

    juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  juce::ignoreUnused(destData);
}

void ReverbMachineAudioProcessor::setStateInformation(const void* data,
                                                      int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
  juce::ignoreUnused(data, sizeInBytes);
}

}  // namespace audio_plugin
// This creates new instances of the plugin.
// This function definition must be in the global namespace.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new audio_plugin::ReverbMachineAudioProcessor();
}
