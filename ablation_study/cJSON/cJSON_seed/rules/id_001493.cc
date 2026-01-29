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
//<ID> 1493
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
    cJSON *embedded = NULL;
    char prebuf[128];
    memset(prebuf, 0, sizeof(prebuf));
    const char json[] = " { \"embedded\" : true , \"empty\" : null } ";

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *added_bool = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddItemToObject(root, "greeting", cJSON_CreateString("hello"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(42.0));

    // step 3: Operate / Validate
    int arr_size = cJSON_GetArraySize(arr);
    embedded = cJSON_GetObjectItem(parsed, "embedded");
    cJSON_bool emb_true = cJSON_IsTrue(embedded);
    const char *greeting = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    (void)prebuf;
    (void)arr_size;
    (void)emb_true;
    (void)greeting;
    (void)added_bool;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}