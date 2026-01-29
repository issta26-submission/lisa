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
//<ID> 582
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(30.0));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "initial_count", 2.0);
    cJSON_AddNumberToObject(root, "root_value", 42.0);

    // step 2: Configure
    cJSON *insert_elem = cJSON_CreateNumber(20.0);
    cJSON_bool insert_result = cJSON_InsertItemInArray(numbers, 1, insert_elem);
    (void)insert_result;
    cJSON_AddNumberToObject(meta, "inserted_value", 20.0);
    cJSON_bool has_numbers = cJSON_HasObjectItem(root, "numbers");
    (void)has_numbers;

    // step 3: Operate and Validate
    cJSON *got_numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *got_numbers_cs = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    (void)got_numbers_cs;
    int arr_size = cJSON_GetArraySize(got_numbers);
    (void)arr_size;
    cJSON *picked_item = cJSON_GetArrayItem(got_numbers, 1);
    double picked_val = cJSON_GetNumberValue(picked_item);
    cJSON_AddNumberToObject(root, "picked", picked_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}