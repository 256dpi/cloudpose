#!/usr/bin/env bash

# Usage: ./connect.sh <ip>

ssh -i ec2.pem "ubuntu@$1"
