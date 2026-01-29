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
//<ID> 535
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *names[] = { "alpha", "beta", "gamma" };
    cJSON *root = cJSON_CreateObject();
    cJSON *names_arr = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", names_arr);
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(names_arr, num_item);
    cJSON *meta = cJSON_CreateObject();
    const char *version_str = cJSON_Version();
    cJSON_AddStringToObject(meta, "cjson_version", version_str);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    double extracted_num = cJSON_GetNumberValue(cJSON_GetArrayItem(names_arr, 3));
    cJSON *scaled = cJSON_CreateNumber(extracted_num * 2.0);
    cJSON_AddItemToObject(meta, "scaled", scaled);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(names_arr, 3);
    cJSON_AddItemToObject(root, "detached_number", detached);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 32;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    double retrieved = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "detached_number"));
    (void)retrieved;
    (void)extracted_num;
    (void)version_str;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}