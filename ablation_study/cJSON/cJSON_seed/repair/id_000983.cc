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
//<ID> 983
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

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToArray(items, s3);
    cJSON *number = cJSON_CreateNumber(10.0);
    double setval = cJSON_SetNumberHelper(number, 42.0);
    cJSON_AddItemToObject(root, "answer", number);
    cJSON *copied_number = cJSON_CreateNumber(setval);
    cJSON_AddItemToObject(root, "copied_answer", copied_number);
    cJSON_AddBoolToObject(root, "flag", 1);

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateString("delta");
    cJSON_ReplaceItemInArray(items, 1, replacement);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    char *second_str = cJSON_GetStringValue(second_item);
    cJSON *copied = cJSON_CreateString(second_str);
    cJSON_AddItemToObject(root, "copied", copied);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}