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
//<ID> 826
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
    cJSON *num1 = cJSON_CreateNumber(1.23);
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *str2 = cJSON_CreateString("beta");
    cJSON *temp = cJSON_CreateString("to_be_removed");
    cJSON *inserted = cJSON_CreateString("inserted");

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(meta, "version", cJSON_Version());
    cJSON_AddItemToObject(meta, "temp", temp);
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, str1);
    cJSON_AddItemToArray(items, str2);
    cJSON_InsertItemInArray(items, 1, inserted);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_free(pretty);
    cJSON_free(compact);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(meta, "temp");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}