#pragma once

#ifndef _MAIN_H_
#define _MAIN_H_

#include "ConsoleController.h"
#include <functional>

// Menu options for different user roles and operations
const std::vector<std::string> MenuOptions = { "Cars", "Customers", "Show active contracts", "New contract", "End active contract" };
const std::vector<std::string> AdminMenuOptions = { "Cars", "Customers", "Show active contracts", "New contract", "End active contract", "Add a user", "Show archived contracts" };
const std::vector<std::string> CarMenuOptions = { "Show available cars", "Show rented cars", "Show cars in service", "Show permanently unavailable cars" };
const std::vector<std::string> AdminCarMenuOptions = { "Show available cars", "Show rented cars", "Show cars in service", "Show permanently unavailable cars", "Add new car", "Move a car" };
const std::vector<std::string> MovingCarMenuOptions = { "Available cars", "Rented cars", "Cars in service", "Permanently unavailable cars" };
const std::vector<std::string> CarMenuAddingOptions = { "Add an available car", "Add a serviced car", "Add a rented car", "Add a permanently unavailable car" };
const std::vector<std::string> CustomerMenuOptions = { "Show customers", "Add a customer" };

// Default message for unknown exceptions
constexpr char UNKNOWNEXCEPTIONMESSAGE[] = "Unknown exception occurred!";

// Placeholder constants for menu choices and maximum years of rent
constexpr int numOfChoisesInDisplay = 0;
constexpr int MAXYEARSOFRENT = 2;

/**
 * @class System
 * @brief Manages the overall car rental system.
 */
class System {
public:
    /**
     * @brief Constructs a new System object.
     * @param output The output stream for displaying messages.
     * @param input The input stream for receiving user input.
     */
    System(std::ostream& output, std::istream& input) : output(output), input(input), user({ "void", "void", "void" }) {}

    /**
     * @brief Starts the system's main loop.
     */
    void Run();

    /**
     * @brief Handles user login.
     * @return True if login is successful, false otherwise.
     */
    bool LogIn();

private:


    /**
     * @brief A universal function to add a car, the type of the car is decided by the parameter
     * @param status the status of the car
     */
    void AddCar(CarStatus status) const;

    /**
     * @brief Adds a new customer to the system.
     */
    void AddCustomer() const;

    /**
     * @brief Adds a new user to the system.
     */
    void AddUser() const;

    /**
     * @brief Displays car menu options based on user role (admin or regular).
     * @param chosenCarMenuOptions The menu options specific to the user role.
     */
    void CarsMenu(const std::vector<std::string>& chosenCarMenuOptions) const;

    /**
     * @brief Displays the menu for adding a new car.
     */
    void AddNewCarMenu() const;

    /**
     * @brief Displays customer menu options.
     */
    void CustomersMenu() const;

    /**
     * @brief Creates a new contract for renting a car.
     */
    void CreateNewContract() const;

    /**
     * @brief Archives an existing contract.
     */
    void ArchiveContract() const;

    /**
     * @brief Moves a car from one status to another (e.g., from available to rented).
     */
    void MoveACar() const;

    /**
     * @brief Checks and returns the number of contracts that are overdue.
     * @return The number of overdue contracts.
     */
    int CheckContractDates() const;

    /**
     * @brief Prompts the user to enter car properties and returns them.
     * @param namesOfProps The names of the properties to be entered.
     * @return The entered properties.
     */
    Properties GetPropsFromInput(const char* namesOfProps) const;

    /**
     * @brief Prompts the user to enter contract properties and returns them.
     * @return A vector containing the entered contract properties.
     */
    std::vector<int> GetContractProperties() const;

    /**
     * @brief Counts the number of words in a string.
     * @param str The string to count words in.
     * @return The number of words.
     */
    int CountWords(const char* str) const;

    /**
     * @brief Calculates the duration of a car rent in hours.
     * @param currentTime The current time.
     * @param returnTime The return time.
     * @return The rent duration in hours.
     */
    int CalculateRentDurationHours(const std::chrono::system_clock::time_point& currentTime, const std::chrono::system_clock::time_point& returnTime) const;

    /**
     * @brief Clears the output and runs the display function
     * @param displayFunction given display function
     */
    void ClearAndDisplay(const std::function<void()>& displayFunction) const;

    /**
     * @brief Displays the menu based on the options
     * @param options the menu options that are going to show up
     * @param showDelayedContracts a boolean deciding if the number of delayed contracts is going to show up
     * @return the number of the chosen menu option
     */
    int DisplayMenuAndGetChoice(const std::vector<std::string>& options, bool showDelayedContracts = false, int delayedContractsNumber = 0) const;

    /**
     * @brief Creates a time point based on manual input of year, month, day, hour, and minute.
     * @param year The year.
     * @param month The month.
     * @param day The day.
     * @param hour The hour.
     * @param min The minute.
     * @return The created time point.
     */
    std::chrono::system_clock::time_point CreateManualTimePoint(int year, int month, int day, int hour, int min) const;

    /**
     * @brief Gets a correct input or throws a runtime error
     * @param input the input stream
     * @return the correct string
     */
    std::string GetUserInputOrCancel(std::istream& input) const;
    
    /**
     * @brief Gets a valid record or throws an error
     * @param path the path of the file we search in
     * @param searchedToken the property of the record we search it by
     * @param entityName the type of the record (car, customer, etc. )
     * @return the found properties
     */
    Properties GetValidRecord(const std::filesystem::path& path, std::string& searchedToken, const std::string& entityName) const;

    User user;
    std::ostream& output; 
    std::istream& input; 
};

#endif // !_MAIN_H_
