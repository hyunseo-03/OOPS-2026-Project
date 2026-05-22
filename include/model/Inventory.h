#pragma once

class Inventory {
private:
    int buns;
    int patties;
    int lettuce;
    int tomato;
    int cheese;
    int sauces;

public:
    Inventory();

    bool hasIngredients() const;
    void useIngredients();
    void addIngredients();

    int getBuns() const;
    int getPatties() const;
    int getLettuce() const;
    int getTomato() const;
    int getCheese() const;
    int getSauces() const;
};