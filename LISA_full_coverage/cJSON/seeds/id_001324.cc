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
//<ID> 1324
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"nums\":[1,2],\"config\":{\"threshold\":10}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *nums = cJSON_GetObjectItem(parsed, "nums");
    cJSON *config = cJSON_GetObjectItem(parsed, "config");
    cJSON *threshold = cJSON_GetObjectItem(config, "threshold");

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *ref_num = cJSON_CreateNumber(3.0);
    cJSON_AddItemReferenceToArray(nums, ref_num);
    cJSON *insert_num = cJSON_CreateNumber(5.0);
    cJSON_InsertItemInArray(nums, 1, insert_num);
    cJSON_SetNumberHelper(threshold, 15.0);

    // step 3: Operate
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_bool ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 4: Validate & Cleanup
    size_t printed_len = strlen(buffer);
    (void)printed_len;
    (void)ok;
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}