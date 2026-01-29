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
//<ID> 1130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *arr2 = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *ref_item = NULL;
    cJSON *replacement0 = NULL;
    const char *first_value = NULL;
    double count_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    item0 = cJSON_CreateString("one");
    item1 = cJSON_CreateString("two");
    item2 = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObject(root, "count", cJSON_CreateNumber(3));
    arr2 = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", arr2);

    // step 3: Operate / Validate
    ref_item = cJSON_GetArrayItem(arr, 1);
    cJSON_AddItemReferenceToArray(arr2, ref_item);
    replacement0 = cJSON_CreateString("zero");
    cJSON_ReplaceItemInArray(arr, 0, replacement0);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "count", cJSON_CreateNumber(100));
    first_value = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    count_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));

    // step 4: Cleanup
    (void)first_value;
    (void)count_value;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}