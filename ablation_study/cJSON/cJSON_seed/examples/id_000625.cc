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
//<ID> 625
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
    int numbers_arr[4] = { 10, 20, 30, 40 };
    cJSON *nums = cJSON_CreateIntArray(numbers_arr, 4);
    cJSON *label = cJSON_CreateString("example_label");
    cJSON *empty_null = cJSON_AddNullToObject(root, "empty"); /* returns pointer to null item */
    cJSON_AddItemToObject(root, "numbers", nums);
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snapshot_len = strlen(snapshot);
    char *tmpbuf = (char *)cJSON_malloc(snapshot_len + 1);
    memcpy(tmpbuf, snapshot, snapshot_len + 1);
    cJSON_Minify(tmpbuf);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(tmpbuf);
    cJSON *parsed_empty = cJSON_GetObjectItem(parsed, "empty");
    cJSON_bool empty_is_null = cJSON_IsNull(parsed_empty);
    cJSON *parsed_nums = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *first_num_item = cJSON_GetArrayItem(parsed_nums, 0);
    double first_value = cJSON_GetNumberValue(first_num_item);
    cJSON_DeleteItemFromObject(parsed, "label");

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(tmpbuf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)version;
    (void)empty_is_null;
    (void)first_value;
    return 66;
}