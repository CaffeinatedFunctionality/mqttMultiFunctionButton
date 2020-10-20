# mqttMultiFunctionButton
This is a multifunction button that will send out mqtt messages. What you do with those messages is on you. 
I built this for use in a footpedal button to turn on an XLights light show when not running as well as 
turn on the speakers when double clicked. The code can be used outside of the project below though.

## Prerequisites
1. Home Assistant with MQTT setup
2. A smart outlet
3. FPP running connected to your MQTT Broker connected Home Assistant
4. Node Red on Home Assistant

## How to
1. Input your internet and MQTT broker information
2. Connect your button to Pin 2. On a nodeMCU this is D4

### Steps to connect the foot pedal
If you are using the foot pedal I used https://www.amazon.com/gp/product/B00EF9D2DY/
1. Use a NodeMCU
2. Connect D4 to NO
3. Connect COM to Ground
4. Profit

## Current Functionality
1. Single Click
2. Double Click

#### Thingiverse Link

https://www.thingiverse.com/thing:4629131
