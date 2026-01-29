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
//<ID> 1067
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
    cJSON *objA = NULL;
    cJSON *objB = NULL;
    cJSON *detached = NULL;
    cJSON_bool equal = 0;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    objA = cJSON_CreateObject();
    objB = cJSON_CreateObject();
    cJSON_AddStringToObject(objA, "name", "ObjectA");
    cJSON_AddStringToObject(objB, "name", "ObjectB");
    cJSON_AddItemToObject(root, "a", objA);
    cJSON_AddItemReferenceToArray(arr, objB);

    // step 3: Operate / Validate
    equal = cJSON_Compare(objA, objB, 1);
    detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_AddItemToObject(root, "b", detached);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}