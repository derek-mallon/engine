/*!\file tester.h
 * \brief Header only simply unit testing framework
 *
 * dsaslfkds
 * **Usage:**
 * 
 *       int test_var;
 *       ENVIROMENT_SETUP{ //Run before each unit test.
 *           test_var = 5;
 *       }
 *       
 *       ENVIROMENT_CLEANUP{ //Run after each unit test.
 *       }
 *       
 *       TESTS
 *           UNIT_TEST_START("test if test_var is 5")
 *               ASSERT(test_var == 5);
 *           UNIT_TEST_END
 *           UNIT_TEST_START("test if test_var++ is 6")
 *               test_var++;
 *               ASSERT(test_var == 6);
 *           UNIT_TEST_END
 *       END_TESTS
 */
#ifndef TESTER
#define TESTER
#include <stdio.h>

#define RED_COLOR_CODE "\x1B[31m"
#define GREEN_COLOR_CODE "\x1B[32m"
#define NORMAL_COLOR_CODE "\x1B[0m"

#define ENVIROMENT_SETUP void enviroment_setup()

#define ENVIROMENT_CLEANUP void enviroment_cleanup()


#define TESTS char* current_test = "none"; int succeeded = 0;int test_counter = 0; int succeeded_tests = 0; void dummy(){} int main(){

#define END_TESTS printf("\n%d out of %d tests succeeded.\n",succeeded_tests,test_counter); if(succeeded_tests != test_counter){return 1;}return 0;}
#define UNIT_TEST_START(name) current_test = name; enviroment_setup(); succeeded = 1; test_counter++;

#define UNIT_TEST_END if(succeeded){succeeded_tests++; printf("\nTEST %d: %s %ssucceeded%s\n",test_counter,current_test,GREEN_COLOR_CODE,NORMAL_COLOR_CODE);}else{printf("\nTEST %d: %s %sfailed%s\n", test_counter,current_test,RED_COLOR_CODE,NORMAL_COLOR_CODE);} enviroment_cleanup();

#define ASSERT(statement) if(statement && succeeded != 0){succeeded = 1;}else{succeeded = 0;}dummy()

#endif
