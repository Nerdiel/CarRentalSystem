#include "Objects.h"


Car::Car(const Properties& props) {
    if (props.size() != 9) {
        throw std::invalid_argument(INVALIDNUMBEROFPROPERTIESMESSAGE);
    }
    Make = props[0];
    Model = props[1];
    Year = std::stoi(props[2]);
    Color = props[3];
    LicencePlate = props[4];
    Motorization = props[5];
    Gearbox = props[6];
    Seats = std::stoi(props[7]);
    CostPerHour = std::stoi(props[8]);
}

Properties Car::GetProperties() const {
    Properties result = { Make,Model,std::to_string(Year),Color,
        LicencePlate,Motorization,Gearbox,
        std::to_string(Seats), std::to_string(CostPerHour) };
    return result;
}

std::string Car::GetMake() const {
    return Make;
}

std::string Car::GetModel() const {
    return Model;
}

int Car::GetYear() const {
    return Year;
}

std::string Car::GetColor() const {
    return Color;
}

std::string Car::GetLicencePlate() const {
    return LicencePlate;
}

std::string Car::GetMotorization() const {
    return Motorization;
}

std::string Car::GetGearbox() const {
    return Gearbox;
}

int Car::GetSeats() const {
    return Seats;
}

int Car::GetCostPerHour() const {
    return CostPerHour;
}


Customer::Customer(const Properties& props) {
    if (props.size() != 5) {
        throw std::invalid_argument(INVALIDNUMBEROFPROPERTIESMESSAGE);
    }
    Name = props[0];
    Surname = props[1];
    EmailAdress = props[2];
    Phone = props[3];
    Adress = props[4];
}

Properties Customer::GetProperties() const {
    Properties result = { Name,Surname,EmailAdress,Phone,Adress };
    return result;
}

std::string Customer::GetName() const {
    return Name;
}

std::string Customer::GetSurname() const {
    return Surname;
}

std::string Customer::GetEmailAdress() const {
    return EmailAdress;
}

std::string Customer::GetPhone() const {
    return Phone;
}

std::string Customer::GetAdress() const {
    return Adress;
}


User::User(const Properties& props) {
    if (props.size() != 3) {
        throw std::invalid_argument(INVALIDNUMBEROFPROPERTIESMESSAGE);
    }
    Username = props[0];
    Password = props[1];
    if (props[2] == "1") {
        Admin = true;
    }
    else {
        Admin = false;
    }
}

Properties User::GetProperties() const {
    Properties result = { Username,Password};
    if (Admin) {
        result.push_back("1");
    }
    else {
        result.push_back("0");
    }
    return result;
}

bool User::GetAdminStatus() const{
    return Admin;
}
