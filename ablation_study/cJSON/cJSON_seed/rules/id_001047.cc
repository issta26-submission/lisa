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
//<ID> 1047
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
    cJSON *elem0 = NULL;
    cJSON *elem1 = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    char *printed_elem0 = NULL;
    char *printed_root = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    elem0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, elem0);
    elem1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddStringToObject(root, "status", "ready");

    // step 3: Operate / Validate
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    printed_elem0 = cJSON_Print(got0);
    printed_root = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed_elem0);
    cJSON_free(printed_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}