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
//<ID> 486
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"orig\",\"flag\":false,\"count\":3}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);

    // step 2: Configure
    cJSON *new_bool = cJSON_CreateBool(1);
    cJSON_bool replace_ok = cJSON_ReplaceItemInObject(root, "flag", new_bool);
    int *nums_buf = (int *)cJSON_malloc(sizeof(int) * 2);
    nums_buf[0] = (int)count_val;
    nums_buf[1] = (int)count_val + 4;
    cJSON *nums_item = cJSON_CreateIntArray(nums_buf, 2);
    cJSON_AddItemToObject(root, "nums", nums_item);

    // step 3: Operate
    char *print_buf = (char *)cJSON_malloc(256);
    cJSON_PrintPreallocated(root, print_buf, 256, 0);
    cJSON *n0 = cJSON_GetArrayItem(nums_item, 0);
    cJSON *n1 = cJSON_GetArrayItem(nums_item, 1);
    double sum = cJSON_GetNumberValue(n0) + cJSON_GetNumberValue(n1) + (double)name_is_string;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Validate & Cleanup
    (void)replace_ok;
    cJSON_free(print_buf);
    cJSON_free(nums_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}