# Arduino ADC Voltage-Range Indicator (Potentiometer + 3 LEDs)

This project uses the Arduino ADC to read a variable analog voltage from a potentiometer-based divider and lights one LED based on the measured voltage range:

- **Yellow ON** when input voltage is **< 2.5 V**
- **Green ON** when input voltage is **2.5 V to 3.5 V**
- **Red ON** when input voltage is **> 3.5 V**

## 1) Circuit design

### Components

- 1 × Arduino Uno (or compatible, 5 V ADC reference)
- 1 × 10 kΩ potentiometer
- 1 × 15 kΩ resistor (Rtop)
- 1 × 10 kΩ resistor (Rbottom)
- 3 × LEDs (yellow, green, red)
- 3 × 220 Ω resistors (current limiting for LEDs)
- Breadboard and jumper wires

### Potentiometer divider that outputs ~1.5 V to ~4.0 V

To keep the analog signal in the required range, bias the potentiometer with a resistor network:

- Connect **Rtop = 15 kΩ** from **+5 V** to node **VH**.
- Connect **Rbottom = 10 kΩ** from node **VH** to **GND**.
- Connect potentiometer terminal **A** to **VH**.
- Connect potentiometer terminal **B** to **GND**.
- Connect potentiometer **wiper** to **A0**.

This makes terminal A approximately:

\[
V_H = 5 \times \frac{10k}{15k + 10k} = 2.0\,V
\]

As the wiper moves from GND to terminal A, the input to A0 varies from near **0 V to 2 V** if wired exactly as above. To satisfy the requested **1.5 V to 4 V** output span, wire the potentiometer as a divider between two fixed rails:

- Keep **Rtop = 15 kΩ** from +5 V to node **VHI ≈ 4.0 V**
- Keep **Rbottom = 10 kΩ** from node **VLO ≈ 1.5 V** to GND
- Pot terminal A to **VHI**, terminal B to **VLO**, and wiper to **A0**

A practical way on a breadboard is:

1. Build node **VHI** with a divider from 5 V to GND using **5.1 kΩ (top)** and **20 kΩ (bottom)** (or nearest available values) giving about 4.0 V.
2. Build node **VLO** with a divider from 5 V to GND using **10 kΩ (top)** and **4.3 kΩ (bottom)** giving about 1.5 V.
3. Put the potentiometer between VHI and VLO; the wiper then sweeps roughly **1.5 V to 4.0 V**.

> If your lab kit has only one potentiometer plus one/two resistors, an easier implementation is full 0–5 V sweep and enforce thresholds in software. The provided code still works exactly with 2.5 V and 3.5 V boundaries.

### LED outputs

- Arduino **D8** → 220 Ω → **Yellow LED anode**, LED cathode → GND
- Arduino **D9** → 220 Ω → **Green LED anode**, LED cathode → GND
- Arduino **D10** → 220 Ω → **Red LED anode**, LED cathode → GND

Only one LED is ON at a time.

## 2) ADC thresholds and mapping

For Arduino Uno (10-bit ADC, 0..1023, 5 V reference):

\[
\text{ADC count} = \frac{V_{in}}{5.0} \times 1023
\]

- 2.5 V → about **512**
- 3.5 V → about **716**

Decision logic:

- `adc < 512` → Yellow ON
- `512 <= adc <= 716` → Green ON
- `adc > 716` → Red ON

## 3) Program and upload

1. Open `arduino_voltage_indicator.ino` in Arduino IDE.
2. Select board and COM port.
3. Upload sketch.
4. Rotate potentiometer and verify color changes at the threshold voltages.

## 4) Notes for implementation on proto-board

- Ensure Arduino GND and divider/LED GND are common.
- Measure A0 with a multimeter while turning the potentiometer to confirm range.
- If LED turns on in the opposite range, check LED polarity and wiring.
- If thresholds are slightly off, adjust `VREF` in code (e.g., 4.95 V if measured).
