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
//<ID> 1830
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
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    char json_buf[] = "  { \"min\":  7, \"keep\" : [ 1 , 2 ] }  ";
    const char *minified_ptr = (const char *)0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    num_item = cJSON_AddNumberToObject(root, "num", 123.456);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3));

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "arr");
    detached_item = cJSON_DetachItemFromArray(retrieved_arr, 1);
    retrieved_num = cJSON_GetObjectItem(root, "num");
    cJSON_Minify(json_buf);
    minified_ptr = json_buf;

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(detached_item != (cJSON *)0) + (int)(retrieved_num != (cJSON *)0) + (int)(minified_ptr[0] == '{');
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(detached_item);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num_item;
    (void)retrieved_arr;
    (void)detached_item;
    (void)retrieved_num;
    (void)minified_ptr;
    (void)json_buf;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}