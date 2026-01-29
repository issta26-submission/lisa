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
//<ID> 480
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.5);
    cJSON *n2 = cJSON_CreateNumber(30.25);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *title = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "title", title);

    // step 2: Configure
    cJSON *note = cJSON_CreateString("note: mixed array");
    cJSON_AddItemToArray(arr, note);

    // step 3: Operate
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON *extracted = cJSON_CreateNumber(second_value * 2.0);
    cJSON_AddItemToObject(root, "extracted_double", extracted);

    // step 4: Validate and Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}