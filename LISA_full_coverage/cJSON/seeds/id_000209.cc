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
//<ID> 209
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
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, n2);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemViaPointer(arr, s1);
    cJSON_AddItemToObject(root, "detached", detached);
    cJSON *subobj = cJSON_CreateObject();
    cJSON_AddItemToObject(subobj, "inner_number", cJSON_CreateNumber(3.14));
    cJSON_AddItemToObject(root, "meta", subobj);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    int array_size = cJSON_GetArraySize(arr);
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON *det_item = cJSON_GetObjectItem(root, "detached");
    char *det_str = cJSON_GetStringValue(det_item);
    int summary = array_size + (int)first_val + (det_str ? (int)det_str[0] : 0) + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}