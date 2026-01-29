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
//<ID> 1834
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
    cJSON *num2 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *retrieved_detached = (cJSON *)0;
    cJSON *retrieved_whitespace_num = (cJSON *)0;
    cJSON *added_number_ptr = (cJSON *)0;
    cJSON_bool add_ok_arr = 0;
    cJSON_bool add_ok_obj = 0;
    int validation = 0;
    char raw_json[] = " { \"whitespace\" :  123 , \"arr\" : [ 10 , 20 , 30 ] } ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(20.0);
    num2 = cJSON_CreateNumber(30.0);

    // step 3: Configure
    add_ok_arr = cJSON_AddItemToArray(arr, num0);
    add_ok_arr = (cJSON_bool)(add_ok_arr + cJSON_AddItemToArray(arr, num1));
    add_ok_arr = (cJSON_bool)(add_ok_arr + cJSON_AddItemToArray(arr, num2));
    add_ok_obj = cJSON_AddItemToObject(root, "arr", arr);
    added_number_ptr = cJSON_AddNumberToObject(root, "whitespace", 123.0);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "arr");
    detached = cJSON_DetachItemFromArray(retrieved_arr, 1); // detach the middle element (20)
    add_ok_obj = (cJSON_bool)(add_ok_obj + cJSON_AddItemToObject(root, "detached", detached));
    retrieved_detached = cJSON_GetObjectItem(root, "detached");
    retrieved_whitespace_num = cJSON_GetObjectItem(root, "whitespace");
    cJSON_Minify(raw_json);

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(num0 != (cJSON *)0) + (int)(num1 != (cJSON *)0) + (int)(num2 != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(detached != (cJSON *)0) + (int)(retrieved_detached != (cJSON *)0) + (int)(retrieved_whitespace_num != (cJSON *)0) + (int)(added_number_ptr != (cJSON *)0) + (int)(raw_json[0] != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num0;
    (void)num1;
    (void)num2;
    (void)retrieved_arr;
    (void)detached;
    (void)retrieved_detached;
    (void)retrieved_whitespace_num;
    (void)added_number_ptr;
    (void)add_ok_arr;
    (void)add_ok_obj;
    (void)raw_json;
    // API sequence test completed successfully
    return 66;
}