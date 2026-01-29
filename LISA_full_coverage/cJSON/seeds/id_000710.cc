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
//<ID> 710
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const int init_nums[] = {7, 8, 9, 10};
    cJSON *init_ints = cJSON_CreateIntArray(init_nums, 4);
    cJSON_AddItemToObject(root, "ints", init_ints);
    cJSON *label = cJSON_CreateString("sample_label");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure (parse external JSON with explicit length)
    char external_json[] = "{\"key\": [10,20,30]} trailing";
    const char *parse_end = NULL;
    cJSON *doc = cJSON_ParseWithLengthOpts(external_json, (size_t)(sizeof(external_json) - 1), &parse_end, 0);
    cJSON *parsed_array = cJSON_GetObjectItem(doc, "key");
    int parsed_count = cJSON_GetArraySize(parsed_array);

    // step 3: Operate (extract values and incorporate into root)
    cJSON *first_item = cJSON_GetArrayItem(parsed_array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *first_num = cJSON_CreateNumber(first_val);
    cJSON_AddItemToObject(root, "parsed_first", first_num);
    cJSON_AddItemToObject(root, "parsed_count", cJSON_CreateNumber((double)parsed_count));
    int init_count = cJSON_GetArraySize(init_ints);
    cJSON_AddItemToObject(root, "ints_size", cJSON_CreateNumber((double)init_count));
    cJSON *small_arr = cJSON_CreateIntArray(init_nums, 2);
    cJSON_AddItemToObject(root, "small_ints", small_arr);

    // step 4: Validate & Cleanup
    cJSON_Delete(doc);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}