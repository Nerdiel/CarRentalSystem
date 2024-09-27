#pragma once



#ifndef _CONSOLECONTROLLER_H_
#define _CONSOLECONTROLLER_H_

#include "FileHandler.h"
#include <ostream>
#include <algorithm>
#include <cstring>
#include <limits>
#include <climits>

constexpr char BORDERCHAR = '#';
constexpr short WIDTH = 100;
constexpr short BORDERWIDTH = 1;
constexpr short SPACING = 2;
constexpr short SAFEINDENTATIONRATIO = 4;

constexpr char CARSCOLUMNSNAMES[] = "MAKE MODEL YEAR COLOR LICENCE_PLATE MOTORIZATION GEARBOX SEATS COST_PER_HOUR";
constexpr char CUSTOMERSCOLUMNSNAMES[] = "NAME SURNAME EMAIL PHONE ADRESS";
constexpr char USERCOLUMNSNAMES[] = "USERNAME PASSWORD ADMIN_STATUS";
constexpr char DATECOLUMNSNAMES[] = "YEAR MONTH DAY HOUR MINUTES";
constexpr char ADDINGNEWRECORDSMESSAGE1[] = "Please provide us with the following information in the correct order.";
constexpr char ADDINGNEWRECORDSMESSAGE2[] = "Or type CANCEL to cancel and return to main menu.";
constexpr char NAME[] = "Car rental system";
constexpr char BACKOPTIONLINE[] = "0. Back to main menu";
constexpr char EXITOPTIONLINE[] = "0. Exit";
constexpr char EMPTYMESSAGE[] = "There is nothing here.";
constexpr char WELCOMEMESSAGE[] = "Welcome! You can now log in.";
constexpr char GOODBYEMESSAGE[] = "Thank you for working with us! Have a great day!";

/**
 * @brief The ConsoleController class handles console input and output for a car rental system.
 */
class ConsoleController {
public:
    /**
     * @brief Displays a menu with a list of options.
     * @param os The output stream to display the menu.
     * @param options A vector containing the menu options.
     * @param inMainMenu A boolean indicating if the menu is the main menu.
     * @param numberOfDelayedContracts The number of delayed contracts.
     */
    static void DisplayMenu(std::ostream& os, const std::vector<std::string>& options, bool inMainMenu, int numberOfDelayedContracts = 0);

    /**
     * @brief Displays the list of chosen cars.
     * @param os The output stream to display the available cars.
     * @param status status of cars we want to display
     */
    static void DisplayCars(std::ostream& os, CarStatus status);

    /**
     * @brief Displays the list of customers.
     * @param os The output stream to display the customers.
     */
    static void DisplayCustomers(std::ostream& os);

    /**
     * @brief Displays the list of active contracts.
     * @param os The output stream to display the active contracts.
     */
    static void DisplayActiveContracts(std::ostream& os);

    /**
     * @brief Displays the list of archived contracts.
     * @param os The output stream to display the archived contracts.
     */
    static void DisplayArchivedContracts(std::ostream& os);

    /**
     * @brief Displays a goodbye message.
     * @param os The output stream to display the goodbye message.
     */
    static void DisplayGoodByeMessage(std::ostream& os);

    /**
     * @brief Displays a message prompting the user to add a new record.
     * @param os The output stream to display the message.
     * @param namesOfColumns The names of the columns for the new record.
     */
    static void DisplayAddingMessage(std::ostream& os, const std::string namesOfColumns);

    /**
     * @brief Displays the login menu.
     * @param os The output stream to display the login menu.
     */
    static void DisplayLogInMenu(std::ostream& os);

    /**
     * @brief Clears the console.
     */
    static void ClearConsole();

    /**
     * @brief Prints a simple message to the output stream
     * @param os The output stream to print the message to.
     * @param message The message to be printed.
     */
    static void PrintMessage(std::ostream& os, const std::string& message);
    
    /**
     * @brief Prints a message to the output stream when the input given by the user is incorrect (for example when logging in)
     * @param os The output stream to print the message to.
     * @param what What is incorrect.
     */
    static void PrintIncorrectMessage(std::ostream& os, const std::string& what);
    
    /**
     * @brief Prints a prompting message
     * @param os The output stream to print the message to.
     * @param what What are we asking of the user to give us info about.
     * @param info The information about said thing.
     */
    static void PrintPromptMessage(std::ostream& os, const std::string& what, const std::string& info);


    // INPUT MANAGING

    /**
     * @brief Gets an integer input from the user within a specified range.
     * @param os The output stream for prompts.
     * @param is The input stream to get the user's input.
     * @param min The minimum valid value.
     * @param max The maximum valid value.
     * @return The integer input from the user.
     */
    static int GetIntInput(std::ostream& os, std::istream& is, int min, int max);

    /**
     * @brief Gets a string input from the user.
     * @param is The input stream to get the user's input.
     * @return The string input from the user.
     */
    static std::string GetStringInput(std::istream& is);

private:
    /**
     * @brief Displays the content of a list of records.
     * @param os The output stream to display the content.
     * @param content A vector of vectors containing the content to display.
     * @param displayFileNames A boolean indicating if file names should be displayed.
     */
    static void DisplayContent(std::ostream& os, const std::vector<std::vector<std::string>>& content, bool displayFileNames = false);

    /**
     * @brief Displays the header for a content section.
     * @param os The output stream to display the header.
     * @param title The title of the content section.
     */
    static void DisplayContentHeader(std::ostream& os, const std::string& title);

    /**
     * @brief Displays the header of the console output.
     * @param os The output stream to display the header.
     */
    static void DisplayHeader(std::ostream& os);

    /**
     * @brief Displays the footer of the console output.
     * @param os The output stream to display the footer.
     * @param inMainMenu A boolean indicating if the footer is for the main menu.
     * @param numberOfDelayedContracts The number of delayed contracts.
     * @param showExitOption A boolean indicating if the exit option should be displayed.
     */
    static void DisplayFooter(std::ostream& os, bool inMainMenu = false, int numberOfDelayedContracts = 0, bool showExitOption = false);

    /**
     * @brief Displays a single menu option.
     * @param option The menu option to display.
     * @param os The output stream to display the option.
     */
    static void DisplayMenuOption(const std::string& option, std::ostream& os);

    /**
     * @brief Calculates the spacing required to center content.
     * @param lengthOfContent The length of the content to center.
     * @return The calculated spacing.
     */
    static size_t CalculateSpacing(size_t lengthOfContent);
};

#endif // !_CONSOLECONTROLLER_H_
