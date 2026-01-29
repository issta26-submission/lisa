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
//<ID> 274
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char json[] = " [ 3.14 , 2.72 , 1.0 ] ";
    cJSON *arr = NULL;
    cJSON *root = NULL;
    cJSON *item0 = NULL;
    cJSON *num_copy = NULL;
    cJSON *size_num = NULL;
    int arr_size = 0;
    double first_val = 0.0;

    // step 2: Setup / Configure
    cJSON_Minify(json);
    arr = cJSON_Parse(json);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "array", arr);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(item0);
    num_copy = cJSON_CreateNumber(first_val);
    cJSON_AddItemToObject(root, "first_value", num_copy);
    size_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "size", size_num);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}