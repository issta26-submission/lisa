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
//<ID> 647
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
    cJSON *null_item = cJSON_CreateNull();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *entry = cJSON_CreateObject();
    cJSON *orig = cJSON_CreateString("original");

    // step 2: Configure
    cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(entry, "field", orig);
    cJSON_AddItemToObject(root, "entry", entry);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(entry, orig, replacement);

    // step 3: Operate & Validate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    cJSON *extra_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, extra_num);

    // step 4: Cleanup
    (void)version;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}