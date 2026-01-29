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
//<ID> 580
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddNumberToObject(info, "version", 1.5);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_InsertItemInArray(arr, 0, n1);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_InsertItemInArray(arr, 1, n2);

    // step 3: Operate and Validate
    cJSON *got_info = cJSON_GetObjectItem(root, "info");
    cJSON *ver_item = cJSON_GetObjectItemCaseSensitive(got_info, "version");
    cJSON_bool has_array = cJSON_HasObjectItem(root, "array");
    (void)has_array;
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    double ver_val = cJSON_GetNumberValue(ver_item);
    double total = first_val + ver_val;
    cJSON_AddNumberToObject(root, "sum", total);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}