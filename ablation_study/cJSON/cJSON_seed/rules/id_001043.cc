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
//<ID> 1043
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
    cJSON *first = NULL;
    cJSON *second = NULL;
    cJSON *copied = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    first = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, first);
    second = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, second);
    cJSON_AddStringToObject(root, "status", "created");

    // step 3: Operate / Validate
    cJSON *got = cJSON_GetArrayItem(arr, 1);
    copied = cJSON_AddStringToObject(root, "copied_item", cJSON_GetStringValue(got));
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}