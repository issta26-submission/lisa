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
//<ID> 583
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddNumberToObject(child, "count", 42.0);
    cJSON_AddStringToObject(child, "status", "ok");
    cJSON_AddItemToObjectCS(root, "child_cs", child);
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "letters", arr);
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_dup", dup_child);
    cJSON_AddNumberToObject(root, "root_num", 3.14);
    cJSON_AddStringToObject(root, "root_name", "rootnode");
    int arr_size = cJSON_GetArraySize(arr);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_child = cJSON_GetObjectItem(parsed, "child_cs");
    cJSON *parsed_count_item = cJSON_GetObjectItem(parsed_child, "count");
    double count_val = cJSON_GetNumberValue(parsed_count_item);
    cJSON *parsed_arr = cJSON_GetObjectItem(parsed, "letters");
    int parsed_arr_size = cJSON_GetArraySize(parsed_arr);
    const char *first_letter = cJSON_GetStringValue(cJSON_GetArrayItem(parsed_arr, 0));

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;arr=%d;parsed_arr=%d;count=%.0f;first=%s",
            version ? version : "null",
            arr_size,
            parsed_arr_size,
            count_val,
            first_letter ? first_letter : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}