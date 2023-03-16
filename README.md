# a collection of sbnd examples

## simulation

```
g4pienux test_sim.mac
```

## detector response

The file generated above seems to break the DER. So use the sim.root for now.
```
run Gaudi test_der.cfg
```

## rec
```
run Gaudi test_rec.cfg
```
