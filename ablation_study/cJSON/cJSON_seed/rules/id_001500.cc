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
//<ID> 1500
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
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *detached = NULL;
    cJSON *parsed = NULL;
    cJSON *tmp_num = NULL;
    cJSON_bool cmp = 0;
    cJSON_bool printed_ok = 0;
    char json_parsed[] = " { \"alpha\" : 123 , \"beta\" : false } ";
    char prebuf[256];
    size_t parsed_len = sizeof(json_parsed) - 1;
    memset(prebuf, 0, sizeof(prebuf));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    item0 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(arr, item0);
    item1 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, item1);
    parsed = cJSON_ParseWithLength(json_parsed, parsed_len);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate / Validate
    tmp_num = cJSON_CreateNumber(1.0);
    cmp = cJSON_Compare(item0, tmp_num, 1);
    cJSON_Delete(tmp_num);
    detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_AddItemToObject(root, "moved", detached);
    printed_ok = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "parsed");

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)cmp;
    (void)printed_ok;
    (void)prebuf;
    return 66;
}