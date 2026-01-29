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
//<ID> 1551
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
    cJSON *first_item = NULL;
    cJSON *second_item = NULL;
    cJSON *new_item = NULL;
    char *unformatted = NULL;
    void *buffer = NULL;
    int cmp_flag = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    first_item = cJSON_CreateString("alpha");
    second_item = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, first_item);
    cJSON_AddItemToArray(arr, second_item);
    buffer = cJSON_malloc((size_t)64);
    memset(buffer, 0, (size_t)64);
    ((char *)buffer)[0] = 'X';

    // step 3: Operate / Validate
    cJSON *retrieved = cJSON_GetArrayItem(arr, 0);
    new_item = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(arr, retrieved, new_item);
    cmp_flag = cJSON_Compare(new_item, cJSON_GetArrayItem(arr, 0), /*case_sensitive=*/1);
    cJSON_AddNumberToObject(root, "compare_flag", (double)cmp_flag);
    cJSON_AddStringToObject(root, "status", "first_element_replaced");

    // step 4: Cleanup
    unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}