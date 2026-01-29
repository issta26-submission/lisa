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
//<ID> 681
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
    cJSON *arr2 = (cJSON *)0;
    cJSON *s0 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *s3 = (cJSON *)0;
    char *printed = (char *)0;
    char minify_buf[] = " { \"min\": [ 10 , 20 , 30 ] } ";
    int arr_size1 = 0;
    int arr_size2 = 0;
    int total_size = 0;
    cJSON_bool added_flag = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("one");
    s1 = cJSON_CreateString("two");
    s2 = cJSON_CreateString("alpha");
    s3 = cJSON_CreateString("beta");

    // step 3: Configure
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    added_flag = cJSON_AddItemToObject(root, "numbers", arr);
    arr2 = cJSON_AddArrayToObject(root, "words");
    cJSON_AddItemToArray(arr2, s2);
    cJSON_AddItemToArray(arr2, s3);

    // step 4: Operate
    cJSON_Minify(minify_buf);
    printed = cJSON_PrintUnformatted(root);
    arr_size1 = cJSON_GetArraySize(arr);
    arr_size2 = cJSON_GetArraySize(arr2);
    total_size = arr_size1 + arr_size2;

    // step 5: Validate
    (void)added_flag;
    (void)total_size;
    (void)minify_buf;
    (void)printed;
    (void)arr_size1;
    (void)arr_size2;
    (void)s0;
    (void)s1;
    (void)s2;
    (void)s3;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}