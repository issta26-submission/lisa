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
//<ID> 243
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
    cJSON *child = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *el0 = (cJSON *)0;
    cJSON *child_ref = (cJSON *)0;
    cJSON *child_num_item = (cJSON *)0;
    char *printed = (char *)0;
    double extracted_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "values");
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);

    // step 3: Configure
    child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "child_number", 3.1415);
    cJSON_AddItemToObject(root, "meta", child);

    // step 4: Operate
    arr_ref = cJSON_GetObjectItem(root, "values");
    el0 = cJSON_GetArrayItem(arr_ref, 0);
    child_ref = cJSON_GetObjectItem(root, "meta");
    child_num_item = cJSON_GetObjectItem(child_ref, "child_number");
    extracted_value = cJSON_GetNumberValue(child_num_item);
    cJSON_AddNumberToObject(root, "extracted", extracted_value);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}