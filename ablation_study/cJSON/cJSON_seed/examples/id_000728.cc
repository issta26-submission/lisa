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
//<ID> 728
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
    double numbers[] = {10.5, 20.25};
    cJSON *mixed = cJSON_CreateDoubleArray(numbers, 2);
    cJSON *str_item = cJSON_CreateString("greeting");
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(mixed, str_item);
    cJSON_AddItemToArray(mixed, false_item);
    cJSON_AddItemToObject(root, "mixed", mixed);
    cJSON_AddItemToObject(root, "temp", cJSON_CreateString("to_be_moved"));
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "temp");
    cJSON_AddItemToArray(mixed, detached);
    (void)version;

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_mixed = cJSON_GetObjectItemCaseSensitive(parsed, "mixed");
    int parsed_size = cJSON_GetArraySize(parsed_mixed);
    cJSON *first_item = cJSON_GetArrayItem(parsed_mixed, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *third_item = cJSON_GetArrayItem(parsed_mixed, 2);
    const char *third_val = cJSON_GetStringValue(third_item);
    (void)parsed_size;
    (void)first_val;
    (void)third_val;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}