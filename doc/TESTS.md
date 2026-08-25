# HOW TO RUN TESTS

## How to Run System Tests

1. Build the project with coverage tracking enabled

   ```sh
   make COVERAGE=1
   ```

   > [!note]
   >
   > You might need to `make clean` first.

2. Run the system tests

   ```sh
   make test-system
   ```

3. Look at the test coverage

   ```sh
   ../report_coverage.sh
   ```

## How to Run Automatically Generated Tests

1. Make sure you have python installed. \
   The test script has been written and ran on python version 3.10.12

   ```sh
   sudo apt-get install python3 python3-venv
   ```

2. Create a virtual environment

   ```sh
   python3 -m venv venv/
   ```

3. Activate the environment

   ```sh
   source venv/bin/activate
   ```

4. Install dependencies

   ```sh
   pip install -r requirements.txt
   ```

5. Provide a ground truth source \
   It is an ABC binary that needs to be present in src/ and named abc_32 (as the
   intended comparison is between 64-bit ABC and 32-bit ABC). The simplest way
   to achieve this is by going into the src/ and running

   ```sh
   cd src
   make clean
   make all BIT=32
   cd ..
   ```

   and afterwards renaming the src/abc binary to src/abc_32

6. Provide the tested binary \
   It is the second ABC binary that will be ran and compared with ground truth.
   It needs to be present in src/ and named abc  If you do not need to know
   about code coverage, it is enough to run

   ```sh
   cd src
   make clean
   make all 
   cd ..
   ```

   To get code coverage after running the script, instead run:

   ```sh
   cd src
   make clean
   make all COVERAGE=1
   cd ..
   ```

7. Run the tests

   ```sh
   python3 cfg_fuzzing.py
   ```

   During the execution, there is a chance a generated program contains an
   infinite loop. In such case it will timeout and print an error message
   indicating this happened. Afterwards the execution of the next generated
   program continues as normal.

8. Interpret the output \
   During the runtime of the tests, progress is constantly printed. It is
   structured as: Number of total tests, Number of tests where the generated
   program successfully ran, Number of tests where the outputs or errors of the
   two ABC versions differed.  After the tests finish running all of the
   differing outputs are printed in the following format: Input program, Output
   of the 64-bit version, Errors of the 64-bit version, Output of the ground
   truth version, Errors of the ground truth version.

9. Run the code coverage script \

   If the `abc` was made with COVERAGE flag set to 1 it is possible to get the
   code coverage report by running the following command

   ```sh
   ./report_coverage.sh
   ```

## How to Run Unit Tests

1. Change directory to `src/`

   ```sh
   cd src
   ```

2. Run the system tests

   ```sh
   make test COVERAGE=1
   ```

   > [!note]
   >
   > You might need to `make clean` first.

3. Look at the test coverage

   ```sh
   ../report_coverage.sh
   ```
