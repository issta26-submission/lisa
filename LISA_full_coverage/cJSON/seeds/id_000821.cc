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
//<ID> 821
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
    cJSON *items = cJSON_CreateArray();
    cJSON *first = cJSON_CreateString("first");
    cJSON *second = cJSON_CreateNumber(2.0);
    cJSON *third = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, first);
    cJSON_AddItemToArray(items, second);
    cJSON_AddItemToArray(items, third);
    cJSON_AddStringToObject(meta, "version", cJSON_Version());
    cJSON_AddNumberToObject(meta, "count", (double)cJSON_GetArraySize(items));
    cJSON *inserted = cJSON_CreateString("inserted");
    cJSON_InsertItemInArray(items, 1, inserted);

    // step 3: Operate & Validate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON *got_version = cJSON_GetObjectItem(meta, "version");
    const char *ver = cJSON_GetStringValue(got_version);
    cJSON *got_item1 = cJSON_GetArrayItem(items, 1);
    const char *val1 = cJSON_GetStringValue(got_item1);
    double count_d = (double)cJSON_GetArraySize(items);
    (void)ver;
    (void)val1;
    (void)count_d;
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_DeleteItemFromObjectCaseSensitive(meta, "count");

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}