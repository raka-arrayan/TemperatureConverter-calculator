# Temperature Converter & calculator

This project was developed collaboratively by a group in order to fulfill the requirements of the advance Programming course in the Computer Engineering program at the University of Indonesia.The code includes several main features that allow users to create accounts, log in, and utilize services such as temperature conversion and a two-number multiplication calculator. The account creation feature (createAccount) enables users to register with their email and password, which are then stored in the accounts.txt file. After creating an account, users are automatically given 10 initial credits, which are recorded in the credits.txt file. This provides an incentive for new users to sign up and start using the services with some initial credits.

The login feature (login) allows users to log into the system with their email and password. Once successfully logged in, users can access additional menu options, including temperature conversion, adding credits using a voucher code, and using the calculator for multiplying two large numbers. The temperature conversion feature (convertTemperature) permits users to convert temperatures from one scale to another, with each conversion deducting one credit from their account. If users are not logged in, they can still perform temperature conversions up to three times before being required to log in.

Additionally, there is a two-number multiplication calculator feature (kalkulator2Bilangan) that utilizes parallelization with OpenMP to improve efficiency when one of the numbers being multiplied is sufficiently large (≥1000). Users can also add credits by entering a valid voucher code, which is verified through the vouchers.txt file. With this combination of features, the code provides a multifunctional platform that includes account management, credit handling, temperature conversion, and efficient mathematical calculations.
