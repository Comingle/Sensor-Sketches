void loop() {
  int out0 = map(Toy.getInput(0), 0, 1023, 0, 255);
  int out1 = map(Toy.getInput(1), 0, 1023, 0, 255);
  Toy.setOutput(0, out0);
  Toy.setOutput(1, out1);
  delay(100);
}
