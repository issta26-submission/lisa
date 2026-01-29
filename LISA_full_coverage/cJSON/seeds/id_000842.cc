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
//<ID> 842
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    char rawjson[] = "  { \"alpha\" : 1, \"beta\" : [ 10, 20 ] }  ";
    cJSON_Minify(rawjson);
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num_item, 2.71828);
    cJSON_AddItemToArray(items, num_item);

    // step 2: Configure
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", obj);
    cJSON_AddStringToObject(obj, "name", "config1");
    cJSON_AddNumberToObject(obj, "threshold", 100.0);
    cJSON *replacement = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(replacement, 200.0);
    cJSON_ReplaceItemInObjectCaseSensitive(obj, "threshold", replacement);

    // step 3: Operate
    cJSON *copy_items = cJSON_Duplicate(items, 1);
    cJSON *copies = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "copies", copies);
    cJSON_AddItemToArray(copies, copy_items);
    char *out = cJSON_PrintUnformatted(root);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 0));
    double threshold_val = cJSON_GetNumberValue(cJSON_GetObjectItem(obj, "threshold"));
    (void)first_val;
    (void)threshold_val;

    // step 4: Validate & Cleanup
    int size = cJSON_GetArraySize(items);
    (void)size;
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}