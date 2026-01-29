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
//<ID> 882
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
    cJSON *dbl_arr = NULL;
    cJSON *str_item = NULL;
    char *printed = NULL;
    double numbers[3];
    int size_before = 0;
    int size_after = 0;
    double version_val = 0.0;
    const char *retrieved_str = NULL;
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    numbers[0] = 1.1;
    numbers[1] = 2.2;
    numbers[2] = 3.3;
    dbl_arr = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToArray(arr, dbl_arr);
    str_item = cJSON_CreateString("entry");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddNumberToObject(root, "version", 2.718);

    // step 3: Operate / Validate
    size_before = cJSON_GetArraySize(arr);
    version_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "version"));
    retrieved_str = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    printed = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromArray(arr, 0);
    size_after = cJSON_GetArraySize(arr);
    (void)size_before;
    (void)size_after;
    (void)version_val;
    (void)retrieved_str;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}