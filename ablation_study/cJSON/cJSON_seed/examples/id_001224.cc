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
//<ID> 1224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"a\":[3,6,9],\"b\":5}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    int extra_nums[4] = {10,20,30,40};
    cJSON *int_arr = cJSON_CreateIntArray(extra_nums, 4);
    cJSON_AddItemToObject(root, "extra", int_arr);
    cJSON *a_array = cJSON_GetObjectItem(parsed, "a");
    cJSON *second = cJSON_GetArrayItem(a_array, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *scaled = cJSON_CreateNumber(second_val * 2.5);
    cJSON_AddItemToObject(root, "scaled", scaled);

    // step 3: Operate & Validate
    cJSON *b_item = cJSON_GetObjectItem(parsed, "b");
    double bval = cJSON_GetNumberValue(b_item);
    cJSON *b_copy = cJSON_CreateNumber(bval);
    cJSON_AddItemToObject(root, "b_copy", b_copy);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}