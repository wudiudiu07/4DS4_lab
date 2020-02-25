# TCL File Generated by Component Editor 9.0sp1
# Mon Jan 25 12:05:37 EST 2010
# DO NOT MODIFY


# +-----------------------------------
# | 
# | PS2_controller_component "PS2_controller_component" v1.0
# | Adam Kinsman, Henry Ko and Nicola Nicolici from McMaster University, Canada 2010.01.25.12:05:37
# | 
# | 
# | C:/Courses/coe4ds4/2010/Test_release/coe4ds4_lab3/experiment2/PS2_controller_component/PS2_controller_component.sv
# | 
# |    ./PS2_controller.sv syn, sim
# |    ./PS2_controller_component.sv syn, sim
# | 
# +-----------------------------------


# +-----------------------------------
# | module PS2_controller_component
# | 
set_module_property DESCRIPTION ""
set_module_property NAME PS2_controller_component
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property GROUP coe4ds4
set_module_property AUTHOR "Adam Kinsman, Henry Ko and Nicola Nicolici from McMaster University, Canada"
set_module_property DISPLAY_NAME PS2_controller_component
set_module_property TOP_LEVEL_HDL_FILE PS2_controller_component.sv
set_module_property TOP_LEVEL_HDL_MODULE PS2_controller_component
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
# | 
# +-----------------------------------

# +-----------------------------------
# | files
# | 
add_file PS2_controller.sv {SYNTHESIS SIMULATION}
add_file PS2_controller_component.sv {SYNTHESIS SIMULATION}
# | 
# +-----------------------------------

# +-----------------------------------
# | parameters
# | 
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point avalon_slave_0
# | 
add_interface avalon_slave_0 avalon end
set_interface_property avalon_slave_0 addressAlignment DYNAMIC
set_interface_property avalon_slave_0 bridgesToMaster ""
set_interface_property avalon_slave_0 burstOnBurstBoundariesOnly false
set_interface_property avalon_slave_0 holdTime 0
set_interface_property avalon_slave_0 isMemoryDevice false
set_interface_property avalon_slave_0 isNonVolatileStorage false
set_interface_property avalon_slave_0 linewrapBursts false
set_interface_property avalon_slave_0 maximumPendingReadTransactions 0
set_interface_property avalon_slave_0 printableDevice false
set_interface_property avalon_slave_0 readLatency 0
set_interface_property avalon_slave_0 readWaitTime 1
set_interface_property avalon_slave_0 setupTime 0
set_interface_property avalon_slave_0 timingUnits Cycles
set_interface_property avalon_slave_0 writeWaitTime 0

set_interface_property avalon_slave_0 ASSOCIATED_CLOCK clock_reset
set_interface_property avalon_slave_0 ENABLED true

add_interface_port avalon_slave_0 address address Input 1
add_interface_port avalon_slave_0 chipselect chipselect Input 1
add_interface_port avalon_slave_0 read read Input 1
add_interface_port avalon_slave_0 write write Input 1
add_interface_port avalon_slave_0 readdata readdata Output 32
add_interface_port avalon_slave_0 writedata writedata Input 32
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point clock_reset
# | 
add_interface clock_reset clock end
set_interface_property clock_reset ptfSchematicName ""

set_interface_property clock_reset ENABLED true

add_interface_port clock_reset clock clk Input 1
add_interface_port clock_reset resetn reset_n Input 1
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point interrupt_sender
# | 
add_interface interrupt_sender interrupt end
set_interface_property interrupt_sender associatedAddressablePoint avalon_slave_0

set_interface_property interrupt_sender ASSOCIATED_CLOCK clock_reset
set_interface_property interrupt_sender ENABLED true

add_interface_port interrupt_sender PS2_code_ready_irq irq Output 1
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point conduit_end
# | 
add_interface conduit_end conduit end

set_interface_property conduit_end ENABLED true

add_interface_port conduit_end PS2_data export Input 1
add_interface_port conduit_end PS2_clock export Input 1
# | 
# +-----------------------------------
