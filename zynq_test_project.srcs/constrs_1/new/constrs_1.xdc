## Regular LEDs
set_property -dict { PACKAGE_PIN R14 IOSTANDARD LVCMOS33 } [get_ports { leds_7bits_tri_o[0] }]
set_property -dict { PACKAGE_PIN P14 IOSTANDARD LVCMOS33 } [get_ports { leds_7bits_tri_o[1] }]
set_property -dict { PACKAGE_PIN N16 IOSTANDARD LVCMOS33 } [get_ports { leds_7bits_tri_o[2] }]
set_property -dict { PACKAGE_PIN M14 IOSTANDARD LVCMOS33 } [get_ports { leds_7bits_tri_o[3] }]

## RGB LED LD4
set_property -dict { PACKAGE_PIN N15 IOSTANDARD LVCMOS33 } [get_ports { leds_7bits_tri_o[4] }]
set_property -dict { PACKAGE_PIN G17 IOSTANDARD LVCMOS33 } [get_ports { leds_7bits_tri_o[5] }]
set_property -dict { PACKAGE_PIN L15 IOSTANDARD LVCMOS33 } [get_ports { leds_7bits_tri_o[6] }]