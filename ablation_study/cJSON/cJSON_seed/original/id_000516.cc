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
//<ID> 516
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_name[] = "device-01";
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString(initial_name));
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(30));
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag_false);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "override");

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t buf_len = (snapshot ? strlen(snapshot) : 0) + 32;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    int size_before = cJSON_GetArraySize(values);
    double first_val_before = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    cJSON_DeleteItemFromArray(values, 1);
    int size_after = cJSON_GetArraySize(values);
    char *first_item_snapshot = cJSON_PrintUnformatted(cJSON_GetArrayItem(values, 0));

    // step 4: Validate & Cleanup
    (void)initial_name;
    (void)size_before;
    (void)first_val_before;
    (void)size_after;
    cJSON_free(snapshot);
    cJSON_free(first_item_snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}