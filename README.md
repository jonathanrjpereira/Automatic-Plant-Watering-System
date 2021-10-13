# Automatic Plant Watering System using Arduino
Meet Sprouter - the Modern Indoor Planter which automatically waters your plants, herbs, vegetables, etc and will revolutionize your gardening game.

**Full Video:**
[![Sprout: Arduino Automatic Self Watering Planter](https://img.youtube.com/vi/lRu-Z6gGrvc/maxresdefault.jpg)](https://youtu.be/lRu-Z6gGrvc)

It consists of an integrated water reservoir from which water is pumped & keeps the plant's soil hydrated.

A soil moisture sensor is calibrated such that it periodically measures the moisture of the soil thereby regulating the water flow. If the soil is too dry, the water pump automatically switches ON and goes OFF when the soil moisture has reached the desired level.

If you're the person who underwaters their plants, Sprouter will ensure you will never have to worry about being a bad gardener again. And if you're the sort of person who overwaters their plants to compensate for absenteeism, it means your not in danger of drowning your plants or seeds.

Sprouter's water reservoir capacity is around 500 ml / 17 fl oz, which allows you to neglect your plants for as long as a month before it needs a refill.

![](https://content.instructables.com/FB2/WUZG/JCAUIIAK/FB2WUZGJCAUIIAK.jpg?auto=webp&fit=bounds&frame=1&height=1024&width=1024)

The optional Bluetooth feature can be used to manually toggle & control the water pump wirelessly from your smartphone.

## Electronic Design
![](https://content.instructables.com/F7N/71AI/JCAUHQAR/F7N71AIJCAUHQAR.jpg?auto=webp&fit=bounds&frame=1&height=1024&width=1024)

**Electronic Components List:-**  
Arduino Nano:  [AliExpress](http://s.click.aliexpress.com/e/bkvVYL0k)  
DC Water Pump:  [AliExpress  
](http://s.click.aliexpress.com/e/bfNLbZuy)Soil Moisture Sensor:  [AliExpress  
](http://s.click.aliexpress.com/e/87kineY)HC-05 Bluetooth Module:  [AliExpress  
](http://s.click.aliexpress.com/e/CZ6XTMk)LM7805 Voltage Regulator:  [AliExpress](http://s.click.aliexpress.com/e/bEYiDSbm)[  
](https://lcsc.com/?s_url=nTajLG)IRF540 MOSFET:  [AliExpress](http://s.click.aliexpress.com/e/cB3xxOSy)  
[](http://s.click.aliexpress.com/e/cB3xxOSy)220 Ohm Resistor:  [AliExpress](http://s.click.aliexpress.com/e/9vWhcpa)  
[](http://s.click.aliexpress.com/e/9vWhcpa)IN4001 Diode:  [AliExpress](http://s.click.aliexpress.com/e/cbX7VpXE)  
[](http://s.click.aliexpress.com/e/cbX7VpXE)Header Pin Strips:  [AliExpress](http://s.click.aliexpress.com/e/cmO4dYcu)  
[](http://s.click.aliexpress.com/e/cmO4dYcu)DC Barrel Jack:  [AliExpress](http://s.click.aliexpress.com/e/EFO7b4M)  
[](http://s.click.aliexpress.com/e/EFO7b4M)Screw Terminal Block:  [AliExpress](http://s.click.aliexpress.com/e/RWeMW4u)  
[](http://s.click.aliexpress.com/e/RWeMW4u)PCB:  [AliExpress  
](http://s.click.aliexpress.com/e/b3XHCl9O)[](http://s.click.aliexpress.com/e/b3XHCl9O)AC-12VDC Adapter:  [AliExpress](http://s.click.aliexpress.com/e/bWW0wsRq)

**Tools:-**  
Soldering Iron:  [AliExpress  
](http://s.click.aliexpress.com/e/bC7ng4wy)Solder Wire:  [AliExpress](http://s.click.aliexpress.com/e/uoYCrrw)

**Power Block**: The 7805 regulates the supply voltage and reduces it to a constant 5V making it suitable to run the Arduino & Soil Moisture Sensor.

**Pump Control**: The MOSFET acts as a switch which is controlled by the Arduino. We use the MOSFET since the Arduino cannot directly power the DC Pump. The resistor connected to the gate of the MOSFET prevents the MOSFET from getting damaged. The flyback diode connected across the pump provides a path for dissipation of stored energy when the pump is switched off. The  _Anode_  of the Diode is connected to the  _Drain_  of the MOSFET. The  _Cathode_  of the Diode is connected to the  _9V_  supply rail. The  _Source_  of the Diode is connected to  _GND_.

**Moisture Sensor**: The sensor feeds an analog value to the Arduino. The threshold level of moisture is calibrated by the user depending on the type of plant used.

**Bluetooth Module**: Uses Serial Communication to transfer data between the Arduino and your Smartphone.

![](https://content.instructables.com/F8H/1M14/JLCKM0N0/F8H1M14JLCKM0N0.jpg?auto=webp&fit=bounds&frame=1&height=1024&width=1024)

## Electronic Assembly
![Electronic Assembly](https://content.instructables.com/ORIG/FIH/9MVM/JCAUIJE2/FIH9MVMJCAUIJE2.jpg?auto=webp&frame=1&width=1024&fit=bounds&md=a0dccf1a63a6bf65b8c7d06fd3377728)
A 1x Scale printable PCB as well as the board view and schematic is available in the GitHub repository.

[Sprouter: GitHub/Electronics](https://github.com/jonathanrjpereira/Automatic-Plant-Watering-System-using-Arduino/tree/master/Electronics)

The repository also contains an A4 size PDF which contains multiple PCB's on a single page. This can be used to make multiple PCB's at a time for mass production

## Software & Bluetooth Configuration
**Software**: The Moisture Sensor is connected to an Analog Input pin of the Arduino. A threshold value determines whether the Pump should be ON/OFF.
You can find the code at  [Sprouter: GitHub/Code](https://github.com/jonathanrjpereira/Automatic-Plant-Watering-System-using-Arduino/blob/master/Code/plant/plant.ino)
Feel free to modify & contribute to the GitHub repository.

**Smartphone App & Bluetooth Configuration**: The HC-05 Bluetooth module is the intermediate block between the Smartphone & Arduino. It uses Serial Communication to send data from the Smartphone to the Arduino & acts as a Remote Control.
The app transmits the value '48' or '49' which represent 'ON' & 'OFF' respectively. The pump can hence be controlled wirelessly.
![](https://content.instructables.com/FSR/I5VA/JCAUIIW1/FSRI5VAJCAUIIW1.png?auto=webp&fit=bounds&frame=1&height=1024&width=1024)
![](https://content.instructables.com/FE6/NC1T/JCAUIIW7/FE6NC1TJCAUIIW7.png?auto=webp&fit=bounds&frame=1&height=1024&width=1024)

Simply open the app, scan for discoverable devices & pair with the HC-05 module. then click on 'Switch Mode' and toggle the onscreen button.
The app is available at  [Bluetooth App](https://play.google.com/store/apps/details?id=com.giumig.apps.bluetoothserialmonitor&hl=en)

## Mechanical Design
![](https://content.instructables.com/FXM/IRSS/JCAUICWN/FXMIRSSJCAUICWN.jpg?auto=webp&fit=bounds&frame=1&height=1024&width=1024)

The main body of Sprouter is a 30cm X 15cm X 19cm Box made out of MDF.

All Mechanical Design steps have been clearly demonstrated in the video attached at the start of the Instructable. You can also check it out at  [Sprout: Video/Mechanical Design](https://www.youtube.com/watch?v=lRu-Z6gGrvc&index=2&list=LLKJ2W0HPvFnYySV5TITcHsg&t=47s)
![](https://content.instructables.com/FW5/R7S4/JCAUICX0/FW5R7S4JCAUICX0.jpg?auto=webp&fit=bounds&frame=1&height=1024&width=1024)

The box is divided into two sections:

1.  The larger section contains the Soil & Plants
2.  The smaller section is further divided into two more sections such that one section contains the Circuit Board while the other contains the Water Reservoir.

The water reservoir is a 500ml plastic bottle.
![](https://content.instructables.com/FEN/36O1/JCAUIU9N/FEN36O1JCAUIU9N.jpg?auto=webp&fit=bounds&frame=1&height=1024&width=1024)
![](https://content.instructables.com/F83/IQRK/JCAUIUDI/F83IQRKJCAUIUDI.jpg?auto=webp&fit=bounds&frame=1&height=1024&width=1024)

The MDF box has 8 separate interlocking faces which can be laser cut and slotted into each other.

The Laser Cutting files, Fusion 360 Design file (3D Design file), isometric as well as orthogonal views of each face can be found at  [Sprout: GitHub/Mechanical Design](https://github.com/jonathanrjpereira/Automatic-Plant-Watering-System-using-Arduino/tree/master/Mechanical%20Design)

You can also find editable Illustrator files in the GitHub repository which can be modified to your specific requirements/dimensions and then can be laser cut.
## Mechanical Assembly: Bottle Preparation
![](https://content.instructables.com/ORIG/FMU/EFZT/JCAUIL8M/FMUEFZTJCAUIL8M.jpg?auto=webp&frame=1&width=1024&fit=bounds&md=415cc17cecc21b20e97dabd7a2a4fdb3)
![](https://content.instructables.com/ORIG/FOV/6MNK/JCAUILB8/FOV6MNKJCAUILB8.jpg?auto=webp&frame=1&width=1024&fit=bounds&md=c0ee561cb0ffc869e7fe2cbb2d62e887)

The water reservoir is a 500ml plastic bottle. A typical 500ml plastic Soda bottle can be used for this.

The maximum diameter of the bottle should be 74mm. The maximum diameter of the cap of the bottle should be 50mm. The maximum height from the base of the bottle to the lowest part of the cap should be 18.5 cm.

The bottle must be cut about 50mm above its base so that the pump can be placed within it. Holes must be cut into the bottle such that the Outlet Pipe and Power wires can be fed through the bottle.
![](https://content.instructables.com/FCT/Y7UJ/JCAUIN0V/FCTY7UJJCAUIN0V.jpg?auto=webp&fit=bounds&frame=1&height=1024&width=1024)

Once the Outlet Pipe and wires have been taken out through their respective holes, the bottle can be sealed again. To seal the bottle we must use an Epoxy Compound which will harden within a few hours. This will prevent any water from leaking out.

The water can be refilled from the top of the bottle by simply opening its cap.

## Mechanical Assembly: Box Preparation
Once you have successfully laser cut the 8 different faces of the box, Apply several coats of a high-quality wood varnish to each side of each face.This makes it highly water repellant & makes it resistant to moisture & humidity.

Mount the Power Jack on the Back Plate too & connect it to the Circuit Board.

![](https://content.instructables.com/FZT/W63Q/JCAUIQ02/FZTW63QJCAUIQ02.jpg?auto=webp&fit=bounds&frame=1&height=1024&width=1024)

Mount the Circuit Board on the Back Plate of the Box such that it fits within their respective section.

Pull the Pump Outlet Pipe through the given holes such that it reaches the Plant soil section. Do the same for the Moisture Sensor wires.

Don't forget to connect the Water Pump to the Circuit Board as shown in the Schematic

Begin interlocking the different faces of the Box and ensure that bottle fits snuggly into its designated area.

Apply wood glue or an adhesive to seal the entire box.

## Mechanical Assembly: Cement Finish
![Mechanical Assembly: Cement](https://content.instructables.com/ORIG/FRS/FPXN/JCAUISTR/FRSFPXNJCAUISTR.jpg?auto=webp&frame=1&width=900&fit=bounds&md=5980c11a333b0066671bf9f55201cd41)
![](https://content.instructables.com/ORIG/FAG/JL83/JCAUISO8/FAGJL83JCAUISO8.jpg?auto=webp&frame=1&width=1024&fit=bounds&md=d37dc4b1da14da8dd4381ce5cc9c9c42)
![](https://content.instructables.com/ORIG/FBA/CJE8/JCAUISPG/FBACJE8JCAUISPG.jpg?auto=webp&frame=1&width=1024&fit=bounds&md=998653fde35fba7e8b8a0f0ea2ea2cd7)
![](https://content.instructables.com/ORIG/F95/3ZTN/JCAUISXA/F953ZTNJCAUISXA.jpg?auto=webp&frame=1&width=1024&fit=bounds&md=af71944d728a21c9130656bd5420bd64)

This step will determine the out texture & final finish of the box as well as give the planter another protective coating.

Apply glue to each face of the box. Then sprinkle some cement over the glue. Use the remaining circular MDF piece which was cut from the Top Plate to smoothen the cement across the surface of each face of the box. Repeat this step for each face of the box as demonstrated in the video.

Once the cement dries, sprinkle with water every 6 hours for 1 day. This will allow the cement to cure, with no cracks and will also prevent water from leaking.

## Add the Soil & Plants
![](https://content.instructables.com/ORIG/F9Q/20DN/JCAUITKF/F9Q20DNJCAUITKF.jpg?auto=webp&frame=1&width=1024&fit=bounds&md=cd0053b1d2efb5e4c54e5f036daa97a8)
![](https://content.instructables.com/ORIG/FUO/VUHW/JCAUITLU/FUOVUHWJCAUITLU.jpg?auto=webp&frame=1&width=1024&fit=bounds&md=27084ae87f44bf6651dd13fc184aa17f)

Once the cement has cured, fill the box with soil.

Remember to heat seal the end of the Outlet pipe before making a hole in it for the dripper. The dripper is used to regulate the water coming out of the pipe so that the water does not flow out of the planter.

Place the Soil Moisture Sensor inside the soil.

Power Sprouter through the Power Jack at the Back Plate & make sure fill the water reservoir to the full level.

Test whether everything works and you should be done.

## Contributing
Are you a programmer, engineer or designer who has a great idea for a new feature/design in Sprouter? Maybe you're just a beginner or you've spotted a bug? Feel free to grab our code, schematics, 3D design files & laser cutting files from Github and tinker with it.
