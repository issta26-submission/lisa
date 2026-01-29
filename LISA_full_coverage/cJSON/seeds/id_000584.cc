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
//<ID> 584
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *item1 = cJSON_CreateObject();
    cJSON_AddStringToObject(item1, "name", "alpha");
    cJSON_AddNumberToObject(item1, "value", 1.0);
    cJSON_AddItemToArray(items, item1);
    cJSON *item2 = cJSON_CreateObject();
    cJSON_AddStringToObject(item2, "name", "beta");
    cJSON_AddNumberToObject(item2, "value", 2.0);
    cJSON_AddItemToArray(items, item2);
    cJSON_AddStringToObject(meta, "temp", "remove_me");

    // step 2: Configure
    cJSON *first = cJSON_GetArrayItem(items, 0);
    char *first_name_before = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(first, "name"));
    char *old_name = cJSON_SetValuestring(cJSON_GetObjectItemCaseSensitive(first, "name"), "ALPHA");
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *meta_temp_before = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "temp"));

    // step 3: Operate
    char *buffered = cJSON_PrintBuffered(root, 256, 1);
    int prelen = 512;
    char *prebuf = (char *)cJSON_malloc(prelen);
    memset(prebuf, 0, prelen);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, prelen, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(meta, "temp");
    char *dup_temp_after = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(dup, "meta"), "temp"));

    // step 4: Validate & Cleanup
    (void)first_name_before;
    (void)old_name;
    (void)meta_temp_before;
    (void)dup_temp_after;
    (void)pre_ok;
    cJSON_free(buffered);
    cJSON_free(prebuf);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}