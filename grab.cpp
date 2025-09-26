#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
using namespace std;

// Function to clear screen (cross-platform)
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Function to display the header
void displayHeader() {
    cout << "=====================================\n";
    cout << "     Welcome to Grab Simulator!\n";
    cout << "=====================================\n";
    cout << "🎉 Promotions:\n";
    cout << "• GrabCar: Every 5 GrabCar orders = LDCW6123 voucher (10% off)\n";
    cout << "• GrabFood: Purchase >= RM20 = 20% off delivery fee\n";
    cout << "=====================================\n";
}

// Function to wait for user input before continuing
void pressEnterToContinue() {
    cout << "\nPress Enter to return to main menu...";
    cin.ignore();
    cin.get();
    clearScreen();
    displayHeader(); // Show header after clearing screen
}

// Structure to store order history
struct Order {
    int orderNumber;
    string serviceType;
    double distance;
    double fare;
    int rating;
};

// Function to calculate taxi fare (Malaysian taxi rates)
double calculateTaxiFare(double distance, int hour) {
    double baseFare = 3.00;  // RM3 flag-down rate
    double perKmRate = 1.20; // RM1.20 per km (slightly cheaper base rate than Grab)

    double taxiFare = baseFare + (distance * perKmRate);

    // Taxi peak hour surcharge (similar to Grab but different multiplier)
    if ((hour >= 7 && hour <= 9) || (hour >= 17 && hour <= 19) || (hour >= 0 && hour <= 6)) {
        taxiFare *= 1.3; // 30% surcharge for taxis during peak hours and midnight
    }

    return taxiFare;
}

// Function to display price comparison
void displayPriceComparison(double grabFare, double taxiFare, double distance) {
    cout << "\n💰 PRICE COMPARISON 💰\n";
    cout << "========================\n";
    cout << fixed << setprecision(2);
    cout << "🚗 GrabCar:  RM " << grabFare << endl;
    cout << "🚕 Taxi:     RM " << taxiFare << endl;

    double difference = fabs(grabFare - taxiFare);
    double percentDiff = (difference / (grabFare < taxiFare ? grabFare : taxiFare)) * 100;

    if (grabFare < taxiFare) {
        cout << "✅ GrabCar is CHEAPER by RM " << difference << " (" << fixed << setprecision(1) << percentDiff << "%)\n";
        cout << "💡 Recommendation: Choose GrabCar for better value!\n";
    } else if (taxiFare < grabFare) {
        cout << "⚠️  Taxi is CHEAPER by RM " << difference << " (" << fixed << setprecision(1) << percentDiff << "%)\n";
        cout << "💡 Recommendation: Consider taxi for cost savings\n";
    } else {
        cout << "🤝 Both services cost the same!\n";
        cout << "💡 Recommendation: Choose based on convenience and preference\n";
    }

    cout << "📊 Cost per km - GrabCar: RM" << (grabFare/distance) << " | Taxi: RM" << (taxiFare/distance) << endl;
    cout << "========================\n";
}
