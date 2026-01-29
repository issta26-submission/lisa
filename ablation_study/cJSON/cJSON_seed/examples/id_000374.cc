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
//<ID> 374
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *title = cJSON_CreateString("sample_doc");
    cJSON_AddItemToObject(root, "title", title);
    cJSON *maybe_missing = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_missing", maybe_missing);
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi);
    cJSON *greet = cJSON_CreateString("hello");
    cJSON *greet_dup = cJSON_Duplicate(greet, 1);
    cJSON_AddItemToObject(root, "greeting", greet_dup);
    cJSON_Delete(greet);

    // step 2: Configure
    int nums[3] = {1, 2, 3};
    cJSON *int_arr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", int_arr);
    double pi_val = cJSON_GetNumberValue(pi);
    char *meta_buf = (char *)cJSON_malloc(64);
    memset(meta_buf, 0, 64);
    sprintf(meta_buf, "pi=%.3f", pi_val);
    cJSON *meta = cJSON_CreateString(meta_buf);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_free(meta_buf);

    // step 3: Operate & Validate
    cJSON_bool was_null = cJSON_IsNull(maybe_missing);
    double fetched_pi = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));
    char status_buf[64];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "%d|%.3f", (int)was_null, fetched_pi);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapstr = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapstr);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}