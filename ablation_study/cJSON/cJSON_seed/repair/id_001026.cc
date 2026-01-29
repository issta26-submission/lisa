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
//<ID> 1026
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "count", 42.5);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddNumberToObject(root, "root_value", 3.14);

    // step 3: Operate and Validate
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *count_item = cJSON_GetObjectItem(meta_item, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(root, "double_count", count_val * 2.0);
    cJSON_bool is_array = cJSON_IsArray(values);
    cJSON_AddBoolToObject(root, "values_is_array", is_array);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 0);
    cJSON_free(buf);
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}