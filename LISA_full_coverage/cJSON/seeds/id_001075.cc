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
//<ID> 1075
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"flag\":false,\"values\":[1.0,2.0]}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    double new_numbers[3] = {3.14, 2.718, 1.618};
    cJSON *double_array = cJSON_CreateDoubleArray(new_numbers, 3);
    cJSON_AddItemToObject(root, "new_values", double_array);
    cJSON *true_item = cJSON_AddTrueToObject(root, "confirmed");

    // step 3: Operate
    cJSON *replacement_number = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", replacement_number);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}