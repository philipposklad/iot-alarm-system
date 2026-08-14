# Bill of Materials

The following components were used to construct the IoT Alarm System prototype.

The project was designed around inexpensive and widely available electronic components, with the goal of maintaining a low overall implementation cost.

| Component | Purpose |
|---|---|
| NodeMCU ESP8266 | Main microcontroller, system control and Wi-Fi connectivity |
| LM317 Adjustable Voltage Regulator | Regulation of the main supply voltage to approximately 3.3 V for the NodeMCU |
| Optocouplers | Electrical isolation between the NodeMCU and external alarm circuitry |
| Transistors | Switching and control of external 12 V loads |
| Resistors | Voltage regulation, biasing and keypad voltage-divider network |
| Capacitors | LM317 regulator stabilization and filtering |
| 3×4 Matrix Keypad | Local alarm control and access-code input |
| Alarm Sensors | Detection of security events |
| Alarm Siren | Main audible alarm output |
| Buzzer | Pre-alarm / local audible indication |
| 12 V Power Supply | Main system power source |
| Gel Battery | Backup power source |
| Prototype Circuit Board | Assembly and interconnection of electronic components |
| Electrical Enclosure | Housing and protection of the implemented system |

## Power Regulation Components

The LM317 regulator circuit was implemented using:

| Component | Value |
|---|---:|
| R1 | 220 Ω |
| R2 | 352 Ω (330 Ω + 22 Ω) |
| C1 | 0.1 μF |
| C2 | 1 μF |

The implemented regulator produced a measured output of approximately **3.33 V** for the NodeMCU supply.

## Cost

The system was intentionally designed using low-cost, commercially available components.

The estimated total hardware cost of the implemented prototype was approximately **€40–50**, depending on component sourcing and sensor selection.

> Prices represent approximate costs during the development of the original prototype and are included only as a reference. Current component prices may differ.

## Notes

The exact sensor configuration can be adapted depending on the installation requirements. The architecture was designed to support common alarm sensors and 12 V peripheral devices without requiring fundamental changes to the main controller.