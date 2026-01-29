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
//<ID> 1737
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *retrieved_data = (cJSON *)0;
    cJSON *retrieved_meta = (cJSON *)0;
    cJSON *retrieved_label = (cJSON *)0;
    cJSON_bool has_data = 0;
    cJSON_bool meta_is_object = 0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Initialize / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    meta = cJSON_CreateObject();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);
    label = cJSON_CreateString("example");

    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "metadata", meta);
    cJSON_AddItemToObject(meta, "label", label);

    // step 3: Operate (use required APIs)
    has_data = cJSON_HasObjectItem(root, "data");
    retrieved_data = cJSON_GetObjectItem(root, "data");
    arr_size = cJSON_GetArraySize(retrieved_data);
    retrieved_meta = cJSON_GetObjectItem(root, "metadata");
    meta_is_object = cJSON_IsObject(retrieved_meta);
    retrieved_label = cJSON_GetObjectItem(retrieved_meta, "label");

    // step 4: Validate
    validation = (int)has_data + (int)(arr_size == 2) + (int)meta_is_object + (int)(retrieved_label != (cJSON *)0);
    (void)validation;

    // step 5: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)meta;
    (void)num0;
    (void)num1;
    (void)label;
    (void)retrieved_data;
    (void)retrieved_meta;
    (void)retrieved_label;
    (void)has_data;
    (void)meta_is_object;
    (void)arr_size;

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}