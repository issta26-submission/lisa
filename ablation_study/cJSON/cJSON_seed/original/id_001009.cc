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
//<ID> 1009
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"root\":{\"values\":[1,2,\"three\"],\"child\":{\"num\":7},\"flag\":false},\"meta\":42}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");
    cJSON *values = cJSON_GetObjectItem(root_obj, "values");
    cJSON *val1 = cJSON_GetArrayItem(values, 1);
    cJSON *val2 = cJSON_GetArrayItem(values, 2);
    cJSON_bool val1_is_num = cJSON_IsNumber(val1);
    cJSON_bool val2_is_num = cJSON_IsNumber(val2);
    cJSON *child = cJSON_GetObjectItem(root_obj, "child");
    cJSON *child_num = cJSON_GetObjectItem(child, "num");
    cJSON_bool child_num_is_num = cJSON_IsNumber(child_num);
    cJSON_bool child_num_is_invalid = cJSON_IsInvalid(child_num);
    cJSON *added_true = cJSON_AddTrueToObject(root_obj, "added_flag");
    cJSON_AddBoolToObject(root, "val1_is_num", val1_is_num);
    cJSON_AddBoolToObject(root, "val2_is_num", val2_is_num);
    cJSON_AddBoolToObject(root, "child_num_is_num", child_num_is_num);
    cJSON_AddBoolToObject(root, "child_num_is_invalid", child_num_is_invalid);

    // step 3: Operate
    double num1 = cJSON_GetNumberValue(val1);
    double num_child = cJSON_GetNumberValue(child_num);
    double sum = num1 + num_child;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 0);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root_obj, "flag");
    cJSON_DeleteItemFromObject(root, "meta");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}