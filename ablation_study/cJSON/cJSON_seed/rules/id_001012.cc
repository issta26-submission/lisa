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
//<ID> 1012
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *darray = NULL;
    cJSON *flag = NULL;
    cJSON *raw = NULL;
    cJSON *item_to_replace = NULL;
    cJSON *newnum = NULL;
    cJSON *dup = NULL;
    cJSON *enabled_item = NULL;
    char *printed = NULL;
    double numbers[3];
    const char *raw_text = "{\"rawKey\":123}";
    int size_after = 0;
    double replaced_value = 0.0;
    cJSON_bool enabled_val = 0;

    numbers[0] = 1.1;
    numbers[1] = 2.2;
    numbers[2] = 3.3;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    darray = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", darray);
    flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag);
    raw = cJSON_CreateRaw(raw_text);
    cJSON_AddItemToObject(root, "meta", raw);
    item_to_replace = cJSON_GetArrayItem(darray, 1);
    newnum = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemViaPointer(darray, item_to_replace, newnum);

    // step 3: Operate / Validate
    enabled_item = cJSON_GetObjectItem(root, "enabled");
    enabled_val = cJSON_IsTrue(enabled_item);
    printed = cJSON_PrintUnformatted(root);
    dup = cJSON_Duplicate(root, 1);
    size_after = cJSON_GetArraySize(cJSON_GetObjectItem(dup, "values"));
    replaced_value = cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(dup, "values"), 1));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}