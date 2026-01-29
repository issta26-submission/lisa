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
//<ID> 1549
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
    cJSON *str_item = NULL;
    cJSON *arr_ref = NULL;
    cJSON *first = NULL;
    cJSON *copied_str = NULL;
    cJSON *retrieved_arr = NULL;
    const char *first_val = NULL;
    char *unformatted = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    str_item = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, str_item);
    // use a duplicate instead of creating a reference to avoid potential circular references
    arr_ref = cJSON_Duplicate(arr, cJSON_True);
    cJSON_AddItemToObject(root, "items_ref", arr_ref);
    cJSON_AddTrueToObject(root, "enabled");

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    first = cJSON_GetArrayItem(retrieved_arr, 0);
    first_val = cJSON_GetStringValue(first);
    copied_str = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "copied", copied_str);
    unformatted = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);
    return 66;
}