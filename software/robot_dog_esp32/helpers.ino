double radToDeg(double angleRad)
{
  return angleRad*RADTODEG;
}

double degToRad(double angleDeg)
{
  return angleDeg*DEGTORAD;
}

double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
