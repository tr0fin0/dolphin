Architecture
============

The hardware design architecture is described with `Mermaid <https://mermaid.js.org/intro/>`_ as follows:

.. mermaid::
    :align: center
    :caption: Hardware Architecture
    :zoom:

    ---
    config:
        theme: neutral
        flowchart:
            curve: basis
    ---
    flowchart LR
        %% style
        classDef component_3V3 fill:#ffffff, stroke:#00aa00, stroke-width:2px, color:#000
        classDef component_5V0 fill:#ffffff, stroke:#ffaa00, stroke-width:2px, color:#000
        classDef component_VBT fill:#ffffff, stroke:#aa0000, stroke-width:2px, color:#000

        style MAINBOARD     fill:#fff, stroke:#000, stroke-width:1.25px
        style POWER         fill:#fff, stroke:#000, stroke-width:1.25px
        style L_MOTOR       fill:#fff, stroke:#000, stroke-width:1.25px
        style R_MOTOR       fill:#fff, stroke:#000, stroke-width:1.25px
        style COMMUNICATION fill:#fff, stroke:#000, stroke-width:1.25px
        style SENSORS       fill:#fff, stroke:#000, stroke-width:1.25px

        linkStyle default stroke:#000000, color:#000000, stroke-width:1px;

        %% nodes
        subgraph MAINBOARD ["mainboard"]
            ESP("ESP32-S3"):::component_3V3
            MUX("mulplexer"):::component_5V0
            LEV("level shifter"):::component_3V3

            subgraph POWER ["power"]
                MINI360("MINI360"):::component_5V0
                KF33BD("KF33BD"):::component_3V3

                MINI360 ==>|"5V"| KF33BD
            end

            MUX    --> LEV
            LEV    --> ESP

            KF33BD ==>|"3V3"| ESP
        end

        PWR("external power source"):::component_VBT
        PWR ==>|"1S - 6S"| MINI360

        USB("USB-C connector"):::component_5V0
        USB --->|"DATA"| ESP
        USB ===>|"5V"| KF33BD

        subgraph L_MOTOR ["L motor"]
            ENC_L("encoder"):::component_3V3
            ESC_L("ESC"):::component_5V0

            ENC_L ---->|"1x PWM"| ESP
            ESP   -->  |"1x PWM"| ESC_L
        end

        LEDS("LED array"):::component_3V3
        ESP -->|"1x RTM"| LEDS

        CONN("12x extra connectors"):::component_5V0
        ESP -->|"1x RTM"| CONN

        subgraph R_MOTOR ["R motor"]
            ENC_R("encoder"):::component_3V3
            ESC_R("ESC"):::component_5V0

            ENC_R ---->|"1x PWM"| ESP
            ESP   -->  |"1x PWM"| ESC_R
        end

        subgraph COMMUNICATION ["communication"]
            RADIO("2.4 GHz radio receiver"):::component_5V0
            IR("infrared receiver"):::component_5V0

            RADIO --->|"6x PWM"| LEV
            IR    --->|"1x IO" | LEV
        end

        subgraph SENSORS ["sensors"]
            ODS("12x obstacle sensors"):::component_5V0
            QRE("4x line sensors"):::component_5V0

            ODS -->|"12x IO"| MUX
            QRE -->|"4x IO" | MUX
        end

        linkStyle 0,6 stroke:#ffaa00, color:#000000, stroke-width:4px;
        linkStyle 3   stroke:#00aa00, color:#000000, stroke-width:4px;
        linkStyle 4   stroke:#aa0000, color:#000000, stroke-width:4px;

.. note::

    Only the main connections are described, check the board pages for details.
