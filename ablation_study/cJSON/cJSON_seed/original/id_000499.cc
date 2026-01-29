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
//<ID> 499
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"data\":[10,20,30],\"flag\":true}";
    cJSON *root = cJSON_Parse(json);
    cJSON *data = cJSON_GetObjectItem(root, "data");
    cJSON *flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool data_is_array = cJSON_IsArray(data);
    int count = cJSON_GetArraySize(data);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(data, 0));

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    double computed = first_val + (double)count + (double)data_is_array;
    cJSON *comp_num = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(meta, "computed", comp_num);
    cJSON_AddFalseToObject(meta, "ok");
    cJSON_AddNullToObject(meta, "note");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    size_t buf_len = 64 + (size_t)count;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    char *snapshot = cJSON_PrintUnformatted(root);
    char *flag_str = cJSON_GetStringValue(flag);
    double extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "computed"));

    // step 4: Validate & Cleanup
    (void)flag_str;
    (void)extracted;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}