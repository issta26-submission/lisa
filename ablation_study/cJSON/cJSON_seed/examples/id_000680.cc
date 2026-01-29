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
//<ID> 680
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
    cJSON *array = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(3.14159);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(array, num_item);
    cJSON_AddItemToArray(array, true_item);
    cJSON_AddItemToArray(array, str_item);
    cJSON_AddItemToObject(root, "data", array);

    // step 2: Configure
    cJSON *dup_array = cJSON_Duplicate(array, 1);
    cJSON_AddItemToObject(root, "data_copy", dup_array);

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
    cJSON *original_first = cJSON_GetArrayItem(array, 0);
    cJSON_bool first_equal = cJSON_Compare(original_first, parsed_first, 1);
    cJSON_bool arrays_equal = cJSON_Compare(array, parsed_data, 1);
    (void)version;
    (void)parsed_size;
    (void)first_equal;
    (void)arrays_equal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}