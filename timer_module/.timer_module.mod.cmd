cmd_/home/kandpal/dnya_training/l_codes/linux-driver-training/timer_module/timer_module.mod := printf '%s\n'   timer_module.o | awk '!x[$$0]++ { print("/home/kandpal/dnya_training/l_codes/linux-driver-training/timer_module/"$$0) }' > /home/kandpal/dnya_training/l_codes/linux-driver-training/timer_module/timer_module.mod