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
//<ID> 1428
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"values\":[10,20,30]}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *values_arr = cJSON_GetObjectItem(root, "values");
    cJSON *second_elem = cJSON_GetArrayItem(values_arr, 1);
    cJSON_bool was_invalid_before = cJSON_IsInvalid(second_elem);

    // step 2: Configure
    cJSON *new_num = cJSON_CreateNumber(99.5);
    cJSON_AddItemToArray(values_arr, new_num);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON *root2 = cJSON_Parse(buf);
    cJSON *values_arr2 = cJSON_GetObjectItem(root2, "values");
    cJSON *fourth_elem = cJSON_GetArrayItem(values_arr2, 3);
    cJSON_bool is_invalid_after = cJSON_IsInvalid(fourth_elem);
    double fourth_value = cJSON_GetNumberValue(fourth_elem);
    (void)was_invalid_before;
    (void)is_invalid_after;
    (void)fourth_value;
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(root2);
    // API sequence test completed successfully
    return 66;
}