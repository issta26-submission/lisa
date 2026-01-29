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
//<ID> 687
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
    cJSON *meta_str = (cJSON *)0;
    cJSON_bool added_meta = 0;
    char json_buf[] = "{ \"description\": \" example \", \"values\" : [ 10 , 20 ] }";
    char *printed = (char *)0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    cJSON_Minify(json_buf);
    root = cJSON_CreateObject();

    // step 3: Configure
    arr = cJSON_AddArrayToObject(root, "numbers");
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    meta_str = cJSON_CreateString(json_buf);
    added_meta = cJSON_AddItemToObject(root, "meta", meta_str);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    arr_size = cJSON_GetArraySize(arr);
    validation_score = (printed != (char *)0) + arr_size;

    // step 5: Validate
    (void)added_meta;
    (void)validation_score;
    (void)num0;
    (void)num1;
    (void)meta_str;
    (void)arr;
    (void)root;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}