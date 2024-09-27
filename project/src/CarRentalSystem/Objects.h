#pragma once

#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#include <string>
#include <vector>
#include <stdexcept>

/**
 * @brief A storage container that represents the properties of either a car, a customer, or a user.
 */
using Properties = std::vector<std::string>;

constexpr char INVALIDNUMBEROFPROPERTIESMESSAGE[] = "Invalid number of properties";

/**
 * @brief Represents a car with various attributes such as make, model, year, etc.
 */
class Car {
public:
    /**
     * @brief Constructs a Car object from a vector of properties.
     * @param props A vector containing car properties.
     * @throws std::invalid_argument if the number of properties is incorrect.
     */
    Car(const Properties& props);

    /**
     * @brief Retrieves the properties of the car.
     * @return A vector containing the car's properties.
     */
    Properties GetProperties() const;

    /**
     * @brief Retrieves the make of the car.
     * @return A string representing the car's make.
     */
    std::string GetMake() const;

    /**
     * @brief Retrieves the model of the car.
     * @return A string representing the car's model.
     */
    std::string GetModel() const;

    /**
     * @brief Retrieves the year of manufacture of the car.
     * @return An integer representing the car's year of manufacture.
     */
    int GetYear() const;

    /**
     * @brief Retrieves the color of the car.
     * @return A string representing the car's color.
     */
    std::string GetColor() const;

    /**
     * @brief Retrieves the license plate of the car.
     * @return A string representing the car's license plate.
     */
    std::string GetLicencePlate() const;

    /**
     * @brief Retrieves the motorization type of the car.
     * @return A string representing the car's motorization type.
     */
    std::string GetMotorization() const;

    /**
     * @brief Retrieves the gearbox type of the car.
     * @return A string representing the car's gearbox type.
     */
    std::string GetGearbox() const;

    /**
     * @brief Retrieves the number of seats in the car.
     * @return An integer representing the number of seats in the car.
     */
    int GetSeats() const;

    /**
     * @brief Retrieves the cost per hour to rent the car.
     * @return An integer representing the cost per hour.
     */
    int GetCostPerHour() const;

private:
    std::string Make;
    std::string Model;
    int Year;
    std::string Color;
    std::string LicencePlate;
    std::string Motorization;
    std::string Gearbox;
    int Seats;
    int CostPerHour;
};

/**
 * @brief Represents a customer with various attributes such as name, surname, email address, etc.
 */
class Customer {
public:
    /**
     * @brief Constructs a Customer object from a vector of properties.
     * @param props A vector containing customer properties.
     * @throws std::invalid_argument if the number of properties is incorrect.
     */
    Customer(const Properties& props);

    /**
     * @brief Retrieves the properties of the customer.
     * @return A vector containing the customer's properties.
     */
    Properties GetProperties() const;

    /**
     * @brief Retrieves the name of the customer.
     * @return A string representing the customer's name.
     */
    std::string GetName() const;

    /**
     * @brief Retrieves the surname of the customer.
     * @return A string representing the customer's surname.
     */
    std::string GetSurname() const;

    /**
     * @brief Retrieves the email address of the customer.
     * @return A string representing the customer's email address.
     */
    std::string GetEmailAdress() const;

    /**
     * @brief Retrieves the phone number of the customer.
     * @return A string representing the customer's phone number.
     */
    std::string GetPhone() const;

    /**
     * @brief Retrieves the address of the customer.
     * @return A string representing the customer's address.
     */
    std::string GetAdress() const;

private:
    std::string Name;
    std::string Surname;
    std::string EmailAdress;
    std::string Phone; // The phone number is represented as a string in case the customer provided their number with the country code where there is a +.
    std::string Adress;
};

/**
 * @brief Represents a user with attributes such as username, password, and admin status.
 */
class User {
public:
    /**
     * @brief Constructs a User object from a vector of properties.
     * @param props A vector containing user properties.
     * @throws std::invalid_argument if the number of properties is incorrect.
     */
    User(const Properties& props);

    /**
     * @brief Retrieves the properties of the user.
     * @return A vector containing the user's properties.
     */
    Properties GetProperties() const;

    /**
     * @brief Retrieves the admin status of the user.
     * @return A boolean indicating whether the user is an admin.
     */
    bool GetAdminStatus() const;

private:
    std::string Username;
    std::string Password;
    bool Admin;
};

#endif // !_OBJECTS_H_
