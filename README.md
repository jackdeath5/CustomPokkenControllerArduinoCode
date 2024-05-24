So, recently I've done probably every mod that may be desirable for a Hori Pokken Pro pad to make what is, at least probably, one of the most overkill Pokken controllers currently out there. This guide primarily consists of documentation I'm leaving behind from my modding, so it's easier for others to do it on their own in the future. For those who want to mod their Pokken controllers though, this is for you!

![Pokken Controller Charizard Colors](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Pokken%20Controller%20Final%20Zard%20Colors.jpg?raw=true)

![Pokken Controller Rainbow Colors](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Pokken%20Controller%20Final%20Rainbow.jpg?raw=true)

Demonstration Video: https://youtu.be/_Nhh7NROFjw 

# Disclaimer
I am not responsible for anything if you screw up and destroy anything. If you have questions about this or the process of making the controller, feel free to ask me on Discord. If you're here for Pokken, you should know where to find me.

# Mods Overview
## This hardware being added in this guide consists of:
1. Modding a fight board in the controller
2. Adding Arduino into the controller
3. Adding LEDs to the controller
4. Custom Vinyl Skin

## The mods being added through this guide consists of:
* Allows using the controller for other consoles and whatever other benefits the the specific fight board used has
* Adding Home Button functionality
* Adding Capture Button functionality
* Adding visible RGB LEDs that are visible on the bottom of the controller (so the it looks **better** than the original LEDs supposed to be in the arcade Pokken controller)
* RGB LEDs can be made to react to when buttons are pressed
* Custom skin can be made to go on the front to better personalize your controller

## Requirements/Materials:
0. Hori Pokken Controller. Obviously you need the controller to do the mod. But if you want to get one do it on, I recommend getting a slightly used one on Ebay for like 25$ USD instead of getting new one for way more. Both the Wii U and the Switch versions of the Pokken controller (and any special editions I'm pretty sure) share the same PCB and shell, so the mod will work with any one. Can't say anything about the legit arcade controllers though.
1. **Soldering skills and equipment**. Unfortunately, soldering is required to do the things here. Soldering really isn't difficult, but if you really want to start doing it, I would recommend getting a **temperature controlled** soldering iron (I recommend the TS100 or a clone of it like the Sequre Mini SQ-001. *Note: you made need to get a power supply brick for these if you don't get these in a kit*), 63/37 **leaded** solder, good tweezers, and a practice board/something to practice on. You don't really need to worry about killing anything on the Pokken controller PCB, but the pads are small and can be annoying to work with. If you really want help with this, feel free to send me questions on the Pokken Community Discord.
2. Brook Fighting Board. This can be any one you want, but note the pros and cons of each.
* Brook Universal Fighting Board - [$95 USD on Amazon](https://www.amazon.com/Universal-Fighting-Compatible-preinstalled-Android-iOS/dp/B08H1TCFB1/) - Works on any console, but more expensive than the other options
* Brook PS4+ Audio Fighting Board - [$65 USD on Amazon](https://www.amazon.com/Fighting-Compatible-X-Input-Preinstalled-PlayStation-4/dp/B0713PGNHD/) - This is less expensive than the Universal board, but doesn't work on Xbox consoles. **This is the one I used in this guide.**
* Brook Zero-Pi Fighting Board - [$25 USD on Amazon](https://www.amazon.com/Fighting-Compatible-Emulator-Terminal-PlayStation-2/dp/B09637ZPSF/) - Cheapest board here, but it doesn't work with PS4 or Xbox consoles **and a bit more work needs to be done with the Arduino because this board is a little weird with how it does the select and capture inputs work**. It would work in theory though.
3. An Ardunio. The Arduino I used for this was the Arduino Pro Mini 328 5V/16Mhz which you can get for [$10 USD on Sparkfun](https://www.sparkfun.com/products/11113) HOWEVER, if you get it at this place, you will need to solder everything to it directly, or solder on and connect pin headers to it to make it work. You will also need something to program it like [an FTDI adapter for 15$ USD on Sparkfun](https://www.sparkfun.com/products/9716) if you go this route, or you can use jumpers and an Arduino Uno R3 to program it(I programmed mine using the Arduino Uno R3 method). I *think* something like the Arduino Pro Micro 5V/16Mhz for [18$ USD on Sparkfun](https://www.sparkfun.com/products/12640) would be better for this because you use a USB cable to program it instead. However, I don't know how it runs on 3.3V power (more on this later), so **I don't know how well it *actually* works because I didn't use this one**. The Arduino used for this ***WILL*** be run off of 3.3V. So if you want to get a 3.3V Arduino, go ahead. Honestly, that's probably the better option. Just note that you may also need to get a logic level shifter for [3$ on Sparkfun](https://www.sparkfun.com/products/12009) in order to run the LEDs from it as well.
4. Individually Addressable RGB LEDs. The ones I used you can find for [22$ on Amazon](https://www.amazon.com/BNTECHGO-Silicone-Flexible-Strands-Stranded/dp/B01MRG7EG5/). The LEDs you are looking for are WS2812b LEDs because they are individually addressable RGB LEDs that run off of 5V. We need them to run off of 5V because they are expected to be powered from the USB 2.0 ports on the Nintendo Switch dock. I got **144 LEDs per meter** because they're dense, so I can fit more if needed, I'll have more of them if/when I'll need them, I have backups in case I screw up and reverse polarity the inputs or something.
5. Wire. Since you're soldering, it's best to get silicone covered wire so you don't burn the cover off the wire when soldering. I got 30AWG wire for [~9$ USD from Amazon](https://www.amazon.com/BNTECHGO-Silicone-Flexible-Strands-Stranded/dp/B01MRG7EG5/?th=1). This wire can be used for all wiring in the controller that needs it.
6. Resistors*. You need these to connect the Arduino for the home/capture buttons. Resistors between 300/500 Ohms should work, such as these for [4$ USD on Amazon](https://www.amazon.com/HONGYETAJA-Resistor-Tolerance-Compliant-Complete/dp/B08TMN21DJ/).
7. Heat shrink and/or duct tape/electrical tape. This is used to cover wires/thing soldered together. You'll also need the tape to cover the Arduino. when putting it in the controller. If you want to get heat shrink, something like [this for 7$ USD on Amazon](https://www.amazon.com/Ginsco-580-pcs-Assorted-Sleeving/dp/B01MFA3OFA/) would definitely be fine, but you really just need heat shrink between 1/24" and 1/12"
8. Hot glue gun/Hot glue. You just need hot glue to cover things and stick things down where needed.
9. Tools. You need something like side cutters/flush cutters/whatever cutting tool and pliers because you're going to be chewing through some plastic in the controller. You also need a relatively small phillips head screwdriver to get into the controller and to the PCB. A multimeter is super nice to have for troubleshooting voltages and continuity, but *technically* not necessary, but I highly recommend having one.
10. Problem solving skills and perseverance because sometimes stuff just don't work. Sometimes you need to troubleshoot. That's just the nature of working with electronics sometimes. You also need to be careful that you don't short anything or reverse polarity anything.

\**Resistors only needed if you are NOT using a 3.3V Arduino and a logic level converter**

# Making the Controller
## Part 1: Adding in the fight board
### Preparing the Shell
1. First start by taking apart the controller, which is pretty straight forward. There are 7 screws on the outside and 2 screws holding the PCB on the inside.
Notes:
- The 2 inside screws are different from the 7 outside screws in length.
- There's no ribbon cables or anything you need to worry about.

![Pokken Controller Disassembly](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Disassembled%20Controller.jpg?raw=true)

2. Now, you need to test fit the board and chew through some of the plastic. To make it fit. Although Gum Mods has done this kind of mod before, they decided to remove material from the PCB to get it to fit instead. *BOI*. Why do that when you can just get that invisible rumble motor's plastic OUTTA here! Rumble motors were decided to not be implemented anyway. So, start chewing through the plastic. You need to chew through enough plastic to fit the board in.
Important Notes: 
* To make the Brook board fit better, I cut I removed all the pins near the bottom of the board.
* When putting it in, keep the Brook board a little bit offset to the right inside the controller. This is to leave room to put the Arduino on the left side of the controller.

![Brook board test fit](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Brook%20board%20test%20fit.jpg?raw=true)

3. Now, once that's done, onto what was the annoying part, for me at least: cutting out the plastic on the top. I was trying to keep as much rigidity as I could in the controller, so I was trying to cut everything just as much as needed, but after finishing, I don't think it mattered. The only plastic I didn't end up cutting out was the part right under L and R buttons. I left all the screws in except for the bottom screw. That hole needs to be filled in with something like hot glue so the internal boards aren't exposed.

![Top Plastic cutout](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Pokken%20Top%20of%20the%20PCB.jpg?raw=true)

### Connecting the Two Boards
4. First thing you need to do is move over the USB cable wires to the Brook board. To do this, I cut off the 4-pin plug part of the cable that came with the Brook board that was used to connect to the USB pins on the Brook Board. After that, I desoldered each wire from the Hori board **ONE AT A TIME** and soldered it to the corresponding wire from the plug. Use the diagram for the Brook board for this-you may need to check the manual- or the pin names may be written on the board. Make sure to put heat shrink around the wires before soldering! Otherwise, you may have to desolder the wires and solder them again to put it over, or you could just cover the solder joint with electrical/duct tape. Be extra careful and make sure that the 5V and GND wires and make sure that they're connected to the right pins! Checking this with a multimeter would be ideal. For the VSS wire, I just extended it with another wire from the Hori PCB so it would still connect to the same thing on the controller USB cord. I'm pretty sure that connecting this wire to the ground wire though would be just fine though.

![USB Wiring Change](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/USB%20Wiring%20Change.jpg?raw=true)

Then try connecting the Brook board to a PC. If it recognizes the board, congratulations, you did it right. If not, try switching the 2 middle pins (should be Data+ and Data-). When it's good and confirmed working, you can detach this so it's not in the way.

5. Now for the annoying part. Now you need solder all of the button wires to the pads on the PCB. To know what to solder the wires to, I've labeled all the pads that you will need to do everything here:

![Labeled Pokken Controller PCB](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Pokken%20Controller%20PCB%20Button%20Labels.png?raw=true)

First thing you will need to do is solder a wire to the GND pin in the middle, and connect that to any of the GND terminals on the Brook board. This is the common ground. Then, you will need to solder a wire to the pads for each button. Choose wire lengths that aren't too long, because you'll have a lot of excess wire, but don't make them too short because they won't reach. It's less annoying to make a wire shorter than it is to make it longer. **Refer to the manual for the specific Brook board used to figure out which pins to connect which wires to.** To connect the wires to the terminals, strip the end of the wire, and then put it in the terminal and screw the screw down on it until it's like, finger tight. It doesn't need to be super tight, like don't take a friken power drill to it. But don't keep them loose either. Give the wires a slight tug after the screws are tightened to see if they're good. If they don't move, you're solid!

![All wires connected](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Controller%20Wires%20Soldered.jpg?raw=true)

After soldering and connecting all the wires, there's still one more thing to do before you can put the Brook board into the controller- cover the metal piece/thing on the back of the Pokken Controller PCB with hot glue. You don't need this to be excessive, but cover it enough so no metal is exposed. This is to ensure that this won't short anything when putting the Brook board in the controller.

![Brook PCB back in the Pokken Controller](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Controller%20PCB%20in%20controller.jpg?raw=true)

6. Now you can put the Brook board in the controller! Fit it in the best you can, making sure to offset it a bit to the right so there is room for the Arduino on the left. Then, you can hot glue it in. I used a good amount of hot glue by the corners by the top two screw holes of the Brook board. ***Make sure that you don't cover the screws holding in the Hori PCB, and make sure you don't glue it to the plastic- you just want to hot glue it to the Hori PCB***.

![PCBs Hot Glued Together](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Side%20of%20boards%20hot%20glued.jpg?raw=true)

7. Now you can plug the USB plug back in and close up the controller if you want because this part is pretty much done! Make sure to plug it into the console and test to make sure all buttons are working properly and fix as needed.

8. (OPTIONAL) If you want to update the Brooks board to the latest firmware, first download the firmware updater for the specific board from the Brook website. Then, look at the manual for the specific board for which buttons to hold down when plugging it into the computer. You may need to press the Home button which is not setup yet to do this, but this isn't a problem. Just connect a wire from the home button terminal to any GND terminal, and then plug the controller in. After updating, you can remove this wire.

## Part 2: Adding in the Arduino
### Notes to keep in mind:
* The Brook fighting board buttons use 3.3V and the WS2812b LEDs use a 5V signal. To do both of these at once something jank needs to be done. This depends on the user and the Arduino used, and I can't say for certain what will work and what won't, so I'll just give some suggestions on what to do so users can choose their own path.
* Knock on wood, but what I did was I ran the 5V/16Mhz Arduino Pro Mini on 3.3V and somehow got away with controlling the LEDs *without* using a logic level converter ¯\_(ツ)_/¯. Will this work for other Arduinos? I have no idea.
* If you're worried about whether it will work or not with other Arduinos because of the jank setup, I would recommend using a 3.3V Arduino and getting a logic level converter to make absolutely sure that everything would work. If you use a 3.3V setup, you won't need the resistors on the pins for the buttons.

### Attaching the Home and Capture Buttons.
1. Attach the R, L, Start and Select buttons to the proper terminals on the Brooks board and wire them to the Arduino. Make sure to put the resistor between the Arduino and the Brooks board pins for the home and capture buttons. This is the wiring diagram for the setup I used. I just soldered wires to the Arduino and connected the other ends to the screw terminals on the Brooks board.
 
![Wiring diagram for home and capture buttons](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Arduino%20Home-Capture%20Wiring.jpg?raw=true)

### Connecting Other Buttons
2. Solder wires to pads for the other buttons you want attached to the Arduino, and you can wire them to the terminals for the corresponding button. Because some pads were acting weird for me (for some reason, IDK), I ended up wiring them like this:

![Wiring diagram for other buttons](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Arduino%20Button%20Wiring.jpg?raw=true)

If you want to wire them differently than pictured, you can change that in the code easily.

### Attaching the LEDs
#### Setting up LEDs to put into Controller
3. First thing I needed to do was get a surface to put the LEDs on in the controller. Resting them on the shell would leave them too far back, so I needed some kind of support. I decided to cut a hot glue stick with scissors and glue it to parts of the shell. This seemed to work out pretty well.

![Hot Glue Stick Structures](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Controller%20Inside%20Plastic%20and%20Glue%20Stick%20Supports.jpg?raw=true)

4. Now with the structure setup, we can prepare the LED strip itself. This will depend on what you want. If you get the super dense LEDs (the 144/meter) like I did, you can fit more LEDs in the controller. ***HOWEVER, you may not be able to use all of them, at least at the same time.*** The Nintendo Switch uses USB 2.0, meaning there is a max power output of 5V at 2.5W. At max power, WS2812b RGB LEDs use 0.3W per LED. This means that at full power, you could have at most, 8 white LEDs at max brightness constantly. But this doesn't include the power draw of the Brook board or the Arduino, so you're probably not able to get 8 LEDs like this. Despite this, I've found that I'm able to run 14 LEDs at continuously at ~3/5 their max brightness and still have everything working fine. When running just white, the voltage drops down to 4.1V on the input soooooo idk how much room that leaves you if you want to experiment with more.

Now that that's mentioned, I can talk about how I setup the LEDs. I cut a strip of 27 LEDs and divided them up into 3 parts: 2 strings of 7 (for the outsides) and 1 string of 13 (for the middle). I cut the strip of 27 into the lengths mentioned and wired them together with the 30AWG wire. Note that everything needs to be going in the same direction (make sure the arrows on the LEDs are all pointing the same direction). **BE CAREFUL THAT YOU DON'T REVERSE THE POLARITY OF THE 5V/GND WIRES OR YOU WILL KILL THE LEDS!!!** Next, I hot glued around the LEDs on the middle strip because that will be by electronics. This will help it not short. I also added some hot glue over the solder joints for the wires connecting the LEDs for A) making sure they don't come apart and B) make sure they don't short with anything else accidentally.

*[will add image soon]*

*This is an LED strip that I killed because I accidentally reverse the polarity on 5V/GND. I thought I needed bigger wires for connecting the LEDs, but after testing, the 30AWG works perfectly fine and is far less of a pain in the butt to solder on and deal with. Note the hot glue covering the solder joints and the parts around the LEDs in the middle and notice all the arrows are pointing int the same direction.*

#### Connecting LEDs to Arduino
5. With the LEDs wired together, you can wire it to everything else. Connect the LEDs 5V wire to the USB and the GND to any GND terminal. The Brook board should have terminals for these. Again, **BE CAREFUL THAT YOU DON'T REVERSE THE POLARITY OF THE 5V/GND WIRES OR YOU WILL KILL THE LEDS!!!!** Then you can connect the middle data pin of the first LED (i.e. at the end *opposite* of the one which the arrows are pointing) to the Arduino. Make sure to put a 300-500 ohm resistor on the LED wire. If using a logic level converter, put the resister on the 5V side which goes to the LEDs.

Normal:
![Wiring diagram for LEDs](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Arduino%20LED%20Wiring.jpg?raw=true)

Logic Level Converter:
![Wiring diagram for LEDs with LLC](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Arduino%20LED%20Wiring%20Logic%20Level%20Converter.jpg?raw=true)

6. Now you can put the LEDs inside the controller. From the perspective of the controller from the back (i.e. you're looking at the electronics inside), I have the LEDs starting from the right side of the controller, and going to the left. I have the middle LEDs hot-glued to the bottom of the Brook board with it as flat as possible against it. You can sandwich it in between the Brook board and the Hori board if you want though. You will need to unscrew the Hori board and take it out regardless to do it either way. If you want, you can put it on top of the Brook board, but I think putting the middle LED strip under the Brook board looks better.

![Controller Internals](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Pokken%20Controller%20Internals.jpg?raw=true)

### Programming the Arduino
7. Now you can program the Arduino. First, you will need to download the Arduino IDE, which you can Google or get to [here](https://www.arduino.cc/en/software). After it's downloaded, open the IDE, and you should have a basic program there.

8. Now, you can add code to the Arduino. If you want to use my code for both the LEDs and the Home/Capture button functionality, you can get it from [here](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/ControllerSketch/ControllerSketch.ino). Copy and paste all of it into the Arduino IDE, replacing whatever is already there. *If you wired the controller buttons to different pins on the Arduino than what is in the diagram, change that at the top of the code (code shown below). Just change the corresponding button label to the one you set.*

![Arduino Code Image](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Arduino%20Button%20Mapping%20Code.jpg?raw=true)

9. Now you can program the Arduino. This is going to depend on the Arduino used for how to setup the IDE to program it. Google this for your specific Arduino. When flashing and testing, make sure the Arduino doesn't touch and short anything on the Brook board accidentally. ***BEFORE CONNECTING THE ARDUINO TO THE USB OR WHATEVER TO PROGRAM IT, MAKE SURE THE POWER IS DISCONNECTED FROM THE BROOK BOARD!!! TO DO THIS, JUST MAKE SURE THE VCC OR 5V WIRE CONNECTING THE ARDUINO TO THE BROOK BOARD IS DISCONNECTED FROM ONE OF THEM. THIS IS TO BE SAFE AND MAKE SURE THAT NOTHING BAD HAPPENS TO THE BROOK BOARD WHEN FLASHING!!*** 

10. Once everything is flashed, you can test it by removing the flashing device/wires/whatever and connecting the Arduino back the Brook board again and plugging in the USB. Everything should work. If not, you can troubleshoot by checking:
- Check wiring
- Check pins/pin mapping in the code
- Check that proper power is getting to everything.

11. And that should be it once it's working! Then you can start assembling everything again to go back in the controller. I covered the Arduino in duct tape/electrical tape to keep it isolated from the Brook board. If you're adding a vinyl skin to the front go to part 4 and do that before doing the final assembly tips (next step).

12. Final assembly tips: 
- Make sure there's nothing really loose in the shell and that everything is held pretty securely. It would suck to have to open it up again if something were to come loose or stop working.
- Make sure that no wires are visible in the clear panel on the bottom of the controller. Keeps it looking clean.
- I hot glued the Arduino/the tape holding it/whatever to things at 3 different points to keep it still. I didn't use too much hot glue to hold it down- just 3 dabs on 3 corners was needed.
- **If you have to use force to put the back of the shell on, something is colliding/in the way of closing it. Find out what it is and fix it.** Crushing things like wires is a pain in the butt to fix.
- Make sure all buttons are responding properly and triggering LEDs properly closing.

## Part 4: Custom Vinyl Skin
This "mod" is separate from the other mods. You don't need any soldering skills. However, **you do need access to a vinyl cutter**.

For anyone that wants to go do this on your own or whatever, here are the dimensions that I found for the controller cover:

Without Constraints:

![Dimensions Without Constriants](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Controller%20Dimensions.jpg?raw=true)

With Constraints:

![Dimensions With Constriants](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Controller%20Dimensions%20With%20Constraints.jpg?raw=true)

1. Determine the image to use. Find or obtain an image that you want to use for the skin. Here's an image of the controller with no background and no face plate if you want to easily photoshop or set something up on Microsoft Powerpoint or whatever for an image to see what it would look like on the controller.

![Pokken Controller Clear Cover](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Pokken%20Controller%20No%20Background.png?raw=true)

2. Determine image size and scale. First, you will need to get the size of the image. With that, you will be able to scale the image to an actual size using it's resolution ratio. The part of the controller that the skin will take up is 4.8"x2.2". The image **DOES NOT** need to fit this ratio- just the part of the image you want as the skin needs to fit these dimensions. If the image is part of a gigantic image, you could crop it. Having an image size matching exactly one of these dimensions is ideal. To do this,you need to do some ratio math:

- Find the nominal dimension you want to use of the image, and use then use the corresponding length or width dimension as a known value.
- The printed image would need to have dimensions being at least 4.8"x2.2". After finding the nominal dimension on the image, you need to find the proper size ratio for the unknown dimension using ratio math (cross multiplying). 

For an example, the image I used is longer than it is wide, with the dimensions 5142x 3000. Since I want most of the image on the skin lengthwise, I use the **longer dimension** of the image, the **length** dimension (5142), as the nominal dimension. Since I'm using the image's **length** dimension as the nominal dimension, I know that it will correspond to the minimum length that needs to be printed: 4.8". I want to keep the same aspect ratio, but I don't know the width the image would be printed at, W.

To find W, I setup the ratio math: (5142/3000) = (4.8/W)

Now you can solve for W through cross multiplying: 5142*W = 4.8*3000 --> W = (4.8*3000)/5142 = 2.8

Now, I know that I will need to have the image printed with the sizes 4.8"x2.8" when I get it printed on vinyl.

***To test final dimensions, you can set the image size and in Microsoft Powerpoint or Google Slides and print it out. You can use that image as a kind of test fit and see if it will work.***

3. Get the image printed on vinyl. Once you have the dimensions of the image, you need to get it printed on vinyl to those exact dimensions. I got mine printed as a [rectangle stickers from Stickermule.com](https://www.stickermule.com/products/rectangle-stickers). I put in the custom size (4.8"x2.8" from the example), and set the amount of stickers to 10 (the minimum allowed to be printed). I was able to get everything for 21$.

4. Cut the image from the vinyl. Since you get 10 stickers, there's some leeway with cutting it because you have multiple tries. Load up the SVG or DXF or whatever to the vinyl cutter's software and cut it. This will depend on the software and the vinyl cutter. ***When importing the controller cover cut shape to the vinyl cutter's you want to ensure the size of the enclosing box is 4.724"x2.126". This is shown below.*** Then, go a head and cut it. **I strongly recommend cutting a test piece before cutting an image to get the image lined up properly for the cut.** I used 2 rulers to line the rectangle up properly after cutting the test piece. (You can find the files [here](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/tree/main/Images/Controller%20Cover))

![Vinyl Cutter Enclosing Box](https://github.com/jackdeath5/CustomPokkenControllerArduinoCode/blob/main/Images/Vinyl%20Cutter%20Enclosing%20BoxSize.jpg?raw=true)

5. Stick it on carefully. Make sure everything is lined up with the buttons when putting it on. I started by lining it up with the 4 middle buttons and the bottom, and then made sure to line up with the 4 buttons on the right. Then everything seemed to line up with the D-pad.

6. This last step is optional, but I ***STRONGLY*** recommend it. Clear coat the top of the controller with the vinyl on it. First, get some clear coat. I would recommend getting something like [Spray Max 2k High Gloss](https://www.amazon.com/Spray-Refinishing-Permanent-Surfaces-3680061/dp/B0043B7UQY/) for 22$. Even though it's more expensive then some other things out there, I'd say it's definitely worth it. As mentioned [on a r/customcontrollers reddit thread](https://www.reddit.com/r/customcontrollers/comments/583jv1/how_to_paint_a_custom_controller_a_step_by_step/), "It's essentially the difference between [https://imgur.com/a/pDE3r](https://imgur.com/a/pDE3r) and [https://imgur.com/a/dhs8h](https://imgur.com/a/pDE3r)." To clear coat it, you'll need to take the the controller apart to get to the front of the controller. Then, I covered everything but the top part with the vinyl and the outline of the top of the controller with painter's tape, and then just applied clear coat. You can look up how to get an even coat on Youtube if you need help with this. I also clear coated the buttons, because the labels are painted on the buttons and I wanted to protect that. I covered the sides of the buttons in painter's tape to make sure the clear coat wouldn't affect the size of the buttons. I did not clear coat the D-pad. I then applied 3-4 layers of clear coat to the controller and then waited about a day or two to let everything dry. Keep drying shell in a place where little to no dust or particles will get on it.

7. Reassemble the controller and boom! Super fancy, super smooth and super nice looking controller.
