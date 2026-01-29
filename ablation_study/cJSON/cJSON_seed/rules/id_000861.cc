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
//<ID> 861
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
    cJSON *str_ref = NULL;
    cJSON *num_node = NULL;
    cJSON *inserted_num = NULL;
    cJSON *got_item = NULL;
    char *printed = NULL;
    const char *str_val = NULL;
    double num_val = 0.0;
    int arr_size = 0;
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    str_ref = cJSON_CreateStringReference("alpha_ref");
    cJSON_InsertItemInArray(arr, 0, str_ref);
    num_node = cJSON_CreateNumber(10.5);
    cJSON_InsertItemInArray(arr, 1, num_node);
    inserted_num = cJSON_CreateNumber(20.25);
    cJSON_InsertItemInArray(arr, 1, inserted_num);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    got_item = cJSON_GetArrayItem(arr, 0);
    str_val = cJSON_GetStringValue(got_item);
    got_item = cJSON_GetArrayItem(arr, 1);
    num_val = cJSON_GetNumberValue(got_item);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)(arr_size + '0');
    buffer[1] = (str_val ? str_val[0] : '\0');
    buffer[2] = (char)(((int)num_val % 10) + '0');

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)buffer;
    return 66;
}