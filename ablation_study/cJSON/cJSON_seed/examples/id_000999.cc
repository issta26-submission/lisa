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
//<ID> 999
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("device-42");
    cJSON_AddItemToObjectCS(meta, "name", name);
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObjectCS(root, "values", int_array);

    // step 2: Configure
    cJSON *status = cJSON_CreateString("operational");
    cJSON_AddItemToObjectCS(root, "status", status);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(int_array);
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *count = cJSON_CreateNumber((double)size + (double)has_meta);
    cJSON_AddItemToObjectCS(root, "count", count);
    cJSON *got_name = cJSON_GetObjectItem(meta, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObjectCS(root, "name_copy", name_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}