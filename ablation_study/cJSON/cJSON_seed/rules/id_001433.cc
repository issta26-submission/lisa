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
//<ID> 1433
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
    cJSON *stritem = NULL;
    cJSON *falseitem = NULL;
    cJSON *rawitem = NULL;
    cJSON *first_in_array = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    const char *extracted_string = NULL;
    int prebuffer = 128;
    cJSON_bool is_raw_flag = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    stritem = cJSON_CreateString("detachable_string");
    falseitem = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, stritem);
    cJSON_AddItemToArray(arr, falseitem);
    cJSON_AddItemToObject(root, "array", arr);
    rawitem = cJSON_CreateRaw("raw_component");
    is_raw_flag = cJSON_IsRaw(rawitem);
    cJSON_AddItemToObject(root, "raw", rawitem);

    // step 3: Operate / Validate
    first_in_array = cJSON_GetArrayItem(arr, 0);
    detached = cJSON_DetachItemViaPointer(arr, first_in_array);
    extracted_string = cJSON_GetStringValue(detached);
    printed = cJSON_PrintBuffered(root, prebuffer + ((int)is_raw_flag * prebuffer), 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}