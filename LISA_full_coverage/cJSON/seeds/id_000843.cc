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
//<ID> 843
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
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(n2, 3.1415);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "replace_me", cJSON_CreateString("old_value"));

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("new_value");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "replace_me", replacement);
    char json_text[] = " { \"parsed_obj\" : { \"x\" : 10 , \"y\" : 20 } , \"vals\" : [ 5 , 6 ] } ";
    cJSON_Minify(json_text);
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON_AddItemToObject(root, "parsed_copy", cJSON_Duplicate(parsed, 1));

    // step 3: Operate
    char *compact = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(arr);
    double second_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    char *replaced_str = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(root, "replace_me"));
    (void)arr_size;
    (void)second_val;
    (void)replaced_str;

    // step 4: Validate & Cleanup
    cJSON_free(compact);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}