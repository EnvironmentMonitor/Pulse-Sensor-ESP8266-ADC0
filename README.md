# Pulse-Sensor-ESP8266-ADC0

This requires the Pulse sensor to run at 3v !!!!

The signal pin of the pulse sensor should have a 

2.2K inline to the ADC Pin

1K to GND from the ADC Pin

The original sketch should be modified or use the sketch from here.....

This replaces the Hardware interupt with a Tcker routine ......

Processing is required for the Full screen heartbeat display

Add an MPU6050 and send a tweet with Biometric Sensor results on Freefall Interupt....

<blockquote class="twitter-tweet" data-lang="en-gb"><p lang="en" dir="ltr">Fall Detected: 1 Heart Rate: 84 BPM IBI: 718 mS Body Temperature: 21C Attitude Angle: 40 via an <a href="https://twitter.com/hashtag/ESP8266?src=hash">#ESP8266</a></p>&mdash; MCU Automation (@MCUAutomation) <a href="https://twitter.com/MCUAutomation/status/691233600855904256">January 24, 2016</a></blockquote> <script async src="//platform.twitter.com/widgets.js" charset="utf-8"></script>

