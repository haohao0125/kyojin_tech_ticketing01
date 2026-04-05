// Ticket System.cpp 

#include <iostream>
#include <fstream>   //to use ifstream and ofstream to read and write file data
#include <string>   //to use string
#include <vector>   //to use vector
#include <conio.h>  //to use _getch() for password hiding
#include <cctype>  //to use check validation isdigit in the validation addnewDate
#include <iomanip> //to use the fixed << setprecision(2)
#include <sstream> //to use the ostringstream 

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
string showDate, loginRole;


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
    loadPrices();
    cout << "Current Price List : "<<endl;

    for (int row = 1; row <= sizeRow; row++)
    {
        cout << "Row " << row << " = " << ticketPrice[row - 1] << " RM";
        cout << endl;
    }
}
void loadShowDateData()
{

    ifstream infile("showdatedata.txt");

    while (infile >> showDate)
    {
        showDateData.push_back(showDate);
    }

    infile.close();

}
void addShowDates()
{   
    while (true)
    {
        bool correctDate = true;
        bool correctMonth = true;
        bool correctYear = true;
        bool valid = true;

        cout << "Enter a date of the Show (DD-MM-YYYY): ";
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

        if (valid && correctDate && correctMonth && correctYear)
            break;
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
            std::cout << "Successfully added a new  date !";

            ofstream updateDate("showdatedata.txt");
            for (auto& u : showDateData)
            {
                updateDate << u << endl;
            }

            updateDate.close();
        }

        else
        {
            cout << "Invalid Input ! Date already Exist !";
        }  

        file.close(); 
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
                    printGreen("login Success !\n\n");
                    successlogin = true;
                    u.attempts = 0;
                    loginRole = u.roles;
                }

                else if (u.attempts >= 3)
                {
                    printRed("Username Blocked ! 3 Attempts failed , Contact admin to reset attempts !\n\n");
                }

                else
                {
                    printRed("Login Failed , Wrong password !\n");
                    u.attempts++;
                    printRed(to_string(u.attempts) + " Wrong Attempts !\n");
                    cout << "3 Wrong attempts will block your account !\n";
                }
            }
        }

        if (!userfound)
        {
            printRed("Login Failed ! Username did not exist !\n\n");
        }

        ofstream outfile("user.txt");
        for (auto& u : userDataVector)
        {
            outfile << u.uname << " " << u.pword << " " << u.roles << " " << u.attempts << "\n";
        }

        outfile.close();

        loadShowDateData();

    }
}
void logout()
{
    cout << "Log out Success !\n";
    successlogin = false;

    login();
}
void showAvailableDates()
{
    int inputOption;
    bool validInput = false;

    for (int i = 1; i <= showDateData.size(); i++)
    {
        cout << i << ". " << showDateData[i - 1]<<endl;
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
void bookticket()
{
    int bookcount, rowbook, colbook;
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
    cout << "How many seat you would like to book ? (input 0 to cancel and back to main menu)" << endl;
    cout << "Ticket count : ";
    cin >> bookcount;
    cout << endl;

    if (bookcount == 0)
    {
        return; // go back to menu
    }

    for (int c = 0; c < bookcount; c++)
    {
        cout << "Input the seat you wanted to book ! \n";
        cout << "Row : ";
        cin >> rowbook;
        cout << "Col : ";
        cin >> colbook;

        if (rowbook < 1 || rowbook > 15 || colbook < 1 || colbook > 20)
        {
            printRed("Invalid seat! Try again.\n");
            cout << endl;
            c--; // retry
        }
        else if (seat[rowbook - 1][colbook - 1] > 0)
        {
            cout << "Seat already booked!\n";
            cout << endl;
            c--; // retry
        }
        else
        {
            seat[rowbook - 1][colbook - 1] = static_cast<int>(ticketPrice[rowbook - 1]);
            printGreen("Ticket Booked Successfully\n");
            cout<< endl;

            saveSeats();
        }

    }

}
void administratorMenu()
{
    while (true)
    {

        char menuOption;

        cout << "\n================================================================\n";
        cout << "Hello Administrator ! \nPress the number to access the menu !\n";
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
void editPriceList()
{

    int rowEdit = 0;
    double newPrice = 0.00;
    bool changingPrice = true;

    cout<< "\n================================================================\n";

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

            }
        }
    }

    


}
void FinancialReport()
{
    showAvailableDates();
    loadSeats();
    loadPrices();

    int ticketSold[sizeRow] = { 0 };
    int salesPerRow[sizeRow] = { 0 };
    int totalSales = 0;

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
        cout << "Row " << row + 1 << ". Ticket Sold = " << ticketSold[row] << ". Total Sales on Row " << row+1 << " = " << salesPerRow[row] << " RM" << endl;
        totalSales = totalSales + salesPerRow[row];
    }
    
    printGreen("\nTotal Sales = " + to_string(totalSales) + " RM \n");

}
void managerMenu()
{
    while (true)
    {
        char menuOption;

        cout << "\n================================================================\n";
        cout << "Hello Manager ! \nPress the number to access the menu !\n";
        cout << "1. Financial Report" << endl;
        cout << "2. Add a new Show (date)" << endl;
        cout << "3. Edit Price list" << endl;
        cout << "4. Log out\n" << endl;


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

        case '4': logout();
            break;

        }
    }
}
void showMenu(string loginRole)
{
    if (loginRole == "administrator")
    {
        administratorMenu();
    }

    else if (loginRole == "manager")
    {
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
