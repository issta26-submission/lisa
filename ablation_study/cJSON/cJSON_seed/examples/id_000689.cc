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
//<ID> 689
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
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(1.23);
    cJSON *str_item = cJSON_CreateString("hello");

    // step 2: Configure
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *flag_copy = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag_copy);
    (void)version;

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_data = cJSON_GetObjectItem(parsed, "data");
    int parsed_size = cJSON_GetArraySize(parsed_data);
    cJSON *parsed_first = cJSON_GetArrayItem(parsed_data, 0);
    cJSON *parsed_second = cJSON_GetArrayItem(parsed_data, 1);
    cJSON_bool equal_first = cJSON_Compare(true_item, parsed_first, 1);
    cJSON_bool equal_second = cJSON_Compare(num_item, parsed_second, 1);
    (void)parsed_size;
    (void)equal_first;
    (void)equal_second;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}