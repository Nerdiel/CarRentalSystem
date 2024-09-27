#include "ConsoleController.h"

void ConsoleController::DisplayMenu(std::ostream& os, const std::vector<std::string>& options, bool inMainMenu, int numberOfDelayedContracts) {
	DisplayHeader(os);
	for (size_t i = 0; i < options.size(); ++i) {
		DisplayMenuOption(std::to_string(i + 1) + "." + options[i], os);
	}
	DisplayFooter(os, inMainMenu, numberOfDelayedContracts);

}

void ConsoleController::DisplayCars(std::ostream& os, CarStatus status) {
	DisplayHeader(os);

	switch (status) {
	case Available:
		DisplayContentHeader(os, "Available cars");
		DisplayContent(os, FileReader::GetCars(CarStatus::Available));
		break;
	case Serviced:
		DisplayContentHeader(os, "Serviced cars");
		DisplayContent(os, FileReader::GetCars(CarStatus::Serviced));
		break;
	case Rented:
		DisplayContentHeader(os, "Rented cars");
		DisplayContent(os, FileReader::GetCars(CarStatus::Rented));
		break;
	case PermanentlyUnavailable:
		DisplayContentHeader(os, "Permanently unavailable cars");
		DisplayContent(os, FileReader::GetCars(CarStatus::PermanentlyUnavailable));
		break;
	}

	DisplayFooter(os);
}

void ConsoleController::DisplayCustomers(std::ostream& os) {
	DisplayHeader(os);

	DisplayContentHeader(os, "Customers");

	DisplayContent(os, FileReader::GetCustomers());

	DisplayFooter(os);
}

void ConsoleController::DisplayActiveContracts(std::ostream& os) {
	DisplayHeader(os);

	DisplayContentHeader(os, "Active contracts");

	DisplayContent(os, FileReader::GetActiveContracs(), true);

	DisplayFooter(os);
}

void ConsoleController::DisplayArchivedContracts(std::ostream& os) {
	DisplayHeader(os);

	DisplayContentHeader(os, "Archived contracts");

	DisplayContent(os, FileReader::GetArchivedContracs(), true);

	DisplayFooter(os);
}

void ConsoleController::DisplayAddingMessage(std::ostream& os, const std::string namesOfColumns) {
	DisplayHeader(os);

	os << std::string(CalculateSpacing(strlen(ADDINGNEWRECORDSMESSAGE1)), ' ') << ADDINGNEWRECORDSMESSAGE1 << std::endl;
	os << std::string(CalculateSpacing(strlen(ADDINGNEWRECORDSMESSAGE2)), ' ') << ADDINGNEWRECORDSMESSAGE2 << std::endl;
	os << std::string(CalculateSpacing(namesOfColumns.length()), ' ') << namesOfColumns << std::endl;

	DisplayFooter(os);
}

void ConsoleController::ClearConsole() {
#ifdef _WIN32
	std::system("cls");
#else
	std::system("clear");
#endif
}

void ConsoleController::PrintMessage(std::ostream& os, const std::string& message) {
	os << message << std::endl;
}

void ConsoleController::PrintIncorrectMessage(std::ostream& os, const std::string& what) {
	os << what << " is incorrect, try again. Or type CANCEL to cancel." << std::endl;
}

void ConsoleController::PrintPromptMessage(std::ostream& os, const std::string& what, const std::string& info) {
	std::string possessivePronoun = "its";
	if (what == "customer") { possessivePronoun = "their"; }
	os << "Please choose a " << what << ", write " << possessivePronoun << " " << info << " here : " << std::endl;
}

void ConsoleController::DisplayGoodByeMessage(std::ostream& os) {
	for (size_t i = 0; i < BORDERWIDTH; ++i) {
		os << std::string(WIDTH, BORDERCHAR) << std::endl;
	}
	for (size_t i = 0; i < SPACING; ++i) {
		os << std::endl;
	}

	int spacing = WIDTH / SAFEINDENTATIONRATIO;


	// Display bye bye man
	os << "	     /" << std::endl;
	os << "	   O/ " << std::endl;
	os << "	  /|  " << std::endl;
	os << "	 / |  " << std::string(spacing, ' ') << GOODBYEMESSAGE << std::string(spacing, ' ') << std::endl;
	os << "	   |  " << std::endl;
	os << "	  / \\ " << std::endl;
	os << "	 /   \\" << std::endl;

	for (size_t i = 0; i < SPACING; ++i) {
		os << std::endl;
	}
	for (size_t i = 0; i < BORDERWIDTH; ++i) {
		os << std::string(WIDTH, BORDERCHAR) << std::endl;
	}
}

int ConsoleController::GetIntInput(std::ostream& os, std::istream& is, int min, int max) {
	std::string inputOption;
	while (true) {
		is >> inputOption;

		if (inputOption == "CANCEL") {
			return -1;
		}

		// Try to convert the inputOption to an integer
		try {
			int selectedOption = std::stoi(inputOption);

			// Check if the selectedOption is within the valid range
			if (selectedOption >= min && selectedOption <= max) {
				return selectedOption; // Return the valid option
			}
			else {
				os << "Invalid option. Please enter a number between " << min << " and " << max << "." << std::endl;
			}
		}
		catch (const std::exception&) {
			os << "Invalid option. Please enter a number between " << min << " and " << max << "." << std::endl;
		}

		// Clear the input stream to ignore any remaining characters
		is.clear();
		is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

std::string ConsoleController::GetStringInput(std::istream& is) {
	std::string inputOption;
	std::getline(is, inputOption);
	return inputOption;
}

void ConsoleController::DisplayContent(std::ostream& os, const std::vector<std::vector<std::string>>& content, bool displayFileNames) {
	std::vector<size_t> maxSizes;
	if ((!displayFileNames && content.size() == 1) || (displayFileNames && content.size() == 0)) {
		os << std::endl << std::string(CalculateSpacing(strlen(EMPTYMESSAGE)), ' ') << EMPTYMESSAGE << std::endl;
		return;
	}
	for (size_t i = 0; i < content[0].size(); ++i) {
		maxSizes.push_back(0);
	}
	size_t maxTotalSize = 0;
	for (auto&& line : content) {
		size_t totalSize = 0;
		for (size_t i = 0; i < line.size(); ++i) {
			if (line[i].size() > maxSizes[i]) {
				maxSizes[i] = line[i].size();
			}
			totalSize += line[i].size();
		}
		if (totalSize > maxTotalSize) {
			maxTotalSize = totalSize;
		}
	}
	size_t spacing = CalculateSpacing(maxTotalSize + content[0].size() - 1);
	for (size_t x = 0; x < content.size(); ++x) {
		os  << std::string(CalculateSpacing(maxTotalSize + content[0].size() - 1), ' ');
		for (size_t i = 0; i < content[x].size(); ++i) {
			os << content[x][i] << std::string(maxSizes[i] - content[x][i].size(), ' ');
			if (i < content[x].size() - 1) {
				os << ' ';
			}
		}
		os << std::string(spacing, ' ') << std::endl;
	}
}

void ConsoleController::DisplayContentHeader(std::ostream& os, const std::string& title) {
	size_t spacing = CalculateSpacing(title.size());
	os << std::string(spacing, ' ') << title << std::endl;
}

void ConsoleController::DisplayLogInMenu(std::ostream& os) {
	DisplayHeader(os);

	std::cout << std::string(CalculateSpacing(strlen(WELCOMEMESSAGE)), ' ') << WELCOMEMESSAGE << std::string(CalculateSpacing(strlen(WELCOMEMESSAGE)), ' ') << std::endl;
	
	DisplayFooter(os, false, 0, true);
}

void ConsoleController::DisplayHeader(std::ostream& os) {
	for (size_t i = 0; i < BORDERWIDTH; ++i) {
		os << std::string(WIDTH, BORDERCHAR) << std::endl;
	}
	for (size_t i = 0; i < SPACING; ++i) {
		os << std::endl;
	}
	os << std::string(CalculateSpacing(strlen(NAME)), ' ') << NAME << std::endl << std::endl;
}

void ConsoleController::DisplayFooter(std::ostream& os, bool inMainMenu, int numberOfDelayedContracts, bool showExitOption) {
	for (size_t i = 0; i < SPACING; ++i) {
		os << std::endl;
	}
	if(showExitOption){}
	else if (inMainMenu) {
		if (numberOfDelayedContracts == 0) {
			os << EXITOPTIONLINE << std::endl;
		}
		else {
			std::ostringstream oss;
			std::string firstHalfOfMessage = "There is ";
			std::string secondHalfOfMessage = " delayed contract!";
			if (numberOfDelayedContracts > 1) {
				firstHalfOfMessage = "There are ";
				secondHalfOfMessage = " delayed contracts!";
			}
			oss << firstHalfOfMessage << numberOfDelayedContracts << secondHalfOfMessage;
			std::string delayedContractsMessage = oss.str();
			int spacing = WIDTH - static_cast<int>(strlen(EXITOPTIONLINE)) - static_cast<int>(delayedContractsMessage.size());
			if (spacing <= 0) {
				spacing = SAFEINDENTATIONRATIO;
			}
			os << EXITOPTIONLINE << std::string(spacing, ' ') << delayedContractsMessage << std::endl;
		}
	}
	else {
		os << BACKOPTIONLINE << std::endl;
	}
	for (size_t i = 0; i < BORDERWIDTH; ++i) {
		os << std::string(WIDTH, BORDERCHAR) << std::endl;
	}
}

void ConsoleController::DisplayMenuOption(const std::string& option, std::ostream& os) {
	size_t spacing = CalculateSpacing(option.length());
	os  << std::string(spacing, ' ')
		<< option << std::string(spacing, ' ')
		<< std::endl;
}

size_t ConsoleController::CalculateSpacing(size_t lengthOfContent){
	int spacing = (WIDTH - static_cast<int>(lengthOfContent)) / 2; //Divided by two because we have two equal sides
	size_t result = 0;
	if (spacing <= 1) {
		result = WIDTH / SAFEINDENTATIONRATIO;
	}
	else {
		result = spacing;
	}
	return result;
}