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
//<ID> 1509
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *values = NULL;
    cJSON *detached = NULL;
    cJSON *cmp_target = NULL;
    cJSON *dup_root = NULL;
    cJSON_bool cmp_res = 0;
    char json[] = " { \"name\": \"Alice\", \"values\": [1, 2, 3], \"nested\": { \"x\": true } } ";
    size_t len = sizeof(json) - 1;
    char prebuf[128];
    memset(prebuf, 0, sizeof(prebuf));

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, len);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    values = cJSON_GetObjectItem(parsed, "values");

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromArray(values, 1);
    cmp_target = cJSON_CreateNumber(2);
    cmp_res = cJSON_Compare(detached, cmp_target, 1);
    (void)cmp_res;
    cJSON_Delete(cmp_target);
    cJSON_Delete(detached);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "imported");
    dup_root = cJSON_Duplicate(root, 1);
    (void)dup_root;
    (void)prebuf;

    // step 4: Cleanup
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}