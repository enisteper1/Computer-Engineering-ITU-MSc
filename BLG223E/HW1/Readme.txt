Because of the building the code exceeds timeout (2 seconds) it exits with error
In order to eliminate it I had to run command as below:
calico testcalico.yaml --debug --timeout 5