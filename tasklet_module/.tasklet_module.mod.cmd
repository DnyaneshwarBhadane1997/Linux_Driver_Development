cmd_/home/kandpal/dnya_training/l_codes/linux-driver-training/tasklet_module/tasklet_module.mod := printf '%s\n'   tasklet_module.o | awk '!x[$$0]++ { print("/home/kandpal/dnya_training/l_codes/linux-driver-training/tasklet_module/"$$0) }' > /home/kandpal/dnya_training/l_codes/linux-driver-training/tasklet_module/tasklet_module.mod