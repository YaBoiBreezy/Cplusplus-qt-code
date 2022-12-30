#ifndef BATTERY_H
#define BATTERY_H

//The batter class is responsible for holding the battery level of the device
class Battery {
    public:
        //constructor
        Battery();
        //get the battery's charge level
        int getCharge();
        //set the battery's charge level and return 1 if successful, 0 if not
        bool setCharge(int);

    private:
        int charge; //int precision is sufficient for this application
};

#endif // BATTERY_H
