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
//<ID> 994
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
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *num_mid = NULL;
    cJSON *first_item = NULL;
    char *printed = NULL;
    double first_val = 0.0;
    cJSON_bool is_arr = 0;
    cJSON_bool add_ok = 0;
    cJSON_bool insert_ok = 0;
    int prebuffer = 128;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    add_ok = cJSON_AddItemToObject(root, "items", arr);
    num1 = cJSON_CreateNumber(30.0);
    insert_ok = cJSON_InsertItemInArray(arr, 0, num1);
    num0 = cJSON_CreateNumber(10.0);
    insert_ok = cJSON_InsertItemInArray(arr, 0, num0);
    num_mid = cJSON_CreateNumber(20.0);
    insert_ok = cJSON_InsertItemInArray(arr, 1, num_mid);

    // step 3: Operate / Validate
    is_arr = cJSON_IsArray(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first_item);
    printed = cJSON_PrintBuffered(root, prebuffer, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}