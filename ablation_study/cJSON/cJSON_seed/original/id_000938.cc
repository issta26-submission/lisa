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
//<ID> 938
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"meta\":\"test\",\"values\": [1.5, 2.5]}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);
    size_t parsed_offset = (size_t)(parse_end - initial_json);

    // step 2: Configure
    float extra_nums[] = { 3.14f, 4.5f, -1.0f };
    int extra_count = 3;
    cJSON *float_arr = cJSON_CreateFloatArray(extra_nums, extra_count);
    cJSON_AddItemToObjectCS(root, "extra_values", float_arr);
    cJSON *label = cJSON_CreateString("example");
    cJSON_AddItemToObjectCS(root, "label", label);
    cJSON *arr_ref = cJSON_CreateArrayReference(float_arr);
    cJSON_AddItemToObjectCS(root, "extra_values_ref", arr_ref);

    // step 3: Operate
    int arr_size = cJSON_GetArraySize(float_arr);
    cJSON *first_item = cJSON_GetArrayItem(float_arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *computed_num = cJSON_CreateNumber(first_val + 10.0);
    cJSON_AddItemToObjectCS(root, "first_plus_ten", computed_num);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    size_t bufsize = 256;
    void *buf = cJSON_malloc(bufsize);
    memset(buf, 0, bufsize);
    char *prebuf = (char *)buf;
    cJSON_bool ok = cJSON_PrintPreallocated(root, prebuf, (int)bufsize, 1);
    (void)ok;
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}