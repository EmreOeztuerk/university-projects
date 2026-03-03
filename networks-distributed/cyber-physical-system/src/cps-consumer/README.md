# Cyber-Physical Systems Software Architecture Docker Template Consumer Extension

This extension adds energy consumers to the base template. The consumption amount is based on BDEW standard load profiles and dynamic scaling factors.


## Project Organization
Please start the base template first (MQTT broker and tick generator), then this extension.

```
.
├── docs                - documents used in README.md
├── README.md           - this file
├── configs
│   └── slp.csv           - load profiles consumers
├── scripts
│   ├── build.sh        - builds custom images                      (step 1)
│   ├── observe.sh      - opens the dashboard URL                   (step 3a)
│   ├── run.sh          - creates containers                        (step 2)
│   ├── stop.sh         - stops containers                          (step 4)
│   └── subscribe.sh    - subscribes to all topics of mqttbroker    (step 3b)
└── src
    └── gen_energy_consumer - a generic energy comsuner
```




## Energy Consumer

A generic consumer based on BDEW standard load profiles. Configuration is done via environment variables.


### Example Consumers

After running `run.sh`, four example consumers will start:

 - `consumer/housingcomplex/1` - residential building (H0 profile)
 - `consumer/g0/1` - general commercial
 - `consumer/g1/1` - office/administration
 - `consumer/g5/1` - bakery

### Configuration parameters

  - `EC_NAME`: BDEW profile type (H0, G0-G6, L0-L2)

 - `EC_BASE_DEMAND`: Base annual consumption per unit (kWh)

 - `EC_POPULATION_FACTOR`: Multiplier for consumption units



They all get their basic consumption from the standard load profiles in 'slp.csv'. The _slp_ is based on [this examples](https://www.bdew.de/energie/standardlastprofile-strom/), but calculated to changes per 30 seconds. There are some more profiles for different companies, as they can be found in the reference.

You can configure the demand of power via the `run.sh`.
The demand of power is mainly based on the `slp_data` and the `POPULATION_FACTOR`. The population factor is to distinguish complexes with different amount of residents, or to describe business with a higher demand.



## MQTT Topics

### gen_energy_consumer

**subscribes**:

- `tickspeed/tick`: 15-minute time synchronization
- `consumer/{{TYPE}}/{{NUMBER}}/scale`: Dynamic scaling factor

**publishes**:

- `consumer/{{TYPE}}/{{NUMBER}}/demand`: Current power demand calculated as: `(BDEW_profile_value × (1 + random_offset)) × scale_factor`





