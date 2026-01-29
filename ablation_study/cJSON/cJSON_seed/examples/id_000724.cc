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
//<ID> 724
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
    double numbers[3];
    numbers[0] = 3.14;
    numbers[1] = 2.718;
    numbers[2] = 1.618;
    cJSON *values = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *misc = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("delta");
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(misc, str_item);
    cJSON_AddItemToArray(misc, false_item);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToObject(root, "misc", misc);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("example_label"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_values = cJSON_GetObjectItemCaseSensitive(parsed, "values");
    cJSON *first_val_item = cJSON_GetArrayItem(parsed_values, 0);
    double first_val = cJSON_GetNumberValue(first_val_item);
    cJSON *parsed_misc = cJSON_GetObjectItemCaseSensitive(parsed, "misc");
    cJSON *parsed_first_misc = cJSON_GetArrayItem(parsed_misc, 0);
    const char *first_misc_str = cJSON_GetStringValue(parsed_first_misc);
    cJSON *detached_misc = cJSON_DetachItemFromObjectCaseSensitive(parsed, "misc");
    cJSON *detached_first = cJSON_GetArrayItem(detached_misc, 0);
    const char *detached_first_str = cJSON_GetStringValue(detached_first);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(detached_misc);
    cJSON_Delete(root);
    (void)version;
    (void)first_val;
    (void)first_misc_str;
    (void)detached_first_str;
    // API sequence test completed successfully
    return 66;
}