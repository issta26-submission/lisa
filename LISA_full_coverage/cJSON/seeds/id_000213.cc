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
//<ID> 213
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names_arr[] = { "one", "two", "three" };
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON_AddItemToObject(root, "names", names);
    cJSON *count = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "count", count);
    double new_count = cJSON_SetNumberHelper(count, 3.0);
    (void)new_count;

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "tag", "child");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_dup);

    // step 3: Operate and Validate
    cJSON *to_detach = cJSON_GetArrayItem(names, 1);
    cJSON *detached = cJSON_DetachItemViaPointer(names, to_detach);
    cJSON_AddItemToObject(root, "detached", detached);
    char *printed = cJSON_PrintUnformatted(root);
    char *tmp_buf = (char *)cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    cJSON_Minify(printed);
    cJSON *parsed = cJSON_Parse(printed);
    int remaining = cJSON_GetArraySize(names);
    cJSON *first = cJSON_GetArrayItem(names, 0);
    const char *first_val = cJSON_GetStringValue(first);
    const char *det_val = cJSON_GetStringValue(detached);
    double verify_count = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    (void)remaining;
    (void)first_val;
    (void)det_val;
    (void)verify_count;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}