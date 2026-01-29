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
//<ID> 1342
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text1 = "{\"arr\":[10,20,30],\"flag\":true}";
    const char *json_text2 = "{\"arr\":[100,200,300],\"remove\":123} trailing";
    const char *parse_end = NULL;
    cJSON *root1 = cJSON_Parse(json_text1);
    cJSON *root2 = cJSON_ParseWithOpts(json_text2, &parse_end, 0);

    // step 2: Configure
    cJSON *arr1 = cJSON_GetObjectItem(root1, "arr");
    cJSON *arr2 = cJSON_GetObjectItem(root2, "arr");
    cJSON *replacement = cJSON_CreateNumber(999.0);
    cJSON_ReplaceItemInArray(arr1, 1, replacement);
    cJSON_AddNullToObject(root1, "nullable");
    cJSON_AddItemToObject(root1, "merged", root2);

    // step 3: Operate
    cJSON_DeleteItemFromObjectCaseSensitive(root2, "remove");
    double replaced_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr1, 1));
    cJSON *marker = cJSON_CreateNumber(replaced_value);
    cJSON_AddItemToObject(root1, "marker", marker);

    // step 4: Validate & Cleanup
    cJSON *dup = cJSON_Duplicate(root1, 1);
    cJSON_bool equal = cJSON_Compare(root1, dup, 1);
    cJSON_AddBoolToObject(root1, "equal_copy", equal);
    cJSON_Delete(dup);
    cJSON_Delete(root1);
    // API sequence test completed successfully
    return 66;
}