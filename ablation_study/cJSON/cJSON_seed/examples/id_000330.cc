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
//<ID> 330
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(numbers, n0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddNumberToObject(root, "pi", 3.14159);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(4.5);
    cJSON_ReplaceItemInArray(numbers, 1, replacement);

    // step 3: Operate & Validate
    char *printed = cJSON_Print(root);
    cJSON *numbers_obj = cJSON_GetObjectItem(root, "numbers");
    cJSON *pi_obj = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_obj);
    cJSON_AddNumberToObject(root, "pi_times_two", pi_val * 2.0);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}