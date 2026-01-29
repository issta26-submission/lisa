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
//<ID> 1502
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
    cJSON *arr = NULL;
    cJSON *detached = NULL;
    cJSON *dup = NULL;
    cJSON_bool cmp_res = 0;
    cJSON_bool printed_ok = 0;
    char json[] = " { \"alpha\" : 1, \"beta\" : [10, 20, \"twenty\"], \"gamma\" : \"x\" } ";
    char prebuf[256];
    memset(prebuf, 0, sizeof(prebuf));
    parsed = cJSON_ParseWithLength(json, sizeof(json) - 1);

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    arr = cJSON_GetObjectItem(parsed, "beta");
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(30.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("hello"));

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromArray(arr, 2);
    cJSON_AddItemToObject(root, "detached", detached);
    dup = cJSON_Duplicate(detached, 1);
    cJSON_AddItemToObject(root, "copy", dup);
    cmp_res = cJSON_Compare(detached, dup, 1);
    printed_ok = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 1);
    (void)cmp_res;
    (void)printed_ok;
    (void)prebuf;
    cJSON_DeleteItemFromObjectCaseSensitive(root, "detached");

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}