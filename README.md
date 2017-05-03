# jumpAndRun
Simple jump and run game for Arduino (requires an I²C display and an INT0 attached momentary button)

# Utilization
* Place the New LiquidCrystal library in your libraries path. It can be obtained from [here](https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home).
* Consider the connection pins. For the Arduino uno: SDA (A4) and SCL (A5). For further references check [here](https://www.arduino.cc/en/Reference/Wire).
* The pin assignments within the sketch are based on how the I²C to Display converter is designed. For my case (~5€ from ebay) it worked. Your mileage may vary.

