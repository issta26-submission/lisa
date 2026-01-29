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
//<ID> 645
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
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *str1 = cJSON_CreateString("initial");
    cJSON *maybe_null = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, maybe_null);
    cJSON_AddItemToObject(root, "items", arr);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool ok_print1 = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)ok_print1;

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(arr, arr->child, replacement);
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool ok_print2 = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    (void)ok_print2;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}