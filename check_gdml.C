void check_gdml(const char* name = "/pioneer/main/geometry/generator/gdml/test_output.gdml") {
    gSystem->Load("libGeom");
    TGeoManager::Import(name);
    TGeoVolume *top = gGeoManager->GetTopVolume();
    top->Draw();
}
