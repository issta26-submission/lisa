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
//<ID> 203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, str1);

    // step 2: Configure
    cJSON *subobj = cJSON_CreateObject();
    cJSON *subnum = cJSON_CreateNumber(3.5);
    cJSON_AddItemToObject(subobj, "subnum", subnum);
    cJSON_AddItemToArray(array, subobj);
    cJSON *to_detach = cJSON_GetArrayItem(array, 1);
    cJSON *detached = cJSON_DetachItemViaPointer(array, to_detach);
    cJSON_AddItemToObject(root, "detached_item", detached);

    // step 3: Operate and Validate
    int size_before = cJSON_GetArraySize(array);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first);
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    (void)size_before;
    (void)first_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}