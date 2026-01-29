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
//<ID> 650
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
    double values[3] = {1.5, 2.5, 3.5};
    cJSON *num_array = cJSON_CreateDoubleArray(values, 3);
    cJSON *meta = cJSON_CreateObject();
    cJSON *label = cJSON_CreateString("initial");

    // step 2: Configure
    cJSON_AddItemToObject(root, "numbers", num_array);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "label", label);
    cJSON *new_label = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObjectCaseSensitive(meta, "label", new_label);

    // step 3: Operate & Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "numbers");
    int count_before = cJSON_GetArraySize(retrieved);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_Minify(snapshot);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_nums = cJSON_GetObjectItem(parsed, "numbers");
    int count_after = cJSON_GetArraySize(parsed_nums);
    (void)version;
    (void)count_before;
    (void)count_after;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}