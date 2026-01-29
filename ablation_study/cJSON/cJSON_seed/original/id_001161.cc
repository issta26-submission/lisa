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
//<ID> 1161
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "init_num", 10.0);
    cJSON *raw_item = cJSON_CreateRaw("unparsed_fragment");
    cJSON_AddItemToObject(root, "init_raw", raw_item);
    const char *json1 = "{\"parsed_num\": 7.25}";
    cJSON *parsed1 = cJSON_ParseWithLength(json1, (size_t)strlen(json1));
    cJSON_AddItemToObject(root, "parsed1", parsed1);
    const char *json2 = "{\"parsed_num2\": 2.5}TRAIL";
    const char *endptr = NULL;
    cJSON *parsed2 = cJSON_ParseWithOpts(json2, &endptr, 0);
    cJSON_AddItemToObject(root, "parsed2", parsed2);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_AddItemToObject(root, "dup_copy", dup);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));

    // step 3: Operate
    cJSON *num_item = cJSON_GetObjectItem(root, "init_num");
    double init_val = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(root, "init_val_copy", init_val);
    cJSON *p1_num = cJSON_GetObjectItem(parsed1, "parsed_num");
    double p1_val = cJSON_GetNumberValue(p1_num);
    cJSON_AddNumberToObject(root, "p1_val", p1_val);
    cJSON *p2_num = cJSON_GetObjectItem(parsed2, "parsed_num2");
    double p2_val = cJSON_GetNumberValue(p2_num);
    cJSON_bool raw_flag = cJSON_IsRaw(cJSON_GetObjectItem(root, "init_raw"));
    cJSON_AddBoolToObject(root, "init_raw_is_raw", raw_flag);
    double total = init_val + p1_val + p2_val;
    cJSON_AddNumberToObject(root, "total", total);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}