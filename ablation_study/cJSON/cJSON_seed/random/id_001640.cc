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
//<ID> 1640
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
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *detached_arr = (cJSON *)0;
    cJSON *detached_elem = (cJSON *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_label = 0;
    cJSON_bool readded_elem = 0;
    int array_size = 0;
    int validation_score = 0;
    char json_buf[] = " { \"x\" : [ 10 , 20 ] } ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString("one");
    item2 = cJSON_CreateNumber(2.0);

    // step 3: Configure
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    added_label = cJSON_AddItemToObject(root, "label", cJSON_CreateString("mylist"));

    // step 4: Operate
    cJSON_Minify(json_buf);
    parsed = cJSON_Parse(json_buf);
    detached_arr = cJSON_DetachItemViaPointer(root, arr);
    detached_elem = cJSON_DetachItemFromArray(arr, 1);
    readded_elem = cJSON_AddItemToArray(arr, detached_elem);
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);

    // step 5: Validate
    label = cJSON_GetObjectItem(root, "label");
    array_size = cJSON_GetArraySize(arr);
    validation_score = (int)added_arr + (int)added_label + (int)readded_elem + (int)(label != (cJSON *)0 && cJSON_IsString(label)) + array_size + (int)(parsed != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)detached_arr;
    (void)detached_elem;
    (void)item1;
    (void)item2;
    (void)label;
    // API sequence test completed successfully
    return 66;
}