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
//<ID> 901
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metadata = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", metadata);
    double dvals[4] = {1.1, 2.2, 3.3, 4.4};
    cJSON *darr = cJSON_CreateDoubleArray(dvals, 4);
    cJSON_AddItemToObject(metadata, "values", darr);

    // step 2: Configure
    cJSON *id_ref = cJSON_CreateStringReference("constant-id-123");
    cJSON_AddItemToObject(metadata, "id", id_ref);
    cJSON *active_item = cJSON_AddTrueToObject(metadata, "active");
    cJSON *status_ref = cJSON_CreateStringReference("initial");
    cJSON_AddItemToObject(root, "status", status_ref);

    // step 3: Operate
    cJSON *new_status = cJSON_CreateStringReference("replaced");
    cJSON_bool replace_ok = cJSON_ReplaceItemInObjectCaseSensitive(root, "status", new_status);
    (void)replace_ok;
    cJSON *item0 = cJSON_GetArrayItem(darr, 0);
    cJSON *item1 = cJSON_GetArrayItem(darr, 1);
    double sum01 = cJSON_GetNumberValue(item0) + cJSON_GetNumberValue(item1);
    cJSON_AddNumberToObject(metadata, "sum01", sum01);
    void *buf = cJSON_malloc(512);
    memset(buf, 0, 512);
    char *prebuf = (char *)buf;
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, 512, 1);
    (void)pre_ok;
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}