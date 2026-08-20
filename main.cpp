#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include<algorithm>

using namespace std;

int main()
{
    int choice = 0;
    string groupName;

    vector<string> members;
    vector<double> balances;

    do
    {
        // Main menu
        cout << "\n=========================\n";
        cout << "      SPLITMATE APP\n";
        cout << "=========================\n";
        cout << "1. Create a group\n";
        cout << "2. Add an expense\n";
        cout << "3. View balances\n";
        cout << "4. Settle debts\n";
        cout << "5. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        // Option 1: Create a group
        if (choice == 1)
        {
            int numberOfMembers;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nEnter group name: ";
            getline(cin, groupName);

            cout << "Enter number of members: ";
            cin >> numberOfMembers;

            if (numberOfMembers <= 0)
            {
                cout << "The group must have at least one member.\n";
                continue;
            }

            members.clear();
            balances.assign(numberOfMembers, 0.0);

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            for (int i = 0; i < numberOfMembers; i++)
            {
                string memberName;

                cout << "Enter member " << i + 1 << " name: ";
                getline(cin, memberName);

                members.push_back(memberName);
            }

            cout << "\nGroup created successfully!\n";
            cout << "Group: " << groupName << "\n";
            cout << "Members:\n";

            for (int i = 0; i < members.size(); i++)
            {
                cout << i + 1 << ". " << members[i] << "\n";
            }
        }

        // Option 2: Add an expense
        else if (choice == 2)
        {
            if (members.empty())
            {
                cout << "\nPlease create a group first.\n";
            }
            else
            {
                string expenseName;
                double amount;
                int payer;

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "\nEnter expense name: ";
                getline(cin, expenseName);

                cout << "Enter expense amount: ";
                cin >> amount;

                if (amount <= 0)
                {
                    cout << "The expense amount must be greater than zero.\n";
                    continue;
                }

                cout << "\nWho paid for this expense?\n";

                for (int i = 0; i < members.size(); i++)
                {
                    cout << i + 1 << ". " << members[i] << "\n";
                }

                cout << "Enter member number: ";
                cin >> payer;

                if (payer < 1 || payer > members.size())
                {
                    cout << "Invalid member number.\n";
                    continue;
                }

                double share = amount / members.size();

                // Every member owes their share
                for (int i = 0; i < balances.size(); i++)
                {
                    balances[i] -= share;
                }

                // The payer should receive the amount they paid
                balances[payer - 1] += amount;

                cout << fixed << setprecision(3);
                cout << "\nExpense added successfully!\n";
                cout << "Expense: " << expenseName << "\n";
                cout << "Amount: " << amount << " OMR\n";
                cout << "Paid by: " << members[payer - 1] << "\n";
                cout << "Share per person: " << share << " OMR\n";
            }
        }

        // Option 3: View balances
        else if (choice == 3)
        {
            if (members.empty())
            {
                cout << "\nPlease create a group first.\n";
            }
            else
            {
                cout << fixed << setprecision(3);
                cout << "\nBalances for " << groupName << ":\n";

                for (int i = 0; i < members.size(); i++)
                {
                    cout << members[i] << ": ";

                    if (balances[i] > 0.0005)
                    {
                        cout << "gets " << balances[i] << " OMR\n";
                    }
                    else if (balances[i] < -0.0005)
                    {
                        cout << "owes " << -balances[i] << " OMR\n";
                    }
                    else
                    {
                        cout << "settled\n";
                    }
                }
            }
        }

        // Option 4: Settle debts
        else if (choice == 4){
              if (members.empty()){
                  cout << "\nPlease create a group first.\n";
               }
              else{
               // Copy balances so the original values are not changed
                  vector<double> settlementBalances = balances;

                  bool hasDebt = false;

                 cout << fixed << setprecision(3);
                 cout << "\nSettlement plan:\n";

                 while (true){
                     int creditor = -1;
                     int debtor = -1;

                     // Find the person who should receive the most
                     // and the person who owes the most
                     for (int i = 0; i < settlementBalances.size(); i++){
                         if (creditor == -1 ||
                             settlementBalances[i] > settlementBalances[creditor])
                         {
                             creditor = i;
                         }

                         if (debtor == -1 ||
                             settlementBalances[i] < settlementBalances[debtor])
                         {
                             debtor = i;
                         }
                     }

                     // Stop when everyone is settled
                     if (creditor == -1 ||
                         debtor == -1 ||
                         settlementBalances[creditor] <= 0.0005 ||
                         settlementBalances[debtor] >= -0.0005)
                     {
                         break;
                     }

                     double payment = min(settlementBalances[creditor],-settlementBalances[debtor]);
                     

                     cout << members[debtor]
                          << " pays "
                          << members[creditor]
                          << " "
                          << payment
                          << " OMR\n";

                     hasDebt = true;

                     settlementBalances[creditor] -= payment;
                     settlementBalances[debtor] += payment;
                }

                 if (!hasDebt){
                     cout << "Everyone is already settled.\n";
                }
            }
        }

        // Option 5: Exit
        else if (choice == 5)
        {
            cout << "\nThank you for using SplitMate!\n";
        }

        // Invalid menu option
        else
        {
            cout << "\nInvalid choice. Please select from 1 to 5.\n";
        }

    } while (choice != 5);

    return 0;
}