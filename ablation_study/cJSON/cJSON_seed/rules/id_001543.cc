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
//<ID> 1543
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *str_item = NULL;
    cJSON *arr_ref = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddTrueToObject(root, "status");
    cJSON_AddItemToObject(root, "greeting", cJSON_CreateString("hello"));
    arr = cJSON_AddArrayToObject(root, "items");
    num1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num2);
    str_item = cJSON_CreateString("world");
    cJSON_AddItemToArray(arr, str_item);

    // step 3: Operate / Validate
    // Use a duplicate (deep copy) of the array instead of a direct reference
    // to avoid potential double-free issues when deleting the parent.
    arr_ref = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "items_ref", arr_ref);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
}