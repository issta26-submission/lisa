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
//<ID> 846
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *entry = cJSON_CreateObject();
    cJSON_AddStringToObject(entry, "note", "initial");
    cJSON_AddItemToObject(root, "entry", entry);
    cJSON *num = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(arr, num);

    // step 2: Configure
    cJSON_SetNumberHelper(num, 42.0);
    cJSON *elem2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, elem2);

    // step 3: Operate
    char text[] = " { \"foo\" : [ 1, 2 , 3 ] } ";
    cJSON_Minify(text);
    cJSON *parsed = cJSON_Parse(text);
    cJSON *new_entry = cJSON_CreateObject();
    cJSON *new_num = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(new_entry, "replaced_value", new_num);
    cJSON_AddStringToObject(new_entry, "status", "updated");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "entry", new_entry);

    // step 4: Validate & Cleanup
    int size = cJSON_GetArraySize(arr);
    double first = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    char *out = cJSON_PrintUnformatted(root);
    (void)size;
    (void)first;
    cJSON_free(out);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}