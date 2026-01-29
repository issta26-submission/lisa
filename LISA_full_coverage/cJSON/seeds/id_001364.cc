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
//<ID> 1364
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    const char *json = "{\"nums\":[4,5,6],\"val\":7}";
    size_t json_len = strlen(json);
    cJSON *parsed = cJSON_ParseWithLength(json, json_len);
    float fnums[3] = {1.5f, 2.5f, 3.75f};
    cJSON *float_arr = cJSON_CreateFloatArray(fnums, 3);

    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "source", parsed);
    cJSON_AddItemToObject(root, "floats", float_arr);
    cJSON *floats_ref = cJSON_CreateArrayReference(float_arr);
    cJSON_AddItemToObject(root, "floats_ref", floats_ref);
    cJSON *dup_source = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "dup_source", dup_source);

    cJSON *nums = cJSON_GetObjectItem(parsed, "nums");
    cJSON *first = cJSON_GetArrayItem(nums, 0);
    cJSON_bool first_is_num = cJSON_IsNumber(first);
    cJSON *marker = cJSON_CreateNumber((double)first_is_num);
    cJSON_AddItemToObject(root, "first_is_number", marker);

    int buf_len = 1024;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 0);
    cJSON_Minify(buf);
    cJSON_free(buf);
    cJSON_Delete(root);
    return 66;
}