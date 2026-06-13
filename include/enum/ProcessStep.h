#pragma once

enum class ProcessStep {
  Idle,
  PreparingIngredients,
  GrillPatty,
  AddSauce,
  AssembleBurger,
  QualityCheck,
  PackBurger,
  Done,
};