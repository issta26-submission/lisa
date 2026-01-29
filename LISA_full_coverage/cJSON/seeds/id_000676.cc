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
//<ID> 676
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
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "obj", obj);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(obj, "flag", flag_true);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObject(obj, "flag", replacement);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "arr_size", (double)arr_size);

    // step 3: Operate & Validate
    cJSON *current_flag = cJSON_GetObjectItem(obj, "flag");
    cJSON_bool was_bool = cJSON_IsBool(current_flag);
    cJSON_bool is_string = cJSON_IsString(current_flag);
    cJSON_AddBoolToObject(root, "flag_was_bool", was_bool);
    cJSON_AddBoolToObject(root, "flag_is_string", is_string);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}