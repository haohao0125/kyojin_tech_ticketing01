// Ticket System.cpp 

#include <iostream>
#include <fstream>   //to use ifstream and ofstream to read and write file data
#include <string>   //to use string
#include <vector>   //to use vector
#include <conio.h>  //to use _getch() for password hiding
#include <cctype>  //to use check validation isdigit in the validation addnewDate
#include <iomanip> //to use the fixed << setprecision(2)
#include <sstream> //to use the ostringstream 
#include <cstdlib> // to use system("cls") to clear console data

using namespace std;


namespace Color {
    const string RED = "\033[31m";
    const string GREEN = "\033[32m";
    const string RESET = "\033[0m";
}
int const sizeRow = 15 , sizeCol = 20;
int seat[sizeRow][sizeCol];
double ticketPrice[sizeRow];
bool successlogin = false;
vector<string> showDateData;
vector<string> paymentMethodData;
string showDate , loginRole , paymentMethod;

void pauseScreen()
{
    cout << endl;
    cout << "\nPress any key to continue...";
    (void)_getch();
    system("cls");
}
void printRed(const string& text) {
    cout << Color::RED << text << Color::RESET;
}
void printGreen(const string& text) {
    cout << Color::GREEN << text << Color::RESET;
}
string getPassword()
{
    string password;
    char ch;

    while (true)
    {
        ch = _getch(); // get character without showing it

        if (ch == 13) 
        { break; } // 13 is enter key in ASCII Code

        else if (ch == 8) 
        { // Backspace
            if (!password.empty()) 
            {
                password.pop_back();
                cout << "\b \b"; // remove * from screen
            }
        }
        else 
        {
            password += ch;
            cout<< "*"; // show *
        }
    }

    cout << endl;
    return password;
}
void saveSeats()
{
    ofstream file(showDate);

    for (int row = 0; row < 15; row++)
    {
        for (int col = 0; col < 20; col++)
        {
            file << seat[row][col] << " ";
        }
        file << endl;
    }

    file.close();

} 
void loadSeats()
{
    ifstream file(showDate);

    if (!file) // if file doesn't exist yet
    {
        // initialize all seats to 0
        for (int row = 0; row < 15; row++)
        {
            for (int col = 0; col < 20; col++)
            {
                seat[row][col] = 0;
            }
        }
        return;
    }

    for (int row = 0; row < 15; row++)
    {
        for (int col = 0; col < 20; col++)
        {
            file >> seat[row][col];
        }
    }

    file.close();
}
void loadPrices()
{
    ifstream fileRead("priceList.txt");

    for (int row = 0; row < 15; row++)
    {
        fileRead >> ticketPrice[row];
    }

    fileRead.close();
}
void savePrices()
{
    ofstream fileSave("priceList.txt");

    for (int row = 0; row < 15; row++)
    {
        fileSave << fixed << setprecision(2) << ticketPrice[row] << endl;
    }

    fileSave.close();
}
void showPriceList()
{
    system("cls");
    loadPrices();
    cout << "Current Price List : "<<endl;

    for (int row = 1; row <= sizeRow; row++)
    {
        cout << "Row " << row << " = " << ticketPrice[row - 1] << " RM";
        cout << endl;
    }

    if (loginRole == "administrator")
    {
        pauseScreen();
    }
}
void editPriceList()
{
    system("cls");
    int rowEdit = 0;
    double newPrice = 0.00;
    bool changingPrice = true;

    cout << "\n================================================================\n";

    loadPrices();
    showPriceList();

    cout << endl;

    while (changingPrice)
    {
        cout << "Input 0 to abort and back to Main Menu!" << endl;
        cout << "Input the row you wanted to edit the price : ";
        cin >> rowEdit;
        cout << endl;

        if (rowEdit < 0 or rowEdit>15)
        {
            printRed("Invalid Input ! , Please input the correct row !\n");
            cout << endl;
        }

        else if (rowEdit == 0)
        {
            changingPrice = false;
            cout << "Back to Main Menu....." << endl;
            break;
        }

        else
        {
            cout << "Input the new prices for that row (RM) : ";
            cin >> newPrice;
            cout << endl;

            if (newPrice < 1)
            {
                printRed("Please input the correct price !\n");
                cout << endl;
            }

            else
            {
                ostringstream text;
                text << fixed << setprecision(2) << newPrice;

                ticketPrice[rowEdit - 1] = newPrice;

                savePrices();

                printGreen("Successfully update the price for row ");
                printGreen(to_string(rowEdit) + " To " + text.str() + " (RM)\n");
                break;
            }
        }
    }

    pauseScreen();
}
void loadShowDateData()
{
    showDateData.clear();

    ifstream infile("showdatedata.txt");

    while (infile >> showDate)
    {
        showDateData.push_back(showDate);
    }

    infile.close();

}
void showAvailableDates()
{
    int inputOption;
    bool validInput = false;

    cout << "Current Show Date : " << endl;

    for (int i = 1; i <= showDateData.size(); i++)
    {
        cout << i << ". " << showDateData[i - 1] << endl;
    }

    while (!validInput)
    {
        cout << endl;
        cout << "Input the Date you want to choose : ";
        cin >> inputOption;

        if (inputOption<1 || inputOption> showDateData.size())
        {
            printRed("Invalid Input ! Please input the correct number to choose !");
        }

        else
        {
            validInput = true;
        }

    }

    showDate = showDateData[inputOption - 1];

}
void addShowDates()
{
    cout << "Current Show Date : " << endl;

    for (int i = 1; i <= showDateData.size(); i++)
    {
        cout << i << ". " << showDateData[i - 1] << endl;
    }


    while (true)
    {
        bool correctDate = true;
        bool correctMonth = true;
        bool correctYear = true;
        bool valid = true;

        cout << "Enter a date of the Show (Put in format DD-MM-YYYY): ";
        cin >> showDate;


        if (showDate.length() != 10)
        {
            valid = false;
        }

        for (int i = 0; i < showDate.length() && valid; i++)
        {
            if (i == 2 || i == 5)
            {
                if (showDate[i] != '-')
                    valid = false;
            }
            else
            {
                if (!isdigit(showDate[i]))
                    valid = false;
            }
        }

        if (!valid)
            printRed("Input Invalid ! Input the correct Date , Month and Year using correct number and format !\n");

        if (valid)
        {
            int day = stoi(showDate.substr(0, 2));
            int month = stoi(showDate.substr(3, 2));
            int year = stoi(showDate.substr(6, 4));
            int daysInMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

            if (year < 2026)
            {
                printRed("Invalid Year! , Please input the correct Year!\n");
                correctYear = false;
            }

            if (month < 1 || month>12)
            {
                printRed("Invalid Month! , Please input the correct Month!\n");
                correctMonth = false;
            }

            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            {
                daysInMonth[1] = 29;
            }

            if (correctMonth == true)
            {
                if (day < 1 || day > daysInMonth[month - 1])
                {
                    printRed("Invalid Date! , Please input the correct Date!\n");
                    correctDate = false;
                }

            }

            if (correctDate && correctMonth && correctYear)
                break;
        }
    }


    ifstream file(showDate);

    if (!file) // if file doesn't exist yet
    {
        // initialize all seats to 0
        for (int row = 0; row < 15; row++)
        {
            for (int col = 0; col < 20; col++)
            {
                seat[row][col] = 0;
            }
        }

        saveSeats();
        showDateData.push_back(showDate);
        printGreen("Successfully added a new  date !");

        ofstream updateDate("showdatedata.txt");
        for (auto& u : showDateData)
        {
            updateDate << u << endl;
        }

        updateDate.close();

        pauseScreen();
    }

    else
    {
        printRed("Invalid Input ! Date already Exist !");
    }

    file.close();
    pauseScreen();
}
void loadPaymentMethodData()
{
    paymentMethodData.clear();

    ifstream infile("PaymentOption.txt");

    while (infile >> paymentMethod)
    {
        paymentMethodData.push_back(paymentMethod);
    }

    infile.close();
}
void showPaymentMethod()
{
    for (int i = 1; i <= paymentMethodData.size(); i++)
    {
        cout << i << ". " << paymentMethodData[i - 1] << endl;
    }
}
void addPaymentMethod()
{
    loadPaymentMethodData();
    showPaymentMethod();

    while (true)
    {
        bool validInput = true;

        cout << "Input New Payment Method (press 0 to cancel) : ";
        cin >> paymentMethod;

        for (auto& s : paymentMethodData)
        {
            if (s == paymentMethod)
            {
                printRed("Payment Method already Exist !\n");
                validInput = false;
            }
        }

        if (validInput == true)
        {
            break;
        }
    }

    if (paymentMethod != "0")
    {
        printGreen("Successfully add Payment Method!");
        paymentMethodData.push_back(paymentMethod);

        ofstream addPaymentMethod("PaymentOption.txt");
        for (auto& u : paymentMethodData)
        {
            addPaymentMethod << u << endl;
        }

        addPaymentMethod.close();
    }

    else
    {
        cout << "Going Back to main menu ....";
    }
    
    pauseScreen();

}
void choosePaymentMethod()
{
    int inputOption;
    bool validInput = false;

    loadPaymentMethodData();

    cout << "=========Payment Method========== \n";

    for (int i = 1; i <= paymentMethodData.size(); i++)
    {
        cout << i << ". " << paymentMethodData[i - 1] << endl;
    }

    while (!validInput)
    {
        cout << endl;
        cout << "Input the payment method you want to choose : ";
        cin >> inputOption;

        if (cin.fail())
        {
            cin.clear(); // reset error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
            printRed("Invalid Input! Please enter a NUMBER.\n");
            continue;
        }

        if (inputOption > 0 && inputOption < paymentMethodData.size())
        {
            validInput = true;
            break;
        }

        else
        {
            printRed("Invalid Input ! Please input the correct number to choose.\n");
            validInput = false;
        }

    }

    paymentMethod = paymentMethodData[inputOption - 1];

    return;

}
void login()
{
    string username, password;

    while (successlogin == false)
    {
        struct user
        {
            string uname;
            string pword;
            string roles;
            int attempts;
        };

        system("cls");

        cout << "\n================================================================\n";
        cout << "                      Login to System                           \n";
        cout << "================================================================\n";
        cout << "Username : ";
        cin >> username;
        cout << "Password : ";
        password = getPassword();

        vector<user> userDataVector;
        string fileuname, filepword, fileroles;
        int loginattempts;


        ifstream infile("user.txt");

        while (infile >> fileuname >> filepword >> fileroles >> loginattempts)
        {
            userDataVector.push_back({ fileuname , filepword, fileroles , loginattempts });
        }

        infile.close();

        bool userfound = false;

        for (auto& u : userDataVector)
        {
            if (username == u.uname)
            {
                userfound = true;
                if (password == u.pword && u.attempts != 3)
                {
                    printGreen("login Success !\n");
                    successlogin = true;
                    u.attempts = 0;
                    loginRole = u.roles;
                    loadShowDateData();
                    pauseScreen();
                }

                else if (u.attempts >= 3)
                {
                    printRed("Username Blocked ! 3 Attempts failed , Contact admin to reset attempts !\n");
                    pauseScreen();
                }

                else
                {
                    printRed("Login Failed , Wrong password !\n");
                    u.attempts++;
                    printRed(to_string(u.attempts) + " Wrong Attempts !\n");
                    cout << "3 Wrong attempts will block your account !";
                    pauseScreen();
                }
            }
        }

        if (!userfound)
        {
            printRed("Login Failed ! Username did not exist !\n\n");
            pauseScreen();
        }

        ofstream outfile("user.txt");
        for (auto& u : userDataVector)
        {
            outfile << u.uname << " " << u.pword << " " << u.roles << " " << u.attempts << "\n";
        }

        outfile.close();

        

    }
}
void logout()
{
    system("cls");
    cout << "Log out Success !\n";
    successlogin = false;
    pauseScreen();
    exit(0);
}
void printReceipt(int bookcount, int rowbook[], int colbook[], double totalPayment)
{
    cout << "\n=====================================\n";
    cout << "            RECEIPT\n";
    cout << "=====================================\n";
    cout << "Date: " << showDate << endl;
    cout << "-------------------------------------\n";

    for (int i = 0; i < bookcount; i++)
    {
        cout << i + 1 << ". Row " << rowbook[i]
            << " Col " << colbook[i]
            << " | RM " << fixed << setprecision(2)
            << ticketPrice[rowbook[i] - 1] << endl;
    }

    cout << "Payment Method : " << paymentMethod << endl;

    cout << "-------------------------------------\n";
    cout << "Total: " << fixed << setprecision(2) << totalPayment << " RM" << endl;
    cout << "=====================================\n";

    while (true)
    {
        int input;
        bool validInput;

        cout << "\nPress 1 if Payment completed and press 2 if cancel\n";
        cin >> input;

        if (input == 1)
        {
            printGreen("Payment Successful !\n");
            printGreen("You booking is success !\n");
            saveSeats();
            pauseScreen();
            break;
        }
       
        if (input == 2)
        {
            return;
        }

        if (cin.fail())
        {
            cin.clear(); // reset error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
            printRed("Invalid Input! Please enter a NUMBER.\n");
            continue;
        }

        else
        {
            printRed("Invalid Input ! Please input the proper option\n");
        }


    }
    

    

    
}
double proceedToPayment(int bookcount, int rowbook[], int colbook[])
{
    system("cls");
    char playerOption;
    double totalPayment = 0.00 ;

    cout << "=========Ticket Details========== \n";
    cout << "Date : " << showDate << endl;

    for (int c = 0; c < bookcount; c++)
    {
        cout << c + 1 << ". Row " << rowbook[c] << " Col " << colbook[c] << ". Price = " << ticketPrice[rowbook[c]-1] << " RM." << endl;
        totalPayment = totalPayment + ticketPrice[rowbook[c]-1];
    }


    while (true)
    {
        cout << "\nGrand Total " << totalPayment << " RM."<< endl;
        cout << "\nis Seat confirmed and proceed to payment ? (Y to proceed , N to Cancel) \n";
        cin >> playerOption;

        if (playerOption == 'Y' || playerOption == 'y')
        {
            break;
        }

        else if (playerOption == 'N' || playerOption == 'n')
        {
            system("cls");
            return totalPayment;
        }

        else
        {
            printRed("Invalid Input ! Please input the correct option !\n");
        }
    }

    choosePaymentMethod();
    printReceipt(bookcount ,rowbook , colbook , totalPayment );

}
void bookticket()
{
    system("cls");
    int bookcount;
    int rowbook[10] = { 0 };
    int colbook[10] = { 0 };
    showAvailableDates();
    loadSeats();
    loadPrices();

    cout << "                                         Seat                                              " << endl;
    cout << "===========================================================================================" << endl;
    cout << "\t1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20" << endl;

    for (int row = 0; row < 15; row++)
    {
        cout << "Row " << (row + 1) << "\t";

        for (int col = 0; col < 20; col++)
        {
            if (seat[row][col] == 0)
            {
                printRed("X   ");
            }

            else
            {
                printGreen("O   ");
            }
        }

        cout << endl;
    }

    cout << endl;

    while (true)
    {
        cout << "How many seat you would like to book ? (input 0 to cancel and back to main menu)" << endl;
        cout << "Ticket count (Maximum 10) : ";
        cin >> bookcount;
        cout << endl;

        if (bookcount > 10)
        {
            printRed("Maximum purchase is 10 !");
            cout << endl;
            cout << endl;

        }

        else if (bookcount == 0)
        {
            system("cls");
            return; // go back to menu
        }

        else
        {
            break;
        }

    }
    

    for (int c = 0; c < bookcount; c++)
    {
        cout << "Input the seat you wanted to book ! \n";
        cout << "Row : ";
        cin >> rowbook[c];
        cout << "Col : ";
        cin >> colbook[c];

        if (rowbook[c] < 1 || rowbook[c] > 15 || colbook[c] < 1 || colbook[c] > 20)
        {
            printRed("Invalid seat! Try again.\n");
            cout << endl;
            c--; // retry
        }
        else if (seat[rowbook[c] - 1][colbook[c] - 1] > 0)
        {
            printRed("Seat already booked!\n");
            cout << endl;
            c--; // retry
        }
        else
        {
            seat[rowbook[c] - 1][colbook[c] - 1] = static_cast<int>(ticketPrice[rowbook[c] - 1]);
            cout<< endl;
        }

    }

    proceedToPayment(bookcount, rowbook, colbook);
}
void FinancialReport()
{
    system("cls");

    cout << "Show Date : \n";
    showAvailableDates();
    loadSeats();
    loadPrices();

    int ticketSold[sizeRow] = { 0 };
    int salesPerRow[sizeRow] = { 0 };
    int totalTicketSold = 0;
    int totalSales = 0;

    cout << "                                                 Seat                                             " << endl;
    cout << "======================================================================================================" << endl;
    cout << "\t 1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20" << endl;

    for (int row = 0; row < 15; row++)
    {
        cout << "Row " << (row + 1) << "\t";

        for (int col = 0; col < 20; col++)
        {
            if (seat[row][col] == 0)
            {
                printRed(" X   ");
            }

            else
            {
                printGreen(to_string(seat[row][col]) + "   ");
            }
        }

        cout << endl;
    }

    cout << endl;


    for (int row = 0; row < 15; row++)
    {
        for (int col = 0; col < 20; col++)
        {
            if (seat[row][col] > 0)
            {
                ticketSold[row]++;
                salesPerRow[row] += seat[row][col];
            }
        }
    }

    cout << "\n================================================================\n";
    cout << "Total Seat Sold for Show Date : " << showDate << " are :" << endl;

    for (int row = 0; row < 15; row++)
    {
        cout << "Row " << row + 1 << ". Ticket Sold = " << ticketSold[row] 
            << ". \t\t| Total Sales on Row " << row + 1 << " = " << salesPerRow[row] << " RM" << endl;
        totalSales = totalSales + salesPerRow[row];
        totalTicketSold = totalTicketSold + ticketSold[row];
    }

    printGreen("\nTotal Sales = " + to_string(totalSales) + " RM \n");
    printGreen("Total Ticket Sold = " + to_string(totalTicketSold) + " Ticket.\n");
    cout << "Total Available seat for Show Date : " << showDate << " are " << (300 - totalTicketSold) << endl;

    pauseScreen();
}
void registerNewUser()
{
    system("cls");
    string username, password, role;
    int optionRegister;
    bool haveCharacter = false, haveNumber = false;

    struct user
    {
        string uname;
        string pword;
        string roles;
        int attempts;
    };

    vector<user> userDataVector;
    string fileuname, filepword, fileroles;
    int loginattempts;

    ifstream infile("user.txt");

    while (infile >> fileuname >> filepword >> fileroles >> loginattempts)
    {
        userDataVector.push_back({ fileuname , filepword, fileroles , loginattempts });
    }

    infile.close();


    while (true)
    {
        cout << "Select roles to register : " << endl;
        cout << "1. Administrator" << endl;
        cout << "2. Manager" << endl;
        cin >> optionRegister;

        if (optionRegister == 1)
        {
            role = "administrator";
            break;
        }

        else if (optionRegister == 2)
        {
            role = "manager";
            break;
        }

        else
        {
            printRed("Invalid Input , Please input the correct option");

        }

    }



    while (true)
    {
        cout << endl;
        cout << "\nUsername Min 5 character and Max 12 character ! " << endl;
        cout << "Input your desired username : ";
        cin >> username;

        bool userfound = false;

        for (auto& u : userDataVector)
        {
            if (username == u.uname)
            {
                userfound = true;
            }
        }


        if (username.length() < 5)
        {
            printRed("Invalid Input , Minimum 5 character");
        }

        else if (username.length() > 12)
        {
            printRed("Invalid Input , MAximum 12 character");
        }

        else if (userfound == true)
        {
            printRed("Username already registered ! Please use the other username !");
        }

        else
        {
            break;

        }
    }

    while (true)
    {
        cout << endl;
        cout << "\nPassword Minimum 5 character and Maximal 12 character with combination character and number ! " << endl;
        cout << "Input your desired password : ";
        cin >> password;

        for (char c : password)
        {
            if (isdigit(c))
            {
                haveNumber = true;
            }

            else if (isalpha(c))
            {
                haveCharacter = true;
            }
        }

        if (password.length() < 5)
        {
            printRed("Invalid Input , Minimum 5 character");
        }

        else if (password.length() > 12)
        {
            printRed("Invalid Input , MAximum 12 character");
        }

        else if (!haveNumber || !haveCharacter)
        {
            printRed("Invalid Input , Should have at least 1 character or 1 number");
        }

        else
        {
            userDataVector.push_back({ username , password, role , 0 });
            break;
        }
    }

    ofstream outfile("user.txt");
    for (auto& u : userDataVector)
    {
        outfile << u.uname << " " << u.pword << " " << u.roles << " " << u.attempts << "\n";
    }

    outfile.close();

    printGreen("Success Create User !");
    cout << endl;

    pauseScreen();

}
void administratorMenu()
{
    while (true)
    {

        char menuOption;

        cout << "\n===========Administrator==========\n";
        cout << "Press the number to access the menu !\n";
        cout << "1. Book Ticket" << endl;
        cout << "2. View Pricelist" << endl;
        cout << "3. Log out\n" << endl;

        cout << "Input Number to Access : ";
        cin >> menuOption;

        while (true)
        {
            if (menuOption != '1' && menuOption != '2' && menuOption != '3')
            {
                cout << "Invalid Input , Please input the correct option !\n";
                cin >> menuOption;
            }

            else
                break;
        }

        switch (menuOption)
        {
        case '1': bookticket();
            break;
        case '2': showPriceList();
            break;
        case '3': logout();
            break;
        }
    }

}
void managerMenu()
{
    while (true)
    {
        char menuOption;

        cout << "\n==========Manager==========\n";
        cout << "Press the number to access the menu !\n";
        cout << "1. Financial Report" << endl;
        cout << "2. Add a new Show (date)" << endl;
        cout << "3. Edit Price list" << endl;
        cout << "4. Add New Payment Method" << endl;
        cout << "5. Register new User" << endl;
        cout << "6. Log out\n" << endl;


        cout << "Input Number to Access : ";
        cin >> menuOption;

        switch (menuOption)
        {
        case '1': FinancialReport();
            break;

        case '2': addShowDates();
            break;

        case '3': editPriceList();
            break;

        case '4': addPaymentMethod();
            break;

        case '5': registerNewUser();
            break;

        case '6': logout();
            break;

        }
    }
}
void showMenu(string loginRole)
{
    if (loginRole == "administrator")
    { 
        system("cls");
        administratorMenu();
    }

    else if (loginRole == "manager")
    {
        system("cls");
        managerMenu();
    }
}


int main()
{   
    login();

    while (successlogin = true)
    {
        showMenu(loginRole);
    }
}
