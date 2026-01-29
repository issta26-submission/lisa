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
//<ID> 636
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
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "original", arr);
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "copy", arr_dup);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);
    sprintf((char *)buffer, "ver=%s;snap=%s", version ? version : "null", snapshot);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_original = cJSON_GetObjectItem(parsed, "original");
    cJSON *newnum = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(parsed_original, newnum);
    cJSON *parsed_dup = cJSON_Duplicate(parsed_original, 1);
    cJSON_AddItemToObject(parsed, "added_dup", parsed_dup);
    cJSON_DeleteItemFromObject(root, "copy");

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}