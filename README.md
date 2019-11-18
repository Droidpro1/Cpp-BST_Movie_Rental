# A C++ implementation of a DVD kiosk backend

Problem: Redbox needs a program to track inventory and to generate a report for their DVD rental kiosks. Given a log of transactions including renting and returning DVDs as well as adding and removing DVD titles, the program will need to process each transaction and create a report after all transactions have been processed. The generated report will list all DVD titles stored in the kiosk as well as how many of each disc are in the kiosk

       o Insert 
       o Search 
       o Delete
       o Any other functions you plan to add to the class

Details

    • The inventory will be held in a binary search tree that you will create

    • Use the DVD title to determine node placement in the tree

    • The binary tree will be seeded with an inventory file

    • Once seeded, the program will parse a transaction log to update the inventory

    • There are five possible transactions

    o Add new title

    o Add copies for an existing title

        o Remove copies of an existing title

        o Rent a DVD

        o Return a DVD

    • Add new title

        o Create a new node and insert it into the tree

    • Add copies of an existing title

        o Find the title in the tree and increase the number of available copies by the amount listed

    • Remove copies of an existing title

        o Find the title in the tree and reduce the number of available copies by the amount listed

        o If number available is zero and no copies are rented out, delete the node from the tree

        o There will not be more copies removed than available.

    • Rent a DVD

        o Reduce available amount by one and increase rented amount by one

    • Return a DVD

        o Increase available amount by one and reduce rented amount by one

Classes

    • Node

        o Members
            ▪ Title (string)

            ▪ Available (integer)

            ▪ Rented (integer)

            ▪ Left (node pointer)

            ▪ Right (node pointer)

        o Methods
            ▪ Overloaded constructor

            ▪ Mutators

            ▪ Accessors

    • Binary Search Tree

        o Binary Search Tree class must be templated (-5 points if not)
           o  Members

        ◦ Root (node pointer) o Methods

        ◦ Mutator

        ◦ Accessor

        ◦ Insert (recursive) (-5 points if not)

        ◦ Search (recursive) (-5 points if not)

        ◦ Delete

        ◦ Other methods that are necessary to interact with a binary search tree

            ▪ Remember methods should be generic enough to be used on a binary tree regardless of the problem.

User Interface and Input: There is no user interface for this program.

Input will be given in two files, inventory.dat and transaction.log. The program will read inventory.dat first. This will create the binary tree. Each line (except the last line which may not have a newline character) in inventory.dat will be formatted as follows (NOTE: <text> represents a variable value):

“<title>”,&lt;quantity available&gt;,&lt;quantity rented&gt;

After processing the inventory file, begin processing transaction.log. Each line of the file should follow one of the following formats (Note: the last line may not end with a newline character):

    • add “<title>”,<number to add>

    • remove “<title>”,<number to remove>

    • rent “<title>”

    • return “<title>”

The transaction file may contain errors due to network disruptions from the main server. For each line in the transaction log, validate that it follows one of the formats listed above. If it is the correct format, process the transaction. If the line is invalid, write the line to an error file (as described below). All numbers are expected to be integers. To be valid, the line must follow the format exactly. Also, do not assume that a title in the transaction log will be in the tree.

Output: A file named error.log will be created if any lines of transaction.log are invalid. Error.log will contain all invalid entries of the transaction file.

At the end of the program, create a formatted report to display each title, the number of copies available to rent for that title as well as the number of copies that are currently rented. The titles should be listed in alphabetical order (without the double quotes). The report should be arranged in three formatted columns that line up the data nicely:

    • Title

    • Copies available

    • Copies rented

Write the report to a file named redbox_kiosk.txt.
