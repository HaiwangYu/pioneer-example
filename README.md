# a collection of sbnd examples

## simulation

```
g4pienux example.mac
```

## detector response
```
./run Gaudi test_der.cfg
```

## rec
```
./run Gaudi test_rec.cfg
```

## plot gdml

```C++
void draw_gdml(char* name = "PEN_Calo_Hybrid.gdml") {
    gSystem->Load("libGeom");
    TGeoManager::Import(name);
    TGeoVolume *top = gGeoManager->GetTopVolume();
    top->Draw();
}
