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
//<ID> 344
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *s2 = cJSON_CreateString("world");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON *raw = cJSON_CreateRaw("{\"rawkey\":123}");
    cJSON_AddItemToObject(root, "raw", raw);
    cJSON *maybe = cJSON_AddNullToObject(root, "maybe");

    // step 2: Configure
    int count_before = cJSON_GetArraySize(arr);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 3: Operate & Validate
    cJSON_AddNumberToObject(root, "count_before", (double)count_before);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON *first_copy = cJSON_CreateString(first_str);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    cJSON_AddBoolToObject(root, "printed_ok", printed_ok);
    cJSON *rendered = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "rendered", rendered);
    int count_after = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count_after", (double)count_after);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}