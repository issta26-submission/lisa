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
//<ID> 867
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
    cJSON *num_item = NULL;
    cJSON *num_obj = NULL;
    char *printed = NULL;
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool insert_ok = 0;
    double extracted_num = 0.0;
    const char *str_val = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);
    str_ref = cJSON_CreateStringReference("hello_ref");
    cJSON_InsertItemInArray(arr, 0, str_ref);
    num_item = cJSON_CreateNumber(42.5);
    insert_ok = cJSON_InsertItemInArray(arr, 1, num_item);
    num_obj = cJSON_AddNumberToObject(root, "value", 42.5);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    str_val = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    extracted_num = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    buffer[0] = (printed ? printed[0] : 0);
    buffer[1] = (str_val ? str_val[0] : 0);
    buffer[2] = (char)(extracted_num);
    (void)insert_ok;
    (void)num_obj;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    return 66;
}