#pragma once

class Burger {
private:
  bool prepared;
  bool grilled;
  bool sauced;
  bool packed;

public:
  Burger();

  void setPrepared(bool value);
  void setGrilled(bool value);
  void setSauced(bool value);
  void setPacked(bool value);

  bool isPrepared() const;
  bool isGrilled() const;
  bool isSauced() const;
  bool isPacked() const;

  bool isComplete() const;

  void reset();
};