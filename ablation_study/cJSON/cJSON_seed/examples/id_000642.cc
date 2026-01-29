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
//<ID> 642
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *str1 = cJSON_CreateString("hello");
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToArray(list, num1);
    cJSON_AddItemToArray(list, str1);
    cJSON_AddItemToObject(root, "maybe", null_item);
    cJSON_AddItemToObject(root, "list", list);

    // step 3: Operate & Validate
    cJSON *num2 = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemViaPointer(list, num1, num2);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    int count = cJSON_GetArraySize(list);
    cJSON *first = cJSON_GetArrayItem(list, 0);
    double value = cJSON_GetNumberValue(first);
    (void)version;
    (void)count;
    (void)value;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}