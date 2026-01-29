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
//<ID> 1164
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
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "data", data);
    cJSON *n_count = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(meta, "count", n_count);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(data, s1);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "note", "initial");
    cJSON_AddNullToObject(meta, "empty");
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    cJSON_bool isobj = cJSON_IsObject(detached_meta);
    cJSON *empty_item = cJSON_GetObjectItem(detached_meta, "empty");
    cJSON_bool isnull = cJSON_IsNull(empty_item);
    double updated = cJSON_SetNumberHelper(cJSON_GetObjectItem(detached_meta, "count"), 10.5);
    cJSON_AddItemToArray(data, detached_meta);
    cJSON *computed = cJSON_CreateNumber(updated + (double)isobj + (double)isnull);
    cJSON_AddItemToObject(detached_meta, "computed", computed);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}