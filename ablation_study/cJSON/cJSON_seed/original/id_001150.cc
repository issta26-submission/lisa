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
//<ID> 1150
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 2: Configure
    char *prev = cJSON_SetValuestring(name, "updated");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    (void)prev;
    (void)meta_is_obj;
    cJSON_AddStringToObject(root, "status", "prepared");

    // step 3: Operate
    cJSON *detached_count = cJSON_DetachItemFromObject(root, "count");
    cJSON_bool detached_invalid = cJSON_IsInvalid(detached_count);
    (void)detached_invalid;
    cJSON_AddItemToObject(meta, "detached_count", detached_count);
    cJSON *meta_copy = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_copy);

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