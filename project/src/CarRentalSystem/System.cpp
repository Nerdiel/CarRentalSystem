#include "System.h"


void System::Run() {
	// Let the user log in
	if (!LogIn()) {
		ClearAndDisplay([&]() { ConsoleController::DisplayGoodByeMessage(output); });
		return;
	}


	std::vector<std::string> chosenMenuOptions = user.GetAdminStatus() ? AdminMenuOptions : MenuOptions;
	std::vector<std::string> chosenCarMenuOptions = user.GetAdminStatus() ? AdminCarMenuOptions : CarMenuOptions;

	int delayedContractsNumber = CheckContractDates();
	bool exitRequested = false;

	// Main loop
	try {
		while (!exitRequested) {

			int chosenOption = DisplayMenuAndGetChoice(chosenMenuOptions, true, delayedContractsNumber);

			std::vector<std::function<void()>> actions = {
				[&]() { ConsoleController::DisplayGoodByeMessage(output); exitRequested = true; },  // Case 0
				[&]() { CarsMenu(chosenCarMenuOptions); },
				[&]() { CustomersMenu(); },
				[&]() { ConsoleController::DisplayActiveContracts(output); ConsoleController::GetIntInput(output, input, 0, numOfChoisesInDisplay); },
				[&]() { CreateNewContract(); },
				[&]() { ArchiveContract(); },
				[&]() { AddUser(); },
				[&]() { ConsoleController::DisplayArchivedContracts(output); ConsoleController::GetIntInput(output, input, 0, numOfChoisesInDisplay); }
			};

			if (chosenOption >= 0 && chosenOption < actions.size()) {
				actions[chosenOption]();  // Execute the corresponding action
			}

		}
	}
	catch (...) {
		output << UNKNOWNEXCEPTIONMESSAGE << std::endl;
		return;
	}

}

bool System::LogIn() {
	ConsoleController::DisplayLogInMenu(output);

	StorageVector users = FileReader::GetUsers();

	ConsoleController::PrintMessage(output, "Username: ");

	std::string username = ConsoleController::GetStringInput(input);

	// Main login loop
	while (true) {
		if (username == "CANCEL") {
			return false;
		}

		auto it = std::find_if(users.begin(), users.end(), [&username](const std::vector<std::string>& user) {
			return !user.empty() && user[0] == username;
			});

		if (it != users.end()) {
			ConsoleController::PrintMessage(output, "Password: ");
			std::string password = ConsoleController::GetStringInput(input);

			while (true) {
				if (password == "CANCEL") {
					return false;
				}

				if ((*it)[1] == password) {
					user = User(*it);
					return true;
				}
				else {
					ConsoleController::PrintIncorrectMessage(output, "Password");
					password = ConsoleController::GetStringInput(input);
				}
			}
		}
		else {
			ConsoleController::PrintIncorrectMessage(output, "Username");
			username = ConsoleController::GetStringInput(input);
		}
	}

	return false;
}

void System::AddCar(CarStatus status) const {
	Properties givenProps = GetPropsFromInput(CARSCOLUMNSNAMES);
	if (givenProps.empty()) return;

	switch (status) {
	case Available:
		FileWriter::AddCar(givenProps, CarStatus::Available);
		break;
	case Serviced:
		FileWriter::AddCar(givenProps, CarStatus::Serviced);
		break;
	case Rented:
		FileWriter::AddCar(givenProps, CarStatus::Rented);
		break;
	case PermanentlyUnavailable:
		FileWriter::AddCar(givenProps, CarStatus::PermanentlyUnavailable);
		break;
	}
}

void System::AddCustomer() const {
	Properties givenProps = GetPropsFromInput(CUSTOMERSCOLUMNSNAMES);
	if (givenProps.size() != 0) {
		FileWriter::AddCustomer(givenProps);
	}
}

void System::AddUser() const {
	Properties givenProps = GetPropsFromInput(USERCOLUMNSNAMES);
	if (givenProps.size() != 0) {
		FileWriter::AddUser(givenProps);
	}
}

std::vector<int> System::GetContractProperties() const {
	std::vector<int> dateProps;
	ConsoleController::DisplayAddingMessage(output, DATECOLUMNSNAMES);
	int words = CountWords(DATECOLUMNSNAMES);


	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
	std::tm nowDate;
#ifdef _WIN32
	localtime_s(&nowDate, &nowTime); // Use localtime_s on Windows
#else
	localtime_r(&nowTime, &nowDate); // Use localtime_r on POSIX systems
#endif

	std::vector<int> nowDateVector = { nowDate.tm_year + 1900,nowDate.tm_mon + 1,nowDate.tm_mday,nowDate.tm_hour,nowDate.tm_min };

	std::vector<int> dateMaxes = { nowDateVector[0] + MAXYEARSOFRENT,12,31,23,60 };
	std::vector<int> dateMinsBasic = { nowDate.tm_year + 1900,1,1,0,0 };

	std::vector<int> chosenMins = nowDateVector;

	int dateProp = 0;

	for (int i = 0; i < words; ++i) {
		if (i != 0 && dateProp != nowDateVector[i - 1]) {
			chosenMins = dateMinsBasic;
		}
		dateProp = ConsoleController::GetIntInput(output, input, chosenMins[i], dateMaxes[i]);
		if (dateProp == -1) {
			dateProps.clear();
			return dateProps;
		}
		dateProps.push_back(dateProp);
	}

	return dateProps;
}

Properties System::GetPropsFromInput(const char* namesOfProps) const {
	Properties givenProps;
	ConsoleController::DisplayAddingMessage(output, namesOfProps);
	int words = CountWords(namesOfProps);

	input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::string property;

	for (int i = 0; i < words; ++i) {
		property = ConsoleController::GetStringInput(input);
		if (property == "CANCEL") {
			givenProps.clear();
			return givenProps;
		}
		givenProps.push_back(property);
	}

	return givenProps;
}

int System::CountWords(const char* str) const {
	int count = 1;

	for (int i = 0; str[i] != '\0'; ++i) {
		if (str[i] == ' ') {
			++count;
		}
	}

	return count;
}

void System::CarsMenu(const std::vector<std::string>& chosenCarMenuOptions) const {
	int chosenCarMenuOption = DisplayMenuAndGetChoice(chosenCarMenuOptions);

	std::vector<std::function<void()>> actions = {
		[]() { /* Do nothing */ },
		[&]() {	ConsoleController::DisplayCars(output,CarStatus::Available); ConsoleController::GetIntInput(output, input, 0, numOfChoisesInDisplay); },
		[&]() {	ConsoleController::DisplayCars(output,CarStatus::Rented);ConsoleController::GetIntInput(output, input, 0, numOfChoisesInDisplay); },
		[&]() {	ConsoleController::DisplayCars(output,CarStatus::Serviced);	ConsoleController::GetIntInput(output, input, 0, numOfChoisesInDisplay); },
		[&]() {	ConsoleController::DisplayCars(output,CarStatus::PermanentlyUnavailable); ConsoleController::GetIntInput(output, input, 0, numOfChoisesInDisplay); },
		[&]() { AddNewCarMenu(); },
		[&]() { MoveACar(); }
	};

	if (chosenCarMenuOption >= 0 && chosenCarMenuOption < actions.size()) {
		actions[chosenCarMenuOption]();
	}
}


void System::AddNewCarMenu() const {
	int chosenCarMenuAddingOption = DisplayMenuAndGetChoice(CarMenuAddingOptions);

	std::vector<std::function<void()>> actions = {
		[]() { /* Do nothing */ },
		[&]() {	AddCar(CarStatus::Available); },
		[&]() {	AddCar(CarStatus::Serviced); },
		[&]() {	AddCar(CarStatus::Rented); },
		[&]() {	AddCar(CarStatus::PermanentlyUnavailable); }
	};

	if (chosenCarMenuAddingOption >= 0 && chosenCarMenuAddingOption < actions.size()) {
		actions[chosenCarMenuAddingOption]();
	}
}

void System::CustomersMenu() const {
	int chosenCustomerMenuOption = DisplayMenuAndGetChoice(CustomerMenuOptions);

	std::vector<std::function<void()>> actions = {
		[]() { /* Do nothing */ },
		[&]() {	ConsoleController::DisplayCustomers(output); ConsoleController::GetIntInput(output, input, 0, numOfChoisesInDisplay); },
		[&]() {	AddCustomer(); }
	};

	if (chosenCustomerMenuOption >= 0 && chosenCustomerMenuOption < actions.size()) {
		actions[chosenCustomerMenuOption]();
	}
}

void System::CreateNewContract() const {
	ConsoleController::DisplayCars(output,CarStatus::Available);

	// Choosing car phase
	ConsoleController::PrintPromptMessage(output, "car", "licence plate");
	input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::string licencePlate;
	Properties foundProps;
	try {
		foundProps = GetValidRecord(ConcatPaths(SOURCEFILES, AVAILABLECARS), licencePlate, "Car");
	}
	catch (std::runtime_error) {
		return;
	}

	Car rentedCar(foundProps);
	ClearAndDisplay([&]() { ConsoleController::DisplayCustomers(output); });


	// Choosing customer phase
	ConsoleController::PrintPromptMessage(output, "customer", "phone number");
	std::string phoneNumber;
	try {
		foundProps = GetValidRecord(ConcatPaths(SOURCEFILES, CUSTOMERS), phoneNumber, "Customer");
	}
	catch (std::runtime_error) {
		return;
	}

	Customer rentingCustomer(foundProps);

	ConsoleController::ClearConsole();

	// Creating due date phase
	std::vector<int> dateProps = GetContractProperties();
	if (dateProps.size() == 0) {
		return;
	}

	std::chrono::system_clock::time_point dueDate = CreateManualTimePoint(dateProps[0], dateProps[1], dateProps[2], dateProps[3], dateProps[4]);

	FileWriter::CreateNewContract(rentingCustomer, rentedCar, dueDate);

	ConsoleController::ClearConsole();

	// This step has to be last in case the user cancels the creation somewhere in the process.
	FileWriter::DeleteRecordInFile(ConcatPaths(SOURCEFILES,AVAILABLECARS), licencePlate);
	FileWriter::AddCar(rentedCar,CarStatus::Rented);
}

void System::ArchiveContract() const {
	StorageVector contracts = FileReader::GetActiveContracs();
	ConsoleController::DisplayActiveContracts(output);

	// Choosing contract phase
	ConsoleController::PrintPromptMessage(output, "contract", "name");
	input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::string name = ConsoleController::GetStringInput(input);
	bool found = false;

	while (!found) {
		if (name == "CANCEL") {
			return;
		}

		for (const auto& innerVec : contracts) {
			if (!innerVec.empty() && innerVec[0] == name) {
				found = true;
				break;
			}
		}

		if (!found) {
			ConsoleController::PrintIncorrectMessage(output, "Contract");
			name = ConsoleController::GetStringInput(input);
		}
	}


	// Get the licence plate
	std::stringstream ss(name);
	std::string token;
	size_t indexOfPlate = 2;
	for (size_t i = 0; i < indexOfPlate; ++i) {
		std::getline(ss, token, '_');
	}

	std::string licencePlate = token;
	Properties foundProps;
	foundProps = FileReader::FindRecordByToken(ConcatPaths(SOURCEFILES, RENTEDCARS), licencePlate);
	Car rentedCar(foundProps);

	FileWriter::DeleteRecordInFile(ConcatPaths(SOURCEFILES,RENTEDCARS), licencePlate);
	FileWriter::AddCar(rentedCar,CarStatus::Available);

	FileWriter::MoveFileToFolder(ConcatPaths(SOURCEFILES, ACTIVECONTRACTS), ConcatPaths(SOURCEFILES,ARCHIVEDCONTRACTS), name + ".txt");
}

void System::MoveACar() const {
	ClearAndDisplay([&]() { ConsoleController::DisplayMenu(output, MovingCarMenuOptions, false); });

	// Choose from which file to move the car
	ConsoleController::PrintMessage(output, "Choose from where to move the car: ");
	int fromOption = ConsoleController::GetIntInput(output, input, 0, MovingCarMenuOptions.size());
	if (fromOption == -1) { return; }
	std::filesystem::path fromPath;

	ConsoleController::ClearConsole();

	std::vector<std::function<void()>> fromActions = {
		[&]() { }, // Case 0
		[&]() { ConsoleController::DisplayCars(output,CarStatus::Available); fromPath = ConcatPaths(SOURCEFILES, AVAILABLECARS); },
		[&]() { ConsoleController::DisplayCars(output,CarStatus::Rented); fromPath = ConcatPaths(SOURCEFILES, RENTEDCARS); },
		[&]() { ConsoleController::DisplayCars(output,CarStatus::Serviced); fromPath = ConcatPaths(SOURCEFILES, SERVICEDCARS); },
		[&]() { ConsoleController::DisplayCars(output,CarStatus::PermanentlyUnavailable); fromPath = ConcatPaths(SOURCEFILES, PERMANENTLYUNAVAILABLECARS); }
	};

	if (fromOption == 0) return;
	if (fromOption >= 0 && fromOption < fromActions.size()) {
		fromActions[fromOption]();
	}

	// Choosing car phase
	ConsoleController::PrintPromptMessage(output, "car", "licence plate");
	input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::string licencePlate;
	Properties foundProps;
	try {
		foundProps = GetValidRecord(fromPath, licencePlate, "Car");
	}
	catch (std::runtime_error) {
		return;
	}

	Car chosenCar(foundProps);

	ClearAndDisplay([&]() { ConsoleController::DisplayMenu(output, MovingCarMenuOptions, false); });
	
	// Choose the destination file
	ConsoleController::PrintMessage(output, "Choose where to move the car: ");
	int toOption = ConsoleController::GetIntInput(output, input, 0, MovingCarMenuOptions.size());

	std::vector<std::function<void()>> toActions = {
		[&]() { return; }, // Case 0
		[&]() { FileWriter::AddCar(chosenCar,CarStatus::Available); },
		[&]() { FileWriter::AddCar(chosenCar,CarStatus::Rented); },
		[&]() { FileWriter::AddCar(chosenCar,CarStatus::Serviced); },
		[&]() { FileWriter::AddCar(chosenCar,CarStatus::PermanentlyUnavailable); }
	};

	if (toOption == 0) return;
	if (toOption >= 0 && toOption < toActions.size()) {
		toActions[toOption]();
	}

	// This step has to be last in case the user cancels the creation somewhere in the process.
	FileWriter::DeleteRecordInFile(fromPath, licencePlate);

}

int System::CheckContractDates() const {
	StorageVector activeContracts = FileReader::GetActiveContracs();
	int counterOfDelayedContracts = 0;

	for (const auto& contract : activeContracts) {
		std::string contractName = contract[0];

		// Find the position of the first underscore after name_plate_
		size_t first_underscore = contractName.find('_', contractName.find('_') + 1);

		// Extract the year, month, day, hour, and minute substrings
		int year, month, day, hour, minute;
		std::stringstream ss(contractName.substr(first_underscore + 1));
		char delim;

		ss >> year >> delim >> month >> delim >> day >> delim >> hour >> delim >> minute;

		std::chrono::system_clock::time_point dueDate = CreateManualTimePoint(year, month, day, hour, minute);
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

		if (dueDate < now) {
			counterOfDelayedContracts++;
		}
	}

	return counterOfDelayedContracts;
}

int System::CalculateRentDurationHours(const std::chrono::system_clock::time_point& currentTime, const std::chrono::system_clock::time_point& returnTime) const {
	auto duration = duration_cast<std::chrono::hours>(returnTime - currentTime);
	return duration.count();
}

std::chrono::system_clock::time_point System::CreateManualTimePoint(int year, int month, int day, int hour, int min) const {
	std::tm local_time = {};
	local_time.tm_year = year - 1900;
	local_time.tm_mon = month - 1;
	local_time.tm_mday = day;
	local_time.tm_hour = hour - 1;
	local_time.tm_min = min;
	local_time.tm_sec = 0;

#ifdef _WIN32
	__time64_t time = _mkgmtime64(&local_time);
#else
	std::time_t time = std::mktime(&local_time);
#endif

	return std::chrono::system_clock::from_time_t(time);
}

void System::ClearAndDisplay(const std::function<void()>& displayFunction) const {
	ConsoleController::ClearConsole();
	displayFunction();
}

int System::DisplayMenuAndGetChoice(const std::vector<std::string>& options, bool showDelayedContracts, int delayedContractsNumber) const{
	ClearAndDisplay([&]() { ConsoleController::DisplayMenu(output, options, showDelayedContracts, delayedContractsNumber); });
	int choice = ConsoleController::GetIntInput(output, input, 0, options.size());
	ConsoleController::ClearConsole();
	return choice;
}

std::string System::GetUserInputOrCancel(std::istream& input) const{
	std::string inputStr = ConsoleController::GetStringInput(input);
	if (inputStr == "CANCEL") {
		throw std::runtime_error("User cancelled operation");
	}
	return inputStr;
}

Properties System::GetValidRecord(const std::filesystem::path& path, std::string& searchedToken, const std::string& entityName) const{
    while (true) {
        searchedToken = GetUserInputOrCancel(input);
        Properties record = FileReader::FindRecordByToken(path, searchedToken);
        if (!record.empty()) {
            return record;
        }
        ConsoleController::PrintIncorrectMessage(output, entityName);
    }
}
