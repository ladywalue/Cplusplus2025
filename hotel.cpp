// This program simulates hotel booking system
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>
#include <iomanip>

using namespace std;

//  1. Constants and global variables 
const int MAX_ROOMS = 300;
const int MAX_SINGLE_ROOMS = 150;
const int MAX_DOUBLE_ROOMS = 150;

// Structure to represent a hotel room
struct Room 
{
    int number;           // Room number
    bool isSingle;        // true = single room, false = double room
    bool isBooked;        // Booking status
    int reservationId;    // Unique reservation ID
    string guestName;     // Name of the guest
    int nights;           // Number of nights stayed
    double basePrice;     // Price per night
    double discountRate;  // Applied discount rate
    bool includesBreakfast; // True if BF included
};

// Global variables
vector<Room> rooms;       // Collection of all rooms
int totalRooms = 0;       // Total number of rooms in hotel
int singleRoomsCount = 0; // Count of single rooms
int doubleRoomsCount = 0; // Count of double rooms

// 2. Function declarations
void initializeRooms();
void displayMainMenu();
bool isRoomAvailable(int roomNumber, bool requireSingle);
int makeReservation();
void viewReservations();
void searchReservation();
void displayAvailableRooms();
double calculateFinalPrice(int roomNumber, int nights, double discount);
int generateReservationId();
double getRandomDiscount();
int getValidatedInput(const string& prompt, int min, int max);
bool bookRoom(int roomNumber, const string& guestName, int nights);

// 3. Main function
int main() 
{
    // Seed random number generator with current time
    srand(static_cast<unsigned int>(time(0)));
    
    cout << "========================================\n";
    cout << "      HOTEL ROOM RESERVATION SYSTEM\n";
    cout << "========================================\n\n";
    
    // Initialize hotel rooms with random configuration
    initializeRooms();
    
    int choice;                // User's menu choice
    bool continueProgram = true; // Control program loop
    
    // Main program loop
    while (continueProgram) {
        displayMainMenu();                         
        choice = getValidatedInput("Enter your choice (1-5): ", 1, 5);
        
        // Process user choice
        switch (choice) {
            case 1:
                makeReservation();    // Create new reservation
                break;
            case 2:
                viewReservations();   // Display all current reservations
                break;
            case 3:
                searchReservation();  // Search for specific reservation
                break;
            case 4:
                displayAvailableRooms(); // Show available rooms
                break;
            case 5:
                cout << "\nThank you for using the Hotel Reservation System!\n";
                continueProgram = false; // Exit program
                break;
        }
    }
    
    return 0;
}

// ====================== Below are FUNCTION DEFINITIONS ======================

/**
 * Initializes all rooms in the hotel with random configuration
 * - Random total rooms (40-300, even number)
 * - Equal split between single and double rooms
 * - Random pricing within specified ranges
 */
void initializeRooms() 
{
    // Generate random even number between 40 and 300
    totalRooms = 40 + 2 * (rand() % 131); // (300-40)/2 = 130, +1 for inclusive range
    if (totalRooms % 2 != 0) totalRooms++; // Ensure even number
    
    // Split equally between single and double rooms
    singleRoomsCount = totalRooms / 2;
    doubleRoomsCount = totalRooms / 2;
    
    // Generate random base prices
    int singleBasePrice = 80 + rand() % 21;   // Random price: 80-100 EUR
    int doubleBasePrice = 120 + rand() % 31;  // Random price: 120-150 EUR
    
    // Display initialization details
    cout << "Initializing hotel with " << totalRooms << " rooms...\n";
    cout << "Single rooms: " << singleRoomsCount 
         << " (Price: " << singleBasePrice << " EUR/night)\n";
    cout << "Double rooms: " << doubleRoomsCount 
         << " (Price: " << doubleBasePrice << " EUR/night)\n\n";
    
    // Resize vector to hold all rooms
    rooms.resize(totalRooms);
    
    // Initialize each room
    for (int i = 0; i < totalRooms; i++) {
        rooms[i].number = i + 1;           // Room numbers start from 1
        rooms[i].isBooked = false;         // Initially not booked
        rooms[i].reservationId = 0;        // No reservation yet
        rooms[i].nights = 0;               // No nights booked
        rooms[i].includesBreakfast = false; // Not book breakfast
        rooms[i].discountRate = 0.0;       // Initialize discount rate

        // First half: single rooms, second half: double rooms
        if (i < singleRoomsCount) {
            rooms[i].isSingle = true;      // Single room
            rooms[i].basePrice = singleBasePrice;
        } else {
            rooms[i].isSingle = false;     // Double room
            rooms[i].basePrice = doubleBasePrice;
        }
    }
    
    cout << "Room initialization completed successfully!\n\n";
}

// Displays the main menu with all available options

void displayMainMenu() {
    cout << "\n============ MAIN MENU ============\n";
    cout << "1. Make a new reservation\n";
    cout << "2. View all reservations\n";
    cout << "3. Search for a reservation\n";
    cout << "4. Display available rooms\n";
    cout << "5. Exit program\n";
    cout << "===================================\n";
}

// Checks if a room is available for booking
// @param roomNumber The room number to check
// @param requireSingle If true, checks for single room specifically
// @return True if room is available, false otherwise

bool isRoomAvailable(int roomNumber, bool requireSingle = false) 
{
    // Validate room number range
    if (roomNumber < 1 || roomNumber > totalRooms) 
    {
        cout << "Error: Invalid room number!\n";
        return false;
    }
    
    // Check if room type matches requirement
    if (requireSingle && !rooms[roomNumber-1].isSingle) 
    {
        cout << "Error: Room " << roomNumber << " is not a single room!\n";
        return false;
    }
    
    // Check if room is already booked
    if (rooms[roomNumber-1].isBooked) 
    {
        cout << "Error: Room " << roomNumber << " is already booked!\n";
        return false;
    }
    
    return true; // Room is available
}

// Handles the complete reservation process
// Return Reservation ID if successful, -1 if failed

int makeReservation() {
    cout << "\n======== NEW RESERVATION ========\n";
    
    // Select room type
    cout << "Select room type:\n";
    cout << "1. Single room (1 person)\n";
    cout << "2. Double room (2 persons)\n";
    
    int roomTypeChoice = getValidatedInput("Enter choice (1-2): ", 1, 2);
    bool requireSingle = (roomTypeChoice == 1); // true for single, false for double
    
    // Show currently available rooms
    displayAvailableRooms();
    
    // Choose booking method
    cout << "\nBooking method:\n";
    cout << "1. Let system assign a random available room\n";
    cout << "2. Choose a specific room number\n";
    
    int bookingMethod = getValidatedInput("Enter choice (1-2): ", 1, 2);
    
    int selectedRoom = -1; // Will hold the selected room number
    
    if (bookingMethod == 1) 
    {
        // Collect all available rooms of required type
        vector<int> availableRooms;
        for (int i = 0; i < totalRooms; i++) {
            if (!rooms[i].isBooked) { // Room is not booked
                if ((requireSingle && rooms[i].isSingle) || 
                    (!requireSingle && !rooms[i].isSingle)) {
                    availableRooms.push_back(i + 1); // Add to available list
                }
            }
        }
        
        // Check if any rooms are available
        if (availableRooms.empty()) 
        {
            cout << "No available rooms of selected type!\n";
            return -1; // Reservation failed
        }
        
        // Randomly select from available rooms
        selectedRoom = availableRooms[rand() % availableRooms.size()];
        cout << "System assigned room: " << selectedRoom << "\n";
        
    } else {
        // Manual room selection
        selectedRoom = getValidatedInput("Enter room number to book (1-" + to_string(totalRooms) + "): ", 1, totalRooms);
    }
    
    // Validate room availability
    if (!isRoomAvailable(selectedRoom, requireSingle)) {
        return -1; // Reservation failed
    }
    
    // Get guest information
    cout << "Enter guest name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string guestName;
    getline(cin, guestName);
    
    // Get number of nights
    int nights = getValidatedInput("Enter number of nights (1-30): ", 1, 30);
    
    // Apply random discount
    double discount = getRandomDiscount();
    
    // Calculate final price
    double finalPrice = calculateFinalPrice(selectedRoom, nights, discount);
    
    // Book breakfast to get 5% off of total price
    cout << "\nAdd breakfast to reservation? (5% discount on total price)\n";
    cout << "1. Yes, include breakfast (5% discount)\n";
    cout << "2. No, skip breakfast\n";
    int breakfastChoice = getValidatedInput("Enter choice (1-2): ", 1, 2);
    bool hasBreakfast = (breakfastChoice == 1);

    // Apply 5% if booking breakfast
    if (hasBreakfast) {
        finalPrice = finalPrice * 0.95; // Apply 5% discount
        cout << "Breakfast discount applied!\n";
    }
    
    // Generate unique reservation ID
    int reservationId = generateReservationId();
    
    // Display reservation summary for confirmation
    cout << "\n======== RESERVATION SUMMARY ========\n";
    cout << "Reservation ID: " << reservationId << "\n";
    cout << "Guest: " << guestName << "\n";
    cout << "Room: " << selectedRoom << " (" 
         << (rooms[selectedRoom-1].isSingle ? "Single" : "Double") << ")\n";
    cout << "Nights: " << nights << "\n";
    cout << "Base price: " << rooms[selectedRoom-1].basePrice << " EUR/night\n";
    cout << "Discount: " << (discount * 100) << "%\n";
    cout << "Breakfast: " << (hasBreakfast ? "Yes (5% discount applied)" : "No") << "\n";
    cout << "Total price: " << fixed << setprecision(2) << finalPrice << " EUR\n";
    cout << "====================================\n";
    
    // Confirm reservation
    cout << "\nConfirm reservation? (1=Yes, 2=No): ";
    int confirm;
    cin >> confirm;
    
    if (confirm == 1) 
    {
        // Update room booking information
        rooms[selectedRoom-1].isBooked = true;
        rooms[selectedRoom-1].reservationId = reservationId;
        rooms[selectedRoom-1].guestName = guestName;
        rooms[selectedRoom-1].nights = nights;
        rooms[selectedRoom-1].discountRate = discount;
        rooms[selectedRoom-1].includesBreakfast = hasBreakfast; // Save breakfast choice
        
        cout << "\nReservation confirmed!\n";
        cout << "Your reservation ID is: " << reservationId << "\n";
        cout << "Please save this number for future reference.\n";
        
        return reservationId; // Return successful reservation ID
    } else {
        cout << "Reservation cancelled.\n";
        return -1; // Reservation cancelled
    }
}

// Displays all current reservations in the hotel

void viewReservations() {
    cout << "\n======== ALL RESERVATIONS ========\n";
    
    bool hasReservations = false; // Flag to check if any reservations exist
    
    for (int i = 0; i < totalRooms; i++) {
        if (rooms[i].isBooked) {
            hasReservations = true;
            double finalPrice = calculateFinalPrice(i+1, rooms[i].nights, rooms[i].discountRate);
            
            // Apply breakfast discount
            if (rooms[i].includesBreakfast)
            {
                finalPrice = finalPrice * 0.95;  // 5% discount
            }

            // Display reservation details
            cout << "Room " << rooms[i].number << ":\n";
            cout << "  Reservation ID: " << rooms[i].reservationId << "\n";
            cout << "  Guest: " << rooms[i].guestName << "\n";
            cout << "  Type: " << (rooms[i].isSingle ? "Single" : "Double") << "\n";
            cout << "  Nights: " << rooms[i].nights << "\n";
            cout << "  Breakfast: " << (rooms[i].includesBreakfast ? "Yes" : "No") << "\n";
            cout << "  Total paid: " << fixed << setprecision(2) << finalPrice << " EUR\n";
            cout << "  Discount applied: " << (rooms[i].discountRate * 100) << "%\n";
            if (rooms[i].includesBreakfast) {
                cout << "  + Additional 5% breakfast discount\n";
            }
            cout << "------------------------------------\n";
        }
    }
    
    if (!hasReservations) {
        cout << "No reservations found.\n";
    }
}

// Searches for reservations by ID or guest name

void searchReservation() 
{
    cout << "\n======== SEARCH RESERVATION ========\n";
    cout << "Search by:\n";
    cout << "1. Reservation ID\n";
    cout << "2. Guest name\n";
    
    int searchType = getValidatedInput("Enter choice (1-2): ", 1, 2);
    
    bool found = false; // Flag to track if any results found
    
    if (searchType == 1) {
        // Search by reservation ID
        int searchId = getValidatedInput("Enter reservation ID: ", 10000, 99999);
        
        for (int i = 0; i < totalRooms; i++) 
        {
            if (rooms[i].isBooked && rooms[i].reservationId == searchId) {
                found = true;
                double finalPrice = calculateFinalPrice(i+1, rooms[i].nights, rooms[i].discountRate);
                
                // Apply breakfast discount
                if (rooms[i].includesBreakfast) {
                    finalPrice = finalPrice * 0.95;
                }
                
                cout << "\nReservation found:\n";
                cout << "Room: " << rooms[i].number << "\n";
                cout << "Guest: " << rooms[i].guestName << "\n";
                cout << "Type: " << (rooms[i].isSingle ? "Single" : "Double") << "\n";
                cout << "Nights: " << rooms[i].nights << "\n";
                cout << "Breakfast: " << (rooms[i].includesBreakfast ? "Yes" : "No") << "\n";
                cout << "Total paid: " << fixed << setprecision(2) << finalPrice << " EUR\n";
                break; // Stop searching after finding the reservation
            }
        }
    } else {
        // Search by guest name
        cout << "Enter guest name to search: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string searchName;
        getline(cin, searchName);
        
        // Convert to lowercase for case-insensitive search
        transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);
        
        for (int i = 0; i < totalRooms; i++) {
            if (rooms[i].isBooked) {
                string roomGuestName = rooms[i].guestName;
                transform(roomGuestName.begin(), roomGuestName.end(), roomGuestName.begin(), ::tolower);
                
                // Check if search term is contained in guest name
                if (roomGuestName.find(searchName) != string::npos) {
                    if (!found) {
                        cout << "\nReservations found:\n";
                        found = true;
                    }
                    
                    double finalPrice = calculateFinalPrice(i+1, rooms[i].nights, rooms[i].discountRate);
                    
                    // Apply breakfast discount
                    if (rooms[i].includesBreakfast) {
                        finalPrice = finalPrice * 0.95;
                    }
                    
                    // Display reservation details
                    cout << "------------------------------------\n";
                    cout << "Room: " << rooms[i].number << "\n";
                    cout << "Reservation ID: " << rooms[i].reservationId << "\n";
                    cout << "Guest: " << rooms[i].guestName << "\n";
                    cout << "Type: " << (rooms[i].isSingle ? "Single" : "Double") << "\n";
                    cout << "Nights: " << rooms[i].nights << "\n";
                    cout << "Breakfast: " << (rooms[i].includesBreakfast ? "Yes" : "No") << "\n";
                    cout << "Total paid: " << fixed << setprecision(2) << finalPrice << " EUR\n";
                }
            }
        }
    }
    
    if (!found) {
        cout << "No reservations found.\n";
    }
}

// Displays all currently available rooms

void displayAvailableRooms() {
    cout << "\n======== AVAILABLE ROOMS ========\n";
    
    int availableSingles = 0; // Counter for available single rooms
    int availableDoubles = 0; // Counter for available double rooms
    
    // Display available single rooms
    cout << "Single rooms available:\n";
    for (int i = 0; i < totalRooms; i++) {
        if (rooms[i].isSingle && !rooms[i].isBooked) {
            // Format output: 10 rooms per line
            if (availableSingles % 10 == 0 && availableSingles > 0) cout << "\n";
            cout << setw(4) << rooms[i].number;
            availableSingles++;
        }
    }
    if (availableSingles == 0) cout << "None";
    cout << "\n\n";
    
    // Display available double rooms
    cout << "Double rooms available:\n";
    for (int i = 0; i < totalRooms; i++) {
        if (!rooms[i].isSingle && !rooms[i].isBooked) {
            // Format output: 10 rooms per line
            if (availableDoubles % 10 == 0 && availableDoubles > 0) cout << "\n";
            cout << setw(4) << rooms[i].number;
            availableDoubles++;
        }
    }
    if (availableDoubles == 0) cout << "None";
    cout << "\n\n";
    
    // Display summary
    cout << "Summary: " << availableSingles << " single rooms, " 
         << availableDoubles << " double rooms available.\n";
}

/**
 * Calculates the final price for a reservation
 * @param roomNumber The room number
 * @param nights Number of nights
 * @param discount Discount rate (0.0 to 1.0)
 * @return Final price after discount
 */
double calculateFinalPrice(int roomNumber, int nights, double discount) {
    double basePrice = rooms[roomNumber-1].basePrice;
    double total = basePrice * nights;
    return total * (1.0 - discount); // Apply discount
}

/**
 * Generates a unique reservation ID
 * @return Random reservation ID between 10000 and 99999
 */
int generateReservationId() 
{
    return 10000 + rand() % 90000; // Generates number in range 10000-99999
}

/**
 * Generates a random discount rate
 * @return Discount rate: 0.00 (0%), 0.10 (10%), or 0.20 (20%)
 */
double getRandomDiscount() 
{
    int discountType = rand() % 3; // Random number: 0, 1, or 2
    
    switch (discountType) {
        case 0: return 0.00;   // 0% discount
        case 1: return 0.10;   // 10% discount
        case 2: return 0.20;   // 20% discount
        default: return 0.00;  // Fallback
    }
}

/**
 * Validates user input to ensure it's within specified range
 * @param prompt Message to display to user
 * @param min Minimum acceptable value
 * @param max Maximum acceptable value
 * @return Validated user input
 */
int getValidatedInput(const string& prompt, int min, int max) {
    int value;               // User's input
    bool validInput = false; // Validation flag
    
    while (!validInput) {
        cout << prompt; // Display prompt
        
        if (cin >> value) { // Check if input is a number
            if (value >= min && value <= max) {
                validInput = true; // Input is within range
            } else {
                cout << "Error: Please enter a number between " << min << " and " << max << ".\n";
            }
        } else {
            // Handle non-numeric input
            cout << "Error: Invalid input! Please enter a number.\n";
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }
    }
    
    return value; // Return validated input
}

/**
 * Books a room with provided information
 * @param roomNumber Room to book
 * @param guestName Name of guest
 * @param nights Number of nights
 * @return True if booking successful, false otherwise
 */
bool bookRoom(int roomNumber, const string& guestName, int nights) {
    // Validate room number
    if (roomNumber < 1 || roomNumber > totalRooms) {
        cout << "Invalid room number!\n";
        return false;
    }
    
    // Check if room is already booked
    if (rooms[roomNumber-1].isBooked) {
        cout << "Room is already booked!\n";
        return false;
    }
    
    // Update room information
    rooms[roomNumber-1].isBooked = true;
    rooms[roomNumber-1].guestName = guestName;
    rooms[roomNumber-1].nights = nights;
    rooms[roomNumber-1].reservationId = generateReservationId();
    rooms[roomNumber-1].discountRate = getRandomDiscount();
    rooms[roomNumber-1].includesBreakfast = false; // Default no breakfast
    
    return true; // Booking successful
}