cmd_/home/kandpal/dnya_training/l_codes/linux-driver-training/shared_q_module/Module.symvers := sed 's/ko$$/o/' /home/kandpal/dnya_training/l_codes/linux-driver-training/shared_q_module/modules.order | scripts/mod/modpost    -o /home/kandpal/dnya_training/l_codes/linux-driver-training/shared_q_module/Module.symvers -e -i Module.symvers   -T -