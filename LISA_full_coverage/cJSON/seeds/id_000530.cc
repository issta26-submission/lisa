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
//<ID> 530
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names_arr[] = { "alpha", "beta", "gamma" };
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON_AddItemToObject(root, "names", names);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(root, "count", count);
    const char *version_str = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(version_str);
    cJSON_AddItemToObject(root, "lib_version", version_item);

    // step 2: Configure
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    int detach_index = (int)count_val - 1;
    cJSON *detached = cJSON_DetachItemFromArray(cJSON_GetObjectItem(root, "names"), detach_index);
    const char *detached_value = cJSON_GetStringValue(detached);
    cJSON *last_name = cJSON_CreateString(detached_value);
    cJSON_AddItemToObject(root, "last_name", last_name);
    cJSON_Delete(detached);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 32;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    double final_count = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    const char *final_last = cJSON_GetStringValue(cJSON_GetObjectItem(root, "last_name"));
    (void)final_count;
    (void)final_last;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}