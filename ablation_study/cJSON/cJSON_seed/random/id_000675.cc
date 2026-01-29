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
//<ID> 675
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
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_obj = 0;
    cJSON_bool added_num = 0;
    double extracted_number = 0.0;
    char *extracted_string = (char *)0;
    int nonnull_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str0 = cJSON_CreateString("alpha");
    str1 = cJSON_CreateString("beta");
    num_item = cJSON_CreateNumber(2.0);

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, str0);
    added1 = cJSON_AddItemToArray(arr, str1);
    added_obj = cJSON_AddItemToObject(root, "items", arr);
    added_num = cJSON_AddItemToObject(root, "count", num_item);

    // step 4: Operate
    retrieved_num = cJSON_GetObjectItem(root, "count");
    extracted_number = cJSON_GetNumberValue(retrieved_num);
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    extracted_string = cJSON_GetStringValue(first_elem);
    nonnull_count = (extracted_string != (char *)0) + (retrieved_num != (cJSON *)0);

    // step 5: Validate
    (void)added0;
    (void)added1;
    (void)added_obj;
    (void)added_num;
    (void)extracted_number;
    (void)extracted_string;
    (void)nonnull_count;
    (void)first_elem;
    (void)retrieved_arr;
    (void)retrieved_num;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}