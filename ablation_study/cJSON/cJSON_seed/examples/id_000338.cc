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
//<ID> 338
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
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "pi", 3.14159);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 1.0);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateNumber(9.0);
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    cJSON_AddNumberToObject(root, "pi_plus_one", pi_val + 1.0);
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}