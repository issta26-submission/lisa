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
//<ID> 1266
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "name", "sequence");
    cJSON_AddNumberToObject(meta, "version", 1.0);

    // step 3: Operate
    cJSON *got_values = cJSON_GetObjectItem(root, "values");
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(got_values, 1, replacement);
    cJSON *replaced_item = cJSON_GetArrayItem(got_values, 1);
    cJSON_bool replaced_is_number = cJSON_IsNumber(replaced_item);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);

    // step 4: Validate & Cleanup
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *version_item = cJSON_GetObjectItem(got_meta, "version");
    cJSON_bool version_is_number = cJSON_IsNumber(version_item);
    (void)replaced_is_number;
    (void)version_is_number;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}