# Smart-Parking
An automated smart parking barrier system, featuring a dynamic day/night tariff engine, non-blocking FSM, ultrasonic detection, and OLED status display.

## List of Features

- **Automated Barrier Actuation:** Controlled angular motion for barrier arm opening and closing via pulse-width modulation. Non-blocking trajectory and timing execution without stalling main system loops.

- **Proximity & Vehicle Detection:** Real-time non-contact distance measurement for vehicle approach detection, including input signal conditioning to match microcontroller logic levels and software filtering to prevent false triggers.

- **Safety Clearance Interlock (Failsafe Mechanism):** Clearance-based closure ensuring the barrier descends only after a vehicle completely leaves the entrance zone, paired with an anti-crush safety routine that halts movement and reopens the arm if an obstruction is detected during descent.

- **Dynamic Time-Based Tariff Engine:** Accelerated virtual clock allowing rapid demonstration of full day-to-night operating cycles, featuring a dual-rate billing scheme with standard day peak rates and reduced off-peak night rates.

- **Parking Capacity & Occupancy Tracking:** In-memory database tracking occupied spots, arrival timestamps, and live availability, with an automatic full-capacity lockout preventing entry when all spots are taken.

- **Live Status Display & Checkout Telemetry:** Digital display interface presenting system status in real time (virtual time, active tariff tier, remaining free spots, and barrier movement) along with an exit summary view showing total parked duration and the calculated fee.

- **Event-Driven Non-Blocking Architecture:** Central Finite State Machine (FSM) coordinating sensor inputs, display updates, fee calculation, and motor actuation using a periodic non-blocking hardware time base.
