# ARES-ROVER
A Semi Autonomous, Wi-Fi controlled smart rover designed to learn about robotics, embedded systems, telemetry systems, and autonomous behavior with an ESP32 microcontroller.

I made this project as a sort of robotics learning project. Since it uses motors, battery system, microcontrollers and motor drivers, as well as some sensors and the autonomous behaviour behind it, it would a very useful project for this reason.

# IMAGES

<img width="471" height="489" alt="Screenshot 2026-06-12 094204" src="https://github.com/user-attachments/assets/ca1dcd74-15f3-494b-97d9-f12840f9b1c9" />

<img width="771" height="517" alt="Screenshot 2026-06-12 094148" src="https://github.com/user-attachments/assets/f8c5185d-47ee-4759-b9b5-7258bf93c4c4" />

<img width="609" height="546" alt="Screenshot 2026-06-12 094045" src="https://github.com/user-attachments/assets/cbd918a1-48e6-4c41-a072-e6c3f0f6b823" />

# FULL CAD MODEL

<img width="699" height="485" alt="Screenshot 2026-06-10 202117" src="https://github.com/user-attachments/assets/4d9978ef-7457-49c8-8533-e45f1425bb0c" />

# WIRING DIAGRAM (ABSTRACTION)

<img width="522" height="447" alt="Screenshot 2026-06-05 142831" src="https://github.com/user-attachments/assets/01b8742f-cee1-4bbd-9abb-d42d40e4ccf2" />

# BILL OF MATERIALS
[ARES ROVER-bom.csv](https://github.com/user-attachments/files/28873271/ARES.ROVER-bom.csv)

| Name | Purpose | Quantity | Total Cost (USD) | Link | Distributor |
|---:|---|---|---|---|---:|
| ESP32 38 PIN BOARD | Main controller for the entire rover system | 1 | 5.20 | https://www.aliexpress.com/item/1005005655238798.html | Aliexpress |
| MPU6050 GY-521 6DOF IMU Module | Motion Sensing, acceleration and tilt tracking (additional rover awareness) | 1 | 6.00 | https://www.aliexpress.com/item/1005010057794277.html | AliExpress |
| Rosin Core Solder Wire | For making the electrical connections | 1 | 5.00 | https://www.aliexpress.com/item/1005007053733373.html | Aliexpress |
| Double Sided tape | Holding together of some componets | 1 | 3.00 | https://www.aliexpress.com/item/1005009222733373.html | Aliexpress |
| 3d printed chassis | The body of the rover that holds all other components | 1 | 12.00 | https://stasis.hackclub.com/dashboard/projects/cmp0sdeow004g01mrhgvder89 | printing-legion |
| Heat Shrink Sleeves pack | Insulating soldered electrical joints | 1 | 4.00 | https://www.aliexpress.com/item/1005009535285865.html | Aliexpress |
| Rocker Switch 10pcs pack | Power ON/OFF | 1 | 1.50 | https://www.aliexpress.com/item/4000973563250.html | AliExpress |
| LM2596 Buck Converter | Provides stable voltage for the electronics | 1 | 2.00 | https://www.aliexpress.com/item/1005007055625007.html | Aliexpress |
| 2S BMS Protection Board | Protects the lithium battery system | 1 | 4.00 | https://www.aliexpress.com/item/1005003654327044.html | AliExpress |
| 2S Battery Holder | Holding the batteries in series | 1 | 3.60 | https://www.aliexpress.com/item/1005006647556237.html | AliExpress |
| INR 30Q 18650 Rechargeable Lithium Battery pack + Charger | Power supply for the Rover | 1 | 23.00 | https://www.aliexpress.com/item/1005007906925735.html | AliExpress |
| TT Motors with encoder and Wheels | Rover movement and speed and distance measurement | 4 | 30.00 | https://www.aliexpress.com/item/1005005737025799.html | AliExpress |
| TB6612 DRV8833 Dual Motor Driver | Controls the rovers motors safely | 1 | 3.00 | https://www.aliexpress.com/p/shoppingcart/index.html | AliExpress |
