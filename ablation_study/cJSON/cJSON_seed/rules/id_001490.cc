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
//<ID> 1490
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
    cJSON *parsed = NULL;
    cJSON *str_item = NULL;
    cJSON *fetched = NULL;
    const char *name_val = NULL;
    int arr_size = 0;
    cJSON_bool enabled_check = 0;
    char json[] = " { \"embedded\" : true , \"name\" : \"example\" } ";
    char prebuf[128];
    memset(prebuf, 0, sizeof(prebuf));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddBoolToObject(root, "enabled", 1);
    str_item = cJSON_CreateString("first");
    cJSON_AddItemToArray(arr, str_item);
    parsed = cJSON_Parse(json);
    cJSON_AddItemToObject(root, "embedded", parsed);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    fetched = cJSON_GetArrayItem(arr, 0);
    name_val = cJSON_GetStringValue(cJSON_GetObjectItem(parsed, "name"));
    enabled_check = cJSON_IsTrue(cJSON_GetObjectItem(root, "enabled"));
    (void)prebuf;
    (void)arr_size;
    (void)fetched;
    (void)name_val;
    (void)enabled_check;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}