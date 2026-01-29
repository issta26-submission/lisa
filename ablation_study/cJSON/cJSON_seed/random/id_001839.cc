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
//<ID> 1839
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    cJSON *added_number = (cJSON *)0;
    int validation = 0;
    char json_buf[] = " { \"alpha\" :  42 , \"beta\" : [ 7 , 8 , 9 ] } ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(7.0);
    num1 = cJSON_CreateNumber(8.0);
    (void)cJSON_AddItemToArray(arr, num0);
    (void)cJSON_AddItemToArray(arr, num1);
    (void)cJSON_AddItemToObject(root, "nums", arr);
    added_number = cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 3: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "nums");
    detached_item = cJSON_DetachItemFromArray(retrieved_arr, 1);
    (void)cJSON_AddItemToObject(root, "detached", detached_item);
    cJSON_Minify(json_buf);

    // step 4: Validate
    validation = (int)(root != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(detached_item != (cJSON *)0) + (int)(added_number != (cJSON *)0) + (int)(json_buf[0] != '\0');
    (void)validation;

    // step 5: Cleanup
    cJSON_Delete(root);
    (void)arr;
    (void)num0;
    (void)num1;
    (void)retrieved_arr;
    (void)detached_item;
    (void)added_number;
    (void)json_buf;
    (void)validation;

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}