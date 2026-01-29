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
//<ID> 1588
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
    cJSON *nested = NULL;
    cJSON *rawitem = NULL;
    cJSON *ref = NULL; /* will be used for a duplicated nested object */
    cJSON *second = NULL;
    cJSON *copy = NULL;
    const char *second_str = NULL;
    char *out = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    item0 = cJSON_CreateString("first");
    item1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    nested = cJSON_AddObjectToObject(root, "nested");
    rawitem = cJSON_AddRawToObject(nested, "payload", "{\"a\":1,\"b\":true}");
    ref = cJSON_Duplicate(nested, 1);
    cJSON_AddItemToObject(root, "nested_ref", ref);

    // step 3: Operate / Validate
    second = cJSON_GetArrayItem(arr, 1);
    second_str = cJSON_GetStringValue(second);
    copy = cJSON_CreateString(second_str);
    cJSON_AddItemToObject(root, "second_copy", copy);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
}