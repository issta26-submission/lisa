#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 787
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *numbersObj = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *retrievedNumbers = (cJSON *)0;
    cJSON *retrievedPi = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool addedNum1 = 0;
    cJSON_bool addedNum2 = 0;
    cJSON_bool addedNumbersObj = 0;
    double valAnswer = 0.0;
    double valPi = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    numbersObj = cJSON_CreateObject();

    // step 3: Configure
    num1 = cJSON_CreateNumber(42.0);
    num2 = cJSON_CreateNumber(3.14159);
    addedNum1 = cJSON_AddItemToObject(numbersObj, "answer", num1);
    addedNum2 = cJSON_AddItemToObject(numbersObj, "pi", num2);
    addedNumbersObj = cJSON_AddItemToObject(root, "numbers", numbersObj);

    // step 4: Operate
    retrievedNumbers = cJSON_GetObjectItem(root, "numbers");
    retrievedPi = cJSON_GetObjectItemCaseSensitive(retrievedNumbers, "pi");
    valAnswer = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(retrievedNumbers, "answer"));
    valPi = cJSON_GetNumberValue(retrievedPi);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (root != (cJSON *)0) + (numbersObj != (cJSON *)0) + (num1 != (cJSON *)0) + (num2 != (cJSON *)0) + (retrievedNumbers != (cJSON *)0) + (retrievedPi != (cJSON *)0) + (printed != (char *)0) + ((valAnswer == 42.0) ? 1 : 0) + ((valPi > 3.14 && valPi < 3.15) ? 1 : 0) + (int)addedNum1 + (int)addedNum2 + (int)addedNumbersObj;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}