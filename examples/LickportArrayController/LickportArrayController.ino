#include <LickportArrayController.h>


LickportArrayController dev;

void setup()
{
  dev.setup();
  dev.startServer();
}

void loop()
{
  dev.update();
}
