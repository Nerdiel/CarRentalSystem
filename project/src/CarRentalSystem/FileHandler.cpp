#include "FileHandler.h"

// Function for concatanating paths
std::filesystem::path ConcatPaths(const std::filesystem::path& a, const std::filesystem::path& b) {
	std::filesystem::path c = std::filesystem::path(a) += b;
	return std::filesystem::path(a) += b;
}

StorageVector FileReader::GetInfo(const std::filesystem::path& what) {
	std::ifstream inputFile;
	std::filesystem::path name = ConcatPaths(SOURCEFILES, what);

	inputFile.open(name);

	StorageVector result;

	if (!inputFile.is_open()) {
		std::cerr << ERRORMESSAGE << std::endl;
		return result;
	}

	std::string line;
	while (std::getline(inputFile, line)) { //Go through the file by lines after skipping the first line
		std::stringstream ss(line);
		std::vector<std::string> tokens;
		std::string token;

		while (std::getline(ss, token, DELIMITER)) { //Split each line by a specified delimiter and create a vector from the tokens
			tokens.push_back(token);
		}
		result.push_back(tokens); //Add the new vector to the result vector of vectors
	}

	inputFile.close();
	return result;
}

StorageVector FileReader::GetCars(CarStatus status) {
	switch (status) {
	case Available:
		return GetInfo(AVAILABLECARS);
	case Serviced:
		return GetInfo(SERVICEDCARS);
	case Rented:
		return GetInfo(RENTEDCARS);
	case PermanentlyUnavailable:
		return GetInfo(PERMANENTLYUNAVAILABLECARS);
	}	
}

StorageVector FileReader::GetCustomers() {
	return GetInfo(CUSTOMERS);
}

StorageVector FileReader::GetUsers() {
	return GetInfo(USERS);
}

StorageVector FileReader::GetActiveContracs() {
	return GetNamesOfFiles(ConcatPaths(SOURCEFILES, ACTIVECONTRACTS));
}

StorageVector FileReader::GetArchivedContracs() {
	return GetNamesOfFiles(ConcatPaths(SOURCEFILES, ARCHIVEDCONTRACTS));
}

Properties FileReader::FindRecordByToken(const std::filesystem::path& filename, const std::string& token) {
	Properties Props;

	std::ifstream inputFile(filename);
	if (!inputFile.is_open()) {
		std::cerr << ERRORMESSAGE << std::endl;
		return Props;
	}

	std::string line;
	bool found = false;
	while (std::getline(inputFile, line)) {
		std::istringstream iss(line);
		std::string currentToken;
		while (std::getline(iss, currentToken, DELIMITER)) {
			if (currentToken == token) { // Check for exact match
				found = true;
				break;
			}
		}
		if (found) {
			break;
		}
	}
	inputFile.close();

	if (found) {
		std::istringstream iss(line);
		std::string token;
		while (std::getline(iss, token, DELIMITER)) {
			Props.push_back(token);
		}
	}

	return Props;
}

StorageVector FileReader::GetNamesOfFiles(const std::filesystem::path& directoryPath) {
	StorageVector fileNames;
	try {
		for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
			if (std::filesystem::is_regular_file(entry.status())) {
				std::vector<std::string> singleFileName;
				singleFileName.push_back(entry.path().stem().string()); // We use the name of the file without the extension
				fileNames.push_back(singleFileName);
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Filesystem error: " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "General exception: " << e.what() << std::endl;
	}
	return fileNames;
}



void FileWriter::CreateNewContract(const Customer& customer, const Car& car, const std::chrono::system_clock::time_point& dueDate) {
	std::ofstream outputFile;

	std::filesystem::path name = ConcatPaths(SOURCEFILES, ConcatPaths( ACTIVECONTRACTS, (customer.GetSurname() + "_" + car.GetLicencePlate() + "_" + TimePointToString(dueDate, "_") + ".txt")));


	outputFile.open(name);

	if (!outputFile.is_open()) {
		std::cerr << ERRORMESSAGE << std::endl;
		return;
	}


	std::time_t dueDateTime = std::chrono::system_clock::to_time_t(dueDate);
	std::tm dueDateStruct;
#ifdef _WIN32
	localtime_s(&dueDateStruct, &dueDateTime); // Use gmtime_s on Windows
#else
	localtime_r(&dueDateTime, &dueDateStruct); // Use gmtime_r on POSIX systems
#endif

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
	std::tm nowDate;
#ifdef _WIN32
	localtime_s(&nowDate, &nowTime); // Use localtime_s on Windows
#else
	localtime_r(&nowTime, &nowDate); // Use localtime_r on POSIX systems
#endif

	auto duration = dueDate - now;

	auto hours = std::chrono::duration_cast<std::chrono::hours>(duration).count();

	// Contract format

	outputFile << "Contract Details:" << std::endl;
	outputFile << "Customer: " << customer.GetName() << " " << customer.GetSurname() << std::endl;
	outputFile << "Car: " << car.GetMake() << " " << car.GetModel() << ", License Plate: " << car.GetLicencePlate() << std::endl;
	outputFile << "Due Date: " << std::put_time(&dueDateStruct, "%d. %m. %Y") << std::endl;
	outputFile << "Due Hours: " << std::put_time(&dueDateStruct, "%H:%M") << std::endl;
	outputFile << "Hours of rent: " << hours << std::endl;
	outputFile << "Total price: " << hours * car.GetCostPerHour()<< "Kc" << std::endl;
	outputFile << std::endl;
	outputFile << "Signed on: " << std::put_time(&nowDate, "%d. %m. %Y") << std::endl;
	outputFile << std::endl;
	outputFile << "Customers signutare:......." << std::endl;
	outputFile << "Representative signutare:......." << std::endl;

	outputFile.close();
}

void FileWriter::DeleteRecordInFile(const std::filesystem::path& filename, const std::string& token) {
	std::ifstream inputFile(filename);
	if (!inputFile.is_open()) {
		std::cerr << ERRORMESSAGE << std::endl;
		return;
	}

	std::vector<std::string> lines;
	std::string line;
	while (std::getline(inputFile, line)) {
		lines.push_back(line);
	}
	inputFile.close();

	auto it = std::find_if(lines.begin(), lines.end(), [&](const std::string& str) {
		std::istringstream iss(str);
		std::string currentToken;
		while (std::getline(iss, currentToken, DELIMITER)) {
			if (currentToken == token) { // Check for exact match
				return true;
			}
		}
		return false;
		});

	if (it != lines.end()) {
		lines.erase(it);

		std::ofstream outputFile(filename);
		if (!outputFile.is_open()) {
			std::cerr << ERRORMESSAGE << std::endl;
			return;
		}
		for (const auto& line : lines) {
			outputFile << line << std::endl;
		}
		outputFile.close();
	}

	return;
}

void FileWriter::MoveFileToFolder(const std::filesystem::path& sourceFolder, const std::filesystem::path& destinationFolder, const std::string& fileName) {
	std::filesystem::path sourcePath = ConcatPaths(sourceFolder, fileName);
	std::filesystem::path destinationPath = ConcatPaths(destinationFolder,fileName);

	try {
		if (std::filesystem::exists(sourcePath) && std::filesystem::is_regular_file(sourcePath)) {
			std::filesystem::rename(sourcePath, destinationPath);
		}
	}
	catch (const std::exception& e) {
		throw e;
	}
}

void FileWriter::AddInfo(const Properties& props, const std::filesystem::path& what) {
	std::ofstream outputFile;
	std::filesystem::path name = ConcatPaths(SOURCEFILES, what);
	outputFile.open(name, std::ios_base::app);

	if (!outputFile.is_open()) {
		std::cerr << ERRORMESSAGE << std::endl;
		return;
	}

	for (size_t i = 0; i < props.size(); ++i) {
		outputFile << props[i];
		if (i < props.size() - 1) {
			outputFile << DELIMITER;
		}
	}
	outputFile << std::endl;

	outputFile.close();
}

void FileWriter::AddCar(const Car& car, CarStatus status) {
	switch (status) {
	case Available:
		AddInfo(car.GetProperties(), AVAILABLECARS);
		break;
	case Serviced:
		AddInfo(car.GetProperties(), SERVICEDCARS);
		break;
	case Rented:
		AddInfo(car.GetProperties(), RENTEDCARS);
		break;
	case PermanentlyUnavailable:
		AddInfo(car.GetProperties(), PERMANENTLYUNAVAILABLECARS);
		break;
	}
}

void FileWriter::AddCustomer(const Customer& customer) {
	AddInfo(customer.GetProperties(), CUSTOMERS);
}

void FileWriter::AddUser(const User& user) {
	AddInfo(user.GetProperties(), USERS);
}

std::string FileWriter::TimePointToString(const std::chrono::system_clock::time_point& timePoint, const std::string& delimiter) {
	std::time_t timeT = std::chrono::system_clock::to_time_t(timePoint);

	std::tm timeStruct;
#ifdef _WIN32
	localtime_s(&timeStruct, &timeT); // Use localtime_s for local time on Windows
#else
	localtime_r(&timeT,&timeStruct); // Use localtime_r for local time on POSIX systems
#endif

	std::ostringstream oss;
	oss << std::put_time(&timeStruct, ("%Y" + delimiter + "%m" + delimiter + "%d" + delimiter + "%H" + delimiter + "%M").c_str());

	return oss.str();
}