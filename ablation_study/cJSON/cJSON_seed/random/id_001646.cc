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
//<ID> 1646
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
    cJSON *item_str1 = (cJSON *)0;
    cJSON *item_str2 = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *detached_via_ptr = (cJSON *)0;
    cJSON *detached_by_index = (cJSON *)0;
    char json_buffer[] = " { \"alpha\" :  123 , \"beta\" : [ \"x\" , \"y\" ] } ";
    int validation_score = 0;
    cJSON_bool added_flag = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_str1 = cJSON_CreateString("one");
    item_str2 = cJSON_CreateString("two");
    num_item = cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 3: Configure
    added_flag = cJSON_AddItemToArray(arr, item_str1);
    added_flag = cJSON_AddItemToArray(arr, item_str2);
    added_flag = cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    cJSON_Minify(json_buffer);
    detached_via_ptr = cJSON_DetachItemViaPointer(arr, item_str1);
    detached_by_index = cJSON_DetachItemFromArray(arr, 0);
    added_flag = cJSON_AddItemToArray(arr, detached_via_ptr);
    added_flag = cJSON_AddItemToObject(root, "moved", detached_by_index);

    // step 5: Validate
    validation_score = (int)cJSON_GetArraySize(arr) + (int)cJSON_HasObjectItem(root, "moved") + (int)(num_item != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)added_flag;
    (void)json_buffer;
    // API sequence test completed successfully
    return 66;
}