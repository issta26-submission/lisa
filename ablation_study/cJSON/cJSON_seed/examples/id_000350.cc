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
//<ID> 350
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *words[] = { "apple", "banana", "cherry" };
    cJSON *str_arr = cJSON_CreateStringArray(words, 3);
    cJSON *list = cJSON_CreateArray();
    cJSON *item0 = cJSON_CreateString("first");
    cJSON *item1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(list, item0);
    cJSON_AddItemToArray(list, item1);
    cJSON_AddItemToObject(root, "strings", str_arr);
    cJSON_AddItemToObject(root, "list", list);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(10.0);
    cJSON_SetNumberHelper(num, 99.5);
    cJSON_AddItemToObject(root, "magic", num);
    cJSON *replacement = cJSON_CreateString("replaced-second");
    cJSON_ReplaceItemInArray(list, 1, replacement);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(str_arr);
    cJSON *first_str = cJSON_GetArrayItem(str_arr, 0);
    const char *first_val = cJSON_GetStringValue(first_str);
    double magic_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "magic"));
    double combined = size + magic_val;
    cJSON *combined_num = cJSON_CreateNumber(combined);
    cJSON_AddItemToObject(root, "combined", combined_num);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}