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

int main() {
    int choice, hour, rating;
    double distance, foodPrice, deliveryDistance;

    // Variables for order count and ratings
    int totalOrderCount = 0;
    int carOrderCount = 0;
    int carVoucherCount = 0;
    vector<Order> orderHistory;

    displayHeader(); // Display header at startup

    do {
        cout << "1. GrabCar\n";
        cout << "2. GrabFood\n";
        cout << "3. Available Voucher\n";
        cout << "4. Order History\n";
        cout << "5. Rating History\n";
        cout << "6. Price Comparison Tool\n";
        cout << "0. Exit\n";
        cout << "Choose your service: ";
        cin >> choice;

        double fare = 0.0;
        string serviceName;
        bool usedFoodDiscount = false;

        if (choice >= 1 && choice <= 2) {
            clearScreen(); // Clear screen when entering GrabCar or GrabFood
            displayHeader(); // Show header

            switch(choice) {
                case 1: { // GrabCar
                    cout << "Enter distance (km): ";
                    cin >> distance;
                    fare = 3.0 + (distance * 1.5);
                    serviceName = "GrabCar";

                    // Apply GrabCar voucher if available
                    double originalFare = fare;
                    if (carVoucherCount > 0) {
                        cout << "Use LDCW6123 voucher for 10% discount? (y/n): ";
                        char useVoucher;
                        cin >> useVoucher;
                        if (useVoucher == 'y' || useVoucher == 'Y') {
                            double discount = fare * 0.10;
                            fare -= discount;
                            carVoucherCount--;
                            cout << "✅ LDCW6123 voucher applied! Discount: RM " << discount << endl;
                            cout << "Car vouchers remaining: " << carVoucherCount << endl;
                        }
                    }
                    break;
                }

                case 2: { // GrabFood
                    cout << "Enter food price (RM): ";
                    cin >> foodPrice;
                    cout << "Enter distance from restaurant to your location (km): ";
                    cin >> deliveryDistance;

                    // Calculate delivery fee based on distance
                    double deliveryFee = 3.0 + (deliveryDistance * 1.5);

                    // Apply GrabFood discount if food price >= RM20
                    if (foodPrice >= 20.0) {
                        double foodDiscount = deliveryFee * 0.20;
                        deliveryFee -= foodDiscount;
                        usedFoodDiscount = true;
                        cout << "✅ GrabFood discount applied! Delivery fee discount: RM " << foodDiscount << endl;
                    }

                    fare = foodPrice + deliveryFee;
                    serviceName = "GrabFood";
                    distance = deliveryDistance; // Store delivery distance
                    break;
                }
            }

            // Time-based surge pricing
            cout << "Enter current hour (0 - 23): ";
            cin >> hour;

            double surgeMultiplier = 1.0;
            if ((hour >= 7 && hour <= 9) || (hour >= 17 && hour <= 19)) {
                surgeMultiplier = 1.5;
                cout << "⚠ Peak hour! Surge pricing applied (x1.5)\n";
            }
            fare *= surgeMultiplier;

            // Show price comparison for GrabCar only
            if (choice == 1) {
                double taxiFare = calculateTaxiFare(distance, hour);
                displayPriceComparison(fare, taxiFare, distance);

                // Ask if user wants to proceed with GrabCar
                cout << "Do you want to proceed with GrabCar booking? (y/n): ";
                char proceed;
                cin >> proceed;
                if (proceed != 'y' && proceed != 'Y') {
                    cout << "Booking cancelled. Returning to main menu...\n\n";
                    continue;
                }
            }

            // Final output
            cout << fixed << setprecision(2);
            cout << "\n--- Grab Simulator Receipt ---\n";
            cout << "Service: " << serviceName << endl;
            if (choice == 2) { // GrabFood details
                cout << "Food Price: RM " << foodPrice << endl;
                cout << "Delivery Distance: " << deliveryDistance << " km" << endl;
                double originalDeliveryFee = 3.0 + (deliveryDistance * 1.5);
                if (usedFoodDiscount) {
                    cout << "Original Delivery Fee: RM " << originalDeliveryFee << endl;
                    cout << "Discounted Delivery Fee: RM " << (originalDeliveryFee * 0.80) << endl;
                } else {
                    cout << "Delivery Fee: RM " << originalDeliveryFee << endl;
                }
            } else {
                cout << "Distance: " << distance << " km" << endl;
            }
            cout << "Total Fare: RM " << fare << endl;

            // Rating system
            cout << "Rate your experience (1-5 stars): ";
            cin >> rating;
            while (rating < 1 || rating > 5) {
                cout << "Please enter a valid rating (1-5): ";
                cin >> rating;
            }

            // Save order details
            Order newOrder;
            newOrder.orderNumber = totalOrderCount + 1;
            newOrder.serviceType = serviceName;
            newOrder.distance = distance;
            newOrder.fare = fare;
            newOrder.rating = rating;
            orderHistory.push_back(newOrder);

            totalOrderCount++;

            // Only count GrabCar orders for car vouchers
            if (choice == 1) {
                carOrderCount++;

                // Check if user qualifies for car voucher (every 5 GrabCar orders)
                if (carOrderCount % 5 == 0) {
                    carVoucherCount++;
                    cout << "🎉 Congratulations! You've earned a LDCW6123 voucher for GrabCar!\n";
                    cout << "Car vouchers: " << carVoucherCount << endl;
                }
            }

            cout << "Thank you for using Grab!\n";
            pressEnterToContinue(); // Wait for user input before returning to main menu
        }
        else if (choice == 3) { // Available Voucher
            clearScreen(); // Clear screen
            displayHeader(); // Show header

            cout << "\n--- Available Voucher ---\n";

            cout << "🚗 GrabCar Vouchers:\n";
            if (carVoucherCount > 0) {
                cout << "✅ LDCW6123 Vouchers Available: " << carVoucherCount << endl;
                cout << "Discount: 10% off total fare\n";
                cout << "How to use: Will be offered during GrabCar checkout\n";
            } else {
                cout << "No car vouchers available\n";
                int carOrdersNeeded = 5 - (carOrderCount % 5);
                if (carOrdersNeeded == 5) {
                    cout << "Complete 5 GrabCar orders to earn your first car voucher\n";
                } else {
                    cout << "Complete " << carOrdersNeeded << " more GrabCar orders to earn next car voucher\n";
                }
            }

            cout << "\n🍔 GrabFood Promotions:\n";
            cout << "✅ Always available: Purchase >= RM20 = 20% off delivery fee\n";
            cout << "Condition: Food price must be RM20 or more\n";
            cout << "Discount: 20% off delivery fee only\n";

            pressEnterToContinue(); // Wait for user input
        }
        else if (choice == 4) { // Order History
            clearScreen(); // Clear screen
            displayHeader(); // Show header

            cout << "\n--- Order History ---\n";
            cout << "Total Orders Completed: " << totalOrderCount << endl;
            cout << "GrabCar Orders: " << carOrderCount << endl;
            cout << "GrabFood Orders: " << (totalOrderCount - carOrderCount) << endl;
            cout << "Available Car Vouchers: " << carVoucherCount << endl;

            if (totalOrderCount > 0) {
                cout << "\nDetailed Order History:\n";
                cout << "======================\n";
                for (const auto& order : orderHistory) {
                    cout << "Order #" << order.orderNumber << endl;
                    cout << "Service: " << order.serviceType << endl;
                    if (order.serviceType == "GrabCar") {
                        cout << "Distance: " << order.distance << " km" << endl;
                    } else if (order.serviceType == "GrabFood") {
                        cout << "Delivery Distance: " << order.distance << " km" << endl;
                    }
                    cout << "Fare: RM " << fixed << setprecision(2) << order.fare << endl;
                    cout << "Rating: " << order.rating << "/5" << endl;
                    cout << "------------------------" << endl;
                }
            } else {
                cout << "No orders completed yet\n";
            }

            pressEnterToContinue(); // Wait for user input
        }
        else if (choice == 5) { // Rating History
            clearScreen(); // Clear screen
            displayHeader(); // Show header

            cout << "\n--- Rating History ---\n";
            if (totalOrderCount > 0) {
                double totalRatingPoints = 0;
                for (const auto& order : orderHistory) {
                    totalRatingPoints += order.rating;
                }
                double averageRating = totalRatingPoints / totalOrderCount;

                cout << "Total Orders: " << totalOrderCount << endl;
                cout << "GrabCar Orders: " << carOrderCount << endl;
                cout << "GrabFood Orders: " << (totalOrderCount - carOrderCount) << endl;
                cout << "Average Rating: " << fixed << setprecision(1) << averageRating << "/5\n";
                cout << "Total Rating Points: " << totalRatingPoints << endl;

                cout << "\nDetailed Rating History:\n";
                cout << "=======================\n";
                for (const auto& order : orderHistory) {
                    cout << "Order #" << order.orderNumber << " (" << order.serviceType << "): ";
                    cout << order.rating << "/5 stars" << endl;
                }
            } else {
                cout << "No orders completed yet\n";
            }

            pressEnterToContinue(); // Wait for user input
        }
        else if (choice == 6) { // Price Comparison Tool
            clearScreen(); // Clear screen
            displayHeader(); // Show header

            cout << "\n--- Price Comparison Tool ---\n";
            cout << "Compare GrabCar vs Traditional Taxi fares\n";
            cout << "Enter distance (km): ";
            double compDistance;
            cin >> compDistance;
            cout << "Enter current hour (0-23): ";
            int compHour;
            cin >> compHour;

            // Calculate both fares
            double grabFare = 3.0 + (compDistance * 1.5);
            double surgeMultiplier = 1.0;
            if ((compHour >= 7 && compHour <= 9) || (compHour >= 17 && compHour <= 19)) {
                surgeMultiplier = 1.5;
                cout << "⚠ Peak hour detected! Surge pricing will be applied\n";
            }
            grabFare *= surgeMultiplier;

            double taxiFare = calculateTaxiFare(compDistance, compHour);

            displayPriceComparison(grabFare, taxiFare, compDistance);

            // Additional insights
            cout << "\n📋 Additional Insights:\n";
            cout << "• GrabCar: Cashless, GPS tracking, driver ratings\n";
            cout << "• Taxi: Cash/card accepted, traditional service\n";
            cout << "• Peak hours: 7-9 AM, 5-7 PM\n";
            cout << "• Taxi also charges extra during midnight (12-6 AM)\n";

            pressEnterToContinue(); // Wait for user input
        }
        else if (choice != 0) {
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    cout << "Thank you for using Grab Simulator!\n";
    return 0;
}
