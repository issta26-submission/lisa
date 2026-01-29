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
//<ID> 1583
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *inner = NULL;
    cJSON *objAdded = NULL;
    cJSON *objRef = NULL;
    cJSON *arr = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *list = NULL;
    cJSON *picked = NULL;
    cJSON *copied = NULL;
    char *out = NULL;
    const char *raw_json = "{\"embedded_key\":123}";
    const char *picked_value = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    inner = cJSON_CreateObject();
    cJSON_AddItemToObject(inner, "name", cJSON_CreateString("inner"));
    cJSON_AddItemToObject(inner, "value", cJSON_CreateNumber(42.0));
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("one");
    item1 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObject(inner, "list", arr);
    objAdded = cJSON_AddObjectToObject(root, "child");
    cJSON_AddRawToObject(objAdded, "embedded", raw_json);
    objRef = cJSON_CreateObjectReference(inner);
    cJSON_AddItemReferenceToObject(root, "inner_ref", objRef);

    // step 3: Operate / Validate
    list = cJSON_GetObjectItem(inner, "list");
    picked = cJSON_GetArrayItem(list, 1);
    picked_value = cJSON_GetStringValue(picked);
    copied = cJSON_CreateString(picked_value);
    cJSON_AddItemToObject(root, "picked_copy", copied);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(inner);
    // API sequence test completed successfully
    return 66;
}