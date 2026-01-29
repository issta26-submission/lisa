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
//<ID> 828
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
    cJSON *items = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(1.234);
    cJSON *str0 = cJSON_CreateString("alpha");
    cJSON *num1 = cJSON_CreateNumber(42.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, num0);
    cJSON_AddItemToArray(items, str0);
    cJSON_InsertItemInArray(items, 1, num1);
    cJSON_AddStringToObject(meta, "version", cJSON_Version());
    cJSON_AddStringToObject(meta, "note", "temporary");

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    int count = cJSON_GetArraySize(items);
    cJSON *middle = cJSON_GetArrayItem(items, 1);
    double middle_val = cJSON_GetNumberValue(middle);
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_version = cJSON_GetObjectItemCaseSensitive(got_meta, "version");
    const char *version_str = cJSON_GetStringValue(got_version);
    (void)count;
    (void)middle_val;
    (void)version_str;
    cJSON_DeleteItemFromObjectCaseSensitive(got_meta, "note");

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}