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
//<ID> 851
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *parsed = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_numbers_obj = 0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_extracted = 0;
    cJSON_bool added_parsed = 0;
    cJSON_bool is_number_flag = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(42.0);
    num2 = cJSON_CreateNumber(13.5);

    // step 3: Configure
    added_numbers_obj = cJSON_AddItemToObject(root, "numbers", arr);
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 1);
    is_number_flag = cJSON_IsNumber(detached);
    parsed = cJSON_Parse("{\"meta\":1}");
    added_extracted = cJSON_AddItemToObject(parsed, "extracted", detached);
    added_parsed = cJSON_AddItemToObject(root, "parsed", parsed);
    printed = cJSON_Print(root);

    // step 5: Validate
    validation_score = (int)added_numbers_obj + (int)added_num1 + (int)added_num2 + (int)added_extracted + (int)added_parsed + (int)is_number_flag + (printed != (char *)0);
    (void)validation_score;
    if (printed) { cJSON_free(printed); }

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}