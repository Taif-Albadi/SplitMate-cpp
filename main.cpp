#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Expense
{
    string name;
    double amount;
    int paidBy;
    vector<int> participants;
};

int main()
{
    int choice = 0;
    string groupName;

    vector<string> members;
    vector<double> balances;
    vector<Expense> expenses;

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
        cout << "5. View expense history\n";
        cout << "6. Exit\n";

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
            expenses.clear();

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
        else if (choice == 2){
            if (members.empty()){
                cout << "\nPlease create a group first.\n";
            }
            else{
            string expenseName;
            double amount;
            int payer;
            int numberOfParticipants;
            vector<int> participants;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nEnter expense name: ";
            getline(cin, expenseName);

            cout << "Enter expense amount: ";
            cin >> amount;

            if (amount <= 0){
                cout << "The expense amount must be greater than zero.\n";
                continue;
            }

            cout << "\nMembers:\n";

            for (int i = 0; i < members.size(); i++){
                cout << i + 1 << ". " << members[i] << "\n";
            }

            cout << "\nWho paid? Enter member number: ";
            cin >> payer;

            if (payer < 1 || payer > members.size()){
                cout << "Invalid member number.\n";
                continue;
            }

            cout << "How many members participated in this expense? ";
            cin >> numberOfParticipants;

            if (numberOfParticipants < 1 ||
            numberOfParticipants > members.size()){
                cout << "Invalid number of participants.\n";
                continue;
            }

            cout << "Enter the participant numbers:\n";

            for (int i = 0; i < numberOfParticipants; i++){
                int participant;

                cout << "Participant " << i + 1 << ": ";
                cin >> participant;

                if (participant < 1 || participant > members.size()){
                    cout << "Invalid member number. Try again.\n";
                    i--;
                    continue;
                }

                int participantIndex = participant - 1;

                if (find(participants.begin(),
                         participants.end(),
                         participantIndex) != participants.end()){
                    cout << "This member was already selected. Try again.\n";
                    i--;
                    continue;
                }

                participants.push_back(participantIndex);
            }

            double share = amount / participants.size();

            // Only selected participants owe a share
            for (int participantIndex : participants){
                balances[participantIndex] -= share;
            }

            // The payer should receive the amount they paid
            balances[payer - 1] += amount;

            // Save the expense details
            Expense newExpense;

            newExpense.name = expenseName;
            newExpense.amount = amount;
            newExpense.paidBy = payer - 1;
            newExpense.participants = participants;

            expenses.push_back(newExpense);

            cout << fixed << setprecision(3);
            cout << "\nExpense added successfully!\n";
            cout << "Expense: " << expenseName << "\n";
            cout << "Amount: " << amount << " OMR\n";
            cout << "Paid by: " << members[payer - 1] << "\n";
            cout << "Share per participant: " << share << " OMR\n";

            cout << "Participants: ";

            for (int participantIndex : participants){
                cout << members[participantIndex] << " ";
            }

            cout << "\n";
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
        // Option 5: View expense history
        else if (choice == 5){
            if (expenses.empty()){
               cout << "\nNo expenses have been added yet.\n";
           }
           else{
               cout << fixed << setprecision(3);
               cout << "\nExpense history for " << groupName << ":\n";

               for (int i = 0; i < expenses.size(); i++){
                   cout << "\nExpense " << i + 1 << ":\n";
                   cout << "Name: " << expenses[i].name << "\n";
                   cout << "Amount: " << expenses[i].amount << " OMR\n";

                   cout << "Paid by: "
                        << members[expenses[i].paidBy]
                        << "\n";

                    cout << "Participants: ";
                   for (int participantIndex : expenses[i].participants){
                       cout << members[participantIndex] << " ";
                   }

                   cout << "\n";

                   double share =
                       expenses[i].amount /
                       expenses[i].participants.size();

                   cout << "Share per participant: "
                        << share
                        << " OMR\n";
                }
            }
        }

        // Option 6: Exit
        else if (choice == 6)
        {
            cout << "\nThank you for using SplitMate!\n";
        }

        // Invalid menu option
        else
        {
            cout << "\nInvalid choice. Please select from 1 to 6.\n";
        }

    } while (choice != 6);

    return 0;
}