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
//<ID> 913
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"root\":{\"keep\":true,\"old\":[10,20],\"name\":\"example\"}}";
    size_t json_len = sizeof(json) - 1;
    cJSON *doc = cJSON_ParseWithLength(json, json_len);
    cJSON *root = cJSON_GetObjectItemCaseSensitive(doc, "root");
    cJSON *oldArr = cJSON_GetObjectItemCaseSensitive(root, "old");

    // step 2: Configure
    cJSON *detached_name = cJSON_DetachItemFromObject(root, "name");
    cJSON *combined = cJSON_AddArrayToObject(root, "combined");
    cJSON *old0 = cJSON_GetArrayItem(oldArr, 0);
    cJSON *old1 = cJSON_GetArrayItem(oldArr, 1);
    double v0 = cJSON_GetNumberValue(old0);
    double v1 = cJSON_GetNumberValue(old1);
    cJSON *n0 = cJSON_CreateNumber(v0 * 2.0);
    cJSON *n1 = cJSON_CreateNumber(v1 * 3.0);

    // step 3: Operate
    cJSON_AddItemToArray(combined, n0);
    cJSON_AddItemToArray(combined, n1);
    cJSON_AddItemToArray(combined, detached_name);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(doc);
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    char *prebuf = (char *)buf;
    cJSON_bool ok = cJSON_PrintPreallocated(doc, prebuf, 256, 1);
    (void)ok;
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(doc);

    // API sequence test completed successfully
    return 66;
}