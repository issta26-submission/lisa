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
//<ID> 1508
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *parsed = NULL;
    cJSON *dup_parsed = NULL;
    cJSON *detached = NULL;
    cJSON_bool cmp_parsed_dup = 0;
    cJSON_bool cmp_detached_num = 0;
    cJSON_bool printed_ok = 0;
    char json[] = " { \"embedded\" : true , \"value\" : 123 } ";
    char prebuf[256];
    memset(prebuf, 0, sizeof(prebuf));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddStringToObject(root, "ToDelete", "remove_me");
    parsed = cJSON_ParseWithLength(json, (size_t)(sizeof(json) - 1));
    cJSON_AddItemToObject(root, "imported", parsed);

    // step 3: Operate / Validate
    dup_parsed = cJSON_Duplicate(parsed, 1);
    cmp_parsed_dup = cJSON_Compare(parsed, dup_parsed, 1);
    detached = cJSON_DetachItemFromArray(arr, 1);
    cmp_detached_num = cJSON_Compare(detached, num2, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "ToDelete");
    printed_ok = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 1);

    // step 4: Cleanup
    cJSON_Delete(dup_parsed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}