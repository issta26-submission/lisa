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
//<ID> 659
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
    double initial_vals[3] = {1.5, 2.5, 3.25};
    cJSON *values = cJSON_CreateDoubleArray(initial_vals, 3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddStringToObject(root, "creator", "fuzzer");
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "version_major", 1.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_values = cJSON_GetObjectItemCaseSensitive(parsed, "values");
    int original_size = cJSON_GetArraySize(parsed_values);
    double replacement_vals_static[3] = {10.0, 20.0, 30.0};
    cJSON *replacement_array = cJSON_CreateDoubleArray(replacement_vals_static, original_size);
    cJSON_ReplaceItemInObjectCaseSensitive(parsed, "values", replacement_array);
    int replaced_size = cJSON_GetArraySize(cJSON_GetObjectItemCaseSensitive(parsed, "values"));
    (void)version;
    (void)original_size;
    (void)replaced_size;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}