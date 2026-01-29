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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *n3 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *marker_obj = (cJSON *)0;
    cJSON *size_item = (cJSON *)0;
    cJSON *marker_item = (cJSON *)0;
    double marker_value = 0.0;
    int size_before = 0;
    int size_after = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateString("one");
    n2 = cJSON_CreateString("two");
    n3 = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "marker", cJSON_CreateTrue());

    // step 3: Operate
    size_before = cJSON_GetArraySize(arr);
    cJSON *list_in_root = cJSON_GetObjectItem(root, "list");
    replacement = cJSON_CreateNumber(123.0);
    cJSON_ReplaceItemInObject(root, "marker", replacement);
    detached = cJSON_DetachItemFromObject(root, "list");
    size_after = cJSON_GetArraySize(detached);
    size_item = cJSON_CreateNumber((double)size_after);
    cJSON_AddItemToObject(root, "list_size", size_item);
    marker_obj = cJSON_GetObjectItem(root, "marker");
    marker_value = cJSON_GetNumberValue(marker_obj);
    marker_item = cJSON_CreateNumber(marker_value);
    cJSON_AddItemToObject(root, "marker_value", marker_item);

    // step 4: Validate
    cJSON_AddNumberToObject(root, "size_delta", (double)(size_before - size_after));

    // step 5: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}