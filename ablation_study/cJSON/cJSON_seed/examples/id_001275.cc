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
//<ID> 1275
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
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON_AddNullToObject(child, "maybe_null");
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 2: Configure
    int count = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(child, "count", (double)count);
    cJSON *desc = cJSON_CreateString("example_collection");
    cJSON_AddItemToObject(child, "desc", desc);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_val = cJSON_GetStringValue(first);
    (void)first_val;
    cJSON *got_count_item = cJSON_GetObjectItem(child, "count");
    double got_count = cJSON_GetNumberValue(got_count_item);
    cJSON_AddNumberToObject(root, "verified_count", got_count);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}