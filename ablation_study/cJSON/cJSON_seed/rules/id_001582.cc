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
//<ID> 1582
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *meta = NULL;
    cJSON *nested = NULL;
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *arr = NULL;
    cJSON *objref = NULL;
    cJSON *picked = NULL;
    cJSON *dup = NULL;
    char *out = NULL;
    const char *picked_str = NULL;
    double picked_num = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    meta = cJSON_AddObjectToObject(root, "meta");
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "nested", nested);
    num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(nested, "answer", num);
    str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(nested, "greet", str);
    cJSON_AddRawToObject(nested, "rawfield", "{\"r\":true}");
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    objref = cJSON_CreateObjectReference(nested);
    cJSON_AddItemToArray(arr, objref);

    // step 3: Operate / Validate
    picked = cJSON_GetArrayItem(arr, 0);
    picked_num = cJSON_GetNumberValue(cJSON_GetObjectItem(picked, "answer"));
    picked_str = cJSON_GetStringValue(cJSON_GetObjectItem(picked, "greet"));
    dup = cJSON_Duplicate(picked, 1);
    cJSON_AddItemToObject(root, "picked_dup", dup);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}