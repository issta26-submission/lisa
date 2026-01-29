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
//<ID> 512
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"arr\": [10,20], \"flag\": false, \"len\": 0}";
    size_t buffer_length = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, buffer_length, &parse_end, 1);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    int original_count = cJSON_GetArraySize(arr);
    cJSON *new_len_item = cJSON_CreateNumber((double)original_count);
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Setup / Configure
    cJSON_ReplaceItemInObjectCaseSensitive(root, "len", new_len_item);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", true_item);

    // step 3: Operate / Validate
    cJSON *retrieved_len = cJSON_GetObjectItem(root, "len");
    double retrieved_value = cJSON_GetNumberValue(retrieved_len);
    cJSON_AddNumberToObject(root, "len_copy", retrieved_value);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}