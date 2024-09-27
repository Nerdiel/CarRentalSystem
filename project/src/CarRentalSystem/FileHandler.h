#pragma once
#ifndef _FILEHANDLER_H_
#define _FILEHANDLER_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <filesystem>
#include "Objects.h"

const std::filesystem::path SOURCEFILES = std::filesystem::current_path() += "/src/";
const std::filesystem::path SERVICEDCARS = "Cars/repair_shop.txt";
const std::filesystem::path RENTEDCARS = "Cars/rented.txt";
const std::filesystem::path AVAILABLECARS = "Cars/available.txt";
const std::filesystem::path PERMANENTLYUNAVAILABLECARS = "Cars/permanently_unavailable.txt";
const std::filesystem::path USERS = "Users/users.txt";
const std::filesystem::path CUSTOMERS = "Customers/customers.txt";
const std::filesystem::path ACTIVECONTRACTS = "Customers/Contracts/Active/";
const std::filesystem::path ARCHIVEDCONTRACTS = "Customers/Contracts/Archived/";
constexpr char ERRORMESSAGE[] = "Error opening file.";

constexpr char DELIMITER = '#';


/**
* @brief A storage container that represents the properties of either all the cars, all the customers or all the users.
*/
using StorageVector = std::vector<Properties>;

/*
* @brief A simple function that easily concatenates two paths
*/
std::filesystem::path ConcatPaths(const std::filesystem::path& a, const std::filesystem::path& b);

enum CarStatus { Available, Serviced, Rented, PermanentlyUnavailable };

class FileReader {
public:
    /**
     * @brief Universal function to get the info about cars in a file. The file is decided by the parameter
     * @param status the status of the cars you want to get
     * @return A StorageVector containing the properties of all the cars.
     */
    static StorageVector GetCars(CarStatus status);

    /**
     * @brief Get info about all the customers.
     * @return A StorageVector containing the properties of all customers.
     */
    static StorageVector GetCustomers();

    /**
     * @brief Get info about all the users.
     * @return A StorageVector containing the properties of all users.
     */
    static StorageVector GetUsers();

    /**
     * @brief Get the names of files with active contracts.
     * @return A StorageVector containing the names of files with active contracts.
     */
    static StorageVector GetActiveContracs();

    /**
     * @brief Get the names of files with archived contracts.
     * @return A StorageVector containing the names of files with archived contracts.
     */
    static StorageVector GetArchivedContracs();

    /**
     * @brief This function finds a certain record that has a property that equals the given token. Then returns the whole line.
     * @param filename The name of the file to search.
     * @param token The token to search for.
     * @return Properties containing the matching record.
     */
    static Properties FindRecordByToken(const std::filesystem::path& filename, const std::string& token);

private:
    /**
     * @brief A private function that returns info of everything that the user requests.
     * @param what A path specifying the type of information to retrieve.
     * @return A StorageVector containing the requested information.
     */
    static StorageVector GetInfo(const std::filesystem::path& what);

    /**
     * @brief A private function that returns the names of files in the given directory.
     * @param directoryPath The path of the directory to search.
     * @return A StorageVector containing the names of files in the directory.
     */
    static StorageVector GetNamesOfFiles(const std::filesystem::path& directoryPath);
};

class FileWriter {
public:
    /**
     * @brief Universal function add a car to a file. The file is decided by the parameter
     * @param status the status of the cars you want to add
     */
    static void AddCar(const Car& car, CarStatus status);

    /**
     * @brief Add a customer to the customer list.
     * @param customer The customer to add to the customer list.
     */
    static void AddCustomer(const Customer& customer);

    /**
     * @brief Add a user to the user list.
     * @param user The user to add to the user list.
     */
    static void AddUser(const User& user);

    /**
     * @brief Delete a record in the given file. This function searches the file for the line with the token inside. If it finds it, it deletes it, if not, it does nothing.
     * @param filename The name of the file to delete the record from.
     * @param token The token to search for in the file.
     */
    static void DeleteRecordInFile(const std::filesystem::path& filename, const std::string& token);

    /**
     * @brief Create a new contract between a user and a customer.
     * @param customer The customer involved in the contract.
     * @param car The car involved in the contract.
     * @param dueDateStruct The due date of the contract.
     */
    static void CreateNewContract(const Customer& customer, const Car& car, const std::chrono::system_clock::time_point& dueDateStruct);

    /**
     * @brief Move a file between two folders.
     * @param sourceFolder The folder to move the file from.
     * @param destinationFolder The folder to move the file to.
     * @param fileName The name of the file to move.
     */
    static void MoveFileToFolder(const std::filesystem::path& sourceFolder, const std::filesystem::path& destinationFolder, const std::string& fileName);

private:
    /**
     * @brief A private function that writes the given properties to the file of the given name.
     * @param props The properties to write.
     * @param what The name of the file to write the properties to.
     */
    static void AddInfo(const Properties& props, const std::filesystem::path& what);

    /**
     * @brief A private function that returns the time point as a string with the given delimiter.
     * @param timePoint The time point to convert to string.
     * @param delimiter The delimiter to use in the string.
     * @return A string representing the time point.
     */
    static std::string TimePointToString(const std::chrono::system_clock::time_point& timePoint, const std::string& delimiter);
};

#endif

