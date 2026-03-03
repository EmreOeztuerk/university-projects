#!/bin/bash

# Run the wind turbine container
docker run -d \
    --name wind-turbine \
    --network cps-net \
    cps-wind-turbine:0.1 