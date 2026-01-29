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
//<ID> 1024
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"root\":{\"name\":\"alpha\",\"values\":[1,2,3]},\"meta\":\"info\"}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");
    cJSON *values = cJSON_GetObjectItem(root_obj, "values");
    cJSON *name_item = cJSON_GetObjectItem(root_obj, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    int extra_nums[] = {10, 20, 30, 40};
    cJSON *extra_array = cJSON_CreateIntArray(extra_nums, 4);
    cJSON_AddItemToObject(root_obj, "extra_numbers", extra_array);
    cJSON *dup_root = cJSON_Duplicate(root_obj, 1);
    cJSON_AddItemToObject(root, "root_copy", dup_root);

    // step 3: Operate
    cJSON *out = cJSON_CreateObject();
    cJSON *copied_name = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(out, "copied_name", copied_name);
    cJSON *values_copy = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(out, "numbers_copy", values_copy);
    char *out_unformatted = cJSON_PrintUnformatted(out);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(out, print_buf, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(out_unformatted);
    cJSON_free(print_buf);
    cJSON_Delete(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}